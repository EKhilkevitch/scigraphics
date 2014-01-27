
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

namespace
{

  // ------------------------------------------------------------
  
  bool needToRecalculateLimits( const scigraphics::interval<scigraphics::number> Interval );
  void updateLimitsByValue( scigraphics::numberLimits *Limits, scigraphics::number Value, scigraphics::number PosDistance, scigraphics::number NegDistance );
  
  // ------------------------------------------------------------
  
  bool needToRecalculateLimits( const scigraphics::interval<scigraphics::number> Interval )
  {
    const double Epsilon = 1e-14;

    scigraphics::number Distance = Interval.distance();
    assert( Distance >= 0 );
    if ( Distance == 0 )
      return false;
    
    scigraphics::number MinAbs = Interval.minAbs();
    assert( MinAbs >= 0 );
    return ( MinAbs < Epsilon * Distance );
  }
  
  // ------------------------------------------------------------
  
  void updateLimitsByValue( scigraphics::numberLimits *Limits, scigraphics::number Value, scigraphics::number PosDistance, scigraphics::number NegDistance )
  {
    assert( Limits != NULL );
    assert( PosDistance >= 0 );
    assert( NegDistance >= 0 );

    const double DistMultiplier = 1e-15;

    scigraphics::number MinPosValue = + DistMultiplier * PosDistance;
    scigraphics::number MaxNegValue = - DistMultiplier * NegDistance;

    if ( MaxNegValue <= Value && Value <= MinPosValue )
      Value = 0;
  
    Limits->updateLimits( Value );
  }

  // ------------------------------------------------------------
}

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
        
scigraphics::sequence::dataVector::dataVector() : 
  OrderedByX(true) 
{
}

// ------------------------------------------------------------

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

  if ( Points.empty() ) 
  {
    OrderedByX = true;
    return; 
  }

  point_t Last = Points[ Points.size()-1 ];

  if ( Points.size() <= 1 )
  {
    OrderedByX = isValidNumber( Last.x() );
    return;
  }

  if ( ! isValidNumber( Last.x() ) )
  {
    OrderedByX = false;
    return;
  }

  point_t BeforeLast = Points[ Points.size()-1 ];
  OrderedByX = ( Last.x() >= BeforeLast.x() );
  return;
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

  const number PosDistance = Limits->positiveLimits().distance();
  const number NegDistance = Limits->negativeLimits().distance();

  *Limits = numberLimits();

  for ( std::vector<point_t>::const_iterator p = Points.begin(); p != Points.end(); ++p )
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

// ============================================================

scigraphics::sequence::dataUniformVector::dataUniformVector() :
  StepX( 1.0 )
{
}

// ------------------------------------------------------------
        
const scigraphics::sequence::dataUniformVector::point_t scigraphics::sequence::dataUniformVector::at( int_t Index ) const
{
  assert( Values.size() == Errors.size() );

  number X    = valueX( Index ); 
  number ErrX = 0.0;
  number Y    = valueY( Index );
  number ErrY = errorY( Index );

  return point_t( X, Y, ErrX, ErrY );
}

// ------------------------------------------------------------
        
void scigraphics::sequence::dataUniformVector::append( number Y, number ErrY )
{
  if ( ErrY == invalidNumber() || Y == invalidNumber() )
    Y = ErrY = invalidNumber();

  Values.push_back( Y );
  Errors.push_back( ErrY );
  updateLimits();
}

// ------------------------------------------------------------

void scigraphics::sequence::dataUniformVector::setStepX( number StepX )
{
  if ( StepX <= 0 )
    throw std::invalid_argument("StepX must be positive");
  this->StepX = StepX;
  recalculateLimits();
}

// ------------------------------------------------------------
        
void scigraphics::sequence::dataUniformVector::updateLimits()
{
  assert( Values.size() == Errors.size() );

  if ( Values.empty() )
  {
    LimitsY = numberLimits();
    return;
  }

  number LastY = Values.back();
  number LastErr = Errors.back();

  LimitsY.updateLimits( LastY + LastErr );
  LimitsY.updateLimits( LastY - LastErr );
  LimitsX.updateLimits( valueX( Values.size()-1 ) );

  if ( needToRecalculateLimits( LimitsY.positiveLimits() ) || needToRecalculateLimits( LimitsY.negativeLimits() ) )  
    recalculateLimits();
}

// ------------------------------------------------------------

void scigraphics::sequence::dataUniformVector::recalculateLimits()
{
  assert( Values.size() == Errors.size() );

  const int_t Size = size();
  const number PosDistanceY = LimitsY.positiveLimits().distance();
  const number NegDistanceY = LimitsY.negativeLimits().distance();
  const number PosDistanceX = LimitsX.positiveLimits().distance();
  const number NegDistanceX = LimitsX.negativeLimits().distance();

  LimitsY = LimitsX = numberLimits();
  for ( int_t i = 0; i < Size; i++ )
  {
    number Y = valueY( i );
    number Err = errorY( i );
    number X = valueX( i );

    if ( isValidNumbers( Y, Err ) )
    {
      updateLimitsByValue( &LimitsY, Y + Err, PosDistanceY, NegDistanceY );
      updateLimitsByValue( &LimitsY, Y - Err, PosDistanceY, NegDistanceY );
      updateLimitsByValue( &LimitsX, X, PosDistanceX, NegDistanceX );
    }
  }
}

// ------------------------------------------------------------        

void scigraphics::sequence::dataUniformVector::clear()
{
  LimitsX = LimitsY = numberLimits();
  Values.clear();
  Errors.clear();
}

// ------------------------------------------------------------        

const scigraphics::numberLimits scigraphics::sequence::dataUniformVector::limitsY( const interval<number> &LimitsX ) const
{
  if ( LimitsX.min() <= limitsX().totalLimits().min() &&
       LimitsX.max() >= limitsX().totalLimits().max() )
    return LimitsY;
  return data::limitsY( LimitsX );
}

// ============================================================

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

 
