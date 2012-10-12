
// ============================================================

#include "graphics/datasequence.h"

#include <limits>
#include <cmath>
#include <cassert>
#include <stdexcept>

// ============================================================

scigraphics::numberLimits scigraphics::dataSequence::limitsX() const
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

scigraphics::numberLimits scigraphics::dataSequence::limitsY( const interval<number> &LimitsX ) const
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

void scigraphics::dataSequenceVector::append( const point &Point )
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

void scigraphics::dataSequenceVector::updateOrderedByX()
{
  if ( ! OrderedByX )                   return;
  if ( size() <= 1 )                    return; 
  if ( ! isValidNumber( lastPoint().x() ) )
  {
    OrderedByX = false;
    return;
  }
  
  number LastValidX = invalidNumber();
  for ( int i = size()-2; i >= 0; i-- )
  {
    if ( isValidNumber( Points[i].x() ) )
    {
      LastValidX = Points[i].x();
      break;
    }
  }
    
  if ( isValidNumber(LastValidX) && LastValidX > lastPoint().x() )
    OrderedByX = false;
}

// ------------------------------------------------------------

void scigraphics::dataSequenceVector::updateLimitsXY( const point &Point, const coordinateType Type )
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

void scigraphics::dataSequenceVector::updateLimits( number Number, coordinateType Coordinate, numberLimits *Limits )
{
  assert( Limits != NULL );
  assert( isValidNumber(Number) );

  Limits->updateLimits( Number );

  if ( needToRecalculateLimits( Limits->positiveLimits() ) || needToRecalculateLimits( Limits->negativeLimits() ) )
    recalculateLimits( Coordinate, Limits );
}

// ------------------------------------------------------------

void scigraphics::dataSequenceVector::recalculateLimits( coordinateType Coordinate, numberLimits *Limits )
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

void scigraphics::dataSequenceVector::updateLimitsByValue( numberLimits *Limits, number Value, number PosDistance, number NegDistance )
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

bool scigraphics::dataSequenceVector::needToRecalculateLimits( const interval<number> Interval )
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

scigraphics::numberLimits scigraphics::dataSequenceVector::limitsY( const interval<number> &LimitsX ) const
{
  if ( LimitsX.min() <= limitsX().totalLimits().min() &&
       LimitsX.max() >= limitsX().totalLimits().max() )
    return LimitsY;
  return dataSequence::limitsY( LimitsX );
}

// ------------------------------------------------------------

scigraphics::number scigraphics::dataSequenceVector::pointValue( const point &Point, coordinateType Type )
{
  switch ( Type )
  {
    case CrdX: return Point.x();
    case CrdY: return Point.y();
    default: abort(); return 0;
  }
}

// ------------------------------------------------------------

scigraphics::number scigraphics::dataSequenceVector::pointError( const point &Point, coordinateType Type )
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

void scigraphics::dataSequenceVector::clear()
{
  LimitsX = LimitsY = numberLimits();
  OrderedByX = true;
  Points.clear();
}

// ------------------------------------------------------------

std::ostream& scigraphics::operator<<( std::ostream& Stream, const scigraphics::pointSequence& Point )
{
  return ( Stream << "( " << Point.x() << " " << Point.y() << " " << Point.errX() << " " << Point.errY() << " )" );
}
  
// ------------------------------------------------------------

std::ostream& operator<<( std::ostream& Stream, const scigraphics::dataSequence& Data )
{
  std::streamsize Width = Stream.width();

  Stream.width( 12 );
  Stream << "Size ";
  Stream << Data.size() << std::endl;
  for ( scigraphics::dataSequence::iterator i = Data.begin(); i != Data.end(); ++i )
    Stream << (*i) << std::endl;
  Stream.width( Width );

  return Stream;
}

// ============================================================
    
