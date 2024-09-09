
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

#include "scigraphics/axisticks.h"
#include "scigraphics/scale.h"
#include "scigraphics/painter.h"

#include <iostream>
#include <cmath>

// ============================================================

const scigraphics::lineStyle scigraphics::axisTicks::DefaultAxisTickStyle( 3 );
      
// ------------------------------------------------------------

scigraphics::axisTicks::axisTicks() : 
  TickStyle(DefaultAxisTickStyle), 
  Length(8) 
{
} 

// ------------------------------------------------------------

scigraphics::axisTicks::~axisTicks() 
{
}

// ------------------------------------------------------------

const scigraphics::lineStyle& scigraphics::axisTicks::getTickStyle() const 
{ 
  return TickStyle; 
}

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::axisTicks::getTickLength() const 
{ 
  return Length; 
}

// ------------------------------------------------------------

void scigraphics::axisTicks::draw( painter &Painter, const scale &Scale, const std::vector<number> &Ticks ) const
{
  for ( std::vector<number>::const_iterator t = Ticks.begin(); t != Ticks.end(); ++t )
    drawOneTick( Painter, Scale, *t );
}

// ============================================================
      
scigraphics::axisTicksX::axisTicksX( fcoord Y ) : 
  BaseY(Y) 
{
}

// ------------------------------------------------------------

void scigraphics::axisTicksX::drawOneTick( painter &Painter, const scale &Scale, number Value ) const
{
  const fcoord FLength = std::fabs( Painter.wcoord2fcoordY( getTickLength() ) - Painter.wcoord2fcoordY( 0 ) );

  const fcoord X  = Scale.numberToFraction( Value );
  const fcoord Y0 = BaseY;
  const fcoord Y1 = Y0 + ( BaseY < static_cast<fcoord>(0.5) ? +1 : -1 ) * FLength;

  Painter.drawLineF( fpoint(X,Y0), fpoint(X,Y1), getTickStyle() );
}

// ============================================================

scigraphics::axisTicksY::axisTicksY( fcoord X ) : 
  BaseX(X) 
{
}

// ------------------------------------------------------------

void scigraphics::axisTicksY::drawOneTick( painter &Painter, const scale &Scale, number Value ) const
{
  const fcoord FLength = std::fabs( Painter.wcoord2fcoordX( getTickLength() ) - Painter.wcoord2fcoordX( 0 ) );

  const fcoord Y  = Scale.numberToFraction( Value );
  const fcoord X0 = BaseX;
  const fcoord X1 = X0 + ( BaseX < static_cast<fcoord>(0.5) ? +1 : -1 ) * FLength;

  Painter.drawLineF( fpoint(X0,Y), fpoint(X1,Y), getTickStyle() );
}

// ============================================================

