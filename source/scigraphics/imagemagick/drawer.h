
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

#include "scigraphics/drawer.h"

#include <vector>
#include <list>

// ================================================================

namespace Magick 
{
  class Image;
  class Color;
  class Coordinate;
};

// ================================================================

namespace scigraphics
{
  
  // ================================================================

  class imdrawer : public scigraphics::drawer
  {
    private:
      Magick::Image *Image;

      lineStyle *LineStyle;
      brushStyle *BrushStyle;
      textStyle *TextStyle;

      enum 
      {
        LastNoneStyle,
        LastLineStyle,
        LastBrushStyle,
        LastTextStyle,
      } LastStyleUsed;

    private:
      imdrawer( const imdrawer& );
      imdrawer& operator=( const imdrawer& );

      static Magick::Color colorIm( color Color );
      static Magick::Coordinate coordinateIm( const wpoint &Point );
      static std::list< Magick::Coordinate > polygonIm( const std::vector<wpoint> &Points );

      void applyCurrentLineStyle();
      void applyCurrentBrushStyle();
      void applyCurrentTextStyle();

    public:
      imdrawer( size_t SizeX, size_t SizeY );
      ~imdrawer();
      
      void setLineStyle( const lineStyle &Style );
      void setBrushStyle( const brushStyle &Style );
      void setTextStyle( const textStyle &Style );

      void drawLine( const wpoint &A, const wpoint &B );
      void drawRectangle( const wrectangle& Rectangle );
      void drawPolygon( const std::vector<wpoint> &Points );
      void drawText( const std::string &Text, const wrectangle& Rectangle, double RotAngle );
      
      wcoord textWidth( const std::string &Text, const textStyle &Style );
      wcoord textHeight( const std::string &Text, const textStyle &Style );

      wcoord width()  const;
      wcoord height() const;

      void write( const std::string &FileName );
      void display();
  };

  // ============================================================

}

