
// ============================================================

#include "scigraphics/axisticks.h"

#include <cstdio>
#include <iostream>
#include <cmath>

// ============================================================

const scigraphics::lineStyle scigraphics::axisTicks::DefaultAxisTickStyle( 3 );

// ------------------------------------------------------------

void scigraphics::axisTicks::draw( painter &Painter, const scale &Scale, const std::vector<number> &Ticks )
{
  for ( std::vector<number>::const_iterator t = Ticks.begin(); t != Ticks.end(); ++t )
    drawOneTick( Painter, Scale, *t );
}

// ------------------------------------------------------------

void scigraphics::axisTicksX::drawOneTick( painter &Painter, const scale &Scale, number Value )
{
  double FLength = std::fabs( Painter.wcoord2fcoordY( getTickLength() ) - Painter.wcoord2fcoordY( 0 ) );

  fcoord X  = Scale.numberToFraction( Value );
  fcoord Y0 = BaseY;
  fcoord Y1 = Y0 + ( BaseY < 0.5 ? +1 : -1 ) * FLength;

  Painter.drawLineF( fpoint(X,Y0), fpoint(X,Y1), getTickStyle() );
}

// ------------------------------------------------------------

void scigraphics::axisTicksY::drawOneTick( painter &Painter, const scale &Scale, number Value )
{
  double FLength = std::fabs( Painter.wcoord2fcoordX( getTickLength() ) - Painter.wcoord2fcoordX( 0 ) );

  fcoord Y  = Scale.numberToFraction( Value );
  fcoord X0 = BaseX;
  fcoord X1 = X0 + ( BaseX < 0.5 ? +1 : -1 ) * FLength;

  Painter.drawLineF( fpoint(X0,Y), fpoint(X1,Y), getTickStyle() );
}

// ============================================================

