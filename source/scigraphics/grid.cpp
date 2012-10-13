
// ============================================================

#include "scigraphics/grid.h"
#include <iostream>

// ============================================================

const scigraphics::lineStyle scigraphics::grid::DefaultGridLineStyle = lineStyle( scigraphics::lineStyle::Dash );

// ------------------------------------------------------------

void scigraphics::grid::draw( painter &Painter, const scale &Scale, const std::vector<number> &Ticks )
{
  drawMajorGrids( Painter, Scale, Ticks );
}

// ------------------------------------------------------------

void scigraphics::grid::drawMajorGrids( painter &Painter, const scale &Scale, const std::vector<number> &Ticks )
{
  for ( std::vector<number>::const_iterator t = Ticks.begin(); t != Ticks.end(); ++t )
    drawMajorGridOneLine( Painter, Scale, *t );
}

// ------------------------------------------------------------

void scigraphics::gridX::drawMajorGridOneLine( painter &Painter, const scale &Scale, const number Value )
{
  fcoord Coord = Scale.numberToFraction(Value);
  fpoint A( Coord, 0 );
  fpoint B( Coord, 1 );
  Painter.drawLineF( A, B, getLineStyle() );
}

// ------------------------------------------------------------

void scigraphics::gridY::drawMajorGridOneLine( painter &Painter, const scale &Scale, const number Value )
{
  fcoord Coord = Scale.numberToFraction(Value);
  fpoint A( 0, Coord );
  fpoint B( 1, Coord );
  Painter.drawLineF( A, B, getLineStyle() );
}

// ============================================================

