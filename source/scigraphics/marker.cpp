
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

#include "scigraphics/marker.h"

#include <limits>
#include <cmath>
#include <cassert>
#include <iostream>

// ============================================================
      
scigraphics::marker::marker() : 
  NumberOfMarks( 4, 9 ) 
{
}

// ------------------------------------------------------------
      
scigraphics::marker::~marker() 
{
}

// ------------------------------------------------------------
      
void scigraphics::marker::setNumberOfMarks( size_t Min, size_t Max ) 
{ 
  NumberOfMarks.setMinMax( Min, Max ); 
}

// ------------------------------------------------------------

scigraphics::interval<size_t> scigraphics::marker::numberOfMarks() const 
{ 
  return NumberOfMarks; 
}

// ------------------------------------------------------------
      
scigraphics::number scigraphics::marker::baseOrder( number Value )
{ 
  return std::floor( std::log10( Value ) ); 
}

// ------------------------------------------------------------

scigraphics::number scigraphics::marker::epsilonForInterval( interval<number> Interval )
{
  return std::numeric_limits<number>::epsilon() * Interval.distance() * 3;
}

// ------------------------------------------------------------

bool scigraphics::marker::inInterval( number Value, interval<number> Interval )
{
  number Epsilon = 3*epsilonForInterval(Interval);
  interval<number> ExtendedInterval( Interval.min() - Epsilon, Interval.max() + Epsilon );
  return ExtendedInterval.inInterval(Value);
}

// ------------------------------------------------------------

bool scigraphics::marker::vectorInMarkLimits( const std::vector<number> &Marks ) const
{
  return numberOfMarks().inInterval( Marks.size() );
}

// ============================================================
      
std::vector<scigraphics::number> scigraphics::markerLinear::marksWithStep( const interval<number> Interval, number Step ) const
{
  assert( Step > 0 );

  std::vector<scigraphics::number> Result;
  
  number MinValue = std::ceil( Interval.min() / Step - 1.0 ) * Step;
  number MaxValue = Interval.max() + 2*Step;
  number Epsilon  = epsilonForInterval(Interval);

  for ( size_t i = 0; true; i++ )  
  {
    number Number = MinValue + i * Step;
    
    if ( Number > MaxValue )
      break;
    if ( i > 1024 )
      break;

    if ( -Epsilon < Number && Number < Epsilon )
      Number = 0;

    if ( inInterval( Number, Interval ) )
       Result.push_back( Number );
  }

  return Result;
}

// ------------------------------------------------------------

std::vector<scigraphics::number> scigraphics::markerLinear::marksWithStepVector( interval<number> Interval, const number Steps[] ) const
{
  assert( Steps != NULL );

  std::vector<number> Result;
  
  if ( ! isValidNumbers( Interval.min(), Interval.max() ) )
    Interval = interval<number>( -1, +1 );
    
  number BaseOrder = baseOrder( Interval.distance() );
  number BeginOrder = std::pow( 10, BaseOrder - 2 );

  assert( BeginOrder > 0 );

  for ( number Order = BeginOrder; Order < Interval.distance() * 10; Order *= 10 )
  {
    for ( const number *Step = &Steps[0]; *Step != 0; ++Step )
    {
      Result = marksWithStep( Interval, *Step * Order );
      if ( vectorInMarkLimits(Result) )
        return Result;
    }
  }

  return Result;
}

// ------------------------------------------------------------

std::vector<scigraphics::number> scigraphics::markerLinear::marks( const interval<number> Interval ) const 
{ 
  std::vector<number> Result;

  const number GoodSteps[] = { 1, 2, 5, 0 };
  const number BadSteps[]  = { 2.5, 7.5, 1.25, 3.75, 0 };

  Result = marksWithStepVector( Interval, GoodSteps );
  if ( vectorInMarkLimits(Result) )
     return Result;
  
  return marksWithStepVector( Interval, BadSteps );
}

// ============================================================

std::vector<scigraphics::number> scigraphics::markerLogarithm::marksInPositiveInterval( const interval<number> Interval ) const
{
  assert( Interval.min() > 0 );
  assert( Interval.max() > 0 );

  std::vector<number> Result;

  const number Steps[][10] =
  {
    { 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 },
    { 1, 2, 5, 0 },
    { 1, 0 },
    { 1, 5, 0 },
    { 0 }
  };

  const number UnitStep[] = { 1, 0 };

  for ( size_t i = 0; Steps[i][0] != 0; i++ )
  {
    Result = marksWithSteps( Interval, Steps[i], 10 );
    if ( vectorInMarkLimits(Result) )
      return Result;
  }

  for ( unsigned OrderStep = 100; OrderStep <= 100000; OrderStep *= 10 )
  {
    Result = marksWithSteps( Interval, UnitStep, OrderStep );
    if ( vectorInMarkLimits(Result) )
      return Result;
  }

  return Result;
}

// ------------------------------------------------------------
      
std::vector<scigraphics::number> scigraphics::markerLogarithm::marksWithSteps( const interval<number> Interval, const number Steps[], number OrderStep ) const
{
  assert( Steps != NULL );
  assert( OrderStep > 1 );
  
  std::vector<scigraphics::number> Result;

  number BaseOrder = std::max( baseOrder( Interval.min() ) - 2, baseOrder( std::numeric_limits<number>::min() ) * 0.7 );
  number BeginOrderValue = std::pow( 10, BaseOrder );
  number EndOrderValue = Interval.max();

  assert( BeginOrderValue > 0 );

  for ( number Order = BeginOrderValue; Order <= EndOrderValue; Order *= OrderStep )
  {
    for ( size_t i = 0; Steps[i] != 0; i++ )
    {
      number Number = Order * Steps[i];
      if ( inInterval(Number,Interval) )
        Result.push_back( Number );
    }
  }

  return Result;
}

// ============================================================

std::vector<scigraphics::number> scigraphics::markerLogarithmPositive::marks( const interval<number> Interval ) const
{
  if ( Interval.max() <= 0 )
    return marks( interval<number>(0.1,1) );
  if ( Interval.min() <= 0 )
    return marks( interval<number>( Interval.max()*1e-5, Interval.max() ) );
  return marksInPositiveInterval( Interval );
}

// ============================================================

std::vector<scigraphics::number> scigraphics::markerLogarithmNegative::marks( const interval<number> Interval ) const
{
  std::vector<number> Result;
  if ( Interval.min() >= 0 )
    Result = marks( interval<number>(-1,-0.1) );
  else if ( Interval.max() >= 0 )
    Result = marks( interval<number>( Interval.min()*1e-5, Interval.min() ) );
  else
    Result = marksInPositiveInterval( interval<number>( -Interval.min(), -Interval.max() ) );

  for ( size_t i = 0; i < Result.size(); i++ )
    Result[i] *= -1;

  return Result;
}

// ============================================================


