
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

#include "scigraphics/numlimits.h"

#include <cassert>
#include <stdexcept>

// ============================================================

bool scigraphics::numberLimits::isValidInterval( interval<number> Interval )
{
  return isValidNumbers( Interval.min(), Interval.max() );
}

// ------------------------------------------------------------
      
scigraphics::numberLimits::numberLimits() 
{ 
  clear(); 
}

// ------------------------------------------------------------
      
void scigraphics::numberLimits::updateLimits( number Number ) 
{
  updateInterval( &TotalLimits, Number );
  if ( Number > 0 )
    updateInterval( &PositiveLimits, Number );
  if ( Number < 0 )
    updateInterval( &NegativeLimits, Number );
}

// ------------------------------------------------------------

void scigraphics::numberLimits::updateInterval( interval<number> *Interval, number Number )
{
  assert( Interval != NULL );
  if ( ! isValidNumber(Number) )
    return;

  if ( isValidInterval(*Interval) )
    Interval->updateInterval( Number );
  else
    Interval->setMinMax( Number, Number );
}

// ------------------------------------------------------------

void scigraphics::numberLimits::clear()
{
  TotalLimits    = invalidInterval();
  PositiveLimits = invalidInterval();
  NegativeLimits = invalidInterval();
}
      
// ------------------------------------------------------------

void scigraphics::numberLimits::setPositiveInterval( interval<number> Interval )
{
  if ( ! isValidInterval(Interval) )
    PositiveLimits = invalidInterval();
  else if ( Interval.min() <= 0 )
    throw std::invalid_argument( "Positive interval must be really positive" );
  else
    PositiveLimits = Interval;
}

// ------------------------------------------------------------

void scigraphics::numberLimits::setNegativeInterval( interval<number> Interval )
{
  if ( ! isValidInterval(Interval) )
    NegativeLimits = invalidInterval();
  else if ( Interval.max() >= 0 )
    throw std::invalid_argument( "Negative interval must be really negative" );
  else
    NegativeLimits = Interval;
}
      
// ------------------------------------------------------------

void scigraphics::numberLimits::setTotalInterval( interval<number> Interval )
{
  if ( ! isValidInterval(Interval) )
    TotalLimits = invalidInterval();
  else
    TotalLimits = Interval;
}
  
// ============================================================

std::ostream& scigraphics::operator<<( std::ostream& Stream, const scigraphics::numberLimits &Limits )
{
  Stream << "<< " << Limits.negativeLimits() << "; " << Limits.positiveLimits() << "; " << Limits.totalLimits() << " >>";
  return Stream;
}

// ============================================================

