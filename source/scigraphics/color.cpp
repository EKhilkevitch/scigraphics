
// ============================================================

#if _WIN32
#  pragma warning( disable : 4244 )
#  define _CRT_SECURE_NO_WARNINGS 1
#endif

#include "scigraphics/color.h"

#include <algorithm>
#include <limits>
#include <cstdlib>

// ============================================================

scigraphics::color::color( int R, int G, int B, int T )
{
  interval<int> Lims(0,0xFF); 
  RGB = ( Lims.toInterval(T) << 24 ) |
        ( Lims.toInterval(R) << 16 ) | 
        ( Lims.toInterval(G) << 8  ) | 
        ( Lims.toInterval(B) << 0  );
}

// ------------------------------------------------------------

void scigraphics::color::setTransparency( double Part ) 
{ 
  if ( Part < 0 ) return setTransparency(0); 
  else if ( Part > 1 ) return setTransparency(1);
  else RGB = ( RGB & 0x00FFFFFF ) | ( (unsigned char)( Part * 0xFF ) << 24 );
}

// ------------------------------------------------------------

std::string scigraphics::color::name( bool WithPrefix ) const 
{ 
  char Name[32];
  snprintf(Name,sizeof(Name)-1, WithPrefix ? "0x%08X" : "%08X",valueRgb());
  return std::string(Name); 
}

// ------------------------------------------------------------

scigraphics::color scigraphics::color::fromRGB( unsigned R, unsigned G, unsigned B, unsigned T )
{
  return color( R, G, B, T );
}

// ------------------------------------------------------------

scigraphics::color scigraphics::color::fromHSV( unsigned H, unsigned S, unsigned V, unsigned T )
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

unsigned scigraphics::color::maxRGB() const
{
  return std::max( std::max( red(), green() ), blue() );
}

// ------------------------------------------------------------

unsigned scigraphics::color::minRGB() const
{
  return std::min( std::min( red(), green() ), blue() );
}

// ------------------------------------------------------------

unsigned scigraphics::color::hue() const
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

unsigned scigraphics::color::saturation() const
{
  unsigned Min = minRGB();
  unsigned Max = maxRGB();

  if ( Max == 0 )
    return 0;
  return 100 * ( 1 - (double)Min/Max );
}

// ------------------------------------------------------------

unsigned scigraphics::color::value() const
{
  return 100.0 * (double)maxRGB()/0xFF;
}

// ------------------------------------------------------------

void scigraphics::color::darker( int Value )
{
  *this = color( red()-Value, green()-Value, blue()-Value, transparency() ); 
}

// ============================================================

scigraphics::color scigraphics::colorSequence::current() const
{
  if ( Sequence.empty() )
    return color();

  unsigned SequenceIndex = CurrentIndex % Sequence.size();
  return Sequence.at( SequenceIndex );
}

// ------------------------------------------------------------

scigraphics::color scigraphics::colorSequence::next()
{
  gotoNext();
  return current();
}

// ------------------------------------------------------------

void scigraphics::colorSequence::gotoNext()
{
  CurrentIndex += 1;
}

// ------------------------------------------------------------

void scigraphics::colorSequence::clear()
{
  Sequence.clear();
  reset();
}

// ------------------------------------------------------------

void scigraphics::colorSequence::reset()
{
  CurrentIndex = std::numeric_limits<unsigned>::max();
}

// ------------------------------------------------------------

scigraphics::colorSequence scigraphics::colorSequence::defaultColorSequence()
{
  colorSequence Sequence;
  
  Sequence.append( scigraphics::color::Red );
  Sequence.append( scigraphics::color::Blue );
  Sequence.append( scigraphics::color::Green );
  Sequence.append( scigraphics::color::Cyan );
  Sequence.append( scigraphics::color::Magenta );
  Sequence.append( scigraphics::color::DarkRed );
  Sequence.append( scigraphics::color::DarkBlue );
  Sequence.append( scigraphics::color::DarkGreen );
  Sequence.append( scigraphics::color::DarkMagenta );
  Sequence.append( scigraphics::color::DarkYellow );

  return Sequence;
}

// ============================================================

