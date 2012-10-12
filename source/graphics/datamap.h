
#pragma once

#include "graphics/numbers.h"
#include "graphics/numlimits.h"
#include "graphics/datageneral.h"

#include <vector>

namespace graphics 
{

// ============================================================

  class pointMap
  {
    private:
      number X, DX;
      number Y, DY;
      number Z, ErrZ;

    public:
      pointMap() : X(0), DX(1), Y(0), DY(1), Z(0), ErrZ(0) {}
      pointMap( number x, number dx, number y, number dy, number z, number ez = 0 ) : 
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


  class dataMap : public dataPoint<pointMap>
  {
    private:
      interval<number> IntervalX, IntervalY;

    private:
      static numberLimits limitsForInterval( interval<number> Interval );

    public:
      dataMap( interval<number> IX, interval<number> IY ) : 
        IntervalX(IX), 
        IntervalY(IY) {}

      interval<number> intervalX() const { return IntervalX; }
      interval<number> intervalY() const { return IntervalY; }

      void setIntervalX( number Min, number Max ) { IntervalX = interval<number>(Min,Max); };
      void setIntervalY( number Min, number Max ) { IntervalY = interval<number>(Min,Max); }

      virtual numberLimits limitsX() const;
      virtual numberLimits limitsY( const interval<number> &LimitsX ) const;
      virtual numberLimits limitsZ() const;
  };

  class dataMapVector : public dataMap
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
      dataMapVector();
      dataMapVector( size_t SX, interval<number> IX, size_t SY, interval<number> IY );

      size_t sizeX() const { return SizeX; }
      size_t sizeY() const { return SizeY; }
      size_t size() const;

      void resize( size_t SX, size_t SY );

      int indexX( int Index ) const { return Index % sizeX(); }
      int indexY( int Index ) const { return Index / sizeX(); }
      int index( int IndexX, int IndexY ) const { return IndexX + IndexY*sizeX(); }

      number deltaX() const { return intervalX().distance()/sizeX(); }
      number deltaY() const { return intervalY().distance()/sizeY(); }

      number coordinateX( int IndexX ) const { return intervalX().min() + deltaX()*IndexX; }
      number coordinateY( int IndexY ) const { return intervalY().min() + deltaY()*IndexY; }

      void set( int IndexX, int IndexY, number Z, number ErrZ = 0 );
      point at( int Index ) const;
      point at( int IndexX, int IndexY ) const { return at( index(IndexX,IndexY) ); }
      
      numberLimits limitsZ() const;
  };

  std::ostream& operator<<( std::ostream& Stream, const pointMap& Point );
  std::ostream& operator<<( std::ostream& Stream, const dataMap& Data );

// ============================================================

}

