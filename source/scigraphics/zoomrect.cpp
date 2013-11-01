
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

#include "scigraphics/zoomrect.h"
#include "scigraphics/painter.h"
#include "scigraphics/brushstyle.h"
#include "scigraphics/linestyle.h"

#include <iostream>

// ============================================================

scigraphics::zoomRectangle::zoomRectangle() :
  InitPoint(0,0),
  CursorPoint(0,0),
  Visible(false)
{
}
      
// ------------------------------------------------------------

void scigraphics::zoomRectangle::show( wpoint Point )
{
  InitPoint = CursorPoint = Point;
  Visible = true;
}

// ------------------------------------------------------------

void scigraphics::zoomRectangle::pull( wpoint Point )
{
  if ( ! Visible )
    show(Point);
  CursorPoint = Point;
}

// ------------------------------------------------------------

void scigraphics::zoomRectangle::hide()
{
  Visible = false;
}

// ------------------------------------------------------------

void scigraphics::zoomRectangle::draw( painter &Painter )
{
  if ( Visible )
  {
    brushStyle BrushStyle( color::Black,brushStyle::None );
    lineStyle LineStyle( color::Black );
    wrectangle Rectangle( InitPoint, CursorPoint );

    Painter.drawRectangleW( Rectangle, BrushStyle, LineStyle );
  }
}

// ============================================================

