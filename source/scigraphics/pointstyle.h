
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

#include "scigraphics/color.h"

#include <cstdio>
#include <string>

namespace scigraphics
{

// ============================================================

  class pointStyle
  {
    public:
      enum shape
      {
        None,
        Square,
        Cyrcle
      };

    private:
      shape Shape;
      unsigned Width;
      color Color;

    public:

      pointStyle( const color& C = color::Black ) : Shape(Square), Width(4), Color(C) {}

      shape getShape() const { return Shape; } 
      void setShape( shape S ) { Shape = S; }

      unsigned width()   const { return Width; }
      void setWidth( unsigned W ) { Width = W; }

      const color& getColor() const { return Color; }
      void setColor( color C ) { Color = C; }
  };
  
// ======================================================

}

