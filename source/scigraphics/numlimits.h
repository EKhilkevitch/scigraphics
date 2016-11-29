
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

#include <iosfwd>
#include <algorithm>
#include <utility>
#include <limits>

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
      numberLimits();

      void updateLimits( number Number );
      template <class iterator> void updateLimits( iterator Begin, iterator End );
      template <class iterator, class extractor> void updateLimits( iterator Begin, iterator End, extractor Extractor );
      template <class iterator, class comparator, class extractor> void updateLimitsOrdered( iterator Begin, iterator End, comparator Comparator, extractor Extractor );
      void clear();

      inline interval<number> totalLimits()    const { return TotalLimits;    }
      inline interval<number> positiveLimits() const { return PositiveLimits; }
      inline interval<number> negativeLimits() const { return NegativeLimits; }

      bool isValid() const { return isValidInterval(TotalLimits); }
      bool inLimits( number Number ) const { return TotalLimits.inInterval(Number); }
      
      void setPositiveInterval( interval<number> Interval );
      void setNegativeInterval( interval<number> Interval );
      void setTotalInterval( interval<number> Interval );
      
      static bool isValidInterval( interval<number> );
      template <class iterator, class value> static std::pair<iterator,iterator> boundsOfInterval( iterator Begin, iterator End, const value &First, const value &Last );
      template <class iterator, class value, class comparator> static std::pair<iterator,iterator> boundsOfInterval( iterator Begin, iterator End, const value &First, const value &Last, comparator Comparator );
  };

  std::ostream& operator<<( std::ostream& Stream, const numberLimits &Limits );

  // ============================================================
      
  template <class iterator, class extractor> void numberLimits::updateLimits( iterator Begin, iterator End, extractor Extractor )
  {
    while ( Begin != End )
    {
      updateLimits( Extractor(*Begin) );
      ++Begin;
    }
  }
  
  // ------------------------------------------------------------
  
  template <class iterator> void numberLimits::updateLimits( iterator Begin, iterator End )
  {
    struct self 
    {
      number operator()( number Number ) { return Number; }
    };
    updateLimits( Begin, End, self() );
  }
  
  // ------------------------------------------------------------
  
  template <class iterator, class comparator, class extractor> void numberLimits::updateLimitsOrdered( iterator Begin, iterator End, comparator Comparator, extractor Extractor )
  {
    if ( Begin == End )
      return;

    iterator Last = End;
    --Last;

    number FirstValue = Extractor( *Begin );
    number LastValue = Extractor( *Last );

    updateLimits( FirstValue );
    updateLimits( LastValue );

    if ( FirstValue <= 0 && LastValue >= 0 )
    {
      const number NumberNegZero = -10 * std::numeric_limits<number>::min();
      const number NumberPosZero = +10 * std::numeric_limits<number>::min();

      iterator ZeroValue;
      
      ZeroValue = std::lower_bound( Begin, End, NumberNegZero, Comparator );
      updateLimits( Extractor(*ZeroValue) );
      while ( Extractor(*ZeroValue) > NumberNegZero && ZeroValue != Begin )
        --ZeroValue;
      updateLimits( Extractor(*ZeroValue) );

      ZeroValue = std::lower_bound( Begin, End, NumberPosZero, Comparator );
      updateLimits( Extractor(*ZeroValue) );
      while ( Extractor(*ZeroValue) < NumberPosZero && ZeroValue != End )
        ++ZeroValue;
      updateLimits( Extractor(*ZeroValue) );
    }
  }
  
  // ------------------------------------------------------------
  
  template <class iterator, class value, class comparator> std::pair<iterator,iterator> numberLimits::boundsOfInterval( iterator Begin, iterator End, const value &First, const value &Last, comparator Comparator )
  {
    if ( First > Last )
      return boundsOfInterval( Begin, End, Last, First, Comparator );

    iterator Low = std::lower_bound( Begin, End, First, Comparator );
    iterator Hight = std::lower_bound( Low, End, Last, Comparator );
    while ( Hight != End && Comparator( *Hight, Last ) )
      ++Hight;

    return std::make_pair( Low, Hight );
  }

  // ------------------------------------------------------------
  
  template <class iterator, class value> std::pair<iterator,iterator> numberLimits::boundsOfInterval( iterator Begin, iterator End, const value &First, const value &Last )
  {
    return boundsOfInterval( Begin, End, First, Last, std::less<value>() );
  }
  
  // ============================================================

}

