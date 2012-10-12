
// ============================================================

#include "graphics/datamap.h"

#include <limits>
#include <cmath>
#include <cassert>
#include <stdexcept>

// ============================================================

graphics::numberLimits graphics::dataMap::limitsForInterval( interval<number> Interval )
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

graphics::numberLimits graphics::dataMap::limitsX() const
{
  return limitsForInterval( intervalX() );
}

// ------------------------------------------------------------

graphics::numberLimits graphics::dataMap::limitsY( const interval<number> & ) const
{
  return limitsForInterval( intervalY() );
}

// ------------------------------------------------------------

graphics::numberLimits graphics::dataMap::limitsZ() const
{
  numberLimits Limits;
  for ( iterator p = begin(); p != end(); ++p )
    Limits.updateLimits( p->z() );
  return Limits;
}

// ============================================================

graphics::dataMapVector::dataMapVector( size_t SX, interval<number> IX, size_t SY, interval<number> IY ) :
  dataMap( IX, IY ),
  SizeX(0), 
  SizeY(0)
{
  resize( SX, SY );
}

// ------------------------------------------------------------

graphics::dataMapVector::dataMapVector() :
  dataMap( interval<number>(0,1), interval<number>(0,1) ),
  SizeX(0), 
  SizeY(0)
{
}

// ------------------------------------------------------------

void graphics::dataMapVector::resize( size_t SX, size_t SY )
{
  std::vector< value > NewValues( SX * SY );
  
  for ( unsigned ix = 0; ix < std::min( SX, sizeX() ); ix++ )
  {
    for ( unsigned iy = 0; iy < std::min( SY, sizeY() ); iy++ )
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

size_t graphics::dataMapVector::size() const
{
  assert( sizeX()*sizeY() == Values.size() );
  return Values.size();
}

// ------------------------------------------------------------

graphics::dataMapVector::point graphics::dataMapVector::at( int Index ) const
{
  value Val = Values.at(Index);
  number X = coordinateX(indexX(Index));
  number DX = deltaX();
  number Y = coordinateY(indexY(Index));
  number DY = deltaY();
  return point( X, DX, Y, DY, Val.z(), Val.errZ() );
}

// ------------------------------------------------------------
      
void graphics::dataMapVector::set( int IndexX, int IndexY, number Z, number ErrZ )
{
  unsigned Index = index( IndexX, IndexY );
  Values.at(Index) = value( Z, ErrZ );
  LimitsZCache.isValid = false;
}

// ------------------------------------------------------------

graphics::numberLimits graphics::dataMapVector::limitsZ() const 
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

std::ostream& graphics::operator<<( std::ostream& Stream, const graphics::pointMap& Point )
{
  return ( Stream << "( " << Point.x0() << " " << Point.x1() << " " << Point.y0() << " " << Point.y1() << " : " << Point.z() << " )" );
}

// ------------------------------------------------------------

std::ostream& graphics::operator<<( std::ostream& Stream, const graphics::dataMap& Data )
{
  std::streamsize Width = Stream.width();
  Stream.width( 12 );
  Stream << "Size " << Data.size() << std::endl;
  for ( graphics::dataMap::iterator i = Data.begin(); i != Data.end(); ++i )
    Stream << (*i) << std::endl;
  //Stream << "Limits: X" << Data.limitsX() << " Y" << Data.limitsY( Data.limitsX().totalLimits() ) << std::endl;
  Stream.width( Width );

  return Stream;
}
// ============================================================

