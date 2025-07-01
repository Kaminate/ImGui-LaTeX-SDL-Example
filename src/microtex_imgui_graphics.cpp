#include "microtex_imgui_graphics.h"
#include <SDL3/SDL.h>

struct m3
{
  bool operator == ( const m3& ) const = default;
  float
    m00{ 1 }, m01{ 0 }, m02{ 0 },
    m10{ 0 }, m11{ 1 }, m12{ 0 },
    m20{ 0 }, m21{ 0 }, m22{ 1 };
};
struct v3 { float x{}, y{}, z{}; };
m3 Translate( float x, float y )
{
  return {
    1, 0, x,
    0, 1, y,
    0, 0, 1 };
}
m3 Scale( float x, float y )
{
  return {
    x, 0, 0,
    0, y, 0,
    0, 0, 1 };
}
float Dot( const v3& a, const v3& b )
{
  return a.x * b.x + a.y * b.y + a.z * b.z;
}
m3 operator * ( const m3& a, const m3& b)
{
  float m00 = Dot( v3{ a.m00, a.m01, a.m02 }, v3{ b.m00, b.m10, b.m20 } );
  float m01 = Dot( v3{ a.m00, a.m01, a.m02 }, v3{ b.m01, b.m11, b.m21 } );
  float m02 = Dot( v3{ a.m00, a.m01, a.m02 }, v3{ b.m02, b.m12, b.m22 } );
  float m10 = Dot( v3{ a.m10, a.m11, a.m12 }, v3{ b.m00, b.m10, b.m20 } );
  float m11 = Dot( v3{ a.m10, a.m11, a.m12 }, v3{ b.m01, b.m11, b.m21 } );
  float m12 = Dot( v3{ a.m10, a.m11, a.m12 }, v3{ b.m02, b.m12, b.m22 } );
  float m20 = Dot( v3{ a.m20, a.m21, a.m22 }, v3{ b.m00, b.m10, b.m20 } );
  float m21 = Dot( v3{ a.m20, a.m21, a.m22 }, v3{ b.m01, b.m11, b.m21 } );
  float m22 = Dot( v3{ a.m20, a.m21, a.m22 }, v3{ b.m02, b.m12, b.m22 } );
  return {
    m00, m01, m02,
    m10, m11, m12,
    m20, m21, m22 };
}
v3 operator * ( const m3& m, const v3& v)
{
  float x = Dot( v3{ m.m00, m.m01, m.m02 }, v );
  float y = Dot( v3{ m.m10, m.m11, m.m12 }, v );
  float z = Dot( v3{ m.m20, m.m21, m.m22 }, v );
  return { x, y, z };
}

static void TODO()
{
  static int todo;
  todo++;
}
static m3 sTransform;

static ImVec2 Transform( float x, float y )
{
  v3 xyz{ x, y, 1 };
  v3 xyzprime = sTransform * xyz;
  return { xyzprime.x, xyzprime.y };
}

void MicroTeXImGuiGraphics::UnitTest()
{
  m3 a{ 1, 2, 3,
        4, 5, 6,
        7, 8, 9 };
  m3 b{ 11, 22, 33,
        44, 55, 66,
        77, 88, 99 };
  m3 c = a * b;
  m3 cExpected{ 330, 396, 462,
                726, 891, 1056,
                1122, 1386, 1650 };
  SDL_assert( c == cExpected );
}


MicroTeXImGuiFont::MicroTeXImGuiFont( const std::string& file )
{
  mFile = file;
}

bool MicroTeXImGuiFont::operator==( const Font& f ) const
{
  return ((MicroTeXImGuiFont&)f).mFile == mFile;
}

// ---------------------------------------------------------------------------------------------
MicroTeXImGuiTextLayout::MicroTeXImGuiTextLayout( const std::string& src, microtex::FontStyle style, float size )
  : _fontSize( size )
{
  TODO();
}
void MicroTeXImGuiTextLayout::getBounds( microtex::Rect& bounds )
{
  TODO();
}
void MicroTeXImGuiTextLayout::draw( microtex::Graphics2D& g2, float x, float y )
{
  TODO();
}

// ---------------------------------------------------------------------------------------------
auto MicroTexImGuiPlatformFactory::createFont( const std::string& file ) -> microtex::sptr<microtex::Font>
{
  return microtex::sptr<microtex::Font>( new MicroTeXImGuiFont( file ) );
};
auto MicroTexImGuiPlatformFactory::createTextLayout( const std::string& src, microtex::FontStyle style, float size ) -> microtex::sptr<microtex::TextLayout>
{
  return microtex::sptr<microtex::TextLayout>( new MicroTeXImGuiTextLayout( src, style, size ) );
};

// ---------------------------------------------------------------------------------------------

MicroTeXImGuiGraphics::MicroTeXImGuiGraphics()
{
}
MicroTeXImGuiGraphics::~MicroTeXImGuiGraphics()
{
}
void MicroTeXImGuiGraphics::setColor( microtex::color c )
{
  _color = c;
}
auto MicroTeXImGuiGraphics::getColor() const -> microtex::color { return _color; }
void MicroTeXImGuiGraphics::setStroke( const microtex::Stroke& s )
{
  _stroke = s;
}
auto MicroTeXImGuiGraphics::getStroke() const -> const microtex::Stroke& { return _stroke; }
void MicroTeXImGuiGraphics::setStrokeWidth( float w )
{
  _stroke.lineWidth = w;
}
void MicroTeXImGuiGraphics::setDash( const std::vector<float>& dash )
{
  TODO();
}
auto MicroTeXImGuiGraphics::getDash() -> std::vector<float>
{
  TODO();
  return {}; // todo
}
auto MicroTeXImGuiGraphics::getFont() const -> microtex::sptr<microtex::Font> { return _font; }
void MicroTeXImGuiGraphics::setFont( const microtex::sptr<microtex::Font>& font )
{
  _font = std::static_pointer_cast< MicroTeXImGuiFont >( font );
}
auto MicroTeXImGuiGraphics::getFontSize() const -> float { return _fontSize; }
void MicroTeXImGuiGraphics::setFontSize( float size ) { _fontSize = size; }
void MicroTeXImGuiGraphics::translate( float dx, float dy )
{
  sTransform = Translate( dx, dy ) * sTransform;
}
void MicroTeXImGuiGraphics::scale( float sx, float sy )
{
  sTransform = Scale( sx, sy ) * sTransform;
}
void MicroTeXImGuiGraphics::rotate( float angle )
{
  TODO();
  // todo
}
void MicroTeXImGuiGraphics::rotate( float angle, float px, float py )
{
  TODO();
  // todo
}
void MicroTeXImGuiGraphics::reset()
{
  sTransform = {};
  _sx = 1;
  _sy = 1;
  TODO();
  SDL_assert( false );
}
auto MicroTeXImGuiGraphics::sx() const -> float { return _sx; }
auto MicroTeXImGuiGraphics::sy() const -> float { return _sy; }


#include <clocale>
#include <cuchar>
#include <climits>

size_t to_utf8( char32_t codepoint, char* buf )
{
  const char* loc = std::setlocale( LC_ALL, "en_US.utf8" );
  std::mbstate_t state{};
  std::size_t len = std::c32rtomb( buf, codepoint, &state );
  std::setlocale( LC_ALL, loc );
  return len;
}

float MicroTeXImGuiGraphics::sMagic = .9f;
float MicroTeXImGuiGraphics::sMagic2 = 0.09f;

static ImU32 ToImGuiCol32( microtex::color _color )
{
  float r = microtex::color_r( _color ) / 255.0f;
  float g = microtex::color_g( _color ) / 255.0f;
  float b = microtex::color_b( _color ) / 255.0f;
  float a = microtex::color_a( _color ) / 255.0f;
  auto imCol32 = ImGui::ColorConvertFloat4ToU32(ImVec4(r, g, b, a));
  return imCol32;
}

struct UTF8CodepointString
{
  UTF8CodepointString( microtex::u16 glyph )
  {
    const char* loc = std::setlocale( LC_ALL, "en_US.utf8" );
    std::setlocale( LC_ALL, loc );
    std::mbstate_t state{};
    len = std::c16rtomb( buf, glyph, &state );
  }
  char buf[ 10 ]{};
  std::size_t len;
};

void MicroTeXImGuiGraphics::drawGlyph( microtex::u16 glyph, float x, float y )
{
  SDL_assert( _font );

  UTF8CodepointString str( glyph );

  //const char* loc = std::setlocale( LC_ALL, "en_US.utf8" );
  //std::mbstate_t state{};
  //char buf[ 10 ]{};
  //std::size_t len = std::c16rtomb( buf, glyph, &state );
  //std::setlocale( LC_ALL, loc );

  auto drawList = ImGui::GetWindowDrawList();
  ImVec2 pos = Transform( x, y );
  
  ImGui::PushFontSize( _fontSize * sTransform.m11 );
  float fontSize = ImGui::GetFontSize() * sMagic;
  sLastGlyphFontSize = fontSize;

  pos.y -= fontSize;
  pos += ImGui::GetWindowPos();
  pos += ImGui::GetWindowContentRegionMin();

  auto imCol32 = ToImGuiCol32( _color );
  //drawList->AddText( pos, imCol32, buf, buf + len );
  drawList->AddText( pos, imCol32, str.buf, str.buf + str.len );
  ImGui::PopFontSize();
}
bool MicroTeXImGuiGraphics::beginPath( microtex::i32 id )
{
  // not supported
  return false;
}
void MicroTeXImGuiGraphics::moveTo( float x, float y )
{
  TODO();
  SDL_assert( false );
}
void MicroTeXImGuiGraphics::lineTo( float x, float y )
{
  TODO();
  SDL_assert( false );
}
void MicroTeXImGuiGraphics::cubicTo( float x1, float y1, float x2, float y2, float x3, float y3 )
{
  TODO();
  SDL_assert( false );
}
void MicroTeXImGuiGraphics::quadTo( float x1, float y1, float x2, float y2 )
{
  TODO();
  SDL_assert( false );
}
void MicroTeXImGuiGraphics::closePath()
{
  TODO();
  SDL_assert( false );
}
void MicroTeXImGuiGraphics::fillPath( microtex::i32 id )
{
  TODO();
  SDL_assert( false );
}
void MicroTeXImGuiGraphics::drawText( const std::wstring& src, float x, float y )
{
  TODO();
  SDL_assert( false );
}
void MicroTeXImGuiGraphics::drawLine( float x1, float y1, float x2, float y2 )
{
  ImVec2 p0 = Transform( x1, y1 ) + ImGui::GetWindowPos() + ImGui::GetWindowContentRegionMin();
  ImVec2 p1 = Transform( x2, y2 ) + ImGui::GetWindowPos() + ImGui::GetWindowContentRegionMin();
  auto drawList = ImGui::GetWindowDrawList();
  auto imCol32 = ToImGuiCol32( _color );
  float thickness = sLastGlyphFontSize * sMagic2;
  drawList->AddLine( p0, p1, imCol32, thickness );
}
void MicroTeXImGuiGraphics::drawRect( float x, float y, float w, float h )
{
  TODO();
  SDL_assert( false );
}
void MicroTeXImGuiGraphics::fillRect( float x, float y, float w, float h )
{
  TODO();
  SDL_assert( false );
}
void MicroTeXImGuiGraphics::drawRoundRect( float x, float y, float w, float h, float rx, float ry )
{
  TODO();
  SDL_assert( false );
}
void MicroTeXImGuiGraphics::fillRoundRect( float x, float y, float w, float h, float rx, float ry )
{
  TODO();
  SDL_assert( false );
}
