
include( FetchContent )
FetchContent_Declare( imgui # docking branch
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG c2c38beec82aa94cf38cdbaca8ccb82a0e757721 )
FetchContent_MakeAvailable( imgui )

add_library( imgui )
target_sources( imgui
  PRIVATE
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlgpu3.cpp
  INTERFACE
    ${imgui_SOURCE_DIR}/imgui.h
    ${imgui_SOURCE_DIR}/imconfig.h
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.h
    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlgpu3.h )
target_include_directories( imgui
  PRIVATE
    ${imgui_SOURCE_DIR}
  INTERFACE
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends )
target_link_libraries( imgui SDL3::SDL3 )

