
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

#include "scigraphics/axis.h"
#include "scigraphics/painter.h"

// ============================================================

const scigraphics::lineStyle scigraphics::axis::DefaultAxisLineStyle( 3, color::Black );

// ------------------------------------------------------------

scigraphics::axis::axis() : 
  LineStyle(DefaultAxisLineStyle)
{
}

// ------------------------------------------------------------
      
scigraphics::axis::~axis() 
{
}

// ------------------------------------------------------------
      
const scigraphics::lineStyle& scigraphics::axis::getLineStyle() const 
{ 
  return LineStyle; 
}

// ------------------------------------------------------------

void scigraphics::axis::setLineStyle( const lineStyle &S ) 
{ 
  LineStyle = S; 
}

// ============================================================
      
scigraphics::axisX::axisX( fcoord Y ) : 
  BaseY(Y) 
{
}

// ------------------------------------------------------------

void scigraphics::axisX::draw( painter &Painter ) const
{
  Painter.drawLineF( fpoint(0,BaseY), fpoint(1,BaseY), getLineStyle() );
}

// ============================================================
      
scigraphics::axisY::axisY( fcoord X ) : 
  BaseX(X) 
{
}

// ------------------------------------------------------------

void scigraphics::axisY::draw( painter &Painter ) const
{
  Painter.drawLineF( fpoint(BaseX,0), fpoint(BaseX,1), getLineStyle() );
}

// ============================================================

