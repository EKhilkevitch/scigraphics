
// ============================================================

#include "graphics/zoomrect.h"
#include "graphics/brushstyle.h"
#include "graphics/linestyle.h"

#include <iostream>

// ============================================================

graphics::zoomRectangle::zoomRectangle() :
  InitPoint(0,0),
  CursorPoint(0,0),
  Visible(false)
{
}
      
// ------------------------------------------------------------

void graphics::zoomRectangle::show( wpoint Point )
{
  InitPoint = CursorPoint = Point;
  Visible = true;
}

// ------------------------------------------------------------

void graphics::zoomRectangle::pull( wpoint Point )
{
  if ( ! Visible )
    show(Point);
  CursorPoint = Point;
}

// ------------------------------------------------------------

void graphics::zoomRectangle::hide()
{
  Visible = false;
}

// ------------------------------------------------------------

void graphics::zoomRectangle::draw( painter &Painter )
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

