
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

#include "scigraphics/datasequence.h"

#include <limits>
#include <cmath>
#include <cassert>
#include <stdexcept>

// ============================================================

const scigraphics::numberLimits scigraphics::sequence::data::limitsX() const
{
  numberLimits Result;
  
  const iterator Begin = begin();
  const iterator End   = end();

  for ( iterator p = Begin; p != End; ++p )
  {
    if ( ! p->isValid() )
      continue;

    Result.updateLimits( p->x() + p->errX() );
    Result.updateLimits( p->x() - p->errX() );
  }

  return Result;
}

// ------------------------------------------------------------

const scigraphics::numberLimits scigraphics::sequence::data::limitsY( const interval<number> &LimitsX ) const
{
  if ( ! numberLimits::isValidInterval(LimitsX) )
    return numberLimits();

  numberLimits Result;
  
  const iterator Begin = begin();
  const iterator End   = end();

  for ( iterator p = Begin; p != End; ++p )
  {
    if ( ! p->isValid() )
      continue;

    if ( ! LimitsX.inInterval( p->x() ) )
      continue;

    Result.updateLimits( p->y() + p->errY() );
    Result.updateLimits( p->y() - p->errY() );
  }

  return Result;
}

// ============================================================

void scigraphics::sequence::dataVector::append( const point_t &Point )
{ 
  appendPoint( Point );
  if ( Points.size() >= (size_t)std::numeric_limits<int>::max() - 2 )
    throw std::runtime_error( "Data size is too big" );

  if ( Point.isValid() )
  {
    updateLimitsXY( Point, CrdX );
    updateLimitsXY( Point, CrdY );
  }
  updateOrderedByX();
}

// ------------------------------------------------------------

void scigraphics::sequence::dataVector::updateOrderedByX()
{
  if ( ! OrderedByX )      
    return;

  if ( size() <= 1 ) 
    return; 

  if ( ! isValidNumber( last().x() ) )
  {
    OrderedByX = false;
    return;
  }
  
  number LastValidX = invalidNumber();
  int DataSize = size();
  for ( int i = DataSize-2; i >= 0; i-- )
  {
    if ( isValidNumber( Points[i].x() ) )
    {
      LastValidX = Points[i].x();
      break;
    }
  }
    
  if ( isValidNumber(LastValidX) && LastValidX > last().x() )
    OrderedByX = false;
}

// ------------------------------------------------------------

void scigraphics::sequence::dataVector::updateLimitsXY( const point_t &Point, const coordinateType Type )
{
  if ( ! Point.isValid() )
    return;

  numberLimits &Limits = ( Type == CrdX ? LimitsX : LimitsY );
  number Value         = pointValue(Point,Type);
  number Error         = pointError(Point,Type);

  updateLimits( Value + Error, Type, &Limits );
  updateLimits( Value - Error, Type, &Limits );
}

// ------------------------------------------------------------

void scigraphics::sequence::dataVector::updateLimits( number Number, coordinateType Coordinate, numberLimits *Limits )
{
  assert( Limits != NULL );
  assert( isValidNumber(Number) );

  Limits->updateLimits( Number );

  if ( needToRecalculateLimits( Limits->positiveLimits() ) || needToRecalculateLimits( Limits->negativeLimits() ) )
    recalculateLimits( Coordinate, Limits );
}

// ------------------------------------------------------------

void scigraphics::sequence::dataVector::recalculateLimits( coordinateType Coordinate, numberLimits *Limits )
{
  assert( Limits != NULL );

  number PosDistance = Limits->positiveLimits().distance();
  number NegDistance = Limits->negativeLimits().distance();

  *Limits = numberLimits();

  const iterator Begin = begin();
  const iterator End   = end();

  for ( iterator p = Begin; p != End; ++p )
  {
    if ( ! p->isValid() )
      continue;

    number Value = pointValue(*p,Coordinate);
    number Error = pointError(*p,Coordinate);

    updateLimitsByValue( Limits, Value + Error, PosDistance, NegDistance );
    updateLimitsByValue( Limits, Value - Error, PosDistance, NegDistance );
  }

}

// ------------------------------------------------------------

void scigraphics::sequence::dataVector::updateLimitsByValue( numberLimits *Limits, number Value, number PosDistance, number NegDistance )
{
  assert( Limits != NULL );
  assert( PosDistance >= 0 );
  assert( NegDistance >= 0 );

  const double DistMultiplier = 1e-15;

  number MinPosValue = + DistMultiplier * PosDistance;
  number MaxNegValue = - DistMultiplier * NegDistance;

  if ( MaxNegValue <= Value && Value <= MinPosValue )
    Value = 0;

  Limits->updateLimits( Value );
}

// ------------------------------------------------------------

bool scigraphics::sequence::dataVector::needToRecalculateLimits( const interval<number> Interval )
{
  const double Epsilon = 1e-14;

  number Distance = Interval.distance();
  assert( Distance >= 0 );
  if ( Distance == 0 )
    return false;
  
  number MinAbs   = Interval.minAbs();
  assert( MinAbs   >= 0 );
  return ( MinAbs < Epsilon * Distance );
}

// ------------------------------------------------------------

const scigraphics::numberLimits scigraphics::sequence::dataVector::limitsY( const interval<number> &LimitsX ) const
{
  if ( LimitsX.min() <= limitsX().totalLimits().min() &&
       LimitsX.max() >= limitsX().totalLimits().max() )
    return LimitsY;
  return data::limitsY( LimitsX );
}

// ------------------------------------------------------------

scigraphics::number scigraphics::sequence::dataVector::pointValue( const point_t &Point, coordinateType Type )
{
  switch ( Type )
  {
    case CrdX: return Point.x();
    case CrdY: return Point.y();
    default: abort(); return 0;
  }
}

// ------------------------------------------------------------

scigraphics::number scigraphics::sequence::dataVector::pointError( const point_t &Point, coordinateType Type )
{
  if ( ! Point.isValidError() )
    return 0;

  switch ( Type )
  {
    case CrdX: return Point.errX();
    case CrdY: return Point.errY();
    default: abort(); return 0;
  }
}

// ------------------------------------------------------------

void scigraphics::sequence::dataVector::clear()
{
  LimitsX = LimitsY = numberLimits();
  OrderedByX = true;
  Points.clear();
}

// ------------------------------------------------------------

std::ostream& scigraphics::sequence::operator<<( std::ostream& Stream, const point& Point )
{
  return ( Stream << "( " << Point.x() << " " << Point.y() << " " << Point.errX() << " " << Point.errY() << " )" );
}
  
// ------------------------------------------------------------

std::ostream& operator<<( std::ostream& Stream, const scigraphics::sequence::data& Data )
{
  std::streamsize Width = Stream.width();

  Stream.width( 12 );
  Stream << "Size ";
  Stream << Data.size() << std::endl;
  for ( scigraphics::sequence::data::iterator i = Data.begin(); i != Data.end(); ++i )
    Stream << (*i) << std::endl;
  Stream.width( Width );

  return Stream;
}

// ============================================================
    
