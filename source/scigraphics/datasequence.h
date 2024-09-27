
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

        inline number x() const { return X; }
        inline number y() const { return Y; }
        inline number errX() const { return ErrX; }
        inline number errY() const { return ErrY; }

        inline bool isValid() const;
        inline bool isValidError() const;
        
        static inline point invalidPoint();
    };
  
    // ------------------------------------------------------------

    class data
    {
      public:
        typedef point point_t;
        typedef long long int int_t;
        typedef data_iterator< data > iterator;

      protected:
        virtual const point_t get( int_t Index ) const = 0;

      public:
        virtual ~data() = 0;

        virtual int_t size() const = 0;
        inline bool empty() const;

        const point_t at( int_t Index ) const;
        inline const point_t operator[]( int_t Index ) const;
        virtual size_t fill( int_t BeginIndex, size_t Size, std::vector<point_t> *Points ) const;

        const point_t first() const;
        const point_t last()  const;

        inline iterator begin() const;
        inline iterator end() const;

        virtual const numberLimits limitsX() const;
        virtual const numberLimits limitsY( const interval<number> &LimitsX ) const;
        virtual bool isOrderedByX() const;
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
        template <coordinateType Type> static void updateLimits( const point_t &Point, const data &Data, numberLimits *Limits );
        template <coordinateType Type> static void updateLimits( number Number, const data &Data, numberLimits *Limits );
        static bool needToRecalculate( const interval<number> Interval );
        template <coordinateType Type> static void recalculate( const data &Data, numberLimits *Limits );
        static void updateByValue( number Value, number PosDistance, number NegDistance, numberLimits *Limits );
        
        template <coordinateType Type> inline static number pointValue( const point_t &Point );
        template <coordinateType Type> inline static number pointError( const point_t &Point );

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

      protected:
        const point_t get( int_t Index ) const;

      public:
        dataVector();

        int_t size() const;

        void append( const point_t &Point );
        void append( number X, number Y );
        void append( number X, number Y, number ErrY );
        void append( number X, number Y, number ErrX, number ErrY );
        void set( size_t Index, number X, number Y );
        void erase( size_t Index );

        void clear();
        
        const numberLimits limitsX() const { return LimitsCache.limitsX(); }
        const numberLimits limitsY( const interval<number> &LimitsX ) const;
        
        bool isOrderedByX() const;
    };
  
    // ------------------------------------------------------------
    
    class dataUniformVector : public data
    {
      private:
        number StepX, ShiftX;
        std::vector<number> Values, Errors;
        numberLimitsDataCache LimitsCache;
      
      protected:
        const point_t get( int_t Index ) const;

      public:
        dataUniformVector();

        int_t size() const;

        inline number valueX( int_t Index ) const;
        inline number valueY( int_t Index ) const;
        inline number errorY( int_t Index ) const;

        void append( number Y );
        void append( number Y, number ErrY );

        void setStepX( number StepX );
        number stepX() const { return StepX; }

        void setShiftX( number ShiftX );
        number shiftX() const { return ShiftX; }

        void clear();
        
        const numberLimits limitsX() const { return LimitsCache.limitsX(); }
        const numberLimits limitsY( const interval<number> &LimitsX ) const;

        bool isOrderedByX() const;
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
    
    // ------------------------------------------------------------
        
    bool point::isValid() const 
    { 
      return isValidNumbers( y(), x() ); 
    }
    
    // ------------------------------------------------------------
    
    bool point::isValidError() const 
    { 
      return isValidNumbers( errY(), errX() ); 
    }
    
    // ============================================================

    bool data::empty() const 
    { 
      return size() == 0; 
    }
    
    // ------------------------------------------------------------
        
    const data::point_t data::operator[]( int_t Index ) const 
    { 
      return get(Index); 
    }
    
    // ------------------------------------------------------------
        
    data::iterator data::begin() const 
    { 
      return iterator( *this, 0 ); 
    }
    
    // ------------------------------------------------------------
    
    data::iterator data::end() const 
    { 
      return iterator( *this, size() ); 
    }

    // ============================================================
        
    number dataUniformVector::valueX( int_t Index ) const 
    { 
      return static_cast<number>(Index) * stepX() + shiftX(); 
    }
    
    // ------------------------------------------------------------
    
    number dataUniformVector::valueY( int_t Index ) const 
    { 
      return Values[ static_cast<size_t>(Index) ]; 
    }
    
    // ------------------------------------------------------------
    
    number dataUniformVector::errorY( int_t Index ) const 
    { 
      return Errors[ static_cast<size_t>(Index) ]; 
    }
    
    // ============================================================
    
  }
}

