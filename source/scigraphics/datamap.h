
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

  namespace map
  {

    // ============================================================

    class point
    {
      private:
        number X, DX;
        number Y, DY;
        number Z, ErrZ;

      public:
        inline point();
        inline point( number x, number dx, number y, number dy, number z, number ez = 0 );

        number x0() const { return X; }
        number y0() const { return Y; }
        number x1() const { return X + DX; }
        number y1() const { return Y + DY; }
        number z()  const { return Z; }
        number errZ() const { return ErrZ; }

        inline bool isValid() const;
        inline bool isValidError() const;
    };

    // ------------------------------------------------------------

    class data 
    {
      public:
        typedef point point_t;
        typedef long int int_t;
        typedef data_iterator< data > iterator;

      private:
        interval<number> IntervalX, IntervalY;

      private:
        static numberLimits limitsForInterval( interval<number> Interval );
      
      protected:
        virtual const point_t get( int_t Index ) const = 0;

      public:
        data( interval<number> IntervalX, interval<number> IntervalY );
        virtual ~data() = 0;

        interval<number> intervalX() const;
        interval<number> intervalY() const;

        void setIntervalX( number Min, number Max );
        void setIntervalY( number Min, number Max );
        void setIntervalX( interval<number> Interval );
        void setIntervalY( interval<number> Interval );

        virtual int_t size() const = 0;
        inline bool empty() const;
        
        const point_t at( int_t Index ) const;
        inline const point_t operator[]( int_t Index ) const;

        const point_t first() const;
        const point_t last() const;
        
        inline iterator begin() const;
        inline iterator end() const;

        virtual const numberLimits limitsX() const;
        virtual const numberLimits limitsY( const interval<number> &LimitsX ) const;
        virtual const numberLimits limitsZ() const;
    };
    
    // ------------------------------------------------------------

    class dataVector : public data
    {
      private:
        class value
        {
          private:
            number Z, ErrZ;

          public:
            inline value();
            inline value( number z, number ez = 0 );
            number z() const { return Z; }
            number errZ() const { return ErrZ; }
        };

      private:
        size_t SizeX, SizeY;
        std::vector< value > Values;

        mutable struct limitsZCache
        {
          numberLimits LimitsZ;
          bool IsValid;

          limitsZCache();
        } LimitsZCache;
      
      protected:
        const point_t get( int_t Index ) const;

      public:
        dataVector();
        dataVector( size_t SizeX, interval<number> IntervalX, size_t SizeY, interval<number> IntervalY );

        inline int_t sizeX() const;
        inline int_t sizeY() const;
        int_t size() const;

        void resize( size_t SX, size_t SY );

        inline int_t indexX( int_t Index ) const;
        inline int_t indexY( int_t Index ) const;
        inline int_t index( int_t IndexX, int_t IndexY ) const;

        inline number deltaX() const;
        inline number deltaY() const;

        number coordinateX( int_t IndexX ) const;
        number coordinateY( int_t IndexY ) const;
        int_t nearestIndexX( number X ) const;
        int_t nearestIndexY( number Y ) const;

        void set( int_t IndexX, int_t IndexY, number Z, number ErrZ = 0 );
        
        const numberLimits limitsZ() const;
    };
    
    // ------------------------------------------------------------

    std::ostream& operator<<( std::ostream& Stream, const point& Point );
    std::ostream& operator<<( std::ostream& Stream, const data& Data );

    // ============================================================

    point::point() : 
      X(0), 
      DX(1), 
      Y(0), 
      DY(1), 
      Z(0), 
      ErrZ(0) 
    {
    }

    // ------------------------------------------------------------
        
    point::point( number x, number dx, number y, number dy, number z, number ez ) : 
      X(x), 
      DX(dx), 
      Y(y), 
      DY(dy), 
      Z(z), 
      ErrZ(ez) 
    {
    }
    
    // ------------------------------------------------------------
        
    bool point::isValid() const 
    { 
      return isValidNumber( z() ); 
    }
    
    // ------------------------------------------------------------

    bool point::isValidError() const 
    { 
      return isValidNumber( errZ() ); 
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
    
    dataVector::value::value() : 
      Z(invalidNumber()), 
      ErrZ(invalidNumber()) 
    {
    }
    
    // ------------------------------------------------------------
    
    dataVector::value::value( number z, number ez ) : 
      Z(z), 
      ErrZ(ez) 
    {
    }
    
    // ------------------------------------------------------------
        
    dataVector::int_t dataVector::sizeX() const 
    { 
      return static_cast<int_t>(SizeX); 
    }
    
    // ------------------------------------------------------------

    dataVector::int_t dataVector::sizeY() const 
    { 
      return static_cast<int_t>(SizeY); 
    }
    
    // ------------------------------------------------------------
        
    dataVector::int_t dataVector::indexX( int_t Index ) const 
    { 
      return Index % sizeX(); 
    }
    
    // ------------------------------------------------------------
    
    dataVector::int_t dataVector::indexY( int_t Index ) const 
    { 
      return Index / sizeX(); 
    }
    
    // ------------------------------------------------------------
    
    dataVector::int_t dataVector::index( int_t IndexX, int_t IndexY ) const 
    { 
      return IndexX + IndexY*sizeX(); 
    }

    // ------------------------------------------------------------
        
    number dataVector::deltaX() const 
    { 
      return intervalX().distance()/sizeX(); 
    }
    
    // ------------------------------------------------------------

    number dataVector::deltaY() const 
    { 
      return intervalY().distance()/sizeY(); 
    }

    // ============================================================
  }
}

