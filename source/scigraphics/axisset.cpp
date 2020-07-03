
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

#include "scigraphics/axisset.h"
#include "scigraphics/grid.h"
#include "scigraphics/scale.h"
#include "scigraphics/painter.h"
#include "scigraphics/axis.h"
#include "scigraphics/axislabels.h"
#include "scigraphics/axisticks.h"
#include "scigraphics/axistitle.h"

#include <stdexcept>
#include <algorithm>
#include <limits>
#include <vector>
#include <cassert>

// ============================================================

scigraphics::axisSet::axisSet( axis *A, axisTicks *T, axisLabels *L, axisTitle *H, grid *G, scale *S ) :
  Visible(true),
  Axis(A),
  AxisTicks(T),
  AxisLabels(L),
  AxisTitle(H),
  Grid(G),
  Scale(S) 
{
}

// ------------------------------------------------------------

scigraphics::axisSet::~axisSet()
{
  delete Axis;
  delete AxisTicks;
  delete AxisLabels;
  delete AxisTitle;
  delete Grid;
  delete Scale;
}

// ------------------------------------------------------------

void scigraphics::axisSet::setVisible( bool V ) 
{ 
  Visible = V; 
}

// ------------------------------------------------------------

bool scigraphics::axisSet::isVisible() const 
{ 
  return Visible; 
}

// ------------------------------------------------------------

scigraphics::scale* scigraphics::axisSet::getScale() 
{ 
  return Scale; 
}

// ------------------------------------------------------------

const scigraphics::scale* scigraphics::axisSet::getScale() const 
{ 
  return Scale; 
}

// ------------------------------------------------------------

void scigraphics::axisSet::replaceScale( scale *S )
{
  if ( S == NULL )
    throw std::invalid_argument("Scale must be not NULL");

  delete Scale;
  Scale = S;
}

// ------------------------------------------------------------

void scigraphics::axisSet::setNumberLimits( const numberLimits &Limits ) 
{ 
  Scale->setNumberLimits(Limits); 
}

// ------------------------------------------------------------

void scigraphics::axisSet::setAxisTitle( const std::string &Title ) 
{ 
  AxisTitle->setTitle(Title); 
}

// ------------------------------------------------------------

std::string scigraphics::axisSet::getAxisTitle() const 
{ 
  return AxisTitle->title(); 
}
      
// ------------------------------------------------------------

void scigraphics::axisSet::setNumberStyle( const numberStyle &Style ) 
{ 
  AxisLabels->setNumberStyle(Style); 
}

// ------------------------------------------------------------
      
const scigraphics::numberStyle& scigraphics::axisSet::getNumberStyle()
{
  return AxisLabels->getNumberStyle();
}

// ------------------------------------------------------------
      
void scigraphics::axisSet::drawAxis( painter &Painter ) const
{ 
  if ( ! isVisible() )
    return;

  assert( Axis != NULL );
  Axis->draw(Painter); 
}

// ------------------------------------------------------------

void scigraphics::axisSet::drawAxisTicks( painter &Painter ) const
{
  if ( ! isVisible() )
    return;

  assert( Scale != NULL );
  assert( AxisTicks != NULL );

  std::vector<number> Marks = Scale->marks();
  AxisTicks->draw( Painter, *Scale, Marks );
}
      
// ------------------------------------------------------------

void scigraphics::axisSet::drawAxisLabels( painter &Painter ) const
{
  if ( ! isVisible() )
    return;

  assert( Scale != NULL );
  assert( AxisLabels != NULL );

  std::vector<number> Marks = Scale->marks();
  AxisLabels->draw( Painter, *Scale, Marks );
}

// ------------------------------------------------------------
      
void scigraphics::axisSet::drawAxisTitle( painter &Painter ) const
{
  if ( ! isVisible() )
    return;

  assert( AxisTitle != NULL );
  AxisTitle->draw( Painter );
}

// ------------------------------------------------------------

void scigraphics::axisSet::drawGrid( painter &Painter ) const
{
  if ( ! isVisible() )
    return;

  assert( Scale != NULL );
  assert( Grid != NULL );
  
  std::vector<number> Marks = Scale->marks();
  Grid->draw( Painter, *Scale, Marks );
}
      
// ------------------------------------------------------------
      
scigraphics::wcoord scigraphics::axisSet::wcoordDimension( const painter &Painter ) const
{
  return ( getDirection() == AxisDirectionX ) ? 
    Painter.width() : 
    Painter.height();
}

// ------------------------------------------------------------

double scigraphics::axisSet::wpointsPerNumber( const painter &Painter ) const
{
  assert( Scale != NULL );
  interval<number> NumberInterval = Scale->getVisivleInterval();
    
  if ( ! numberLimits::isValidInterval( NumberInterval ) )
    return 0;
  if ( NumberInterval.distance() <= 0 )
    return 0;

  double Dimension = wcoordDimension( Painter ); 
  return Dimension / NumberInterval.distance();
}

// ============================================================

scigraphics::axisSetX::axisSetX( fcoord BaseY ) : 
  axisSet( 
    new axisX(BaseY), 
    new axisTicksX(BaseY), 
    new axisLabelsX(BaseY), 
    new axisTitleX(BaseY),
    new gridX(), 
    new scaleLinear() )
{
}

// ------------------------------------------------------------
      
scigraphics::axisDirection scigraphics::axisSetX::getDirection() const 
{ 
  return AxisDirectionX; 
}

// ------------------------------------------------------------
      
scigraphics::wcoord scigraphics::axisSetX::requiredIndent( bool Used ) const
{
  wcoord Indent = 7;

  if ( ! isVisible() )
    return Indent;
  
  if ( ! AxisTitle->title().empty() )
    Used = true;

  if ( Used )
  {
    Indent += 8;
    Indent += AxisLabels->getTextStyle().getFontSize() + 5;
    if ( ! AxisTitle->title().empty() )
      Indent += AxisTitle->getTextStyle().getFontSize() + 5;
  }

  return Indent;
}

// ============================================================

scigraphics::axisSetY::axisSetY( fcoord BaseX ) : 
  axisSet( 
    new axisY(BaseX), 
    new axisTicksY(BaseX), 
    new axisLabelsY(BaseX), 
    new axisTitleY(BaseX),
    new gridY(), 
    new scaleLinear() )
{
}

// ------------------------------------------------------------
      
scigraphics::axisDirection scigraphics::axisSetY::getDirection() const 
{ 
  return AxisDirectionY; 
}

// ------------------------------------------------------------
      
scigraphics::wcoord scigraphics::axisSetY::requiredIndent( bool Used ) const
{
  wcoord Indent = 7;
  
  if ( ! isVisible() )
    return Indent;
  
  if ( ! AxisTitle->title().empty() )
    Used = true;

  if ( Used )
  {
    Indent += 8;
    Indent += AxisLabels->getTextStyle().getFontSize() * 4 + 5;
    if ( ! AxisTitle->title().empty() )
      Indent += AxisTitle->getTextStyle().getFontSize() + 5;
  }

  return Indent;
}

// ============================================================

scigraphics::axisSetCollection::axisSetCollection() :
  KeepScales1x1(false)
{
  AxisSets.resize( 4 );
  AxisSets.set( AxisLeft,   new axisSetY(0) );
  AxisSets.set( AxisRight,  new axisSetY(1) );
  AxisSets.set( AxisTop,    new axisSetX(1) );
  AxisSets.set( AxisBottom, new axisSetX(0) );

//  AxisSets[ AxisRight ].setVisible(false);
//  AxisSets[ AxisTop ].setVisible(false);
}

// ------------------------------------------------------------

scigraphics::axisSetCollection::~axisSetCollection()
{
}
      
// ------------------------------------------------------------

scigraphics::axisSet& scigraphics::axisSetCollection::at( axisPosition Position )
{
  size_t Index = static_cast<size_t>(Position);
  if ( Index >= AxisSets.size() )
    throw std::out_of_range( "Invalid axis position" );
  return AxisSets[Index];
}

// ------------------------------------------------------------
      
scigraphics::pairScales scigraphics::axisSetCollection::getBottomLeftPairScales()
{
  return pairScales( *(AxisSets[AxisBottom].getScale()), *(AxisSets[AxisLeft].getScale()) ); 
}

// ------------------------------------------------------------

const scigraphics::axisSet& scigraphics::axisSetCollection::at( axisPosition Position ) const
{
  return ( const_cast<axisSetCollection*>(this) )->at(Position);
}

// ------------------------------------------------------------
      
scigraphics::axisSet& scigraphics::axisSetCollection::operator[]( axisPosition Position ) 
{ 
  return at(Position); 
}

// ------------------------------------------------------------

const scigraphics::axisSet& scigraphics::axisSetCollection::operator[]( axisPosition Position ) const 
{ 
  return at(Position); 
}

// ------------------------------------------------------------
      
void scigraphics::axisSetCollection::setAxisNumberLimits( plotLimits::limitsXY *Limits )
{
  if ( Limits == NULL )
    throw std::invalid_argument( "Limits pointer must be not NULL" );
  
  for ( axis_iterator Axis = AxisSets.begin(); Axis != AxisSets.end(); ++Axis )
    Limits->setAxisNumberLimits( &*Axis );
}

// ------------------------------------------------------------
      
void scigraphics::axisSetCollection::drawGrid( painter &Painter )
{
  at( AxisLeft ).drawGrid(Painter);
  at( AxisBottom ).drawGrid(Painter);
}

// ------------------------------------------------------------

void scigraphics::axisSetCollection::drawAxis( painter &Painter )
{
  for ( axis_iterator Axis = AxisSets.begin(); Axis != AxisSets.end(); ++Axis )
    Axis->drawAxis(Painter);
}

// ------------------------------------------------------------

void scigraphics::axisSetCollection::drawAxisTicks( painter &Painter )
{
  for ( axis_iterator Axis = AxisSets.begin(); Axis != AxisSets.end(); ++Axis )
    Axis->drawAxisTicks(Painter);
}

// ------------------------------------------------------------

void scigraphics::axisSetCollection::drawAxisLabels( painter &Painter )
{
  for ( axis_iterator Axis = AxisSets.begin(); Axis != AxisSets.end(); ++Axis )
    Axis->drawAxisLabels(Painter);
}

// ------------------------------------------------------------

void scigraphics::axisSetCollection::drawAxisTitles( painter &Painter )
{
  for ( axis_iterator Axis = AxisSets.begin(); Axis != AxisSets.end(); ++Axis )
    Axis->drawAxisTitle(Painter);
}

// ------------------------------------------------------------

void scigraphics::axisSetCollection::applyScaleChanging( double Value, axisDirection Direction, void (*Operation)( scale *Scale, double Value ) )
{
  assert( Operation != NULL );
  for ( axis_iterator Set = AxisSets.begin(); Set != AxisSets.end(); ++Set )
  {
    assert( &*Set != NULL );
    if ( Set->getDirection() == Direction )
    {
      scale *Scale = Set->getScale();
      assert( Scale != NULL );
      Operation( Scale, Value );
    }
  }
}

// ------------------------------------------------------------
      
double scigraphics::axisSetCollection::getScaleValue( axisDirection Direction, double (*Operation)( const scale *Scale ) ) const
{
  assert( Operation != NULL );
  for ( axis_const_iterator Set = AxisSets.begin(); Set != AxisSets.end(); ++Set )
  {
    assert( &*Set != NULL );
    if ( Set->getDirection() == Direction )
    {
      const scale *Scale = Set->getScale();
      assert( Scale != NULL );
      return Operation(Scale);
    }
  }

  return 0;
}

// ------------------------------------------------------------
      
void scigraphics::axisSetCollection::addScaleShift( scale *Scale, double Shift )
{
  if ( Scale == NULL )
    return;
  Scale->setShift( Scale->shift() + Shift/Scale->zoom() );
}

// ------------------------------------------------------------
      
void scigraphics::axisSetCollection::setScaleShift( scale *Scale, double Shift )
{
  if ( Scale == NULL )
    return;
  Scale->setShift( Shift );
}

// ------------------------------------------------------------

double scigraphics::axisSetCollection::scaleShift( const scale *Scale )
{
  if ( Scale == NULL )
    return 0;
  return Scale->shift();
}

// ------------------------------------------------------------

void scigraphics::axisSetCollection::mulScaleZoom( scale *Scale, double Zoom )
{
  if ( Scale == NULL )
    return;
  Scale->setZoom( Scale->zoom() / Zoom );
}

// ------------------------------------------------------------

void scigraphics::axisSetCollection::setScaleZoom( scale *Scale, double Zoom )
{
  if ( Scale == NULL )
    return;
  Scale->setZoom( Zoom );
}

// ------------------------------------------------------------

double scigraphics::axisSetCollection::scaleZoom( const scale *Scale )
{
  if ( Scale == NULL )
    return 1;
  return Scale->zoom();
}

// ------------------------------------------------------------

void scigraphics::axisSetCollection::resetScale( scale *Scale, double )
{
  if ( Scale == NULL )
    return;
  Scale->reset();
}

// ------------------------------------------------------------

void scigraphics::axisSetCollection::addScaleShift( double Shift, axisDirection Direction )
{
  applyScaleChanging( Shift, Direction, &axisSetCollection::addScaleShift );
}

// ------------------------------------------------------------

void scigraphics::axisSetCollection::setScaleShift( double Shift, axisDirection Direction )
{
  applyScaleChanging( Shift, Direction, &axisSetCollection::setScaleShift );
}

// ------------------------------------------------------------

double scigraphics::axisSetCollection::scaleShift( axisDirection Direction ) const
{
  return getScaleValue( Direction, &axisSetCollection::scaleShift );
}

// ------------------------------------------------------------

void scigraphics::axisSetCollection::mulScaleZoom( double Zoom, axisDirection Direction )
{
  applyScaleChanging( Zoom, Direction, &axisSetCollection::mulScaleZoom );
}

// ------------------------------------------------------------

void scigraphics::axisSetCollection::setScaleZoom( double Zoom, axisDirection Direction )
{
  applyScaleChanging( Zoom, Direction, &axisSetCollection::setScaleZoom );
}

// ------------------------------------------------------------

double scigraphics::axisSetCollection::scaleZoom( axisDirection Direction ) const
{
  return getScaleValue( Direction, &axisSetCollection::scaleZoom );
}

// ------------------------------------------------------------

void scigraphics::axisSetCollection::resetScales( axisDirection Direction )
{
  applyScaleChanging( 0, Direction, &axisSetCollection::resetScale );
}

// ------------------------------------------------------------

void scigraphics::axisSetCollection::resetAllScales()
{
  applyScaleChanging( 0, AxisDirectionX, &axisSetCollection::resetScale );
  applyScaleChanging( 0, AxisDirectionY, &axisSetCollection::resetScale );
}

// ------------------------------------------------------------

void scigraphics::axisSetCollection::setScalesTo1x1( const painter &Painter )
{
  double MinWPointsPerNPoints = minWPointsPerNPoints(Painter);
  if ( MinWPointsPerNPoints <= 0 || MinWPointsPerNPoints >= 1e10 )
    return;

  for ( axis_iterator Set = AxisSets.begin(); Set != AxisSets.end(); ++Set )
  {
    double WPointsPerNPoints = Set->wpointsPerNumber(Painter);
    if ( WPointsPerNPoints <= 0 )
      continue;
    double MulZoom = WPointsPerNPoints / MinWPointsPerNPoints;
    scale *Scale = Set->getScale();
    assert( Scale != NULL );
    addScaleShift( Scale, ( 1 - MulZoom  )/2 );
    mulScaleZoom( Scale, MulZoom );
  }

}

// ------------------------------------------------------------

void scigraphics::axisSetCollection::setScalesTo1x1ifNeeded( const painter &Painter ) 
{
  if ( keepScales1x1() )
    setScalesTo1x1( Painter );
}

// ------------------------------------------------------------
      
double scigraphics::axisSetCollection::minWPointsPerNPoints( const painter &Painter ) const
{
  double MinWPointsPerNPoints = std::numeric_limits<double>::max();
  for ( axis_const_iterator Set = AxisSets.begin(); Set != AxisSets.end(); ++Set )
  {
    double WPointsPerNPoints = Set->wpointsPerNumber(Painter);
    if ( WPointsPerNPoints <= 0 )
      continue;
    MinWPointsPerNPoints = std::min<double>( MinWPointsPerNPoints, WPointsPerNPoints );
  }
  return MinWPointsPerNPoints;
}

// ============================================================


