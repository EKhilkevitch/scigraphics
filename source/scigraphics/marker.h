
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


#pragma once

// ============================================================

#include "scigraphics/numbers.h"
#include "scigraphics/interval.h"

#include <vector>

// ============================================================

namespace scigraphics
{

// ============================================================

  class marker
  {
    private:
      interval<int> NumberOfMarks;
      
    protected:
      static number baseOrder( number Value );
      static number epsilonForInterval( interval<number> Interval );
      static bool inInterval( number Value, interval<number> Interval );
      
      bool vectorInMarkLimits( const std::vector<number> &Marks ) const;

    public:
      marker() : NumberOfMarks(4,9) {}
      virtual ~marker() {}

      void setNumberOfMarks( int Min, int Max ) { NumberOfMarks.setMinMax(Min,Max); }
      interval<int> numberOfMarks() const { return NumberOfMarks; }

      virtual std::vector<number> marks( const interval<number> Interval ) const = 0;
  };

  // ============================================================

  class markerLinear : public marker
  {
    protected:
      std::vector<number> marksWithStep( const interval<number> Interval, number Step ) const;
      std::vector<number> marksWithStepVector( const interval<number> Interval, const number Steps[] ) const;
    public:
      std::vector<number> marks( const interval<number> Interval ) const;
  };

  class markerLogarithm : public marker
  {
    protected:
      std::vector<number> marksWithSteps( const interval<number> Interval, const number Steps[], number OrderStep ) const;
      std::vector<number> marksInPositiveInterval( const interval<number> Interval ) const;
  };

  class markerLogarithmPositive : public markerLogarithm
  {
    public:
      std::vector<number> marks( const interval<number> Interval ) const;
  };
  
  class markerLogarithmNegative : public markerLogarithm
  {
    public:
      std::vector<number> marks( const interval<number> Interval ) const;
  };

// ============================================================

}

