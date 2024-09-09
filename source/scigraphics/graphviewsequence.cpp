
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
#include <limits>
#include <algorithm>
#include <cassert>

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

class scigraphics::sequence::graphViewOrdered::checkIsLessThan
{
  private:
    const pairScales &Scales;

  public:
    explicit checkIsLessThan( const pairScales &Scales );

    inline bool operator()( const fcoord FValue, const sequence::data::point_t &Point ) const;
    inline bool operator()( const sequence::data::point_t &Point, const fcoord FValue ) const;
};
  
// ------------------------------------------------------------

scigraphics::sequence::graphViewOrdered::checkIsLessThan::checkIsLessThan( const pairScales &S ) :
  Scales( S )
{
}

// ------------------------------------------------------------
    
bool scigraphics::sequence::graphViewOrdered::checkIsLessThan::operator()( const fcoord FValue, const sequence::data::point_t &Point ) const 
{ 
  return FValue < Scales.numberToFractionX(Point.x()); 
}

// ------------------------------------------------------------

bool scigraphics::sequence::graphViewOrdered::checkIsLessThan::operator()( const sequence::data::point_t &Point, const fcoord FValue ) const 
{ 
  return Scales.numberToFractionX(Point.x()) < FValue; 
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

  const checkIsLessThan Checker(Scales);

  sequence::data::iterator BeginVisibile = Begin;
  if ( ! Checker( 0, *Begin ) )
    BeginVisibile = std::lower_bound( Begin, End, static_cast<fcoord>(0), Checker );
  if ( BeginVisibile != Begin )
    --BeginVisibile;

  sequence::data::iterator EndVisible = End;
  if ( ! Checker( *(End-1), 1 ) )
    EndVisible = std::upper_bound( BeginVisibile, End, static_cast<fcoord>(1), Checker );
  if ( EndVisible != End )
    ++EndVisible;

  assert( BeginVisibile.index() >= Begin.index() );
  assert( EndVisible.index() <= End.index() );

  drawUnorderedByX( Painter, Scales, BeginVisibile, EndVisible );
}

// ============================================================
          
class scigraphics::sequence::graphViewGeneralLine::pointsWithSameXCoord
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

    inline bool canSeparate( const painter &Painter, fpoint Point );
    inline void append( const painter &Painter, fpoint Point );
    inline void addToPolyline( std::vector<wpoint> *Polyline ) const;
};

// ------------------------------------------------------------

scigraphics::sequence::graphViewGeneralLine::pointsWithSameXCoord::pointsWithSameXCoord() 
{
  clear();
}

// ------------------------------------------------------------

void scigraphics::sequence::graphViewGeneralLine::pointsWithSameXCoord::clear()
{
  X = 0; 
  MinY = MaxY = FirstY = LastY = 0;
  Count = 0;
}

// ------------------------------------------------------------

bool scigraphics::sequence::graphViewGeneralLine::pointsWithSameXCoord::canSeparate( const painter &Painter, fpoint Point )
{
  if ( Count == 0 )
    return true;
  return X != Painter.fcoord2wcoordX( Point.x() );
}

// ------------------------------------------------------------
    
void scigraphics::sequence::graphViewGeneralLine::pointsWithSameXCoord::append( const painter &Painter, fpoint Point )
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
    
void scigraphics::sequence::graphViewGeneralLine::pointsWithSameXCoord::addToPolyline( std::vector<wpoint> *Polyline ) const
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

scigraphics::sequence::graphViewGeneralLine::graphViewGeneralLine( const style &Style ) : 
  graphViewStyle<lineStyle,graphViewOrdered>(Style) 
{
}

// ------------------------------------------------------------

void scigraphics::sequence::graphViewGeneralLine::drawUnorderedByX( painter &Painter, const pairScales &Scales, sequence::data::iterator Begin, sequence::data::iterator End ) const
{
  if ( getStyle().getStyle() == lineStyle::None )
    return;

  if ( Begin == End )
    return;
  
  Painter.setLineStyle( getStyle() );

  const size_t MaxPolylineSize = maxPolylineSize( Painter, Begin, End );
  
  pointsWithSameXCoord PointsWithSameXCoord;
  std::vector<data::point_t> DPointsVector;
  std::vector< wpoint > Polyline;
  Polyline.reserve( MaxPolylineSize );
  
  sequence::data::iterator DataIterator = Begin;

  while ( DataIterator < End )
  {
    DataIterator = fillDataPointsVector( &DPointsVector, DataIterator, End );
    processDataPointsVector( Painter, Scales, DPointsVector, &PointsWithSameXCoord, &Polyline, MaxPolylineSize );
  }
      
  finalizeDrawPolylineAndPointsSameCoord( Painter, &PointsWithSameXCoord, &Polyline );
}

// ------------------------------------------------------------
        
void scigraphics::sequence::graphViewGeneralLine::finalizeDrawPolylineAndPointsSameCoord( painter &Painter, pointsWithSameXCoord *PointsWithSameXCoord, std::vector<wpoint> *Polyline ) const
{
  assert( PointsWithSameXCoord != NULL );
  assert( Polyline != NULL );

  PointsWithSameXCoord->addToPolyline( Polyline );
  PointsWithSameXCoord->clear();

  drawLineBetweenPoints( Painter, Polyline );
  Polyline->clear();
}

// ------------------------------------------------------------
    
scigraphics::sequence::data::iterator scigraphics::sequence::graphViewGeneralLine::fillDataPointsVector( std::vector<data::point_t> *DPointsVector, 
  sequence::data::iterator Iterator, sequence::data::iterator End )
{
  assert( Iterator <= End );

  const size_t SingleFillSize = 2048;
  const data::iterator::difference_type CountOfPointsToEnd = End - Iterator;

  const data::iterator Result = Iterator.fill( std::min<data::iterator::difference_type>( CountOfPointsToEnd, SingleFillSize ), DPointsVector );
    //std::clog << "graphViewGeneralLine::drawUnorderedByX: SingleFillSize = " << SingleFillSize << ", PointsVector.size = " << PointsVector.size() << std::endl;
  
  assert( ! DPointsVector->empty() );
  assert( DPointsVector->size() <= SingleFillSize );
  assert( Result <= End );

  return Result;
}

// ------------------------------------------------------------
        
void scigraphics::sequence::graphViewGeneralLine::processDataPointsVector( painter &Painter, const pairScales &Scales, const std::vector<data::point_t> &DPointsVector, 
  pointsWithSameXCoord *PointsWithSameXCoord, std::vector<wpoint> *Polyline, size_t MaxPolylineSize ) const
{
  assert( PointsWithSameXCoord != NULL );
  assert( Polyline != NULL );
  assert( MaxPolylineSize > 0 );

  std::vector<data::point_t>::const_iterator Point = DPointsVector.begin();

  while ( true )
  {
    Point = processValidPoints( Painter, Scales, PointsWithSameXCoord, Polyline, MaxPolylineSize, Point, DPointsVector.end() );
    if ( Point == DPointsVector.end() )
      break;
    finalizeDrawPolylineAndPointsSameCoord( Painter, PointsWithSameXCoord, Polyline );
    Point = skipInvalidPoints( Point, DPointsVector.end() );
  }
}

// ------------------------------------------------------------
        
std::vector<scigraphics::sequence::data::point_t>::const_iterator scigraphics::sequence::graphViewGeneralLine::processValidPoints( painter &Painter, const pairScales &Scales, 
  pointsWithSameXCoord *PointsWithSameXCoord, std::vector<wpoint> *Polyline, size_t MaxPolylineSize, 
  std::vector<data::point_t>::const_iterator Iterator, std::vector<data::point_t>::const_iterator End ) const
{
#if 0
  std::vector< fcoord > FCoords( 2 * ( End - Iterator ) + 2 );
  assert( sizeof(fpoint) == 2*sizeof(fcoord) );
  const fpoint *FPoints = reinterpret_cast<const fpoint*>( &FCoords[0] );

  const std::vector<data::point_t>::const_iterator Begin = Iterator;
  while ( Iterator != End && Iterator->isValid() )
  {
    const size_t BaseIndex = static_cast<size_t>( Iterator - Begin ) * 2;
    FCoords[ BaseIndex ] = Scales.numberToFractionX( Iterator->x() );
    ++Iterator;
  }

  for ( std::vector<data::point_t>::const_iterator it = Begin; it != Iterator; ++it )
  {
    const size_t BaseIndex = static_cast<size_t>( it - Begin ) * 2;
    FCoords[ BaseIndex+1 ] = Scales.numberToFractionY( it->y() );
    processValidFPoint( Painter, PointsWithSameXCoord, Polyline, MaxPolylineSize, FPoints[ it - Begin ] );
  }

  return Iterator;
#endif

#if 1
  while ( Iterator != End && Iterator->isValid() )
  {
    const fpoint FPoint = Scales.npoint2fpoint( npoint( Iterator->x(), Iterator->y() ) );
    processValidFPoint( Painter, PointsWithSameXCoord, Polyline, MaxPolylineSize, FPoint );
    ++Iterator;
  }

  return Iterator;
#endif
}

// ------------------------------------------------------------
        
void scigraphics::sequence::graphViewGeneralLine::processValidFPoint( painter &Painter, pointsWithSameXCoord *PointsWithSameXCoord, 
          std::vector<wpoint> *Polyline, size_t MaxPolylineSize, const fpoint FPoint ) const
{
  if ( PointsWithSameXCoord->canSeparate( Painter, FPoint ) )
  {
    PointsWithSameXCoord->addToPolyline( Polyline );
    PointsWithSameXCoord->clear();
    if ( Polyline->size() >= MaxPolylineSize )
    {
      const wpoint LastPoint = Polyline->back();
      drawLineBetweenPoints( Painter, Polyline );
      Polyline->clear();
      Polyline->reserve( MaxPolylineSize );
      Polyline->push_back( LastPoint );
    }
  }
  PointsWithSameXCoord->append( Painter, FPoint );
}

// ------------------------------------------------------------
        
std::vector<scigraphics::sequence::data::point_t>::const_iterator scigraphics::sequence::graphViewGeneralLine::skipInvalidPoints( std::vector<data::point_t>::const_iterator Iterator, 
  std::vector<data::point_t>::const_iterator End )
{
  while ( Iterator != End && !Iterator->isValid() )
    ++Iterator;

  return Iterator;
}

// ------------------------------------------------------------
        
size_t scigraphics::sequence::graphViewGeneralLine::maxPolylineSize( const painter &Painter, sequence::data::iterator Begin, sequence::data::iterator End )
{
  assert( Begin <= End );

  data::iterator::difference_type CountOfPoints = End - Begin;
  if ( CountOfPoints >= static_cast<data::iterator::difference_type>(std::numeric_limits<size_t>::max()/4) )
    CountOfPoints = std::numeric_limits<size_t>::max()/4;
  
  const wcoord PainterMaxDimension = std::max( Painter.width(), Painter.height() );

  const size_t MaxPolylineSize = 1024 +
    4*std::min( static_cast<size_t>( PainterMaxDimension ), static_cast<size_t>(CountOfPoints) );

  return MaxPolylineSize;
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

// ------------------------------------------------------------
        
void scigraphics::sequence::graphViewGeneralLine::setLineStyle( lineStyle::style LineStyle )
{
  lineStyle Style = getStyle();
  Style.setStyle( LineStyle );
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
    const fpoint FPoint = Scales.npoint2fpoint( npoint( Point->x(), Point->y() ) );
    Painter.drawPointF( FPoint, getStyle() );
  }
}

// ------------------------------------------------------------

void scigraphics::sequence::graphViewPoints::drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const
{
  const wcoord VCenter = ( Rectangle.up() + Rectangle.down() )/2;
  const wcoord HCenter = ( Rectangle.left() + Rectangle.right() )/2;
  Painter.drawPointW( wpoint(HCenter,VCenter), getStyle() );
}

// ------------------------------------------------------------

void scigraphics::sequence::graphViewPoints::setPointSize( unsigned Size )
{
  pointStyle Style = getStyle();
  Style.setWidth( Size );
  setStyle( Style );
}

// ------------------------------------------------------------
        
void scigraphics::sequence::graphViewPoints::setPointShape( pointStyle::shape Shape )
{
  pointStyle Style = getStyle();
  Style.setShape( Shape );
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

    const npoint NPoint( Point->x(), Point->y() );
    drawHorizontalErrorBar( Painter, Scales, NPoint, Point->errX() );
    drawVerticalErrorBar( Painter, Scales, NPoint, Point->errY() );
  }
}

// ------------------------------------------------------------

void scigraphics::sequence::graphViewErrorBars::drawHorizontalErrorBar( painter &Painter, const pairScales& Scales, const npoint &Point, number ErrX ) const
{
  if ( ErrX <= 0 )
    return;
  const fpoint Min = Scales.npoint2fpoint(npoint( Point.x() - ErrX, Point.y() ));
  const fpoint Max = Scales.npoint2fpoint(npoint( Point.x() + ErrX, Point.y() ));
  Painter.drawHorizontalErrorBarF( Min, Max, getStyle() );
}

// ------------------------------------------------------------

void scigraphics::sequence::graphViewErrorBars::drawVerticalErrorBar( painter &Painter, const pairScales& Scales, const npoint &Point, number ErrY ) const
{
  if ( ErrY <= 0 )
    return;
  const fpoint Min = Scales.npoint2fpoint(npoint( Point.x(), Point.y() - ErrY ));
  const fpoint Max = Scales.npoint2fpoint(npoint( Point.x(), Point.y() + ErrY ));
  Painter.drawVerticalErrorBarF( Min, Max, getStyle() );
}

// ------------------------------------------------------------
        
void scigraphics::sequence::graphViewErrorBars::drawLegendExample( painter &, const wrectangle & ) const 
{
}

// ------------------------------------------------------------

void scigraphics::sequence::graphViewErrorBars::setLineWidth( unsigned Width )
{
  errorBarStyle Style = getStyle();
  Style.setLineWidth( Width );
  setStyle( Style );
}

// ------------------------------------------------------------
        
void scigraphics::sequence::graphViewErrorBars::setHatWidth( unsigned Width )
{
  errorBarStyle Style = getStyle();
  Style.setHatWidth( Width );
  setStyle( Style );
}

// ============================================================

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
    const size_t j = i*2;
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
    const fpoint FPoint = Scales.npoint2fpoint( npoint(Point->x(),Point->y()) );
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
  const sequence::data::iterator End = Data.end();

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
 
