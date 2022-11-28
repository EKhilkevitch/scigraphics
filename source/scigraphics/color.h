
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


#pragma once
  
// ============================================================

#include <string>
#include <vector>
#include <iosfwd>

// ============================================================

namespace scigraphics
{

  // ============================================================

  class color
  {
    public:
      typedef unsigned rgb;
      static const rgb Black          = 0x000000;
      static const rgb Gray           = 0x808080;
      static const rgb White          = 0xFFFFFF;
      static const rgb Blue           = 0x0000FF;
      static const rgb Green          = 0x00FF00;
      static const rgb Red            = 0xFF0000;
      static const rgb Cyan           = 0x00FFFF;
      static const rgb Yellow         = 0xFFFF00;
      static const rgb Magenta        = 0xFF00FF;
      static const rgb DarkRed        = 0x800000;
      static const rgb DarkBlue       = 0x000080;
      static const rgb DarkGreen      = 0x008000;
      static const rgb DarkYellow     = 0x808000;
      static const rgb DarkCyan       = 0x008080;
      static const rgb DarkMagenta    = 0x800080;
      static const rgb Transparency   = 0xFF000000;

    private:
      unsigned RGB;

    public:
      static color fromRGB( unsigned R, unsigned G, unsigned B, unsigned T = 0 );
      static color fromHSV( unsigned H, unsigned S, unsigned V, unsigned T = 0 );

      color( rgb Value = Black );
      color( int R, int G, int B, int T = 0x00 );

      color& operator=( rgb Value );

      unsigned valueRgb()  const { return RGB; }
      unsigned int red()   const { return ( valueRgb() >> 16 ) & 0xFF; }
      unsigned int green() const { return ( valueRgb() >>  8 ) & 0xFF; }
      unsigned int blue()  const { return ( valueRgb() >>  0 ) & 0xFF; }
      unsigned int transparency() const { return ( valueRgb() >> 24 ) & 0xFF; }

      unsigned maxRGB() const;
      unsigned minRGB() const;

      unsigned hue() const;
      unsigned saturation() const;
      unsigned value() const;

      void setTransparency( double Part );
      std::string name( bool WithPrefix = true ) const;

      color darker( double Value ) const;
      color lighter( double Value ) const;

  };

  bool operator==( color C1, color C2 );
  bool operator!=( color C1, color C2 );

  std::ostream& operator<<( std::ostream &Stream, color Color );

  // ============================================================

  class colorSequence
  {
    private:
      std::vector< color > Sequence;
      size_t CurrentIndex;

    public:
      colorSequence();

      void clear();
      void append( color Color );

      size_t size() const;
      bool empty() const;
      color operator[]( size_t Index ) const;

      color current() const;
      color next();
      void gotoNext();
      void reset();

      static colorSequence defaultColorSequence();
  };

  // ============================================================

}

