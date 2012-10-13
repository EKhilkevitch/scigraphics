
// ============================================================

#include "graphics/numlimits.h"
#include <cassert>
#include <stdexcept>

// ============================================================

bool scigraphics::numberLimits::isValidInterval( interval<number> I )
{
  return isValidNumbers( I.min(), I.max() );
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

