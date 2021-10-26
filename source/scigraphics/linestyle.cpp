
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

#include "scigraphics/linestyle.h"

// ============================================================

void scigraphics::lineStyle::setDefault() 
{ 
  Style = Solid;
  Width = 1;
  Color = color::Black;
}

// ------------------------------------------------------------
      
scigraphics::lineStyle::lineStyle() 
{ 
  setDefault(); 
}

// ------------------------------------------------------------

scigraphics::lineStyle::lineStyle( style S ) 
{ 
  setDefault(); 
  Style = S; 
}

// ------------------------------------------------------------

scigraphics::lineStyle::lineStyle( unsigned W ) 
{ 
  setDefault(); 
  Width = W; 
}

// ------------------------------------------------------------

scigraphics::lineStyle::lineStyle( color C )    
{ 
  setDefault(); 
  Color = C; 
}

// ------------------------------------------------------------

scigraphics::lineStyle::lineStyle( unsigned W, color C ) 
{ 
  setDefault(); 
  Color = C; 
  Width = W; 
}

// ------------------------------------------------------------
      
unsigned scigraphics::lineStyle::width() const 
{ 
  if ( Style == None )
    return 0;
  return Width;
}

// ------------------------------------------------------------

void scigraphics::lineStyle::setWidth( unsigned W ) 
{ 
  Width = W; 
}

// ------------------------------------------------------------

scigraphics::lineStyle::style scigraphics::lineStyle::getStyle() const 
{ 
  return Style; 
} 

// ------------------------------------------------------------

void scigraphics::lineStyle::setStyle( style S ) 
{ 
  Style = S; 
}

// ------------------------------------------------------------

scigraphics::color scigraphics::lineStyle::getColor() const 
{ 
  return Color; 
}

// ------------------------------------------------------------

void scigraphics::lineStyle::setColor( color C ) 
{ 
  Color = C; 
}

// ============================================================

