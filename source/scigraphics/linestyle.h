
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

#include "scigraphics/color.h"

// ============================================================

namespace scigraphics
{

  // ============================================================

  class lineStyle
  {
    public:
      enum style
      {
        None,
        Solid,
        Dash
      };

    private:
      style Style;
      unsigned Width;
      color Color;

    protected:
      void setDefault();
    
    public:
      lineStyle(); 
      lineStyle( style Stype );
      lineStyle( unsigned Width );
      lineStyle( color Color );
      lineStyle( unsigned Width, color Color );

      unsigned width() const { return Style == None ? 0 : Width; }
      void setWidth( unsigned W ) { Width = W; }

      style getStyle() const { return Style; } 
      void setStyle( style S ) { Style = S; }

      color getColor() const { return Color; }
      void setColor( color C ) { Color = C; }
  };
  
  // ======================================================

}

