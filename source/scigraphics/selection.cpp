
// ============================================================

#include "scigraphics/selection.h"

// ============================================================

scigraphics::fpoint scigraphics::selection::firstCornerF( const pairScales& Scales ) const
{
  return Scales.npoint2fpoint( firstCorner() );
}

// ------------------------------------------------------------

scigraphics::fpoint scigraphics::selection::secondCornerF( const pairScales& Scales ) const
{
  return Scales.npoint2fpoint( secondCorner() );
}

// ------------------------------------------------------------
      
scigraphics::frectangle scigraphics::selection::getRectangleF( const pairScales& Scales ) const
{ 
  return frectangle( firstCornerF(Scales), secondCornerF(Scales) ); 
}

// ------------------------------------------------------------

void scigraphics::selection::draw( painter &Painter, const pairScales& Scales ) const
{
  frectangle Rectangle = getRectangleF(Scales);

  brushStyle BrushStyle = SelectionStyle.getBrushColor();
  lineStyle  LineStyle  = SelectionStyle.getLineColor();
  
  Painter.drawRectangleF( Rectangle, BrushStyle, LineStyle );
}
      
// ------------------------------------------------------------

bool scigraphics::selection::pointInSelection( const fpoint &Point, const pairScales& Scales ) const
{ 
  return getRectangleF(Scales).contain( Point );
}

// ============================================================
      
void scigraphics::selectionStrip::setIntervalPointsF( const fpoint &A, const fpoint &B, const pairScales &Scales )
{
  npoint NA = Scales.fpoint2npoint( A );
  npoint NB = Scales.fpoint2npoint( B );
  setIntervalPoints( NA, NB );
}

// ------------------------------------------------------------
      
void scigraphics::selectionStrip::shiftPointsF( fpoint From, fpoint To, const pairScales &Scales )
{
  npoint NFrom = Scales.fpoint2npoint( From );
  npoint NTo   = Scales.fpoint2npoint( To );
  shiftPoints( NFrom, NTo );
}

// ------------------------------------------------------------
      
void scigraphics::selectionStrip::shiftPoints( npoint From, npoint To )
{
  number DeltaX = To.x() - From.x();
  number DeltaY = To.y() - From.y();
  shift( DeltaX, DeltaY );
}

// ============================================================
      

