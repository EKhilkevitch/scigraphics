
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
        point() : X(0), DX(1), Y(0), DY(1), Z(0), ErrZ(0) {}
        point( number x, number dx, number y, number dy, number z, number ez = 0 ) : 
           X(x), DX(dx), Y(y), DY(dy), Z(z), ErrZ(ez) {}

        number x0() const { return X; }
        number y0() const { return Y; }
        number x1() const { return X + DX; }
        number y1() const { return Y + DY; }
        number z()  const { return Z; }
        number errZ() const { return ErrZ; }

        bool isValid() const { return isValidNumber( z() ); }
        bool isValidError() const { return isValidNumber( errZ() ); }
    };

    // ------------------------------------------------------------

    class data 
    {
      public:
        typedef point point_t;
        typedef int int_t;
        typedef data_iterator< data > iterator;

      private:
        interval<number> IntervalX, IntervalY;

      private:
        static numberLimits limitsForInterval( interval<number> Interval );

      public:
        data( interval<number> IX, interval<number> IY ) : 
          IntervalX(IX), 
          IntervalY(IY) {}
        virtual ~data() {}

        interval<number> intervalX() const { return IntervalX; }
        interval<number> intervalY() const { return IntervalY; }

        void setIntervalX( number Min, number Max ) { IntervalX = interval<number>(Min,Max); };
        void setIntervalY( number Min, number Max ) { IntervalY = interval<number>(Min,Max); }

        virtual int_t size() const = 0;
        bool empty() const { return size() == 0; }
        
        virtual const point_t at( int_t Index ) const = 0;
        const point_t operator[]( int_t Index ) const { return at(Index); }
        
        iterator begin() const { return iterator( *this, 0 ); }
        iterator end() const { return iterator( *this, size() ); }

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
            value() : Z(invalidNumber()), ErrZ(invalidNumber()) {}
            value( number z, number ez = 0 ) : Z(z), ErrZ(ez) {}
            number z() const { return Z; }
            number errZ() const { return ErrZ; }
        };

      private:
        size_t SizeX, SizeY;
        std::vector< value > Values;

        mutable struct limitsZCache
        {
          numberLimits LimitsZ;
          bool isValid;

          limitsZCache() : isValid(false) {}
        } LimitsZCache;

      public:
        dataVector();
        dataVector( size_t SX, interval<number> IX, size_t SY, interval<number> IY );

        int_t sizeX() const { return SizeX; }
        int_t sizeY() const { return SizeY; }
        int_t size() const;

        void resize( size_t SX, size_t SY );

        int_t indexX( int_t Index ) const { return Index % sizeX(); }
        int_t indexY( int_t Index ) const { return Index / sizeX(); }
        int_t index( int_t IndexX, int_t IndexY ) const { return IndexX + IndexY*sizeX(); }

        number deltaX() const { return intervalX().distance()/sizeX(); }
        number deltaY() const { return intervalY().distance()/sizeY(); }

        number coordinateX( int_t IndexX ) const { return intervalX().min() + deltaX()*IndexX; }
        number coordinateY( int_t IndexY ) const { return intervalY().min() + deltaY()*IndexY; }
        int_t nearestIndexX( number X ) const { return ( X - intervalX().min() )/deltaX(); }
        int_t nearestIndexY( number Y ) const { return ( Y - intervalY().min() )/deltaY(); }

        void set( int_t IndexX, int_t IndexY, number Z, number ErrZ = 0 );
        const point_t at( int_t Index ) const;
        const point_t at( int_t IndexX, int_t IndexY ) const { return at( index(IndexX,IndexY) ); }
        
        const numberLimits limitsZ() const;
    };
    
    // ------------------------------------------------------------

    std::ostream& operator<<( std::ostream& Stream, const point& Point );
    std::ostream& operator<<( std::ostream& Stream, const data& Data );

  // ============================================================

  }
}

