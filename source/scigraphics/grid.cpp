
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

#include "scigraphics/grid.h"
#include "scigraphics/painter.h"
#include "scigraphics/scale.h"

#include <iostream>

// ============================================================

const scigraphics::lineStyle scigraphics::grid::DefaultGridLineStyle = lineStyle( scigraphics::lineStyle::Dash );

// ------------------------------------------------------------

scigraphics::grid::grid() : 
  LineStyle(DefaultGridLineStyle) 
{
}

// ------------------------------------------------------------

scigraphics::grid::~grid() 
{
}

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

