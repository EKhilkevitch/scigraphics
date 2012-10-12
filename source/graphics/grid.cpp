
// ============================================================

#include "graphics/grid.h"
#include <iostream>

// ============================================================

const graphics::lineStyle graphics::grid::DefaultGridLineStyle = lineStyle( graphics::lineStyle::Dash );

// ------------------------------------------------------------

void graphics::grid::draw( painter &Painter, const scale &Scale, const std::vector<number> &Ticks )
{
  drawMajorGrids( Painter, Scale, Ticks );
}

// ------------------------------------------------------------

void graphics::grid::drawMajorGrids( painter &Painter, const scale &Scale, const std::vector<number> &Ticks )
{
  for ( std::vector<number>::const_iterator t = Ticks.begin(); t != Ticks.end(); ++t )
    drawMajorGridOneLine( Painter, Scale, *t );
}

// ------------------------------------------------------------

void graphics::gridX::drawMajorGridOneLine( painter &Painter, const scale &Scale, const number Value )
{
  fcoord Coord = Scale.numberToFraction(Value);
  fpoint A( Coord, 0 );
  fpoint B( Coord, 1 );
  Painter.drawLineF( A, B, getLineStyle() );
}

// ------------------------------------------------------------

void graphics::gridY::drawMajorGridOneLine( painter &Painter, const scale &Scale, const number Value )
{
  fcoord Coord = Scale.numberToFraction(Value);
  fpoint A( 0, Coord );
  fpoint B( 1, Coord );
  Painter.drawLineF( A, B, getLineStyle() );
}

// ============================================================

