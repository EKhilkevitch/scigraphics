
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
#include <algorithm>
#include <cassert>

// ============================================================

namespace scigraphics 
{
  
  // ============================================================
  
  class pointsWithSameXCoord
  {
    private:
      wcoord X;
      wcoord MinY, MaxY, FirstY, LastY;
      size_t Count;

    public:
      pointsWithSameXCoord();
      void clear();

      wpoint min()  const { return wpoint( X, MinY );  }
      wpoint max()  const { return wpoint( X, MaxY );  }
      wpoint last() const { return wpoint( X, LastY ); }

      bool canSeparate( const painter &Painter, fpoint Point );
      void append( const painter &Painter, fpoint Point );
      void addToPolyline( std::vector<wpoint> *Polyline ) const;
  };

  // ============================================================

  class checkIsLessThan
  {
    private:
      const pairScales &Scales;
    public:
      explicit checkIsLessThan( const pairScales &Scales );
      bool operator()( const fcoord FValue, const sequence::data::point_t &Point ) const;
      bool operator()( const sequence::data::point_t &Point, const fcoord FValue ) const;
  };

  // ============================================================
  
  pointsWithSameXCoord::pointsWithSameXCoord() 
  {
    clear();
  }

  // ------------------------------------------------------------
  
  void pointsWithSameXCoord::clear()
  {
    X = 0; 
    MinY = MaxY = FirstY = LastY = 0;
    Count = 0;
  }
  
  // ------------------------------------------------------------
  
  bool pointsWithSameXCoord::canSeparate( const painter &Painter, fpoint Point )
  {
    if ( Count == 0 )
      return true;
    return X != Painter.fcoord2wcoordX( Point.x() );
  }
  
  // ------------------------------------------------------------
      
  void pointsWithSameXCoord::append( const painter &Painter, fpoint Point )
  {
    LastY = Painter.fcoord2wcoordY( Point.y() );
    if ( Count == 0 )
    {
      X = Painter.fcoord2wcoordX( Point.x() );
      MinY = MaxY = FirstY = LastY;
    } else {
      MinY = std::min( MinY, LastY );
      MaxY = std::max( MaxY, LastY );
    }
    Count += 1;
  }
  
  // ------------------------------------------------------------
      
  void pointsWithSameXCoord::addToPolyline( std::vector<wpoint> *Polyline ) const
  {
    assert( Polyline != NULL );

    if ( Count == 0 )
      return;

    Polyline->push_back( wpoint( X, FirstY ) );
   
    if ( MinY != FirstY )
      Polyline->push_back( wpoint( X, MinY ) );

    if ( MaxY != MinY )
      Polyline->push_back( wpoint( X, MaxY ) );

    if ( MaxY != LastY )
      Polyline->push_back( wpoint( X, LastY ) );
  }
  
  // ============================================================

  checkIsLessThan::checkIsLessThan( const pairScales &S ) :
    Scales( S )
  {
  }
  
  // ------------------------------------------------------------
      
  bool checkIsLessThan::operator()( const fcoord FValue, const sequence::data::point_t &Point ) const 
  { 
    return FValue < Scales.numberToFractionX(Point.x()); 
  }
  
  // ------------------------------------------------------------
  
  bool checkIsLessThan::operator()( const sequence::data::point_t &Point, const fcoord FValue ) const 
  { 
    return Scales.numberToFractionX(Point.x()) < FValue; 
  }

  // ============================================================
}


// ============================================================
   
scigraphics::sequence::graphView::graphView() : 
  Visible(true) 
{
}

// ------------------------------------------------------------

scigraphics::sequence::graphView::~graphView() 
{
}

// ------------------------------------------------------------
        
void scigraphics::sequence::graphView::setVisible( bool V ) 
{ 
  Visible = V; 
}

// ------------------------------------------------------------

bool scigraphics::sequence::graphView::isVisible() const 
{ 
  return Visible; 
}

// ------------------------------------------------------------
        
scigraphics::wcoord scigraphics::sequence::graphView::legendExampleWidth() const  
{ 
  return 35; 
}

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::sequence::graphView::legendExampleHeight() const 
{ 
  return 5;  
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
  sequence::data::iterator BeginVisbile = Checker(0,*Begin)   ? Begin : std::lower_bound( Begin,        End, static_cast<fcoord>(0), Checker );
  if ( BeginVisbile != Begin )
    --BeginVisbile;
  sequence::data::iterator EndVisible   = Checker(*(End-1),1) ? End   : std::upper_bound( BeginVisbile, End, static_cast<fcoord>(1), Checker );
  if ( EndVisible != End )
    ++EndVisible;
  drawUnorderedByX( Painter, Scales, BeginVisbile, EndVisible );
}

// ============================================================
        
scigraphics::sequence::graphViewGeneralLine::graphViewGeneralLine( const style &Style ) : 
  graphViewStyle<lineStyle,graphViewOrdered>(Style) 
{
}

// ------------------------------------------------------------

void scigraphics::sequence::graphViewGeneralLine::drawUnorderedByX( painter &Painter, const pairScales& Scales, sequence::data::iterator Begin, sequence::data::iterator End ) const
{
  if ( getStyle().getStyle() == lineStyle::None )
    return;

  if ( Begin == End )
    return;

  const size_t MaxPolylineSize = 2*std::min<size_t>( ( End - Begin ), std::max( Painter.width(), Painter.height() ) ) + 128;
  
  std::vector< wpoint > Polyline;
  Polyline.reserve( MaxPolylineSize );

  pointsWithSameXCoord PointsWithSameXCoord;

  sequence::data::iterator Point = Begin;

  Painter.setLineStyle( getStyle() );
  while ( true )
  {
    if ( Point != End && Point->isValid() )
    {
      fpoint CurrFPoint = Scales.npoint2fpoint(*Point);
      if ( PointsWithSameXCoord.canSeparate( Painter, CurrFPoint ) )
      {
        PointsWithSameXCoord.addToPolyline( &Polyline );
        PointsWithSameXCoord.clear();
        if ( Polyline.size() >= MaxPolylineSize )
        {
          wpoint LastPoint = Polyline.back();
          drawLineBetweenPoints( Painter, &Polyline );
          Polyline.clear();
          Polyline.reserve( MaxPolylineSize );
          Polyline.push_back( LastPoint );
        }
      }
      PointsWithSameXCoord.append( Painter, CurrFPoint );
    } else {
      PointsWithSameXCoord.addToPolyline( &Polyline );
      PointsWithSameXCoord.clear();
      drawLineBetweenPoints( Painter, &Polyline );
//      Painter.drawLineW( Polyline );
      Polyline.clear();
      if ( Point == End )
        break;
    }
    ++Point;
  }

}

// ------------------------------------------------------------

void scigraphics::sequence::graphViewGeneralLine::drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const
{
  const wcoord VCenter = ( Rectangle.up() + Rectangle.down() )/2;

  std::vector<wpoint> Polyline;
  Polyline.push_back( wpoint( Rectangle.left()+1, VCenter ) );
  Polyline.push_back( wpoint( Rectangle.right()-1, VCenter ) );
  
  Painter.setLineStyle( getStyle() );
  drawLineBetweenPoints( Painter, &Polyline );
}

// ------------------------------------------------------------

void scigraphics::sequence::graphViewGeneralLine::setLineWidth( unsigned Width )
{
  lineStyle Style = getStyle();
  Style.setWidth( Width );
  setStyle( Style );
}

// ============================================================
        
scigraphics::sequence::graphViewLine::graphViewLine( const style &Style ) : 
  graphViewGeneralLine(Style) 
{
}

// ------------------------------------------------------------

void scigraphics::sequence::graphViewLine::drawLineBetweenPoints( painter &Painter, std::vector<wpoint> *Points ) const
{
  assert( Points != NULL );
  Painter.drawLineW( *Points );
}

// ============================================================

scigraphics::sequence::graphViewPoints::graphViewPoints( const style &Style ) : 
  graphViewStyle<pointStyle,graphViewOrdered>(Style) 
{
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

void scigraphics::sequence::graphViewPoints::setPointSize( unsigned Size )
{
  pointStyle Style = getStyle();
  Style.setWidth( Size );
  setStyle( Style );
}

// ============================================================
        
scigraphics::sequence::graphViewErrorBars::graphViewErrorBars( const style &Style ) : 
  graphViewStyle<errorBarStyle,graphViewOrdered>(Style) 
{
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
        
void scigraphics::sequence::graphViewErrorBars::drawLegendExample( painter &, const wrectangle & ) const 
{
}

// ------------------------------------------------------------

scigraphics::sequence::graphViewLineHystogram::graphViewLineHystogram( const style &Style ) : 
  graphViewGeneralLine(Style) 
{
}

// ------------------------------------------------------------

void scigraphics::sequence::graphViewLineHystogram::drawLineBetweenPoints( painter &Painter, std::vector<wpoint> *Points ) const
{
  assert( Points != NULL );

  const size_t Size = Points->size();
  if ( Size <= 1 )
    return;

  Points->resize( 2*Size-1, wpoint(0,0) );
  for ( size_t i = Size-1; i >= 1; i-- )
  {
    size_t j = i*2;
    assert( j >= 1 );
    assert( j >= i );
    assert( j < Points->size() );

    wpoint Prev   = (*Points)[i-1];
    wpoint Next   = (*Points)[i];
    wpoint Middle( Next.x(), Prev.y() );

    (*Points)[j] = Next;
    (*Points)[j-1] = Middle;
  }

  Painter.drawLineW( *Points );
}

// ------------------------------------------------------------
      
scigraphics::sequence::graphViewCoveredArea::graphViewCoveredArea( const style &Style ) : 
  graphViewStyle<brushStyle,graphView>(Style) 
{
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
 
