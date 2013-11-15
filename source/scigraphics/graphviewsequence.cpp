
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

#include "scigraphics/graphviewsequence.h"
#include "scigraphics/painter.h"
#include "scigraphics/scale.h"

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

      inline bool operator()( const fcoord FValue, const sequence::data::point_t &Point ) const { return FValue < Scales.numberToFractionX(Point.x()); }
      inline bool operator()( const sequence::data::point_t &Point, const fcoord FValue ) const { return Scales.numberToFractionX(Point.x()) < FValue; }
  };
}

// ============================================================
      
void scigraphics::sequence::graphViewOrdered::draw( painter &Painter, const pairScales& Scales, const sequence::data &Data ) const 
{
  if ( Data.isOrderedByX() ) 
  {
    drawOrderedByX( Painter, Scales, Data.begin(), Data.end() );
  } else {
    drawUnorderedByX( Painter, Scales, Data.begin(), Data.end() );
  }
}

// ------------------------------------------------------------

void scigraphics::sequence::graphViewOrdered::drawOrderedByX( painter &Painter, const pairScales& Scales, sequence::data::iterator Begin, sequence::data::iterator End ) const 
{
  if ( Begin == End )
    return;

  if ( Scales.numberToFractionX((End-1)->x()) < 0 )
    return;
  if ( Scales.numberToFractionX(Begin->x()) > 1 )
    return;

  checkIsLessThan Checker(Scales);
  sequence::data::iterator BeginVisbile = Checker(0,*Begin)   ? Begin : std::lower_bound( Begin,        End, (fcoord)0, Checker );
  if ( BeginVisbile != Begin )
    --BeginVisbile;
  sequence::data::iterator EndVisible   = Checker(*(End-1),1) ? End   : std::upper_bound( BeginVisbile, End, (fcoord)1, Checker );
  if ( EndVisible != End )
    ++EndVisible;
  drawUnorderedByX( Painter, Scales, BeginVisbile, EndVisible );
}

// ============================================================

void scigraphics::sequence::graphViewGeneralLine::drawUnorderedByX( painter &Painter, const pairScales& Scales, sequence::data::iterator Begin, sequence::data::iterator End ) const
{
  if ( getStyle().getStyle() == lineStyle::None )
    return;

  if ( Begin == End )
    return;

  Painter.setLineStyle( getStyle() );
  
  oneCoordinateXPoints OneCoordinateXpoints;

  sequence::data::iterator Point1 = Begin, Point2 = Begin;
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

void scigraphics::sequence::graphViewGeneralLine::drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const
{
  wcoord VCenter = ( Rectangle.up() + Rectangle.down() )/2;
  wpoint Left( Rectangle.left()+1, VCenter );
  wpoint Right( Rectangle.right()-1, VCenter );
  Painter.setLineStyle( getStyle() );
  drawLineBetweenPoints( Painter, Painter.wpoint2fpoint(Left), Painter.wpoint2fpoint(Right) );
}

// ------------------------------------------------------------

void scigraphics::sequence::graphViewLine::drawLineBetweenPoints( painter &Painter, const fpoint Pt1, const fpoint &Pt2 ) const
{
  Painter.drawLineF( Pt1, Pt2 );
}

// ------------------------------------------------------------

void scigraphics::sequence::graphViewPoints::drawUnorderedByX( painter &Painter, const pairScales& Scales, sequence::data::iterator Begin, sequence::data::iterator End ) const
{
  if ( getStyle().getShape() == pointStyle::None )
    return;
  for ( sequence::data::iterator Point = Begin; Point != End; ++Point )
  {
    if ( ! Point->isValid() )
      continue;
    fpoint FPoint = Scales.npoint2fpoint( npoint(*Point) );
    Painter.drawPointF( FPoint, getStyle() );
  }
}

// ------------------------------------------------------------

void scigraphics::sequence::graphViewPoints::drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const
{
  wcoord VCenter = ( Rectangle.up() + Rectangle.down() )/2;
  wcoord HCenter = ( Rectangle.left() + Rectangle.right() )/2;
  Painter.drawPointW( wpoint(HCenter,VCenter), getStyle() );
}

// ------------------------------------------------------------

void scigraphics::sequence::graphViewErrorBars::drawUnorderedByX( painter &Painter, const pairScales& Scales, sequence::data::iterator Begin, sequence::data::iterator End ) const
{
  for ( sequence::data::iterator Point = Begin; Point != End; ++Point )
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

void scigraphics::sequence::graphViewErrorBars::drawHorizontalErrorBar( painter &Painter, const pairScales& Scales, const npoint &Point, number ErrX ) const
{
  if ( ErrX <= 0 )
    return;
  fpoint Min = Scales.npoint2fpoint(npoint( Point.x() - ErrX, Point.y() ));
  fpoint Max = Scales.npoint2fpoint(npoint( Point.x() + ErrX, Point.y() ));
  Painter.drawHorizontalErrorBarF( Min, Max, getStyle() );
}

// ------------------------------------------------------------

void scigraphics::sequence::graphViewErrorBars::drawVerticalErrorBar( painter &Painter, const pairScales& Scales, const npoint &Point, number ErrY ) const
{
  if ( ErrY <= 0 )
    return;
  fpoint Min = Scales.npoint2fpoint(npoint( Point.x(), Point.y() - ErrY ));
  fpoint Max = Scales.npoint2fpoint(npoint( Point.x(), Point.y() + ErrY ));
  Painter.drawVerticalErrorBarF( Min, Max, getStyle() );
}

// ------------------------------------------------------------

void scigraphics::sequence::graphViewLineHystogram::drawLineBetweenPoints( painter &Painter, fpoint Pt1, const fpoint &Pt2 ) const
{
  fpoint PtMiddle( Pt2.x(), Pt1.y() );
  Painter.drawLineF( Pt1, PtMiddle );
  Painter.drawLineF( PtMiddle, Pt2 );
}
      
// ------------------------------------------------------------
      
scigraphics::sequence::data::iterator scigraphics::sequence::graphViewCoveredArea::fillPolygonVector( sequence::data::iterator Begin, sequence::data::iterator End, const pairScales& Scales, 
  std::vector<fpoint> *Polygon )
{
  assert( Polygon != NULL );

  Polygon->clear();
  Polygon->reserve( 1024 );

  sequence::data::iterator Point = Begin;
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

void scigraphics::sequence::graphViewCoveredArea::draw( painter &Painter, const pairScales& Scales, const sequence::data &Data ) const
{
  sequence::data::iterator Iterator = Data.begin();
  sequence::data::iterator End = Data.end();

  while ( Iterator != End )
  {
    std::vector<fpoint> Polygon;
    Iterator = fillPolygonVector( Iterator, End, Scales, &Polygon );
    if ( Polygon.size() >= 3 )
      Painter.drawPolygonF( Polygon, getStyle() );
  } 
}

// ------------------------------------------------------------

void scigraphics::sequence::graphViewCoveredArea::drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const
{
  std::vector< wpoint > Polygon;
  Polygon.push_back( Rectangle.leftUp() );
  Polygon.push_back( Rectangle.leftDown() );
  Polygon.push_back( Rectangle.rightDown() );
  Polygon.push_back( Rectangle.rightUp() );
  Painter.drawPolygonW( Polygon, getStyle() );
}

// ============================================================
 
