
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

#include "scigraphics/datamap.h"

#include <limits>
#include <stdexcept>
#include <ostream>
#include <cmath>
#include <cassert>

// ============================================================

scigraphics::map::data::data( interval<number> IX, interval<number> IY ) : 
  IntervalX(IX), 
  IntervalY(IY) 
{
}

// ------------------------------------------------------------

scigraphics::map::data::~data() 
{
}

// ------------------------------------------------------------
        
scigraphics::interval<scigraphics::number> scigraphics::map::data::intervalX() const 
{ 
  return IntervalX; 
}

// ------------------------------------------------------------

scigraphics::interval<scigraphics::number> scigraphics::map::data::intervalY() const 
{ 
  return IntervalY; 
}

// ------------------------------------------------------------

void scigraphics::map::data::setIntervalX( number Min, number Max )
{ 
  setIntervalX( interval<number>(Min,Max) ); 
}

// ------------------------------------------------------------

void scigraphics::map::data::setIntervalY( number Min, number Max ) 
{ 
  setIntervalY( interval<number>(Min,Max) ); 
}

// ------------------------------------------------------------

void scigraphics::map::data::setIntervalX( interval<number> Interval ) 
{ 
  IntervalX = Interval; 
}

// ------------------------------------------------------------

void scigraphics::map::data::setIntervalY( interval<number> Interval ) 
{ 
  IntervalY = Interval; 
}

// ------------------------------------------------------------
        
const scigraphics::map::data::point_t scigraphics::map::data::at( int_t Index ) const
{
  if ( Index < 0 )
    throw std::out_of_range( "scigraphics::data index is negative" );
  
  const int_t Size = size();
  if ( Index >= Size )
    throw std::out_of_range( "scigraphics::data index is out of range" );

  return get(Index);
}

// ------------------------------------------------------------
        
const scigraphics::map::data::point_t scigraphics::map::data::first() const 
{ 
  const int_t Size = size();
  if ( Size <= 0 )
    return point_t();
  return get(0);
}

// ------------------------------------------------------------

const scigraphics::map::data::point_t scigraphics::map::data::last() const 
{ 
  const int_t Size = size();
  if ( Size <= 0 )
    return point_t();
  return get( Size - 1 );
}

// ------------------------------------------------------------

scigraphics::numberLimits scigraphics::map::data::limitsForInterval( interval<number> Interval )
{
  numberLimits Limits;

  if ( ! numberLimits::isValidInterval(Interval) )
    return Limits;

  Limits.updateLimits( Interval.min() );
  if ( Interval.min() * Interval.max() <= 0 )
  {
    Limits.updateLimits( Interval.min() * 1e-7 );
    Limits.updateLimits( Interval.max() * 1e-7 );
  }
  Limits.updateLimits( Interval.max() );

  return Limits;
}

// ------------------------------------------------------------

const scigraphics::numberLimits scigraphics::map::data::limitsX() const
{
  return limitsForInterval( intervalX() );
}

// ------------------------------------------------------------

const scigraphics::numberLimits scigraphics::map::data::limitsY( const interval<number> & ) const
{
  return limitsForInterval( intervalY() );
}

// ------------------------------------------------------------

const scigraphics::numberLimits scigraphics::map::data::limitsZ() const
{
  numberLimits Limits;
  for ( iterator p = begin(); p != end(); ++p )
    Limits.updateLimits( p->z() );
  return Limits;
}

// ============================================================
  
scigraphics::map::dataVector::limitsZCache::limitsZCache() :
  IsValid( false )
{
}

// ------------------------------------------------------------

scigraphics::map::dataVector::dataVector( size_t SX, interval<number> IX, size_t SY, interval<number> IY ) :
  map::data( IX, IY ),
  SizeX(0), 
  SizeY(0)
{
  resize( SX, SY );
}

// ------------------------------------------------------------

scigraphics::map::dataVector::dataVector() :
  map::data( interval<number>(0,1), interval<number>(0,1) ),
  SizeX(0), 
  SizeY(0)
{
}

// ------------------------------------------------------------

void scigraphics::map::dataVector::resize( size_t SX, size_t SY )
{
  std::vector< value > NewValues( SX * SY );
  
  for ( size_t ix = 0; ix < std::min<size_t>( SX, sizeX() ); ix++ )
  {
    for ( size_t iy = 0; iy < std::min<size_t>( SY, sizeY() ); iy++ )
    {
      size_t Idx = ix + iy * SX;
      NewValues.at( Idx ) = Values.at( index( static_cast<int_t>(ix), static_cast<int_t>(iy) ) );
    }
  }

  SizeX = SX;
  SizeY = SY;
  Values = NewValues;
}

// ------------------------------------------------------------

scigraphics::map::dataVector::int_t scigraphics::map::dataVector::size() const
{
  assert( static_cast<size_t>(sizeX()) * static_cast<size_t>(sizeY()) == Values.size() );
  return static_cast<int_t>( Values.size() );
}

// ------------------------------------------------------------

const scigraphics::map::dataVector::point_t scigraphics::map::dataVector::get( int_t Index ) const
{
  assert( Index >= 0 );
  assert( Index < static_cast<int_t>(Values.size()) );

  const value Val = Values[Index];
  number X = coordinateX(indexX(Index));
  number DX = deltaX();
  number Y = coordinateY(indexY(Index));
  number DY = deltaY();
  return point_t( X, DX, Y, DY, Val.z(), Val.errZ() );
}

// ------------------------------------------------------------
        
scigraphics::number scigraphics::map::dataVector::coordinateX( int_t IndexX ) const 
{ 
  return intervalX().min() + deltaX()*IndexX; 
}

// ------------------------------------------------------------

scigraphics::number scigraphics::map::dataVector::coordinateY( int_t IndexY ) const 
{ 
  return intervalY().min() + deltaY()*IndexY; 
}

// ------------------------------------------------------------

scigraphics::map::dataVector::int_t scigraphics::map::dataVector::nearestIndexX( number X ) const 
{ 
  return static_cast<int_t>( ( X - intervalX().min() )/deltaX() ); 
}

// ------------------------------------------------------------

scigraphics::map::dataVector::int_t scigraphics::map::dataVector::nearestIndexY( number Y ) const 
{ 
  return static_cast<int_t>( ( Y - intervalY().min() )/deltaY() ); 
}

// ------------------------------------------------------------
      
void scigraphics::map::dataVector::set( int_t IndexX, int_t IndexY, number Z, number ErrZ )
{
  const int_t Index = index( IndexX, IndexY );

  if ( Index < 0 )
    throw std::runtime_error( "Index is negative" );

  if ( Index >= static_cast<int_t>(Values.size()) )
    throw std::runtime_error( "Index out of range" );

  Values[Index] = value( Z, ErrZ );
  LimitsZCache.IsValid = false;
}

// ------------------------------------------------------------

const scigraphics::numberLimits scigraphics::map::dataVector::limitsZ() const 
{
  if ( ! LimitsZCache.IsValid )
  {
    LimitsZCache.LimitsZ = numberLimits();
    iterator Begin = begin(), End = end();
    for ( iterator i = Begin; i != End; ++i )
      LimitsZCache.LimitsZ.updateLimits( i->z() );
    LimitsZCache.IsValid = true;
  }
  return LimitsZCache.LimitsZ;
}

// ------------------------------------------------------------

std::ostream& scigraphics::map::operator<<( std::ostream& Stream, const point& Point )
{
  return ( Stream << "( " << Point.x0() << " " << Point.x1() << " " << Point.y0() << " " << Point.y1() << " : " << Point.z() << " )" );
}

// ------------------------------------------------------------

std::ostream& scigraphics::map::operator<<( std::ostream& Stream, const data& Data )
{
  std::streamsize Width = Stream.width();
  Stream.width( 12 );
  Stream << "Size " << Data.size() << std::endl;
  for ( data::iterator i = Data.begin(); i != Data.end(); ++i )
    Stream << (*i) << std::endl;
  //Stream << "Limits: X" << Data.limitsX() << " Y" << Data.limitsY( Data.limitsX().totalLimits() ) << std::endl;
  Stream.width( Width );

  return Stream;
}
// ============================================================

