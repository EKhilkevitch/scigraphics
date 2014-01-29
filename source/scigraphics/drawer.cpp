
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

#include "scigraphics/drawer.h"
#include "scigraphics/color.h"
#include "scigraphics/linestyle.h"
#include "scigraphics/brushstyle.h"
#include "scigraphics/textstyle.h"

#include <cassert>

// ============================================================

unsigned scigraphics::drawer::numOfLinesInString( const std::string &String )
{
  unsigned Count = 0;
  for ( std::string::const_iterator c = String.begin(); c != String.end(); ++c ) 
    if ( *c == '\n' )
      Count++;
  return Count + 1;
}

// ------------------------------------------------------------

scigraphics::wrectangle scigraphics::drawer::plotRectangle() const 
{ 
  return wrectangle( wpoint(0,0), wpoint(width(),height()) ); 
}

// ------------------------------------------------------------
      
void scigraphics::drawer::eraseAll() 
{ 
  eraseRectangle( plotRectangle() ); 
}

// ------------------------------------------------------------

void scigraphics::drawer::eraseRectangle( const wrectangle& Rectangle )
{
  color Color = color::White;
  brushStyle BrushStyle( Color );
  lineStyle LineStyle( Color );

  setBrushStyle( BrushStyle );
  setLineStyle( LineStyle );
  drawRectangle( Rectangle ); 
}

// ------------------------------------------------------------
      
scigraphics::wcoord scigraphics::drawer::textWidth( const std::string &Text, const textStyle &Style )       
{ 
  return Text.length() * Style.getFontSize(); 
}

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::drawer::textHeight( const std::string &Text, const textStyle &Style )
{ 
  return Style.getFontSize() * numOfLinesInString(Text); 
}

// ============================================================
      

