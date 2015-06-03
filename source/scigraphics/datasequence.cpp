
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
  
  struct numberPointXCmp
  {
    bool operator()( const scigraphics::sequence::point &Point, scigraphics::number Number ) { return Point.x() < Number; }
    bool operator()( scigraphics::number Number, const scigraphics::sequence::point &Point ) { return Number < Point.x(); }
  };
  
  // ------------------------------------------------------------

}

// ============================================================

scigraphics::sequence::data::~data()
{
}

// ------------------------------------------------------------

const scigraphics::sequence::data::point_t scigraphics::sequence::data::first() const 
{ 
  return  empty() ? point_t() : at(0); 
}

// ------------------------------------------------------------

const scigraphics::sequence::data::point_t scigraphics::sequence::data::last()  const 
{ 
  return  empty() ? point_t() : at(size()-1); 
}

// ------------------------------------------------------------

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
 
  iterator Begin = begin();
  iterator End   = end();

  if ( isOrderedByX() )
  {
    std::pair<iterator,iterator> BeginEndInLimitsX = numberLimits::boundsOfInterval( Begin, End, LimitsX.min(), LimitsX.max(), numberPointXCmp() );
    Begin = BeginEndInLimitsX.first;
    End = BeginEndInLimitsX.second;
  }

  numberLimits Result;
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

scigraphics::sequence::numberLimitsDataCache::numberLimitsDataCache() :
  RecalculateRequest( false )
{
}

// ------------------------------------------------------------

void scigraphics::sequence::numberLimitsDataCache::clear()
{
  LimitsX = LimitsY = numberLimits();
}

// ------------------------------------------------------------

bool scigraphics::sequence::numberLimitsDataCache::canUseCachedLimitsY( const interval<number> &LimitsX ) const
{
  return ( LimitsX.min() <= limitsX().totalLimits().min() ) &&
         ( LimitsX.max() >= limitsX().totalLimits().max() );
}

// ------------------------------------------------------------

void scigraphics::sequence::numberLimitsDataCache::update( const point_t &Point, const data &Data )
{
  if ( ! Point.isValid() )
    return;
    
  updateLimits( Point, CoordinateX, Data, &LimitsX );
  updateLimits( Point, CoordinateY, Data, &LimitsY );
}
        
// ------------------------------------------------------------

void scigraphics::sequence::numberLimitsDataCache::recalculate( const data &Data )
{
  const number PosDistanceY = LimitsY.positiveLimits().distance();
  const number NegDistanceY = LimitsY.negativeLimits().distance();
  const number PosDistanceX = LimitsX.positiveLimits().distance();
  const number NegDistanceX = LimitsX.negativeLimits().distance();

  const iterator Begin = Data.begin();
  const iterator End = Data.end();

  LimitsY = LimitsX = numberLimits();
  for ( iterator p = Begin; p != End; ++p )
  {
    if ( p->isValid() )
    {
      updateByValue( p->y() + p->errY(), PosDistanceY, NegDistanceY, &LimitsY );
      if ( p->errY() > 0 )
        updateByValue( p->y() - p->errY(), PosDistanceY, NegDistanceY, &LimitsY );

      updateByValue( p->x() + p->errX(), PosDistanceX, NegDistanceX, &LimitsX );
      if ( p->errX() > 0 )
        updateByValue( p->x() - p->errX(), PosDistanceX, NegDistanceX, &LimitsX );
    }
  }

  RecalculateRequest = false;
}

// ------------------------------------------------------------

void scigraphics::sequence::numberLimitsDataCache::setRecalculateRequest()
{
  RecalculateRequest = true;
}

// ------------------------------------------------------------

void scigraphics::sequence::numberLimitsDataCache::recalculateIfRequired( const data &Data )
{
  if ( RecalculateRequest )
    recalculate( Data );
  RecalculateRequest = false;
}

// ------------------------------------------------------------

void scigraphics::sequence::numberLimitsDataCache::updateLimits( const point_t &Point, const coordinateType Coordinate, const data &Data, numberLimits *Limits )
{
  assert( Limits != NULL );
  
  number Value = pointValue( Point, Coordinate );
  number Error = pointError( Point, Coordinate );

  updateLimits( Value + Error, Coordinate, Data, Limits );
  if ( Error != 0 )
    updateLimits( Value - Error, Coordinate, Data, Limits );
}

// ------------------------------------------------------------

void scigraphics::sequence::numberLimitsDataCache::updateLimits( number Number, const coordinateType Coordinate, const data &Data, numberLimits *Limits )
{
  assert( Limits != NULL );
  assert( isValidNumber(Number) );

  Limits->updateLimits( Number );

  if ( needToRecalculate( Limits->positiveLimits() ) || needToRecalculate( Limits->negativeLimits() ) )
    recalculate( Coordinate, Data, Limits );
}

// ------------------------------------------------------------

bool scigraphics::sequence::numberLimitsDataCache::needToRecalculate( const interval<number> Interval )
{
  const number Epsilon = std::numeric_limits<number>::epsilon();

  number Distance = Interval.distance();
  assert( Distance >= 0 );
  if ( Distance == 0 )
    return false;
  
  scigraphics::number MinAbs = Interval.minAbs();
  assert( MinAbs >= 0 );
  return ( MinAbs < Epsilon * Distance );
}

// ------------------------------------------------------------
        
void scigraphics::sequence::numberLimitsDataCache::recalculate( const coordinateType Coordinate, const data &Data, numberLimits *Limits )
{
  assert( Limits != NULL );

  const number PosDistance = Limits->positiveLimits().distance();
  const number NegDistance = Limits->negativeLimits().distance();

  *Limits = numberLimits();

  iterator Begin = Data.begin();
  iterator End = Data.end();

  for ( iterator p = Begin; p != End; ++p )
  {
    if ( ! p->isValid() )
      continue;

    number Value = pointValue( *p, Coordinate );
    number Error = pointError( *p, Coordinate );

    updateByValue( Value + Error, PosDistance, NegDistance, Limits );
    if ( Error != 0 )
      updateByValue( Value - Error, PosDistance, NegDistance, Limits );
  }
}

// ------------------------------------------------------------
        
void scigraphics::sequence::numberLimitsDataCache::updateByValue( number Value, number PosDistance, number NegDistance, numberLimits *Limits )
{
  assert( Limits != NULL );
  assert( PosDistance >= 0 );
  assert( NegDistance >= 0 );

  const number DistMultiplier = std::numeric_limits<number>::epsilon();

  number MinPosValue = + DistMultiplier * PosDistance;
  number MaxNegValue = - DistMultiplier * NegDistance;

  if ( MaxNegValue <= Value && Value <= MinPosValue )
    Value = 0;

  Limits->updateLimits( Value );
}

// ------------------------------------------------------------

scigraphics::number scigraphics::sequence::numberLimitsDataCache::pointValue( const point_t &Point, coordinateType Type )
{
  switch ( Type )
  {
    case CoordinateX: 
      return Point.x();

    case CoordinateY: 
      return Point.y();

    default: 
      assert( false );
      return 0;
  }
}

// ------------------------------------------------------------

scigraphics::number scigraphics::sequence::numberLimitsDataCache::pointError( const point_t &Point, coordinateType Type )
{
  if ( ! Point.isValidError() )
    return 0;

  switch ( Type )
  {
    case CoordinateX: 
      return Point.errX();

    case CoordinateY:
      return Point.errY();

    default:
      assert( false );
      return 0;
  }
}

// ------------------------------------------------------------

const scigraphics::numberLimits& scigraphics::sequence::numberLimitsDataCache::limitsX() const
{
  return LimitsX;
}

// ------------------------------------------------------------

const scigraphics::numberLimits& scigraphics::sequence::numberLimitsDataCache::limitsY() const
{
  return LimitsY;
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

  LimitsCache.update( Point, *this );
  updateOrderedByX();
}
        
// ------------------------------------------------------------

void scigraphics::sequence::dataVector::append( number X, number Y ) 
{ 
  append( point_t(X,Y) );  
}

// ------------------------------------------------------------

void scigraphics::sequence::dataVector::append( number X, number Y, number ErrY ) 
{ 
  append( point_t(X,Y,0,ErrY) ); 
}

// ------------------------------------------------------------

void scigraphics::sequence::dataVector::append( number X, number Y, number ErrX, number ErrY ) 
{ 
  append( point_t(X,Y,ErrX,ErrY) ); 
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

  point_t Last = Points.back();

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

  point_t BeforeLast = Points[ Points.size()-2 ];
  OrderedByX = ( Last.x() >= BeforeLast.x() );
}

// ------------------------------------------------------------

const scigraphics::numberLimits scigraphics::sequence::dataVector::limitsY( const interval<number> &LimitsX ) const
{
  if ( LimitsCache.canUseCachedLimitsY(LimitsX) )
    return LimitsCache.limitsY();
  return data::limitsY( LimitsX );
}

// ------------------------------------------------------------

void scigraphics::sequence::dataVector::clear()
{
  OrderedByX = true;
  Points.clear();
  LimitsCache.clear();
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
  LimitsCache.update( last(), *this );
}

// ------------------------------------------------------------
        
void scigraphics::sequence::dataUniformVector::append( number Y ) 
{ 
  append( Y, 0 ); 
}

// ------------------------------------------------------------

void scigraphics::sequence::dataUniformVector::setStepX( number StepX )
{
  if ( StepX <= 0 )
    throw std::invalid_argument("StepX must be positive");
  this->StepX = StepX;
  LimitsCache.recalculate(*this);
}

// ------------------------------------------------------------

void scigraphics::sequence::dataUniformVector::clear()
{
  Values.clear();
  Errors.clear();
  LimitsCache.clear();
}

// ------------------------------------------------------------        

const scigraphics::numberLimits scigraphics::sequence::dataUniformVector::limitsY( const interval<number> &LimitsX ) const
{
  if ( LimitsCache.canUseCachedLimitsY(LimitsX) )
    return LimitsCache.limitsY();
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

 
