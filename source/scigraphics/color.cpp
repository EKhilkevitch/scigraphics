
/*
 * Copyright 2011,2012 Evgeniy Khilkevitch 
 * 
 * This file is part of scigraphics.
 * 
 * Scigraphics is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Scigraphics is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with scigraphics.  If not, see <http://www.gnu.org/licenses/>.
 */


// ============================================================

#if _MSC_VER
#  pragma warning( disable : 4244 )
#  define _CRT_SECURE_NO_WARNINGS 1
#endif

#include "scigraphics/color.h"
#include "scigraphics/interval.h"

#include <algorithm>
#include <limits>
#include <cstdlib>
#include <cstdio>

// ============================================================
        
scigraphics::color::color( rgb Value ) : 
  RGB(Value) 
{
}

// ------------------------------------------------------------

scigraphics::color::color( const color &Color ) : 
  RGB( Color.RGB ) 
{
}

// ------------------------------------------------------------

scigraphics::color::color( int R, int G, int B, int T )
{
  interval<int> Lims(0,0xFF); 
  RGB = ( Lims.toInterval(T) << 24 ) |
        ( Lims.toInterval(R) << 16 ) | 
        ( Lims.toInterval(G) << 8  ) | 
        ( Lims.toInterval(B) << 0  );
}
        
// ------------------------------------------------------------

scigraphics::color& scigraphics::color::operator=( rgb Value ) 
{ 
  RGB = Value; 
  return *this; 
} 

// ------------------------------------------------------------

scigraphics::color& scigraphics::color::operator=( const color &Color ) 
{ 
  RGB = Color.RGB; 
  return *this; 
}

// ------------------------------------------------------------

void scigraphics::color::setTransparency( double Part ) 
{ 
  if ( Part < 0 ) return setTransparency(0); 
  else if ( Part > 1 ) return setTransparency(1);
  else RGB = ( RGB & 0x00FFFFFF ) | ( static_cast<unsigned char>( Part * 0xFF ) << 24 );
}

// ------------------------------------------------------------

std::string scigraphics::color::name( bool WithPrefix ) const 
{ 
#if _WIN32 && _MSC_VER
  int (__cdecl *snprintf)( char *buffer, size_t count, const char *format, ... ) = _snprintf;
#endif

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
  double Veff = static_cast<double>(V);

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
    return 60.0 * ( static_cast<double>(green()) - blue() )/( Max - Min ) + 0;
  else if ( Max == red() && green() < blue() )
    return 60.0 * ( static_cast<double>(green()) - blue() )/( Max - Min ) + 360;
  else if ( Max == green() )
    return 60.0 * ( static_cast<double>(blue()) - red() )/( Max - Min ) + 120;
  else if ( Max == blue() )
    return 60.0 * ( static_cast<double>(red()) - green() )/( Max - Min ) + 240;
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
  return 100.0 * ( 1.0 - static_cast<double>(Min)/Max );
}

// ------------------------------------------------------------

unsigned scigraphics::color::value() const
{
  return 100.0 * static_cast<double>( maxRGB() )/0xFF;
}

// ------------------------------------------------------------

scigraphics::color scigraphics::color::darker( double Value ) const
{
  Value *= 100.0;
  return color( red()-Value, green()-Value, blue()-Value, transparency() ); 
}

// ------------------------------------------------------------
        
scigraphics::color scigraphics::color::lighter( double Value ) const
{
  return darker( -Value ); 
}

// ------------------------------------------------------------
    
bool scigraphics::operator==( color C1, color C2 ) 
{ 
  return C1.valueRgb() == C2.valueRgb(); 
}

// ------------------------------------------------------------

bool scigraphics::operator!=( color C1, color C2 ) 
{ 
  return ! ( C1 == C2 ); 
}

// ------------------------------------------------------------
    
std::ostream& scigraphics::operator<<( std::ostream &Stream, color Color )
{
  Stream << Color.name();
  return Stream;
}

// ============================================================

scigraphics::colorSequence::colorSequence() 
{
  reset();
}

// ------------------------------------------------------------

void scigraphics::colorSequence::append( color Color ) 
{ 
  Sequence.push_back(Color); 
}

// ------------------------------------------------------------

scigraphics::color scigraphics::colorSequence::current() const
{
  if ( Sequence.empty() )
    return color();

  size_t SequenceIndex = CurrentIndex % Sequence.size();
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
  CurrentIndex = std::numeric_limits<size_t>::max();
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

