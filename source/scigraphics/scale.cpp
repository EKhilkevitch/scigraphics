
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

#include "scigraphics/scale.h"
#include "scigraphics/painter.h"
#include "scigraphics/marker.h"

#include <cmath>
#include <cassert>
#include <iostream>
#include <limits>
#include <stdexcept>

// ============================================================

scigraphics::scale::scale() : 
  Shift(0), 
  Zoom(1), 
  Locked(false), 
  Marker(NULL) 
{
}

// ------------------------------------------------------------

scigraphics::scale::~scale() 
{ 
  delete Marker; 
}
      
// ------------------------------------------------------------

void scigraphics::scale::setMarker( marker *M ) 
{ 
  if ( M == NULL )
    throw std::invalid_argument( "Marker can not be NULL" );

  delete Marker; 
  Marker = M; 
}

// ------------------------------------------------------------
      
const scigraphics::marker* scigraphics::scale::getMarker() const 
{ 
  return Marker; 
}

// ------------------------------------------------------------

scigraphics::marker* scigraphics::scale::getMarker() 
{ 
  return Marker; 
}

// ------------------------------------------------------------
      
void scigraphics::scale::setShift( double S ) 
{ 
  if ( ! isLocked() ) 
    Shift = S; 
}

// ------------------------------------------------------------
      
double scigraphics::scale::shift() const 
{ 
  return Shift; 
}

// ------------------------------------------------------------
     
void scigraphics::scale::setZoom( double Z )  
{ 
  if ( ! isLocked() ) 
    Zoom  = Z; 
}

// ------------------------------------------------------------
      
double scigraphics::scale::zoom()  const 
{ 
  return Zoom; 
}

// ------------------------------------------------------------

void scigraphics::scale::reset() 
{ 
  setZoom(1); 
  setShift(0); 
}

// ------------------------------------------------------------

void scigraphics::scale::setLock( bool L )
{
  Locked = L;
  if ( Locked )
    reset();
}

// ------------------------------------------------------------
      
void scigraphics::scale::lock()   
{ 
  setLock(true);  
}

// ------------------------------------------------------------

void scigraphics::scale::unlocK() 
{ 
  setLock(false); 
}

// ------------------------------------------------------------

bool scigraphics::scale::isLocked() const 
{ 
  return Locked; 
}

// ------------------------------------------------------------

scigraphics::fcoord scigraphics::scale::numberToFraction( number Number ) const 
{
  if ( Number > + std::numeric_limits<number>::max()/4 )
    return +1000;
  if ( Number < - std::numeric_limits<number>::max()/4 )
    return -1000;
  
  number Coordinate = numberToPartOfDistance(Number);
  Coordinate -= Shift;
  Coordinate *= Zoom; 
  return static_cast<fcoord>( Coordinate );
}

// ------------------------------------------------------------

scigraphics::number scigraphics::scale::fractionToNumber( fcoord Coordinate ) const 
{
  double Number = static_cast<double>(Coordinate);
  
  if ( Zoom == 0 )
    Number = 0;
  else
    Number /= Zoom;

  Number += Shift;
  Number = partOfDistanceToNumber(Number);
  return Number;
}
            
// ------------------------------------------------------------

void scigraphics::scale::setNumberLimits( const numberLimits &N ) 
{ 
  NumberLimits = N; 
}

// ------------------------------------------------------------

const scigraphics::numberLimits& scigraphics::scale::getNumberLimits() const 
{ 
  return NumberLimits; 
}

// ------------------------------------------------------------

scigraphics::interval<scigraphics::number> scigraphics::scale::getNumberInterval() const 
{ 
  return getNumberLimits().totalLimits(); 
}

// ------------------------------------------------------------

scigraphics::interval<scigraphics::number> scigraphics::scale::getVisivleInterval() const
{
  number Min = fractionToNumber(0);
  number Max = fractionToNumber(1);
  return interval<number>( Min, Max );
}

// ------------------------------------------------------------

void scigraphics::scale::setNumberOfMarks( unsigned Min, unsigned Max )
{
  assert( Marker != NULL );
  Marker->setNumberOfMarks( Min, Max );
}

// ------------------------------------------------------------

std::vector<scigraphics::number> scigraphics::scale::marks() const
{
  assert( Marker != NULL );
  interval<number> Interval = getVisivleInterval();
  return Marker->marks( Interval );
}

// ============================================================
      
scigraphics::scaleLinear::scaleLinear() 
{ 
  setMarker( new markerLinear() ); 
}

// ------------------------------------------------------------

double scigraphics::scaleLinear::numberToPartOfDistance( number Number ) const 
{ 
  return getNumberLimits().totalLimits().partOfDistance(Number); 
}

// ------------------------------------------------------------

scigraphics::number scigraphics::scaleLinear::partOfDistanceToNumber( double Part ) const 
{ 
  return getNumberLimits().totalLimits().partToDistance(Part); 
}

// ============================================================

double scigraphics::scaleLogarithm::logDistance( interval<number> Interval )
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

scigraphics::scaleLogarithmPositive::scaleLogarithmPositive()
{
  setMarker( new markerLogarithmPositive() );
}

// ------------------------------------------------------------

double scigraphics::scaleLogarithmPositive::numberToPartOfDistance( number Number ) const 
{ 
  if ( Number <= 0 ) 
    return -10;

  interval<number> Limits = getNumberLimits().positiveLimits();

  double LogNumber = std::log10(Number) - std::log10( Limits.min() );
  return LogNumber / logDistance(Limits);
}

// ------------------------------------------------------------

scigraphics::number scigraphics::scaleLogarithmPositive::partOfDistanceToNumber( double Part ) const 
{ 
  interval<number> Limits = getNumberLimits().positiveLimits();
        
  Part = Part * logDistance(Limits) + std::log10( Limits.min() );
  return std::pow( 10, Part );
}

// ------------------------------------------------------------
      
scigraphics::interval<scigraphics::number> scigraphics::scaleLogarithmPositive::getNumberInterval() const 
{ 
  return getNumberLimits().positiveLimits(); 
}

// ============================================================

scigraphics::scaleLogarithmNegative::scaleLogarithmNegative()
{
  setMarker( new markerLogarithmNegative() );
}

// ------------------------------------------------------------

double scigraphics::scaleLogarithmNegative::numberToPartOfDistance( number Number ) const 
{
  if ( Number >= 0 ) 
    return +10;
  
  interval<number> Limits = getNumberLimits().negativeLimits();

  double LogNumber = std::log10( -Limits.min() ) - std::log10(-Number);
  return LogNumber / logDistance(Limits);
}

// ------------------------------------------------------------

scigraphics::number scigraphics::scaleLogarithmNegative::partOfDistanceToNumber( double Part ) const 
{ 
  interval<number> Limits = getNumberLimits().negativeLimits();
        
  Part = std::log10( -Limits.min() ) - Part * logDistance(Limits);
  return - std::pow( 10, Part );
}

// ------------------------------------------------------------
      
scigraphics::interval<scigraphics::number> scigraphics::scaleLogarithmNegative::getNumberInterval() const 
{ 
  return getNumberLimits().negativeLimits(); 
}

// ============================================================
      
scigraphics::scaleSquare::scaleSquare() 
{ 
  setMarker( new markerLinear() ); 
}

// ------------------------------------------------------------

scigraphics::number scigraphics::scaleSquare::sign( number Val )     
{ 
  return Val < 0 ? -1 : Val > 0 ? +1 : 0; 
}

// ------------------------------------------------------------

scigraphics::number scigraphics::scaleSquare::signsqr( number Val )  
{ 
  return Val*Val*sign(Val); 
}

// ------------------------------------------------------------

scigraphics::number scigraphics::scaleSquare::signsqrt( number Val ) 
{ 
  return std::sqrt( std::fabs(Val) ) * sign(Val); 
}

// ------------------------------------------------------------

double  scigraphics::scaleSquare::numberToPartOfDistance( number Number ) const
{
  interval<number> Limits = getNumberLimits().totalLimits();

  double SqrtDistance = signsqrt(Limits.max()) - signsqrt(Limits.min());
  double SqrtNumber = signsqrt( Number ) - signsqrt( Limits.min() );
  return SqrtNumber / SqrtDistance;
}

// ------------------------------------------------------------

scigraphics::number scigraphics::scaleSquare::partOfDistanceToNumber( double Part ) const
{
  interval<number> Limits = getNumberLimits().totalLimits();
  
  double SqrtDistance = signsqrt(Limits.max()) - signsqrt(Limits.min());
  Part = Part * SqrtDistance + signsqrt( Limits.min() );
  return signsqr( Part );
}

// ============================================================

scigraphics::pairScales::pairScales( const scale &SX, const scale &SY ) :
  ScaleX(SX),
  ScaleY(SY) 
{
}

// ------------------------------------------------------------

scigraphics::point<scigraphics::fcoord> scigraphics::pairScales::npoint2fpoint( point<number> Point ) const
{
  fcoord X = numberToFractionX( Point.x() );
  fcoord Y = numberToFractionY( Point.y() );
  return point<fcoord>(X,Y);
}
      
// ------------------------------------------------------------

scigraphics::point<scigraphics::number> scigraphics::pairScales::fpoint2npoint( point<fcoord> Point ) const
{
  number X = fractionToNumberX( Point.x() );
  number Y = fractionToNumberY( Point.y() );
  return point<number>(X,Y);
}

// ------------------------------------------------------------

scigraphics::point<scigraphics::number> scigraphics::pairScales::wpoint2npoint( point<wcoord> Point, const painter &Painter ) const
{
  fpoint FPoint = Painter.wpoint2fpoint(Point);
  npoint NPoint = fpoint2npoint(FPoint);
  return NPoint;
}

// ============================================================
     

