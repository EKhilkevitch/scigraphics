
// ============================================================

#include "graphics/graphviewsequence.h"
#include "graphics/painter.h"

#include <iostream>
#include <cmath>
#include <limits>
#include <cassert>

// ============================================================

namespace scigraphics 
{
  class oneCoordinateXPoints 
  {
    private:
      fcoord X;
      interval<fcoord> Y;

      static fcoord invalidValueX() { return std::numeric_limits<fcoord>::max(); }

    public:
      oneCoordinateXPoints() { clear(); }

      fpoint min() const { return fpoint( X, Y.min() ); }
      fpoint max() const { return fpoint( X, Y.max() ); }

      bool canSeparate( const painter &Painter, fpoint Point ) const { return X == invalidValueX() || Painter.canSepareteXcoordsF( X, Point.x() ); }

      void append( fpoint Point )
      {
        if ( X == std::numeric_limits<fcoord>::max() )
        {
          X = Point.x();
          Y = interval<fcoord>( Point.y(), Point.y() );
        } else {
          Y.updateInterval( Point.y() );
        }
      }

      void flush( painter &Painter, const lineStyle &Style ) const 
      {
        if ( X != invalidValueX() && !Y.isSingular() )
          Painter.drawLineF( min(), max(), Style );
      }

      void clear() { X = invalidValueX(); Y = interval<fcoord>(0,0); }
  };

  class checkIsLessThan
  {
    private:
      const pairScales &Scales;
    public:
      checkIsLessThan( const pairScales &S ) : Scales(S) {}

      inline bool operator()( const fcoord FValue, const dataSequence::point &Point ) const { return FValue < Scales.numberToFractionX(Point.x()); }
      inline bool operator()( const dataSequence::point &Point, const fcoord FValue ) const { return Scales.numberToFractionX(Point.x()) < FValue; }
  };
}

// ============================================================
      
void scigraphics::graphViewSequenceOrderOptimized::draw( painter &Painter, const pairScales& Scales, const dataSequence &Data ) const 
{
  if ( Data.isOrderedByX() ) 
  {
    drawOrderedByX( Painter, Scales, Data.begin(), Data.end() );
  } else {
    drawUnorderedByX( Painter, Scales, Data.begin(), Data.end() );
  }
}

// ------------------------------------------------------------

void scigraphics::graphViewSequenceOrderOptimized::drawOrderedByX( painter &Painter, const pairScales& Scales, dataSequence::iterator Begin, dataSequence::iterator End ) const 
{
  if ( Begin == End )
    return;

  if ( Scales.numberToFractionX((End-1)->x()) < 0 )
    return;
  if ( Scales.numberToFractionX(Begin->x()) > 1 )
    return;

  checkIsLessThan Checker(Scales);
  dataSequence::iterator BeginVisbile = Checker(0,*Begin)   ? Begin : ( std::lower_bound( Begin,        End, (fcoord)0, Checker ) - 1 );
  dataSequence::iterator EndVisible   = Checker(*(End-1),1) ? End   : ( std::upper_bound( BeginVisbile, End, (fcoord)1, Checker ) + 1 );
  drawUnorderedByX( Painter, Scales, BeginVisbile, EndVisible );
}

// ============================================================

void scigraphics::graphViewGeneralLine::drawUnorderedByX( painter &Painter, const pairScales& Scales, dataSequence::iterator Begin, dataSequence::iterator End ) const
{
  if ( getStyle().getStyle() == lineStyle::None )
    return;

  if ( Begin == End )
    return;

  oneCoordinateXPoints OneCoordinateXpoints;

  dataSequence::iterator Point1 = Begin, Point2 = Begin;
  ++Point2;

  for ( ; Point2 != End; Point1++, Point2++ )
  {
    if ( Point1->isValid() && Point2->isValid() )
    {
      fpoint Pt1 = Scales.npoint2fpoint(*Point1);
      fpoint Pt2 = Scales.npoint2fpoint(*Point2);

      if ( OneCoordinateXpoints.canSeparate(Painter,Pt2) || OneCoordinateXpoints.canSeparate(Painter,Pt1) )
      {
        OneCoordinateXpoints.flush(Painter,getStyle());
        
        drawLineBetweenPoints( Painter, Pt1, Pt2 );
        
        OneCoordinateXpoints.clear();
        OneCoordinateXpoints.append( Pt2 );
      } else {
        OneCoordinateXpoints.append( Pt1 );
        OneCoordinateXpoints.append( Pt2 );
      }

    } else {
      OneCoordinateXpoints.flush(Painter,getStyle());
      OneCoordinateXpoints.clear();
    }
  }
    
  OneCoordinateXpoints.flush(Painter,getStyle());

}

// ------------------------------------------------------------

void scigraphics::graphViewGeneralLine::drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const
{
  wcoord VCenter = ( Rectangle.up() + Rectangle.down() )/2;
  wpoint Left( Rectangle.left()+1, VCenter );
  wpoint Right( Rectangle.right()-1, VCenter );
  drawLineBetweenPoints( Painter, Painter.wpoint2fpoint(Left), Painter.wpoint2fpoint(Right) );
}

// ------------------------------------------------------------

void scigraphics::graphViewLine::drawLineBetweenPoints( painter &Painter, const fpoint Pt1, const fpoint &Pt2 ) const
{
  Painter.drawLineF( Pt1, Pt2, getStyle() );
}

// ------------------------------------------------------------

void scigraphics::graphViewPoints::drawUnorderedByX( painter &Painter, const pairScales& Scales, dataSequence::iterator Begin, dataSequence::iterator End ) const
{
  if ( getStyle().getShape() == pointStyle::None )
    return;
  for ( dataSequence::iterator Point = Begin; Point != End; ++Point )
  {
    if ( ! Point->isValid() )
      continue;
    fpoint FPoint = Scales.npoint2fpoint( npoint(*Point) );
    Painter.drawPointF( FPoint, getStyle() );
  }
}

// ------------------------------------------------------------

void scigraphics::graphViewPoints::drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const
{
  wcoord VCenter = ( Rectangle.up() + Rectangle.down() )/2;
  wcoord HCenter = ( Rectangle.left() + Rectangle.right() )/2;
  Painter.drawPointW( wpoint(HCenter,VCenter), getStyle() );
}

// ------------------------------------------------------------

void scigraphics::graphViewErrorBars::drawUnorderedByX( painter &Painter, const pairScales& Scales, dataSequence::iterator Begin, dataSequence::iterator End ) const
{
  for ( dataSequence::iterator Point = Begin; Point != End; ++Point )
  {
    if ( ! (*Point).isValid() )
      continue;

    if ( ! (*Point).isValidError() )
      continue;
 
    drawHorizontalErrorBar( Painter, Scales, *Point, Point->errX() );
    drawVerticalErrorBar( Painter, Scales, *Point, Point->errY() );
  }
}

// ------------------------------------------------------------

void scigraphics::graphViewErrorBars::drawHorizontalErrorBar( painter &Painter, const pairScales& Scales, const npoint &Point, number ErrX ) const
{
  if ( ErrX <= 0 )
    return;
  fpoint Min = Scales.npoint2fpoint(npoint( Point.x() - ErrX, Point.y() ));
  fpoint Max = Scales.npoint2fpoint(npoint( Point.x() + ErrX, Point.y() ));
  Painter.drawHorizontalErrorBarF( Min, Max, getStyle() );
}

// ------------------------------------------------------------

void scigraphics::graphViewErrorBars::drawVerticalErrorBar( painter &Painter, const pairScales& Scales, const npoint &Point, number ErrY ) const
{
  if ( ErrY <= 0 )
    return;
  fpoint Min = Scales.npoint2fpoint(npoint( Point.x(), Point.y() - ErrY ));
  fpoint Max = Scales.npoint2fpoint(npoint( Point.x(), Point.y() + ErrY ));
  Painter.drawVerticalErrorBarF( Min, Max, getStyle() );
}

// ------------------------------------------------------------

void scigraphics::graphViewLineHystogram::drawLineBetweenPoints( painter &Painter, fpoint Pt1, const fpoint &Pt2 ) const
{
  fpoint PtMiddle( Pt2.x(), Pt1.y() );
  Painter.drawLineF( Pt1, PtMiddle, getStyle() );
  Painter.drawLineF( PtMiddle, Pt2, getStyle() );
}
      
// ------------------------------------------------------------
      
scigraphics::dataSequence::iterator scigraphics::graphViewCoveredArea::fillPolygonVector( dataSequence::iterator Begin, dataSequence::iterator End, const pairScales& Scales, 
  std::vector<fpoint> *Polygon )
{
  assert( Polygon != NULL );

  Polygon->clear();
  Polygon->reserve( 1024 );

  dataSequence::iterator Point = Begin;
  while ( Point != End && Point->isValid() )
  {
    fpoint FPoint = Scales.npoint2fpoint( npoint(*Point) );
    Polygon->push_back( FPoint );
    ++Point;
  }

  if ( Point != End )
    ++Point;
  return Point;
}

// ------------------------------------------------------------

void scigraphics::graphViewCoveredArea::draw( painter &Painter, const pairScales& Scales, const dataSequence &Data ) const
{
  dataSequence::iterator Iterator = Data.begin();
  dataSequence::iterator End = Data.end();

  while ( Iterator != End )
  {
    std::vector<fpoint> Polygon;
    Iterator = fillPolygonVector( Iterator, End, Scales, &Polygon );
    if ( Polygon.size() >= 3 )
      Painter.drawPolygonF( Polygon, getStyle() );
  } 
}

// ------------------------------------------------------------

void scigraphics::graphViewCoveredArea::drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const
{
  std::vector< wpoint > Polygon;
  Polygon.push_back( Rectangle.leftUp() );
  Polygon.push_back( Rectangle.leftDown() );
  Polygon.push_back( Rectangle.rightDown() );
  Polygon.push_back( Rectangle.rightUp() );
  Painter.drawPolygonW( Polygon, getStyle() );
}
      
// ------------------------------------------------------------

void scigraphics::graphViewCoveredArea::drawUnorderedByX( painter &, const pairScales& , dataSequence::iterator , dataSequence::iterator  ) const 
{
}

// ------------------------------------------------------------

void scigraphics::graphViewCoveredArea::drawOrderedByX( painter &, const pairScales& , dataSequence::iterator , dataSequence::iterator  ) const 
{
}

// ============================================================
 
