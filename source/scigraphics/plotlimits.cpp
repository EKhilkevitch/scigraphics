
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

#include "scigraphics/plotlimits.h"
#include "scigraphics/numbers.h"
#include "scigraphics/axisset.h"

#include <limits>
#include <cassert>
#include <cmath>
#include <typeinfo>
#include <stdexcept>

// ============================================================

const scigraphics::number scigraphics::plotLimits::AutoScaleMin = - std::numeric_limits<scigraphics::number>::max();
const scigraphics::number scigraphics::plotLimits::AutoScaleMax = + std::numeric_limits<scigraphics::number>::max();

// ============================================================

void scigraphics::plotLimits::limits::updateLimits( const numberLimits &NewLimits )
{
  updateLimitsByInterval( &Limits, NewLimits.totalLimits() );
  updateLimitsByInterval( &Limits, NewLimits.positiveLimits() );
  updateLimitsByInterval( &Limits, NewLimits.negativeLimits() );
}

// ------------------------------------------------------------

void scigraphics::plotLimits::limits::updateLimitsByInterval( numberLimits *Limits, interval<number> Interval )
{
  Limits->updateLimits( Interval.min() );
  Limits->updateLimits( Interval.max() );
}

// ------------------------------------------------------------

void scigraphics::plotLimits::limits::applyStretch( double Stretch )
{
  assert( Limits.totalLimits().min() != AutoScaleMin );
  assert( Limits.totalLimits().max() != AutoScaleMax );

  interval<number> ResultInterval;

  if ( numberLimits::isValidInterval(Limits.totalLimits()) )
  {
    ResultInterval = applyStretchToTotalInterval( Limits.totalLimits(), Stretch );
    Limits.setTotalInterval( ResultInterval );
  }
  
  if ( numberLimits::isValidInterval(Limits.positiveLimits()) )
  {
    ResultInterval = applyStretchToPositiveInterval( Limits.positiveLimits(), Stretch );
    Limits.setPositiveInterval( ResultInterval );
  }
  
  if ( numberLimits::isValidInterval(Limits.negativeLimits()) )
  {
    ResultInterval = applyStretchToNegativeInterval( Limits.negativeLimits(), Stretch );
    Limits.setNegativeInterval( ResultInterval );
  }
}

// ------------------------------------------------------------
          
scigraphics::interval<scigraphics::number> scigraphics::plotLimits::limits::applyStretchToTotalInterval( interval<number> Interval, double Stretch )
{
  number Distance = Interval.distance();

  if ( Distance <= 0 )
    Distance = std::fabs( Interval.max() );
  if ( Distance <= 0 )
    Distance = 1.0;

  number Min = Interval.min() - ( Stretch - 1 ) * Distance;
  number Max = Interval.max() + ( Stretch - 1 ) * Distance;
  return interval<number>( Min, Max );
}

// ------------------------------------------------------------
     
scigraphics::interval<scigraphics::number> scigraphics::plotLimits::limits::applyStretchToPositiveInterval( interval<number> Interval, double Stretch )
{
  number Min = Interval.min() / Stretch;
  number Max = Interval.max() * Stretch;
  return interval<number>( Min, Max );
}

// ------------------------------------------------------------
     
scigraphics::interval<scigraphics::number> scigraphics::plotLimits::limits::applyStretchToNegativeInterval( interval<number> Interval, double Stretch )
{
  number Min = Interval.min() * Stretch;
  number Max = Interval.max() / Stretch;
  return interval<number>( Min, Max );
}

// ============================================================

bool scigraphics::plotLimits::limitsXY::isAxisSetX( const axisSet *Axis ) 
{ 
  return Axis != NULL && Axis->getDirection() == axisSet::DirectionX;  
}

// ------------------------------------------------------------

bool scigraphics::plotLimits::limitsXY::isAxisSetY( const axisSet *Axis ) 
{ 
  return Axis != NULL && Axis->getDirection() == axisSet::DirectionY;  
}

// ------------------------------------------------------------

void scigraphics::plotLimits::limitsXY::updateLimits( const axisSet* AxisSet, const numberLimits &L )
{
  assert( AxisSet != NULL );
  Limits[ AxisSet ].updateLimits(L);
}

// ------------------------------------------------------------

void scigraphics::plotLimits::limitsXY::applyStretch( double StretchX, double StretchY )
{
  for ( limitsMap::iterator L = Limits.begin(); L != Limits.end(); ++L )
  {
    const axisSet *AxisSet = L->first;

    assert( AxisSet != NULL );

    if ( isAxisSetX(AxisSet) )
      L->second.applyStretch( StretchX );
    else if ( isAxisSetY(AxisSet) )
      L->second.applyStretch( StretchY );
    else
      throw std::runtime_error( "Unknown type of axis set" );
  }
}

// ------------------------------------------------------------

void scigraphics::plotLimits::limitsXY::set( const axisSet* AxisSet, const numberLimits &L ) 
{
  assert( AxisSet != NULL );
  Limits[ AxisSet ].set(L);
}

// ------------------------------------------------------------
          
scigraphics::numberLimits scigraphics::plotLimits::limitsXY::get( const axisSet* AxisSet ) const
{
  assert( AxisSet != NULL );
  limitsMap::const_iterator L = Limits.find( AxisSet ); 
  if ( L == Limits.end() )
    return numberLimits();
//    return defaultAxisNumberLimits();
  return L->second.get();
}

// ------------------------------------------------------------

scigraphics::numberLimits scigraphics::plotLimits::limitsXY::defaultAxisNumberLimits()
{
  numberLimits Result;
  Result.setNegativeInterval( interval<number>(-1,-0.1) );
  Result.setPositiveInterval( interval<number>(+1,+0.1) );
  Result.setTotalInterval( interval<number>(-1,+1) );
  return Result;
}

// ------------------------------------------------------------

scigraphics::numberLimits scigraphics::plotLimits::limitsXY::getX() const
{
  for ( limitsMap::const_iterator L = Limits.begin(); L != Limits.end(); ++L )
    if ( isAxisSetX( L->first ) )
      return L->second.get();
  return numberLimits();
}

// ------------------------------------------------------------

scigraphics::numberLimits scigraphics::plotLimits::limitsXY::getY() const
{
  for ( limitsMap::const_iterator L = Limits.begin(); L != Limits.end(); ++L )
    if ( isAxisSetY( L->first ) )
      return L->second.get();
  return numberLimits();
}

// ------------------------------------------------------------
          
void scigraphics::plotLimits::limitsXY::setAxisNumberLimits( axisSet* AxisSet ) const
{
  assert( AxisSet != NULL );
  numberLimits Limits = get( AxisSet );
  AxisSet->setNumberLimits( Limits );
}

// ============================================================

scigraphics::plotLimits::plotLimits()
{
  setStretchFactor(1.01);
}

// ------------------------------------------------------------

scigraphics::interval<scigraphics::number> scigraphics::plotLimits::autoScaleInterval()
{
  return interval< number >( AutoScaleMin, AutoScaleMax );
}

// ------------------------------------------------------------

scigraphics::interval<scigraphics::number> scigraphics::plotLimits::getInterval( const axisSet *AxisSet ) const
{
  assert( AxisSet != NULL );

  forcedIntervalsMap::const_iterator Interval = ForcedIntervals.find( AxisSet );
  if ( Interval == ForcedIntervals.end() )
    return autoScaleInterval();
  return Interval->second;
}

// ------------------------------------------------------------

scigraphics::numberLimits scigraphics::plotLimits::applyForcedLimits( const axisSet *AxisSet, const numberLimits &Limits ) const
{
  assert( AxisSet != NULL );
  interval<number> Interval = getInterval(AxisSet);
  return applyForcedLimitsMinMax( Limits, Interval );
}

// ------------------------------------------------------------

bool scigraphics::plotLimits::isNumberHaveSignum( number Number, int Signum )
{
  if ( Signum == 0 )
    return true;
  if ( Number > 0 && Signum > 0 )
    return true;
  if ( Number < 0 && Signum < 0 )
    return true;
  return false;
}

// ------------------------------------------------------------

scigraphics::interval<scigraphics::number> scigraphics::plotLimits::applyForcedLimitsToSubintervalMinMax( interval<number> LInterval, interval<number> ForceInterval, int Signum )
{
  if ( numberLimits::isValidInterval(LInterval) )
  {
    number Min = ( ForceInterval.min() == AutoScaleMin || !isNumberHaveSignum(ForceInterval.min(),Signum) ) ? LInterval.min() : ForceInterval.min();
    number Max = ( ForceInterval.max() == AutoScaleMax || !isNumberHaveSignum(ForceInterval.max(),Signum) ) ? LInterval.max() : ForceInterval.max();
    LInterval.setMinMax( Min, Max );
  }
  return LInterval;
}

// ------------------------------------------------------------

scigraphics::numberLimits scigraphics::plotLimits::applyForcedLimitsMinMax( const numberLimits &Limits, interval<number> ForceInterval )
{
  numberLimits Result = Limits;
  if ( ForceInterval.min() == AutoScaleMin && ForceInterval.max() == AutoScaleMax )
    return Result;

  interval<number> LInterval = Result.totalLimits();
  LInterval = applyForcedLimitsToSubintervalMinMax( LInterval, ForceInterval, 0 );
  Result.setTotalInterval( LInterval );
  
  LInterval = Result.positiveLimits();
  LInterval = applyForcedLimitsToSubintervalMinMax( LInterval, ForceInterval, +1 );
  Result.setPositiveInterval( LInterval );
  
  LInterval = Result.negativeLimits();
  LInterval = applyForcedLimitsToSubintervalMinMax( LInterval, ForceInterval, -1 );
  Result.setNegativeInterval( LInterval );

  return Result;
}

// ------------------------------------------------------------
      
scigraphics::numberLimits scigraphics::plotLimits::correctInvalidLimits( const numberLimits &Limits ) const
{
  numberLimits Result = Limits;
 
  if ( ! numberLimits::isValidInterval( Result.negativeLimits() ) )
    Result.setNegativeInterval( interval<number>(-1,-0.1) );
  
  if ( ! numberLimits::isValidInterval( Result.positiveLimits() ) )
    Result.setPositiveInterval( interval<number>(+1,+0.1) );

  if ( ! numberLimits::isValidInterval( Result.totalLimits() ) )
    Result.setTotalInterval( interval<number>(-1,+1) );
 
  return Result;
}

// ------------------------------------------------------------

scigraphics::plotLimits::limitsXY scigraphics::plotLimits::limitsForGraphics( const graphCollection& Graphics ) const
{
  limitsXY Result;

  for ( graphCollection::const_iterator g = Graphics.begin(); g != Graphics.end(); ++g )
    updateLimitsXYForGraphic( &Result, *g, Graphics );

  std::set< const axisSet* > Axis = Graphics.setOfGraphAxisSet();
  for ( std::set< const axisSet* >::const_iterator a = Axis.begin(); a != Axis.end(); ++a )
    correctLimitsXYForAxisSet( &Result, *a );

  Result.applyStretch( StretchFactorX, StretchFactorY );

  return Result;
}

// ------------------------------------------------------------
      
void scigraphics::plotLimits::updateLimitsXYForGraphic( limitsXY *Limits, const graph &Graph, const graphCollection &GraphCollection ) const
{
  assert( Limits != NULL );

  const axisSet *AxisSetX = GraphCollection.graphAxisSetX(&Graph);
  const axisSet *AxisSetY = GraphCollection.graphAxisSetY(&Graph);

  assert( AxisSetX != NULL );
  assert( AxisSetY != NULL );

  numberLimits GraphLimitsX = Graph.limitsX();
  GraphLimitsX = applyForcedLimits( AxisSetX, GraphLimitsX );

  numberLimits GraphLimitsY = Graph.limitsY( GraphLimitsX.totalLimits() );
  GraphLimitsY = applyForcedLimits( AxisSetY, GraphLimitsY );

  Limits->updateLimits( AxisSetX, GraphLimitsX );
  Limits->updateLimits( AxisSetY, GraphLimitsY );
}

// ------------------------------------------------------------
      
void scigraphics::plotLimits::correctLimitsXYForAxisSet( limitsXY *Limits, const axisSet *AxisSet ) const
{
  assert( Limits != NULL );
    
  numberLimits AxisLimits = Limits->get( AxisSet );
  AxisLimits = correctInvalidLimits( AxisLimits );
  AxisLimits = applyForcedLimits( AxisSet, AxisLimits );
  Limits->set( AxisSet, AxisLimits );
}

// ============================================================

