
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

#include "scigraphics/graphmap.h"
#include "scigraphics/scale.h"
#include "scigraphics/plotlimits.h"

#include <iostream>
#include <stdexcept>
#include <cassert>

// ============================================================

scigraphics::map::graph::graph( const std::string &Legend ) : 
  View( NULL ),
  Data( NULL ),
  ScaleZ( NULL ),
  ForcedIntervalZ( plotLimits::autoScaleInterval() )
{
  initInternal( NULL, NULL, Legend );
}

// ------------------------------------------------------------
        
scigraphics::map::graph::graph( data *D, graphView *V, const std::string &Legend ) :
  View( NULL ),
  Data( NULL ),
  ScaleZ( NULL ),
  ForcedIntervalZ( plotLimits::autoScaleInterval() )
{
  initInternal( D, V, Legend );
}

// ------------------------------------------------------------

scigraphics::map::graph::~graph()
{
  delete Data;
  delete ScaleZ;
}

// ------------------------------------------------------------
        
void scigraphics::map::graph::initInternal( data *D, graphView *V, const std::string &Legend )
{
  Data = D;
  View = V;
  ForcedIntervalZ = plotLimits::autoScaleInterval();

  ScaleZ = new scaleLinear();
  ScaleZ->setNumberOfMarks( 7, 15 );

  setGridDrawOrder( plotElement::DrawUnderGrid );
  setLegend( Legend );
}

// ------------------------------------------------------------

void scigraphics::map::graph::init()
{
  if ( Data == NULL )
    Data = createData();
  if ( Data == NULL )
    throw std::runtime_error( "createData() must return valid pointer" );

  if ( View == NULL )
    View = createView();
  if ( View == NULL )
    throw std::runtime_error( "createView() must return valid pointer" );
}

// ------------------------------------------------------------
        
scigraphics::map::data& scigraphics::map::graph::getData() 
{
  assert( Data != NULL );
  return *Data; 
}

// ------------------------------------------------------------

const scigraphics::map::data& scigraphics::map::graph::getData() const 
{ 
  assert( Data != NULL );
  return *Data; 
}

// ------------------------------------------------------------
        
scigraphics::map::graphView& scigraphics::map::graph::getView() 
{ 
  assert( View != NULL );
  return *View; 
}

// ------------------------------------------------------------

const scigraphics::map::graphView& scigraphics::map::graph::getView() const 
{ 
  assert( View != NULL );
  return *View; 
}

// ------------------------------------------------------------

void scigraphics::map::graph::updateScaleZInterval() const
{
  assert( Data != NULL );
  assert( ScaleZ != NULL );

  numberLimits LimitsZ = plotLimits::applyForcedLimitsMinMax( limitsZ(), ForcedIntervalZ );
  ScaleZ->setNumberLimits( LimitsZ );
}

// ------------------------------------------------------------

void scigraphics::map::graph::draw( painter &Painter, const pairScales& Scales ) const
{
  assert( View != NULL );
  assert( Data != NULL );
  assert( ScaleZ != NULL );

  updateScaleZInterval();

  View->draw( Painter, Scales, *Data, *ScaleZ );
}

// ------------------------------------------------------------

void scigraphics::map::graph::drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const
{
  assert( View != NULL );
  assert( Data != NULL );
  assert( ScaleZ != NULL );

  updateScaleZInterval();

  View->drawLegendExample( Painter, Rectangle, *ScaleZ );
}
      
// ------------------------------------------------------------

scigraphics::wcoord scigraphics::map::graph::legendExampleWidth()  const
{
  assert( View != NULL );
  return View->legendExampleWidth();
}

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::map::graph::legendExampleHeight() const
{
  assert( View != NULL );
  return View->legendExampleHeight();
}

// ------------------------------------------------------------

scigraphics::numberLimits scigraphics::map::graph::limitsX() const 
{ 
  return getData().limitsX(); 
}

// ------------------------------------------------------------
        
scigraphics::numberLimits scigraphics::map::graph::limitsY( const interval<number> &LimitsX ) const 
{ 
  return getData().limitsY(LimitsX); 
}

// ------------------------------------------------------------

scigraphics::numberLimits scigraphics::map::graph::limitsZ() const 
{ 
  return getData().limitsZ(); 
}

// ------------------------------------------------------------

void scigraphics::map::graph::setForcedIntervalZ( interval<number> Interval ) 
{ 
  ForcedIntervalZ = Interval; 
}

// ------------------------------------------------------------
        
void scigraphics::map::graph::setForcedIntervalZ( number Min, number Max ) 
{ 
  setForcedIntervalZ( interval<number>(Min,Max) ); 
}

// ============================================================

scigraphics::map::graphVector::graphVector( const std::string &Legend  ) : 
  graphSpecified< dataVector, graphViewRectangle >(Legend) 
{
}

// ------------------------------------------------------------
        
scigraphics::map::data::int_t scigraphics::map::graphVector::size() const 
{ 
  return getData().size(); 
}

// ------------------------------------------------------------

scigraphics::map::data::int_t scigraphics::map::graphVector::sizeX() const 
{ 
  return getCastedData().sizeX(); 
}

// ------------------------------------------------------------

scigraphics::map::data::int_t scigraphics::map::graphVector::sizeY() const 
{ 
  return getCastedData().sizeY(); 
}

// ------------------------------------------------------------

void scigraphics::map::graphVector::resize( data::int_t SX, data::int_t SY ) 
{ 
  return getCastedData().resize(SX,SY); 
}

// ------------------------------------------------------------
 
bool scigraphics::map::graphVector::empty() const  
{ 
  return getData().empty(); 
}

// ------------------------------------------------------------
    
void scigraphics::map::graphVector::setIntervalX( number Min, number Max ) 
{ 
  setIntervalX( interval<number>( Min, Max ) ); 
}

// ------------------------------------------------------------

void scigraphics::map::graphVector::setIntervalY( number Min, number Max ) 
{ 
  setIntervalY( interval<number>( Min, Max ) ); 
}

// ------------------------------------------------------------

void scigraphics::map::graphVector::setIntervalX( interval<number> Interval ) 
{ 
  getCastedData().setIntervalX( Interval ); 
}

// ------------------------------------------------------------

void scigraphics::map::graphVector::setIntervalY( interval<number> Interval ) 
{ 
  getCastedData().setIntervalY( Interval ); 
}

// ------------------------------------------------------------
        
scigraphics::interval<scigraphics::number> scigraphics::map::graphVector::intervalX() const
{
  return getCastedData().intervalX();
}

// ------------------------------------------------------------

scigraphics::interval<scigraphics::number> scigraphics::map::graphVector::intervalY() const
{
  return getCastedData().intervalY();
}

// ------------------------------------------------------------
    
scigraphics::number scigraphics::map::graphVector::coordinateX( data::int_t IndexX ) const 
{ 
  return getCastedData().coordinateX(IndexX); 
}

// ------------------------------------------------------------

scigraphics::number scigraphics::map::graphVector::coordinateY( data::int_t IndexY ) const 
{ 
  return getCastedData().coordinateY(IndexY); 
}

// ------------------------------------------------------------

scigraphics::map::data::int_t scigraphics::map::graphVector::nearestIndexX( number X ) const 
{ 
  return getCastedData().nearestIndexX(X); 
}

// ------------------------------------------------------------

scigraphics::map::data::int_t scigraphics::map::graphVector::nearestIndexY( number Y ) const 
{ 
  return getCastedData().nearestIndexY(Y); 
}

// ------------------------------------------------------------

scigraphics::map::data::point_t scigraphics::map::graphVector::at( data::int_t Index ) const 
{ 
  return getData().at(Index); 
}

// ------------------------------------------------------------

scigraphics::map::data::point_t scigraphics::map::graphVector::operator[]( data::int_t Index ) const 
{ 
  return at(Index); 
}

// ------------------------------------------------------------

scigraphics::map::data::int_t scigraphics::map::graphVector::index( data::int_t IndexX, data::int_t IndexY ) const 
{ 
  return getCastedData().index(IndexX,IndexY);
}

// ------------------------------------------------------------

void scigraphics::map::graphVector::set( data::int_t ix, data::int_t iy, number Val ) 
{ 
  getCastedData().set( ix, iy, Val ); 
}

// ------------------------------------------------------------
        
void scigraphics::map::graphVector::setColorStrategy( graphViewRectangle::pointColorStrategy *Strategy )
{
  getCastedView().setColorStrategy( Strategy );
}

// ============================================================

