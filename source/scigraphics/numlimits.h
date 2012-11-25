
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

#pragma once

// ============================================================

#include "scigraphics/interval.h"
#include "scigraphics/numbers.h"

#include <ostream>

namespace scigraphics
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

