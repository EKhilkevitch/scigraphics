
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

#include <algorithm>
#include <ostream>

// ============================================================

namespace scigraphics
{

  // ============================================================

  template <class T> class interval
  {
    protected:
      T Min, Max;

      static inline T rabs( T Number ) { return Number < static_cast<T>(0) ? -Number : +Number;  }

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

      T toInterval( T Value ) const;

      double partOfDistance( T Value ) const;
      T partToDistance( double Part ) const;

      void setMinimum( T XMin );
      void setMaximum( T XMax );
      void setMinMax( T XMin, T XMax );
      
      void updateInterval( T Number );

  };

  // ============================================================
  
  template <class T> bool operator==( const interval<T> &Lims1, const interval<T> &Lims2 );
  template <class T> bool operator!=( const interval<T> &Lims1, const interval<T> &Lims2 );
  template <class T> interval<T> operator+( const interval<T> &Limits, const T Value );
  template <class T> interval<T> operator-( const interval<T> &Limits, const T Value );
  template <class T> std::ostream& operator<<( std::ostream &Stream, interval<T> Interval );
  
  // ============================================================
  
  template <class T> T interval<T>::toInterval( T Value ) const 
  { 
      return Value < min() ? min() :
             Value > max() ? max() :
             Value;
  }

  // ------------------------------------------------------------
  
  template <class T> double interval<T>::partOfDistance( T Value ) const 
  { 
    return isSingular() ? 0 : ( (double)Value - (double)min() )/(double)distance(); 
  }
  
  // ------------------------------------------------------------
      
  template <class T> T interval<T>::partToDistance( double Part ) const 
  { 
    return static_cast<T>( Part * distance() + min() ); 
  }
  
  // ------------------------------------------------------------
  
  template <class T> void interval<T>::setMinimum( T XMin ) 
  { 
    if ( XMin > Max )
    { 
      Min = Max; 
      Max = XMin; 
    } else { 
      Min = XMin; 
    } 
  }
  
  // ------------------------------------------------------------

  template <class T> void interval<T>::setMaximum( T XMax ) 
  { 
    if ( XMax < Min ) 
    { 
      Max = Min; 
      Min = XMax; 
    } else { 
      Max = XMax; 
    } 
  }

  // ------------------------------------------------------------

  template <class T> void interval<T>::setMinMax( T XMin, T XMax )
  {
    Min = std::min(XMin,XMax);
    Max = std::max(XMin,XMax);
  }

  // ------------------------------------------------------------
  
  template <class T> void interval<T>::updateInterval( T Number ) 
  { 
    Min = std::min(Min,Number); 
    Max = std::max(Max,Number); 
  }
  
  // ------------------------------------------------------------

  template <class T> bool operator==( const interval<T> &Lims1, const interval<T> &Lims2 )
  {
    return Lims1.min() == Lims2.min() && Lims1.max() == Lims2.max(); 
  }

  // ------------------------------------------------------------
  
  template <class T> bool operator!=( const interval<T> &Lims1, const interval<T> &Lims2 )
  {
    return ! ( Lims1 == Lims2 );
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

