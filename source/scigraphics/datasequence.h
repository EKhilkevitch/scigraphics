
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
#include "scigraphics/dataiterator.h"

#include <vector>

namespace scigraphics 
{
  namespace sequence
  {

    // ============================================================

    class point
    {
      private:
        number X, Y;
        number ErrX, ErrY;
      public:
        point() : X(0), Y(0), ErrX(0), ErrY(0) {}
        point( number x, number y, number ex = 0, number ey = 0 ) : 
           X(x), Y(y), ErrX(ex), ErrY(ey) {}

        number x() const { return X; }
        number y() const { return Y; }
        number errX() const { return ErrX; }
        number errY() const { return ErrY; }

        bool isValid() const { return isValidNumbers( x(), y() ); }
        bool isValidError() const { return isValidNumbers( errX(), errY() ); }
        
        static inline point invalidPoint() { return point( invalidNumber(), invalidNumber() );  }
    };
  
    // ------------------------------------------------------------

    class data
    {
      public:
        typedef point point_t;
        typedef int int_t;
        typedef data_iterator< data > iterator;

      public:
        virtual ~data() {}

        virtual int_t size() const = 0;
        bool empty() const { return size() == 0; }

        virtual const point_t at( int_t Index ) const = 0;
        const point_t operator[]( int_t Index ) const { return at(Index); }

        const point_t first() const { return  empty() ? point_t() : at(0); }
        const point_t last()  const { return  empty() ? point_t() : at(size()-1); }

        iterator begin() const { return iterator( *this, 0 ); }
        iterator end() const { return iterator( *this, size() ); }

        virtual const numberLimits limitsX() const;
        virtual const numberLimits limitsY( const interval<number> &LimitsX ) const;
        virtual bool isOrderedByX() const { return false; }
    };
  
    // ------------------------------------------------------------
        
    class dataVector : public data
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
        dataVector() : OrderedByX(true) {}

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
  
    // ------------------------------------------------------------

    std::ostream& operator<<( std::ostream& Stream, const point& Point );
    std::ostream& operator<<( std::ostream& Stream, const data& Data );

  // ============================================================

  }
}

