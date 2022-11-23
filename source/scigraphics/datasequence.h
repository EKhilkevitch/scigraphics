
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
#include <iosfwd>

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
        inline point();
        inline point( number x, number y, number ex = 0, number ey = 0 );

        number x() const { return X; }
        number y() const { return Y; }
        number errX() const { return ErrX; }
        number errY() const { return ErrY; }

        bool isValid() const { return isValidNumbers( y(), x() ); }
        bool isValidError() const { return isValidNumbers( errY(), errX() ); }
        
        static inline point invalidPoint();
    };
  
    // ------------------------------------------------------------

    class data
    {
      public:
        typedef point point_t;
        typedef long long int int_t;
        typedef data_iterator< data > iterator;

      public:
        virtual ~data();

        virtual int_t size() const = 0;
        bool empty() const { return size() == 0; }

        virtual const point_t at( int_t Index ) const = 0;
        const point_t operator[]( int_t Index ) const { return at(Index); }

        const point_t first() const;
        const point_t last()  const;

        iterator begin() const { return iterator( *this, 0 ); }
        iterator end() const { return iterator( *this, size() ); }

        virtual const numberLimits limitsX() const;
        virtual const numberLimits limitsY( const interval<number> &LimitsX ) const;
        virtual bool isOrderedByX() const { return false; }
    };
  
    // ------------------------------------------------------------
    
    class numberLimitsDataCache
    {
      public:
        enum coordinateType 
        { 
          CoordinateX, 
          CoordinateY 
        };
        
        typedef data::point_t point_t;
        typedef data::iterator iterator;

      private:
        numberLimits LimitsX, LimitsY;
        bool RecalculateRequest;

      private:
        static void updateLimits( const point_t &Point, const coordinateType Type, const data &Data, numberLimits *Limits );
        static void updateLimits( number Number, const coordinateType Type, const data &Data, numberLimits *Limits );
        static bool needToRecalculate( const interval<number> Interval );
        static void recalculate( const coordinateType Type, const data &Data, numberLimits *Limits );
        static void updateByValue( number Value, number PosDistance, number NegDistance, numberLimits *Limits );
        
        static number pointValue( const point_t &Point, coordinateType Type );
        static number pointError( const point_t &Point, coordinateType Type );

      public:
        numberLimitsDataCache();

        void update( const point_t &Point, const data &Data );
        void recalculate( const data &Data );
        void setRecalculateRequest();
        void recalculateIfRequired( const data &Data );
        void clear();

        bool canUseCachedLimitsY( const interval<number> &LimitsX ) const;

        const numberLimits& limitsX() const;
        const numberLimits& limitsY() const;
    };
    
    // ------------------------------------------------------------
        
    class dataVector : public data
    {
      private:
        std::vector<point_t> Points;

        bool OrderedByX;
        numberLimitsDataCache LimitsCache;

      private:
        void appendPoint( const point_t &Point ); 
        void updateOrderedByX();
        void recalculateOrderedByX();

      public:
        dataVector();

        int_t size() const { return static_cast<int_t>(Points.size()); }
        const point_t at( int_t Index ) const { return Points.at( static_cast<size_t>(Index) ); }

        void append( const point_t &Point );
        void append( number X, number Y );
        void append( number X, number Y, number ErrY );
        void append( number X, number Y, number ErrX, number ErrY );
        void set( size_t Index, number X, number Y );
        void erase( size_t Index );

        void clear();
        
        const numberLimits limitsX() const { return LimitsCache.limitsX(); }
        const numberLimits limitsY( const interval<number> &LimitsX ) const;
        
        bool isOrderedByX() const { return OrderedByX; }
    };
  
    // ------------------------------------------------------------
    
    class dataUniformVector : public data
    {
      private:
        number StepX, ShiftX;
        std::vector<number> Values, Errors;
        numberLimitsDataCache LimitsCache;

      public:
        dataUniformVector();

        int_t size() const { return static_cast<int_t>(Values.size()); }
        const point_t at( int_t Index ) const;

        number valueX( int_t Index ) const { return static_cast<number>(Index) * stepX() + shiftX(); }
        number valueY( int_t Index ) const { return Values.at( static_cast<size_t>(Index) ); }
        number errorY( int_t Index ) const { return Errors.at( static_cast<size_t>(Index) ); }

        void append( number Y );
        void append( number Y, number ErrY );

        void setStepX( number StepX );
        number stepX() const { return StepX; }

        void setShiftX( number ShiftX );
        number shiftX() const { return ShiftX; }

        void clear();
        
        const numberLimits limitsX() const { return LimitsCache.limitsX(); }
        const numberLimits limitsY( const interval<number> &LimitsX ) const;

        bool isOrderedByX() const { return true; }
    };
    
    // ------------------------------------------------------------

    std::ostream& operator<<( std::ostream& Stream, const point& Point );
    std::ostream& operator<<( std::ostream& Stream, const data& Data );

    // ============================================================
  
    point::point() : 
      X(0), 
      Y(0), 
      ErrX(0), 
      ErrY(0)
    {
    }

    // ------------------------------------------------------------
    
    point::point( number x, number y, number ex, number ey ) : 
      X(x), 
      Y(y), 
      ErrX(ex), 
      ErrY(ey) 
    {
    }
    
    // ------------------------------------------------------------
        
    point point::invalidPoint() 
    { 
      return point( invalidNumber(), invalidNumber() );  
    }
    
    // ============================================================

  }
}

