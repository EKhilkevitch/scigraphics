
// ============================================================

#include "graphics/axis.h"

// ============================================================

const graphics::lineStyle graphics::axis::DefaultAxisLineStyle( 3, color::Black );

// ------------------------------------------------------------

void graphics::axisX::draw( painter &Painter )
{
  Painter.drawLineF( fpoint(0,BaseY), fpoint(1,BaseY), getLineStyle() );
}

// ------------------------------------------------------------

void graphics::axisY::draw( painter &Painter )
{
  Painter.drawLineF( fpoint(BaseX,0), fpoint(BaseX,1), getLineStyle() );
}

// ============================================================

