
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

#include "scigraphics/numbers.h"
#include "scigraphics/numlimits.h"
#include "scigraphics/datageneral.h"

#include <vector>

namespace scigraphics 
{

// ============================================================

  class pointSequence
  {
    private:
      number X, Y;
      number ErrX, ErrY;
    public:
      pointSequence() : X(0), Y(0), ErrX(0), ErrY(0) {}
      pointSequence( number x, number y, number ex = 0, number ey = 0 ) : 
         X(x), Y(y), ErrX(ex), ErrY(ey) {}

      number x() const { return X; }
      number y() const { return Y; }
      number errX() const { return ErrX; }
      number errY() const { return ErrY; }

      bool isValid() const { return isValidNumbers( x(), y() ); }
      bool isValidError() const { return isValidNumbers( errX(), errY() ); }
      
      static inline pointSequence invalidPoint() { return pointSequence( invalidNumber(), invalidNumber() );  }
  };

  class dataSequence : public data::data<pointSequence>
  {
    public:
      virtual const numberLimits limitsX() const;
      virtual const numberLimits limitsY( const interval<number> &LimitsX ) const;
      virtual bool isOrderedByX() const { return false; }
  };
      
  class dataSequenceVector : public dataSequence
  {
    private:
      enum coordinateType { CrdX, CrdY };

      std::vector<point_t> Points;
      numberLimits LimitsX, LimitsY;
      bool OrderedByX;

    private:
      void appendPoint( const point_t &Point ) { Points.push_back(Point); }
      void updateLimitsXY( const point_t &Point, const coordinateType Type );
      void updateLimits( number Number, coordinateType Coordinate, numberLimits *Limits );
      void updateOrderedByX();
      static bool needToRecalculateLimits( const interval<number> Interval );
      void recalculateLimits( coordinateType Coordinate, numberLimits *Limits );
      void updateLimitsByValue( numberLimits *Limits, number Value, number PosDistance, number NegDistamce );
      static number pointValue( const point_t &Point, coordinateType Type );
      static number pointError( const point_t &Point, coordinateType Type );

    public:
      dataSequenceVector() : OrderedByX(true) {}

      int_t size() const { return Points.size(); }
      const point_t at( int_t Index ) const { return Points.at(Index); }

      void append( const point_t &Point );
      void append( number X, number Y ) { append( point_t(X,Y) );  }
      void append( number X, number Y, number ErrY ) { append( point_t(X,Y,0,ErrY) ); }
      void append( number X, number Y, number ErrX, number ErrY ) { append( point_t(X,Y,ErrX,ErrY) ); }

      void clear();
      
      const numberLimits limitsX() const { return LimitsX; }
      const numberLimits limitsY( const interval<number> &LimitsX ) const;
      
      bool isOrderedByX() const { return OrderedByX; }
  };

  std::ostream& operator<<( std::ostream& Stream, const pointSequence& Point );
  std::ostream& operator<<( std::ostream& Stream, const dataSequence& Data );

// ============================================================

}

