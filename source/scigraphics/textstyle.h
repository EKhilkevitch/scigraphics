
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
#include <string>

// ============================================================

namespace scigraphics
{

// ============================================================

  class textStyle 
  {
    private:
      std::string Font;
      unsigned FontSize;
      color Color;

    public:
      textStyle( unsigned FS = 12 ) : Font(), FontSize(FS), Color() {}

      const color& getColor() const { return Color; }
      void setColor( color C ) { Color = C; }

      unsigned getFontSize() const { return FontSize; }
      void setFontSize( unsigned S ) { FontSize = S; }

      void setFontName( const std::string &F ) { Font = F; }
      const std::string& getFontName() const { return Font; }
  };
  
// ======================================================

}

