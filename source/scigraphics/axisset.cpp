
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

#include <stdexcept>
#include <typeinfo>
#include <algorithm>

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

void scigraphics::axisSet::replaceScale( scale *S )
{
  if ( S == NULL )
    throw std::invalid_argument("Scale must be not NULL");

  delete Scale;
  Scale = S;
}

// ------------------------------------------------------------
      
void scigraphics::axisSet::drawAxis( painter &Painter ) 
{ 
  if ( ! isVisible() )
    return;

  assert( Axis != NULL );
  Axis->draw(Painter); 
}

// ------------------------------------------------------------

void scigraphics::axisSet::drawAxisTicks( painter &Painter )
{
  if ( ! isVisible() )
    return;

  assert( Scale != NULL );
  assert( AxisTicks != NULL );

  std::vector<number> Marks = Scale->marks();
  AxisTicks->draw( Painter, *Scale, Marks );
}
      
// ------------------------------------------------------------

void scigraphics::axisSet::drawAxisLabels( painter &Painter )
{
  if ( ! isVisible() )
    return;

  assert( Scale != NULL );
  assert( AxisLabels != NULL );

  std::vector<number> Marks = Scale->marks();
  AxisLabels->draw( Painter, *Scale, Marks );
}

// ------------------------------------------------------------
      
void scigraphics::axisSet::drawAxisTitle( painter &Painter )
{
  if ( ! isVisible() )
    return;

  assert( AxisTitle != NULL );
  AxisTitle->draw( Painter );
}

// ------------------------------------------------------------

void scigraphics::axisSet::drawGrid( painter &Painter )
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
  return ( getDirection() == axisSet::DirectionX ) ? Painter.width() : Painter.height();;
}

// ------------------------------------------------------------

double scigraphics::axisSet::wpointsPerNumber( const painter &Painter ) const
{
  assert( Scale != NULL );
  interval<number> NumberInterval = Scale->getNumberInterval();
    
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
      
scigraphics::wcoord scigraphics::axisSetX::requiredIndent( bool Used ) const
{
  wcoord Indent = 7;

  if ( ! isVisible() || !Used )
    return Indent;

  Indent += 8;

  Indent += AxisLabels->getTextStyle().getFontSize() + 5;
  if ( ! AxisTitle->title().empty() )
    Indent += AxisTitle->getTextStyle().getFontSize() + 5;
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
      
scigraphics::wcoord scigraphics::axisSetY::requiredIndent( bool Used ) const
{
  wcoord Indent = 7;
  
  if ( ! isVisible() || !Used )
    return Indent;

  Indent += 8;

  Indent += AxisLabels->getTextStyle().getFontSize() * 4 + 5;
  if ( ! AxisTitle->title().empty() )
    Indent += AxisTitle->getTextStyle().getFontSize() + 5;
  return Indent;
}

// ============================================================

scigraphics::axisSetCollection::axisSetCollection()
{
  assert( PositionsCount == 4 );
  AxisSets.resize( PositionsCount );
  AxisSets[ Left ]   = new axisSetY(0);
  AxisSets[ Right ]  = new axisSetY(1);
  AxisSets[ Top ]    = new axisSetX(1);
  AxisSets[ Bottom ] = new axisSetX(0);
}

// ------------------------------------------------------------

scigraphics::axisSetCollection::~axisSetCollection()
{
}
      
// ------------------------------------------------------------

scigraphics::axisSet& scigraphics::axisSetCollection::at( axisPosition Position )
{
  unsigned Index = (unsigned)Position;
  if ( Index >= AxisSets.size() )
    throw std::out_of_range( "Invalid axis position" );
  return *(AxisSets[Index]);
}

// ------------------------------------------------------------
      
scigraphics::pairScales scigraphics::axisSetCollection::getBottomLeftPairScales()
{
  return pairScales( *(AxisSets[Bottom]->getScale()), *(AxisSets[Left]->getScale()) ); 
}

// ------------------------------------------------------------

const scigraphics::axisSet& scigraphics::axisSetCollection::at( axisPosition Position ) const
{
  return ( const_cast<axisSetCollection*>(this) )->at(Position);
}

// ------------------------------------------------------------
      
void scigraphics::axisSetCollection::setAxisNumberLimits( plotLimits::limitsXY *Limits )
{
  if ( Limits == NULL )
    throw std::invalid_argument( "Limits pointer must be not NULL" );
  
  for ( axis_iterator Axis = AxisSets.begin(); Axis != AxisSets.end(); ++Axis )
    Limits->setAxisNumberLimits( *Axis );
}

// ------------------------------------------------------------
      
void scigraphics::axisSetCollection::drawGrid( painter &Painter )
{
  at( Left ).drawGrid(Painter);
  at( Bottom ).drawGrid(Painter);
}

// ------------------------------------------------------------

void scigraphics::axisSetCollection::drawAxis( painter &Painter )
{
  for ( axis_iterator Axis = AxisSets.begin(); Axis != AxisSets.end(); ++Axis )
    (*Axis)->drawAxis(Painter);
}

// ------------------------------------------------------------

void scigraphics::axisSetCollection::drawAxisTicks( painter &Painter )
{
  for ( axis_iterator Axis = AxisSets.begin(); Axis != AxisSets.end(); ++Axis )
    (*Axis)->drawAxisTicks(Painter);
}

// ------------------------------------------------------------

void scigraphics::axisSetCollection::drawAxisLabels( painter &Painter )
{
  for ( axis_iterator Axis = AxisSets.begin(); Axis != AxisSets.end(); ++Axis )
    (*Axis)->drawAxisLabels(Painter);
}

// ------------------------------------------------------------

void scigraphics::axisSetCollection::drawAxisTitles( painter &Painter )
{
  for ( axis_iterator Axis = AxisSets.begin(); Axis != AxisSets.end(); ++Axis )
    (*Axis)->drawAxisTitle(Painter);
}

// ------------------------------------------------------------

void scigraphics::axisSetCollection::applyScalesChanging( double Value, axisSet::direction Direction, void (*Operation)( scale *Scale, double Value ) )
{
  if ( Operation == NULL )
    throw std::invalid_argument( "Operation pointer must be not NULL" );

  for ( axis_iterator a = AxisSets.begin(); a != AxisSets.end(); ++a )
  {
    axisSet *AxisSet = *a;
    assert( AxisSet != NULL );
    if ( AxisSet->getDirection() == Direction )
    {
      scale *Scale = AxisSet->getScale();
      assert( Scale != NULL );
      Operation( Scale, Value );
    }
  }
}

// ------------------------------------------------------------

void scigraphics::axisSetCollection::addScalesShift( double Shift, axisSet::direction Direction )
{
  applyScalesChanging( Shift, Direction, scale::addScaleShift );
}

// ------------------------------------------------------------

void scigraphics::axisSetCollection::mulScalesZoom( double Zoom, axisSet::direction Direction )
{
  applyScalesChanging( Zoom, Direction, scale::mulScaleZoom );
}

// ------------------------------------------------------------

void scigraphics::axisSetCollection::resetScales( axisSet::direction Direction )
{
  applyScalesChanging( 0, Direction, scale::resetScale );
}

// ------------------------------------------------------------

void scigraphics::axisSetCollection::resetAllScales()
{
  applyScalesChanging( 0, axisSet::DirectionX, scale::resetScale );
  applyScalesChanging( 0, axisSet::DirectionY, scale::resetScale );
}

// ------------------------------------------------------------

void scigraphics::axisSetCollection::resetScalesTo1x1( painter &Painter )
{
  resetAllScales();

  double MinWPointsPerNPoints = minWPointsPerNPoints(Painter);
  if ( MinWPointsPerNPoints <= 0 || MinWPointsPerNPoints >= 1e10 )
    return;

  for ( axis_iterator a = AxisSets.begin(); a != AxisSets.end(); ++a )
  {
    double WPointsPerNPoints = (*a)->wpointsPerNumber(Painter);
    if ( WPointsPerNPoints <= 0 )
      continue;
    double MulZoom = WPointsPerNPoints / MinWPointsPerNPoints;
    scale *Scale = (*a)->getScale();
    assert( Scale != NULL );
    scale::addScaleShift( Scale, ( 1 - MulZoom  )/2 );
    scale::mulScaleZoom( Scale, MulZoom );
  }

}

// ------------------------------------------------------------
      
double scigraphics::axisSetCollection::minWPointsPerNPoints( painter &Painter ) const
{
  double MinWPointsPerNPoints = 1e100;
  for ( axis_const_iterator a = AxisSets.begin(); a != AxisSets.end(); ++a )
  {
    double WPointsPerNPoints = (*a)->wpointsPerNumber(Painter);
    if ( WPointsPerNPoints <= 0 )
      continue;
    MinWPointsPerNPoints = std::min<double>( MinWPointsPerNPoints, WPointsPerNPoints );
  }
  return MinWPointsPerNPoints;
}

// ============================================================


