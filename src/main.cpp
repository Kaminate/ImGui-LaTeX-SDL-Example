#include <microtex.h>

#include "microtex_imgui_graphics.h"

#include "demo.h"


#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlgpu3.h>

static SDL_GPUDevice* sGPUDevice;
static SDL_Window* sWindow;

SDL_AppResult SDL_AppInit( void** , int, char** )
{
  SDL_InitFlags initFlags = SDL_INIT_VIDEO | SDL_INIT_GAMEPAD;
  if( !SDL_Init( initFlags ) )
  {
    SDL_Log( "Couldn't initialize SDL: %s", SDL_GetError() );
    return SDL_APP_FAILURE;
  }

  SDL_assert( SDL_VERSION == SDL_GetVersion() );
  SDL_Log( "SDL compiled (and linked) against version %d.%d.%d.\n",
           SDL_MAJOR_VERSION, SDL_MINOR_VERSION, SDL_MICRO_VERSION );

  int wUnscaled = 1600;
  int hUnscaled = 900;
  float main_scale = SDL_GetDisplayContentScale( SDL_GetPrimaryDisplay() );
  int wScaled = ( int )( main_scale * wUnscaled );
  int hScaled = ( int )( main_scale * hUnscaled );
  SDL_WindowFlags windowFlags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
  SDL_Window* window = SDL_CreateWindow( "Game01", wScaled, hScaled, windowFlags );
  if( !window )
  {
    SDL_LogError( SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError() );
    return SDL_APP_FAILURE;
  }

  bool isDebugMode = false;
#if defined( _DEBUG )
  isDebugMode = true;
#endif

  SDL_GPUShaderFormat gpuShaderFormat = SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_METALLIB;
  SDL_GPUDevice* gpu_device = SDL_CreateGPUDevice( gpuShaderFormat, isDebugMode, nullptr );
  if( !gpu_device )
  {
    SDL_LogError( SDL_LOG_CATEGORY_ERROR, "Could not create gpu device: %s\n", SDL_GetError() );
    return SDL_APP_FAILURE;
  }

  if( !SDL_ClaimWindowForGPUDevice( gpu_device, window ) )
  {
    SDL_LogError( SDL_LOG_CATEGORY_ERROR, "Failed to claim window for gpu device: %s\n", SDL_GetError() );
    return SDL_APP_FAILURE;
  }

  if( !SDL_SetGPUSwapchainParameters( gpu_device, window, SDL_GPU_SWAPCHAINCOMPOSITION_SDR, SDL_GPU_PRESENTMODE_VSYNC ) )
  {
    SDL_LogError( SDL_LOG_CATEGORY_ERROR, "Failed to set gpu swapchain parameters: %s\n", SDL_GetError() );
    return SDL_APP_FAILURE;
  }

  SDL_SetWindowPosition( window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED );
  SDL_ShowWindow( window );

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

  ImGui::StyleColorsDark();

  ImGuiStyle& style = ImGui::GetStyle();
  style.ScaleAllSizes( main_scale );
  style.FontScaleDpi = main_scale;

  ImGui_ImplSDL3_InitForSDLGPU( window );
  ImGui_ImplSDLGPU3_InitInfo init_info
  {
    .Device            { gpu_device },
    .ColorTargetFormat { SDL_GetGPUSwapchainTextureFormat( gpu_device, window ) },
    .MSAASamples       { SDL_GPU_SAMPLECOUNT_1 },
  };
  ImGui_ImplSDLGPU3_Init( &init_info );

  sGPUDevice = gpu_device;
  sWindow = window;


  Demo::Init();


  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent( void* , SDL_Event* event )
{
  ImGui_ImplSDL3_ProcessEvent( event );
  if( event->type == SDL_EVENT_QUIT )
    return SDL_APP_SUCCESS;

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate( void* pAppState )
{
  if( SDL_GetWindowFlags( sWindow ) & SDL_WINDOW_MINIMIZED )
  {
    SDL_Delay( 10 );
    return SDL_APP_CONTINUE;
  }

  ImGui_ImplSDLGPU3_NewFrame();
  ImGui_ImplSDL3_NewFrame();
  ImGui::NewFrame();

  // -------------------
  // update the app here
  // -------------------
  Demo::Update();

  ImGui::Render();
  ImDrawData* draw_data = ImGui::GetDrawData();
  const bool is_minimized = ( draw_data->DisplaySize.x <= 0.0f || draw_data->DisplaySize.y <= 0.0f );

  SDL_GPUCommandBuffer* command_buffer = SDL_AcquireGPUCommandBuffer( sGPUDevice );

  SDL_GPUTexture* swapchain_texture;
  Uint32 w;
  Uint32 h;
  SDL_WaitAndAcquireGPUSwapchainTexture( command_buffer, sWindow, &swapchain_texture, &w, &h );

  if( swapchain_texture && !is_minimized )
  {
    ImGui_ImplSDLGPU3_PrepareDrawData( draw_data, command_buffer );

    // never draw as completely black because its common to draw black debug lines
    float gray = 0.1f;
    SDL_GPUColorTargetInfo target_info
    {
      .texture     { swapchain_texture },
      .clear_color { SDL_FColor { gray, gray, gray, 1 } },
      .load_op     { SDL_GPU_LOADOP_CLEAR },
      .store_op    { SDL_GPU_STOREOP_STORE },
    };
    SDL_GPURenderPass* render_pass = SDL_BeginGPURenderPass( command_buffer, &target_info, 1, nullptr );

    ImGui_ImplSDLGPU3_RenderDrawData( draw_data, command_buffer, render_pass );

    SDL_EndGPURenderPass( render_pass );
  }

  SDL_SubmitGPUCommandBuffer( command_buffer );

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit( void* , SDL_AppResult )
{
  SDL_GPUDevice* gpu_device = sGPUDevice;
  SDL_Window* window = sWindow;
  SDL_WaitForGPUIdle( gpu_device );
  ImGui_ImplSDL3_Shutdown();
  ImGui_ImplSDLGPU3_Shutdown();
  ImGui::DestroyContext();

  SDL_ReleaseWindowFromGPUDevice( gpu_device, window );
  SDL_DestroyGPUDevice( gpu_device );
  SDL_DestroyWindow( window );
}



