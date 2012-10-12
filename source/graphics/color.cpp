
// ============================================================

#if _WIN32
#  pragma warning( disable : 4244 )
#  define _CRT_SECURE_NO_WARNINGS 1
#endif

#include "graphics/color.h"

#include <algorithm>
#include <cstdlib>

// ============================================================

static const graphics::color Colors[] = 
{
  graphics::color::Red,
  graphics::color::Blue,
  graphics::color::Green,
  graphics::color::Cyan,
  graphics::color::Magenta,
  graphics::color::DarkRed,
  graphics::color::DarkBlue,
  graphics::color::DarkGreen,
  graphics::color::DarkMagenta,
  graphics::color::DarkYellow
};
  
static const unsigned ColorsSize = sizeof(Colors)/sizeof(Colors[0]);
static const unsigned BeginColorIndex = 2*ColorsSize-1;
static unsigned ColorIndex = BeginColorIndex;

// ------------------------------------------------------------

void graphics::color::setTransparency( double Part ) 
{ 
  if ( Part < 0 ) return setTransparency(0); 
  else if ( Part > 1 ) return setTransparency(1);
  else RGB = ( RGB & 0x00FFFFFF ) | ( (unsigned char)( Part * 0xFF ) << 24 );
}

// ------------------------------------------------------------

std::string graphics::color::name( bool WithPrefix ) const 
{ 
  char Name[32];
  snprintf(Name,sizeof(Name)-1, WithPrefix ? "0x%08X" : "%08X",valueRgb());
  return std::string(Name); 
}

// ------------------------------------------------------------

graphics::color graphics::color::fromRGB( unsigned R, unsigned G, unsigned B, unsigned T )
{
  return color( R, G, B, T );
}

// ------------------------------------------------------------

graphics::color graphics::color::fromHSV( unsigned H, unsigned S, unsigned V, unsigned T )
{
  /* http://ru.wikipedia.org/wiki/HSV_(%D1%86%D0%B2%D0%B5%D1%82%D0%BE%D0%B2%D0%B0%D1%8F_%D0%BC%D0%BE%D0%B4%D0%B5%D0%BB%D1%8C) */

  H = std::min<unsigned>( H, 360 );
  S = std::min<unsigned>( S, 100 );
  V = std::min<unsigned>( V, 100 );

  double Vmin = ( 100.0 - S ) * V / 100.0;
  double a = ( V - Vmin ) * ( H % 60 )/60.0;
  double Vinc = Vmin + a;
  double Vdec = V - a;
  double Veff = (double)V;

  Veff *= 0xFF/100.0;
  Vmin *= 0xFF/100.0;
  Vinc *= 0xFF/100.0;
  Vdec *= 0xFF/100.0;

  switch ( std::min( H / 60, 5U ) )
  {
    case 0:     return color( Veff, Vinc, Vmin, T );
    case 1:     return color( Vdec, Veff, Vmin, T );
    case 2:     return color( Vmin, Veff, Vinc, T );
    case 3:     return color( Vmin, Vdec, Veff, T );
    case 4:     return color( Vinc, Vmin, Veff, T );
    case 5:     return color( Veff, Vmin, Vdec, T );
    default:    std::abort(); return color();
  }
}

// ------------------------------------------------------------

unsigned graphics::color::maxRGB() const
{
  return std::max( std::max( red(), green() ), blue() );
}

// ------------------------------------------------------------

unsigned graphics::color::minRGB() const
{
  return std::min( std::min( red(), green() ), blue() );
}

// ------------------------------------------------------------

unsigned graphics::color::hue() const
{
  unsigned Min = minRGB();
  unsigned Max = maxRGB();

  if ( Min == Max )
    return 0;
  else if ( Max == red() && green() >= blue() )
    return 60.0 * ( (double)green() - blue() )/( Max - Min ) + 0;
  else if ( Max == red() && green() < blue() )
    return 60.0 * ( (double)green() - blue() )/( Max - Min ) + 360;
  else if ( Max == green() )
    return 60.0 * ( (double)blue() - red() )/( Max - Min ) + 120;
  else if ( Max == blue() )
    return 60.0 * ( (double)red() - green() )/( Max - Min ) + 240;
  else 
    std::abort();
  return 0;
}

// ------------------------------------------------------------

unsigned graphics::color::saturation() const
{
  unsigned Min = minRGB();
  unsigned Max = maxRGB();

  if ( Max == 0 )
    return 0;
  return 100 * ( 1 - (double)Min/Max );
}

// ------------------------------------------------------------

unsigned graphics::color::value() const
{
  return 100.0 * (double)maxRGB()/0xFF;
}

// ------------------------------------------------------------

graphics::color graphics::color::nextColor()
{
  ColorIndex = ( ColorIndex + 1 ) % ColorsSize;
  color NextColor = Colors[ ColorIndex ];

  return NextColor;
}

// ------------------------------------------------------------

graphics::color graphics::color::predefinedColor( unsigned Index )
{
  return Colors[ Index % ColorsSize ];
}

// ------------------------------------------------------------

graphics::color graphics::color::thisColor()
{
  return Colors[ ColorIndex==BeginColorIndex ? 0 : ColorIndex ];
}

// ------------------------------------------------------------

void graphics::color::resetNextColorSequence()
{
  ColorIndex = BeginColorIndex;
}

// ============================================================

