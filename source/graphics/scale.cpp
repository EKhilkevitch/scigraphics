
// ============================================================

#include "graphics/scale.h"
#include "graphics/painter.h"

#include <cmath>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <stdexcept>

// ============================================================

graphics::scale::scale() : 
  Shift(0), 
  Zoom(1), 
  Locked(false), 
  Marker( NULL ) 
{
}

// ------------------------------------------------------------

graphics::scale::~scale() 
{ 
  delete Marker; 
}
      
// ------------------------------------------------------------

void graphics::scale::setMarker( marker *M ) 
{ 
  if ( M == NULL )
    throw std::invalid_argument( "Marker can not be NULL" );

  delete Marker; 
  Marker = M; 
}

// ------------------------------------------------------------
      
void graphics::scale::setShift( double S ) 
{ 
  if ( ! isLocked() ) 
    Shift = S; 
}

// ------------------------------------------------------------
     
void graphics::scale::setZoom( double Z )  
{ 
  if ( ! isLocked() ) 
    Zoom  = Z; 
}

// ------------------------------------------------------------

void graphics::scale::reset() 
{ 
  setZoom(1); 
  setShift(0); 
}

// ------------------------------------------------------------

void graphics::scale::setLock( bool L )
{
  Locked = L;
  if ( Locked )
    reset();
}

// ------------------------------------------------------------

graphics::fcoord graphics::scale::numberToFraction( number Number ) const 
{
  if ( Number > + std::numeric_limits<number>::max()/4 )
    return +1000;
  if ( Number < - std::numeric_limits<number>::max()/4 )
    return -1000;
  
  double Coordinate = numberToPartOfDistance(Number);
  Coordinate -= Shift;
  Coordinate *= Zoom; 
  return Coordinate;
}

// ------------------------------------------------------------

graphics::number graphics::scale::fractionToNumber( fcoord Coordinate ) const 
{
  double Number = Coordinate;
  Number = ( Zoom == 0 ) ? 0 : Number/Zoom;
  Number += Shift;
  Number = partOfDistanceToNumber(Number);
  return Number;
}
            
// ------------------------------------------------------------

graphics::interval<graphics::number> graphics::scale::getVisivleInterval() const
{
  number Min = fractionToNumber(0);
  number Max = fractionToNumber(1);
  return interval<number>( Min, Max );
}

// ------------------------------------------------------------

void graphics::scale::setNumberOfMarks( unsigned Min, unsigned Max )
{
  assert( Marker != NULL );
  Marker->setNumberOfMarks( Min, Max );
}

// ------------------------------------------------------------

std::vector<graphics::number> graphics::scale::marks() const
{
  assert( Marker != NULL );
  interval<number> Interval = getVisivleInterval();
  return Marker->marks( Interval );
}

// ------------------------------------------------------------
      
void graphics::scale::addScaleShift( scale *Scale, double Shift )
{
  if ( Scale == NULL )
    return;
  Scale->setShift( Scale->shift() + Shift/Scale->zoom() );
}

// ------------------------------------------------------------

void graphics::scale::mulScaleZoom( scale *Scale, double Zoom )
{
  if ( Scale == NULL )
    return;
  Scale->setZoom( Scale->zoom() / Zoom );
}

// ------------------------------------------------------------

void graphics::scale::resetScale( scale *Scale, double )
{
  if ( Scale == NULL )
    return;
  Scale->reset();
}

// ============================================================

double graphics::scaleLinear::numberToPartOfDistance( number Number ) const 
{ 
  return getNumberLimits().totalLimits().partOfDistance(Number); 
}

// ------------------------------------------------------------

graphics::number graphics::scaleLinear::partOfDistanceToNumber( double Part ) const 
{ 
  return getNumberLimits().totalLimits().partToDistance(Part); 
}

// ============================================================

double graphics::scaleLogarithm::logDistance( interval<number> Interval )
{
  double Min = Interval.min();
  double Max = Interval.max();
  
  assert( Min != 0 );
  assert( Max != 0 );

  if ( ! isValidNumbers(Min,Max) )
    return 0;

  double LogDistance = std::log10( std::fabs(Max) ) - std::log10( std::fabs(Min) );

  LogDistance = std::fabs( LogDistance );
  if ( LogDistance <= 0 )
    return 1;
  return LogDistance;
}

// ============================================================

double graphics::scaleLogarithmPositive::numberToPartOfDistance( number Number ) const 
{ 
  if ( Number <= 0 ) 
    return -10;

  interval<number> Limits = getNumberLimits().positiveLimits();

  double LogNumber = std::log10(Number) - std::log10( Limits.min() );
  return LogNumber / logDistance(Limits);
}

// ------------------------------------------------------------

graphics::number graphics::scaleLogarithmPositive::partOfDistanceToNumber( double Part ) const 
{ 
  interval<number> Limits = getNumberLimits().positiveLimits();
        
  Part = Part * logDistance(Limits) + std::log10( Limits.min() );
  return std::pow( 10, Part );
}

// ============================================================

double graphics::scaleLogarithmNegative::numberToPartOfDistance( number Number ) const 
{
  if ( Number >= 0 ) 
    return +10;
  
  interval<number> Limits = getNumberLimits().negativeLimits();

  double LogNumber = std::log10( -Limits.min() ) - std::log10(-Number);
  return LogNumber / logDistance(Limits);
}

// ------------------------------------------------------------

graphics::number graphics::scaleLogarithmNegative::partOfDistanceToNumber( double Part ) const 
{ 
  interval<number> Limits = getNumberLimits().negativeLimits();
        
  Part = std::log10( -Limits.min() ) - Part * logDistance(Limits);
  return - std::pow( 10, Part );
}

// ============================================================

graphics::number graphics::scaleSquare::signsqrt( number Val ) 
{ 
  return std::sqrt( std::fabs(Val) ) * sign(Val); 
}

// ------------------------------------------------------------

double  graphics::scaleSquare::numberToPartOfDistance( number Number ) const
{
  interval<number> Limits = getNumberLimits().totalLimits();

  double SqrtDistance = signsqrt(Limits.max()) - signsqrt(Limits.min());
  double SqrtNumber = signsqrt( Number ) - signsqrt( Limits.min() );
  return SqrtNumber / SqrtDistance;
}

// ------------------------------------------------------------

graphics::number graphics::scaleSquare::partOfDistanceToNumber( double Part ) const
{
  interval<number> Limits = getNumberLimits().totalLimits();
  
  double SqrtDistance = signsqrt(Limits.max()) - signsqrt(Limits.min());
  Part = Part * SqrtDistance + signsqrt( Limits.min() );
  return signsqr( Part );
}

// ============================================================

graphics::pairScales::pairScales( const scale &SX, const scale &SY ) :
  ScaleX(SX),
  ScaleY(SY) 
{
}

// ------------------------------------------------------------

graphics::point<graphics::fcoord> graphics::pairScales::npoint2fpoint( point<number> Point ) const
{
  fcoord X = numberToFractionX( Point.x() );
  fcoord Y = numberToFractionY( Point.y() );
  return point<fcoord>(X,Y);
}
      
// ------------------------------------------------------------

graphics::point<graphics::number> graphics::pairScales::fpoint2npoint( point<fcoord> Point ) const
{
  number X = fractionToNumberX( Point.x() );
  number Y = fractionToNumberY( Point.y() );
  return point<number>(X,Y);
}

// ------------------------------------------------------------

graphics::point<graphics::number> graphics::pairScales::wpoint2npoint( point<wcoord> Point, const painter &Painter ) const
{
  fpoint FPoint = Painter.wpoint2fpoint(Point);
  npoint NPoint = fpoint2npoint(FPoint);
  return NPoint;
}

// ============================================================
     

