
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
#include <cmath>
#include <cassert>
#include <stdexcept>

// ============================================================

scigraphics::numberLimits scigraphics::map2d::data::limitsForInterval( interval<number> Interval )
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

const scigraphics::numberLimits scigraphics::map2d::data::limitsX() const
{
  return limitsForInterval( intervalX() );
}

// ------------------------------------------------------------

const scigraphics::numberLimits scigraphics::map2d::data::limitsY( const interval<number> & ) const
{
  return limitsForInterval( intervalY() );
}

// ------------------------------------------------------------

const scigraphics::numberLimits scigraphics::map2d::data::limitsZ() const
{
  numberLimits Limits;
  for ( iterator p = begin(); p != end(); ++p )
    Limits.updateLimits( p->z() );
  return Limits;
}

// ============================================================

scigraphics::map2d::dataVector::dataVector( size_t SX, interval<number> IX, size_t SY, interval<number> IY ) :
  map2d::data( IX, IY ),
  SizeX(0), 
  SizeY(0)
{
  resize( SX, SY );
}

// ------------------------------------------------------------

scigraphics::map2d::dataVector::dataVector() :
  map2d::data( interval<number>(0,1), interval<number>(0,1) ),
  SizeX(0), 
  SizeY(0)
{
}

// ------------------------------------------------------------

void scigraphics::map2d::dataVector::resize( size_t SX, size_t SY )
{
  std::vector< value > NewValues( SX * SY );
  
  for ( unsigned ix = 0; ix < std::min<size_t>( SX, sizeX() ); ix++ )
  {
    for ( unsigned iy = 0; iy < std::min<size_t>( SY, sizeY() ); iy++ )
    {
      unsigned Idx = ix + iy * SX;
      NewValues.at( Idx ) = Values.at( index( ix, iy ) );
    }
  }

  SizeX = SX;
  SizeY = SY;
  Values = NewValues;
}

// ------------------------------------------------------------

scigraphics::map2d::dataVector::int_t scigraphics::map2d::dataVector::size() const
{
  assert( sizeX()*sizeY() == Values.size() );
  return Values.size();
}

// ------------------------------------------------------------

const scigraphics::map2d::dataVector::point_t scigraphics::map2d::dataVector::at( int Index ) const
{
  value Val = Values.at(Index);
  number X = coordinateX(indexX(Index));
  number DX = deltaX();
  number Y = coordinateY(indexY(Index));
  number DY = deltaY();
  return point_t( X, DX, Y, DY, Val.z(), Val.errZ() );
}

// ------------------------------------------------------------
      
void scigraphics::map2d::dataVector::set( int IndexX, int IndexY, number Z, number ErrZ )
{
  unsigned Index = index( IndexX, IndexY );
  Values.at(Index) = value( Z, ErrZ );
  LimitsZCache.isValid = false;
}

// ------------------------------------------------------------

const scigraphics::numberLimits scigraphics::map2d::dataVector::limitsZ() const 
{
  if ( ! LimitsZCache.isValid )
  {
    LimitsZCache.LimitsZ = numberLimits();
    iterator Begin = begin(), End = end();
    for ( iterator i = Begin; i != End; ++i )
      LimitsZCache.LimitsZ.updateLimits( i->z() );
    LimitsZCache.isValid = true;
  }
  return LimitsZCache.LimitsZ;
}

// ------------------------------------------------------------

std::ostream& scigraphics::map2d::operator<<( std::ostream& Stream, const point& Point )
{
  return ( Stream << "( " << Point.x0() << " " << Point.x1() << " " << Point.y0() << " " << Point.y1() << " : " << Point.z() << " )" );
}

// ------------------------------------------------------------

std::ostream& scigraphics::map2d::operator<<( std::ostream& Stream, const data& Data )
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

