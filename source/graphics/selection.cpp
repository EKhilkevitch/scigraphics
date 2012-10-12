
// ============================================================

#include "graphics/selection.h"

// ============================================================

graphics::fpoint graphics::selection::firstCornerF( const pairScales& Scales ) const
{
  return Scales.npoint2fpoint( firstCorner() );
}

// ------------------------------------------------------------

graphics::fpoint graphics::selection::secondCornerF( const pairScales& Scales ) const
{
  return Scales.npoint2fpoint( secondCorner() );
}

// ------------------------------------------------------------
      
graphics::frectangle graphics::selection::getRectangleF( const pairScales& Scales ) const
{ 
  return frectangle( firstCornerF(Scales), secondCornerF(Scales) ); 
}

// ------------------------------------------------------------

void graphics::selection::draw( painter &Painter, const pairScales& Scales ) const
{
  frectangle Rectangle = getRectangleF(Scales);

  brushStyle BrushStyle = SelectionStyle.getBrushColor();
  lineStyle  LineStyle  = SelectionStyle.getLineColor();
  
  Painter.drawRectangleF( Rectangle, BrushStyle, LineStyle );
}
      
// ------------------------------------------------------------

bool graphics::selection::pointInSelection( const fpoint &Point, const pairScales& Scales ) const
{ 
  return getRectangleF(Scales).contain( Point );
}

// ============================================================
      
void graphics::selectionStrip::setIntervalPointsF( const fpoint &A, const fpoint &B, const pairScales &Scales )
{
  npoint NA = Scales.fpoint2npoint( A );
  npoint NB = Scales.fpoint2npoint( B );
  setIntervalPoints( NA, NB );
}

// ------------------------------------------------------------
      
void graphics::selectionStrip::shiftPointsF( fpoint From, fpoint To, const pairScales &Scales )
{
  npoint NFrom = Scales.fpoint2npoint( From );
  npoint NTo   = Scales.fpoint2npoint( To );
  shiftPoints( NFrom, NTo );
}

// ------------------------------------------------------------
      
void graphics::selectionStrip::shiftPoints( npoint From, npoint To )
{
  number DeltaX = To.x() - From.x();
  number DeltaY = To.y() - From.y();
  shift( DeltaX, DeltaY );
}

// ============================================================
      

