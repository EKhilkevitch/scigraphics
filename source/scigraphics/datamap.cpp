
// ============================================================

#include "scigraphics/datamap.h"

#include <limits>
#include <cmath>
#include <cassert>
#include <stdexcept>

// ============================================================

scigraphics::numberLimits scigraphics::dataMap::limitsForInterval( interval<number> Interval )
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

scigraphics::numberLimits scigraphics::dataMap::limitsX() const
{
  return limitsForInterval( intervalX() );
}

// ------------------------------------------------------------

scigraphics::numberLimits scigraphics::dataMap::limitsY( const interval<number> & ) const
{
  return limitsForInterval( intervalY() );
}

// ------------------------------------------------------------

scigraphics::numberLimits scigraphics::dataMap::limitsZ() const
{
  numberLimits Limits;
  for ( iterator p = begin(); p != end(); ++p )
    Limits.updateLimits( p->z() );
  return Limits;
}

// ============================================================

scigraphics::dataMapVector::dataMapVector( size_t SX, interval<number> IX, size_t SY, interval<number> IY ) :
  dataMap( IX, IY ),
  SizeX(0), 
  SizeY(0)
{
  resize( SX, SY );
}

// ------------------------------------------------------------

scigraphics::dataMapVector::dataMapVector() :
  dataMap( interval<number>(0,1), interval<number>(0,1) ),
  SizeX(0), 
  SizeY(0)
{
}

// ------------------------------------------------------------

void scigraphics::dataMapVector::resize( size_t SX, size_t SY )
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

size_t scigraphics::dataMapVector::size() const
{
  assert( sizeX()*sizeY() == Values.size() );
  return Values.size();
}

// ------------------------------------------------------------

scigraphics::dataMapVector::point scigraphics::dataMapVector::at( int Index ) const
{
  value Val = Values.at(Index);
  number X = coordinateX(indexX(Index));
  number DX = deltaX();
  number Y = coordinateY(indexY(Index));
  number DY = deltaY();
  return point( X, DX, Y, DY, Val.z(), Val.errZ() );
}

// ------------------------------------------------------------
      
void scigraphics::dataMapVector::set( int IndexX, int IndexY, number Z, number ErrZ )
{
  unsigned Index = index( IndexX, IndexY );
  Values.at(Index) = value( Z, ErrZ );
  LimitsZCache.isValid = false;
}

// ------------------------------------------------------------

scigraphics::numberLimits scigraphics::dataMapVector::limitsZ() const 
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

std::ostream& scigraphics::operator<<( std::ostream& Stream, const scigraphics::pointMap& Point )
{
  return ( Stream << "( " << Point.x0() << " " << Point.x1() << " " << Point.y0() << " " << Point.y1() << " : " << Point.z() << " )" );
}

// ------------------------------------------------------------

std::ostream& scigraphics::operator<<( std::ostream& Stream, const scigraphics::dataMap& Data )
{
  std::streamsize Width = Stream.width();
  Stream.width( 12 );
  Stream << "Size " << Data.size() << std::endl;
  for ( scigraphics::dataMap::iterator i = Data.begin(); i != Data.end(); ++i )
    Stream << (*i) << std::endl;
  //Stream << "Limits: X" << Data.limitsX() << " Y" << Data.limitsY( Data.limitsX().totalLimits() ) << std::endl;
  Stream.width( Width );

  return Stream;
}
// ============================================================

