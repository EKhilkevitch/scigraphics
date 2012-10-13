
// ============================================================

#include "scigraphics/zoomrect.h"
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

