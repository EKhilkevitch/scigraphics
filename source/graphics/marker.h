
#pragma once

// ============================================================

#include "graphics/numbers.h"
#include "graphics/numlimits.h"

#include <vector>

// ============================================================

namespace graphics
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

