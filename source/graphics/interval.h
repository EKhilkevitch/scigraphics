
#pragma once

#include <algorithm>
#include <ostream>
#include <cmath>

// ============================================================

namespace graphics
{

// ============================================================

template <class T> class interval
{
  protected:
    T Min, Max;

    static inline T rabs( T Number ) { return std::fabs(Number); } // Number < 0 ? -Number : +Number;  }

  public:
    interval() : 
      Min( 0 ), 
      Max( 0 ) {}

    interval( T MinVal, T MaxVal ) : 
      Min(std::min(MinVal,MaxVal)),
      Max(std::max(MinVal,MaxVal)) {}

    inline T min() const       { return Min; }
    inline T max() const       { return Max; }
    inline T maxAbs() const    { return std::max( rabs(Min), rabs(Max) ); }
    inline T minAbs() const    { return std::min( rabs(Min), rabs(Max) ); }
    inline T distance() const  { return Max - Min; }

    bool isSingular() const { return ( Min >= Max ); }

    inline bool inInterval( T Value ) const              { return min() <= Value && Value <= max(); } 
    inline bool inIntervalRStrict( T Value ) const       { return min() <= Value && Value <  max(); }
    inline bool inIntervalClosed( T Value ) const        { return min() <  Value && Value <  max(); }

    T toInterval( T Value ) const 
    { 
        return Value < min() ? min() :
               Value > max() ? max() :
               Value;
    }

    double partOfDistance( T Value ) const 
    { 
      return isSingular() ? 0 : ( (double)Value - (double)min() )/(double)distance(); 
    }

    T partToDistance( double Part ) const { return static_cast<T>( Part * distance() + min() ); }

    void setMinimum( T XMin ) 
    { 
      if ( XMin > Max )
        { Min = Max; Max = XMin; } 
      else 
        { Min = XMin; } 
    }

    void setMaximum( T XMax ) 
    { 
      if ( XMax < Min ) 
        { Max = Min; Min = XMax; } 
      else 
        { Max = XMax; } 
    }

    void setMinMax( T XMin, T XMax )
    {
      Min = std::min(XMin,XMax);
      Max = std::max(XMin,XMax);
    }
    
    void updateInterval( T Number ) 
    { 
      Min = std::min(Min,Number); 
      Max = std::max(Max,Number); 
    }

};

// ============================================================

template <class T> bool operator==( const interval<T> &Lims1, const interval<T> &Lims2 )
{
  return Lims1.min() == Lims2.min() && Lims1.max() == Lims2.max(); 
}

// ------------------------------------------------------------

template <class T> interval<T> operator+( const interval<T> &Limits, const T Value )
{
  return interval<T>( Limits.min() + Value, Limits.max() + Value );
}

// ------------------------------------------------------------

template <class T> interval<T> operator-( const interval<T> &Limits, const T Value )
{
  return Limits + (-Value);
}

// ------------------------------------------------------------

template <class T> std::ostream& operator<<( std::ostream &Stream, interval<T> Interval )
{
  Stream << "[ " << Interval.min() << " .. " << Interval.max() << " ]";
  return Stream;
}

// ============================================================


}

// ============================================================

