
// ============================================================

#include "scigraphics/axis.h"

// ============================================================

const scigraphics::lineStyle scigraphics::axis::DefaultAxisLineStyle( 3, color::Black );

// ------------------------------------------------------------

void scigraphics::axisX::draw( painter &Painter )
{
  Painter.drawLineF( fpoint(0,BaseY), fpoint(1,BaseY), getLineStyle() );
}

// ------------------------------------------------------------

void scigraphics::axisY::draw( painter &Painter )
{
  Painter.drawLineF( fpoint(BaseX,0), fpoint(BaseX,1), getLineStyle() );
}

// ============================================================

