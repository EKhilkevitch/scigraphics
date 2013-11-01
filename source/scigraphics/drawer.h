
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

#include "scigraphics/numbers.h"
#include "scigraphics/geometry.h"

#include <string>
#include <vector>

// ============================================================

namespace scigraphics
{

// ============================================================

  class lineStyle;
  class brushStyle;
  class textStyle;
  class color;

// ============================================================


  class drawer
  {
    protected:
      static unsigned numOfLinesInString( const std::string &String );

    public:
      virtual wcoord width() const  = 0;
      virtual wcoord height() const = 0;
      wrectangle plotRectangle() const { return wrectangle( wpoint(0,0), wpoint(width(),height()) ); }

      virtual void eraseAll() { eraseRectangle( plotRectangle() ); }

      virtual void setLineStyle( const lineStyle &Style ) = 0;
      virtual void setBrushStyle( const brushStyle &Style ) = 0;
      virtual void setTextStyle( const textStyle &Style ) = 0;
      
      virtual void drawLine( const wpoint &A, const wpoint &B ) = 0;
      virtual void drawRectangle( const wrectangle& Rectangle ) = 0;
      virtual void drawPolygon( const std::vector<wpoint> &Points ) = 0;
      virtual void drawText( const std::string &Text, const wrectangle& Rectangle, double RotAngle = 0 ) = 0;
      virtual void eraseRectangle( const wrectangle& Rectangle );
      virtual void flush() {}

      virtual wcoord textWidth( const std::string &Text, const textStyle &Style );
      virtual wcoord textHeight( const std::string &Text, const textStyle &Style );
      
      virtual ~drawer() {}
  };

// ============================================================

}

