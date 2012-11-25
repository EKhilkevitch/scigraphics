
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

namespace scigraphics
{

// ============================================================

  class selectionStyle
  {
    private:
      color BrushColor;
      color LineColor;

    public:
      selectionStyle( color BC = color::Yellow, color LC = color::Magenta ) : 
        BrushColor(BC), 
        LineColor(LC) 
      { 
        BrushColor.setTransparency(0.5);
      }

      const color& getBrushColor() const { return BrushColor; }
      void setBrushColor( color C ) { BrushColor = C; }

      const color& getLineColor() const { return LineColor; }
      void setLineColor( color C ) { LineColor = C; }
  };
  

// ======================================================

}

