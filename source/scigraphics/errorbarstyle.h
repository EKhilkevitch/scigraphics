
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
#include "scigraphics/linestyle.h"

namespace scigraphics
{

// ============================================================

  class errorBarStyle
  {
    private:
      color Color;
      int HatWidth;
      int LineWidth;

    public:
      errorBarStyle( color C = color::Black ) : Color(C), HatWidth(8), LineWidth(1) {}

      int hatWidth() const { return HatWidth; }
      int lineWidth() const { return LineWidth; }

      const color& getColor() const { return Color; }
      void setColor( color C ) { Color = C; } 

      lineStyle getLineStyle() const { return lineStyle( lineWidth(), getColor() ); }
  };
  
// ======================================================

}

