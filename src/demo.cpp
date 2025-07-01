#include "demo.h"

#include <microtex.h>

#include "microtex_imgui_graphics.h"

static microtex::Render* render{};

MicroTeXImGuiGraphics sGraphics2D;

static const char* sExample_Simple = R"_(\frac{\pi}{2})_";
static const char* sExample_Integral = R"_(\int_{\textcolor{red}{a}}^b \frac{x}{\ln(x)} \, \mathrm{d}x)_";
static const char* sExample_Color = R"_(\textcolor{red}{x}\textcolor{green}{y}z)_";
static const char* sExample_Curr;
static const char* sExample_Next = sExample_Integral;
static float sExampleSizeMultipler = 10;

void Demo::Init()
{
  const char* fontClmPath = "firamath/FiraMath-Regular.clm2" ;
  const char* fontOtfPath = "firamath/FiraMath-Regular.otf" ;
  const char* fontTtfPath = "firamath/FiraMath-Regular.ttf" ;

  microtex::FontSrcFile fontSrc( fontClmPath, fontOtfPath );
  microtex::MicroTeX::init( fontSrc );


  ImGui::GetIO().Fonts->AddFontFromFileTTF( fontTtfPath );


  microtex::PlatformFactory::registerFactory( "", std::make_unique< MicroTexImGuiPlatformFactory >() );
  MicroTeXImGuiGraphics::UnitTest();
}

void Demo::Update()
{
  ImGui::ShowDemoWindow();

  const char* examples[]{
    sExample_Simple,
    sExample_Integral,
    sExample_Color,
  };

  for( const char* ex : examples )
  {
    if( ImGui::Button( ex ) )
      sExample_Next = ex;
  }

  bool dirty = false;
  dirty |= ImGui::DragFloat( "Size Multiplier", &sExampleSizeMultipler, 0.1f, 0.1f, 100.0f );
  dirty |= ImGui::DragFloat( "Magic", &MicroTeXImGuiGraphics::sMagic, 0.1f, 0.1f, 100.0f );
  dirty |= ImGui::DragFloat( "Magic2", &MicroTeXImGuiGraphics::sMagic2, 0.001f, 0.001f, 100.0f );

  if( sExample_Curr != sExample_Next )
  {
    sExample_Curr = sExample_Next;
    dirty = true;
  }

  if( sExample_Curr && dirty )
  {
    float width = 0; // unlimited
    float textSize = ImGui::GetFontSize() * sExampleSizeMultipler;
    float lineSpace = 0; // ???
    microtex::color _color = microtex::getColor( "white" );
    render = microtex::MicroTeX::parse( sExample_Curr, width, textSize, lineSpace, _color );
  }

  if( ImGui::Begin( "Equation" ) )
    if(render)
      render->draw( sGraphics2D, 0, 0 );
  ImGui::End();

}

