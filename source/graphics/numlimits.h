
// ============================================================

#pragma once

// ============================================================

#include "graphics/interval.h"
#include "graphics/numbers.h"

#include <ostream>

namespace graphics
{

// ============================================================

  class numberLimits 
  {
    private:
      interval<number> TotalLimits;
      interval<number> PositiveLimits, NegativeLimits;

      static void updateInterval( interval<number> *Interval, number Number );
      static inline interval<number> invalidInterval() { return interval<number>( invalidNumber(), invalidNumber() ); }

    public:
      numberLimits() { clear(); }

      void updateLimits( number N );
      void clear();

      inline interval<number> totalLimits()    const { return TotalLimits;    }
      inline interval<number> positiveLimits() const { return PositiveLimits; }
      inline interval<number> negativeLimits() const { return NegativeLimits; }

      bool isValid() const { return isValidInterval(TotalLimits); }

      bool inLimits( number N ) const { return TotalLimits.inInterval(N); }
      
      static bool isValidInterval( interval<number> );

      void setPositiveInterval( interval<number> Interval );
      void setNegativeInterval( interval<number> Interval );
      void setTotalInterval( interval<number> Interval );
  };

  std::ostream& operator<<( std::ostream& Stream, const numberLimits &Limits );

// ============================================================

}

