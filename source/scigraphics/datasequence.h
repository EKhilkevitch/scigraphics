
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

  class dataSequence : public dataPoint<pointSequence>
  {
    public:
      virtual numberLimits limitsX() const;
      virtual numberLimits limitsY( const interval<number> &LimitsX ) const;
      virtual bool isOrderedByX() const { return false; }
  };
      
  class dataSequenceVector : public dataSequence
  {
    private:
      enum coordinateType { CrdX, CrdY };

      std::vector<point> Points;
      numberLimits LimitsX, LimitsY;
      bool OrderedByX;

    private:
      void appendPoint( const point &Point ) { Points.push_back(Point); }
      void updateLimitsXY( const point &Point, const coordinateType Type );
      void updateLimits( number Number, coordinateType Coordinate, numberLimits *Limits );
      void updateOrderedByX();
      static bool needToRecalculateLimits( const interval<number> Interval );
      void recalculateLimits( coordinateType Coordinate, numberLimits *Limits );
      void updateLimitsByValue( numberLimits *Limits, number Value, number PosDistance, number NegDistamce );
      static number pointValue( const point &Point, coordinateType Type );
      static number pointError( const point &Point, coordinateType Type );

    public:
      dataSequenceVector() : OrderedByX(true) {}

      size_t size() const { return Points.size(); }
      point at( int Index ) const { return Points.at(Index); }

      void append( const point &Point );
      void append( number X, number Y ) { append( point(X,Y) );  }
      void append( number X, number Y, number ErrY ) { append( point(X,Y,0,ErrY) ); }
      void append( number X, number Y, number ErrX, number ErrY ) { append( point(X,Y,ErrX,ErrY) ); }

      void clear();
      
      numberLimits limitsX() const { return LimitsX; }
      numberLimits limitsY( const interval<number> &LimitsX ) const;
      
      bool isOrderedByX() const { return OrderedByX; }
  };

  std::ostream& operator<<( std::ostream& Stream, const pointSequence& Point );
  std::ostream& operator<<( std::ostream& Stream, const dataSequence& Data );

// ============================================================

}

