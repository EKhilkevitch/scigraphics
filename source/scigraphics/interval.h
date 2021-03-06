
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

#include <ostream>
#include <istream>

// ============================================================

namespace scigraphics
{

  // ============================================================

  template <class T> class interval
  {
    private:
      T Min, Max;

    private:
      static T abs( T Number );
      static T min( T Number1, T Number2 );
      static T max( T Number1, T Number2 );

    public:
      interval();
      interval( T MinVal, T MaxVal );

      inline T min() const       { return Min; }
      inline T max() const       { return Max; }
      inline T maxAbs() const    { return max( abs(Min), abs(Max) ); }
      inline T minAbs() const    { return min( abs(Min), abs(Max) ); }
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

      interval<T>& operator+=( const T Value );
      interval<T>& operator-=( const T Value );
      interval<T>& operator*=( const T Value );
      interval<T>& operator/=( const T Value );

      static bool isEquals( const interval<T> &Lims1, const interval<T> &Lims2, T Difference = 0 );
  };

  // ============================================================
 
  template <class T> bool operator==( const interval<T> &Lims1, const interval<T> &Lims2 );
  template <class T> bool operator!=( const interval<T> &Lims1, const interval<T> &Lims2 );
  template <class T> interval<T> operator+( interval<T> Limits, const T Value );
  template <class T> interval<T> operator-( interval<T> Limits, const T Value );
  template <class T> interval<T> operator*( interval<T> Limits, const T Value );
  template <class T> interval<T> operator/( interval<T> Limits, const T Value );
  
  template <class T> std::ostream& operator<<( std::ostream &Stream, interval<T>  Interval );
  template <class T> std::istream& operator>>( std::istream &Stream, interval<T> &Interval );
  
  // ============================================================
      
  template <class T> T interval<T>::abs( T Number ) 
  { 
    return Number < static_cast<T>(0) ? -Number : +Number;  
  }
  
  // ------------------------------------------------------------
  
  template <class T> T interval<T>::min( T Number1, T Number2 )
  {
    return Number1 < Number2 ? Number1 : Number2;
  }
  
  // ------------------------------------------------------------
  
  template <class T> T interval<T>::max( T Number1, T Number2 )
  {
    return Number1 > Number2 ? Number1 : Number2;
  }
  
  // ------------------------------------------------------------
      
  template <class T> interval<T>::interval() : 
    Min( 0 ), 
    Max( 0 ) 
  {
  }
  
  // ------------------------------------------------------------

  template <class T> interval<T>::interval( T MinVal, T MaxVal ) : 
    Min( min(MinVal,MaxVal) ),
    Max( max(MinVal,MaxVal) ) 
  {
  }
  
  // ------------------------------------------------------------
  
  template <class T> T interval<T>::toInterval( T Value ) const 
  { 
      return Value < min() ? min() :
             Value > max() ? max() :
             Value;
  }

  // ------------------------------------------------------------
  
  template <class T> double interval<T>::partOfDistance( T Value ) const 
  { 
    if ( isSingular() )
      return 0;
    return ( static_cast<double>(Value) - static_cast<double>(min()) ) / static_cast<double>(distance());
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
    Min = min(XMin,XMax);
    Max = max(XMin,XMax);
  }

  // ------------------------------------------------------------
  
  template <class T> void interval<T>::updateInterval( T Number ) 
  { 
    Min = min(Min,Number); 
    Max = max(Max,Number); 
  }
  
  // ------------------------------------------------------------
      
  template <class T> interval<T>& interval<T>::operator+=( const T Value )
  {
    Min += Value;
    Max += Value;
    return *this;
  }
  
  // ------------------------------------------------------------
  
  template <class T> interval<T>& interval<T>::operator-=( const T Value )
  {
    *this += -Value;
    return *this;
  }
  
  // ------------------------------------------------------------
  
  template <class T> interval<T>& interval<T>::operator*=( const T Value )
  {
    Min *= Value;
    Max *= Value;
    if ( Value < static_cast<T>(0) )
      std::swap( Min, Max );
    return *this;
  }
  
  // ------------------------------------------------------------

  template <class T> interval<T>& interval<T>::operator/=( const T Value )
  {
    Min /= Value;
    Max /= Value;
    if ( Value < static_cast<T>(0) )
      std::swap( Min, Max );
    return *this;
  }
  
  // ------------------------------------------------------------
      
  template <class T> bool interval<T>::isEquals( const interval<T> &Lims1, const interval<T> &Lims2, T Difference )
  {
    return 
      abs( Lims1.min() - Lims2.min() ) <= Difference &&
      abs( Lims1.max() - Lims2.max() ) <= Difference;
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

  template <class T> interval<T> operator+( interval<T> Limits, const T Value )
  {
    Limits += Value;
    return Limits;
  }

  // ------------------------------------------------------------

  template <class T> interval<T> operator-( interval<T> Limits, const T Value )
  {
    Limits -= Value;
    return Limits;
  }

  // ------------------------------------------------------------

  template <class T> interval<T> operator*( interval<T> Limits, const T Value )
  {
    Limits *= Value;
    return Limits;
  }

  // ------------------------------------------------------------

  template <class T> interval<T> operator/( interval<T> Limits, const T Value )
  {
    Limits /= Value;
    return Limits;
  }

  // ------------------------------------------------------------

  template <class T> std::ostream& operator<<( std::ostream &Stream, interval<T> Interval )
  {
    Stream << "[ " << Interval.min() << " .. " << Interval.max() << " ]";
    return Stream;
  }
  
  // ------------------------------------------------------------
  
  template <class T> std::istream& operator>>( std::istream &Stream, interval<T> &Interval )
  {
#define INTERVAL_RET_ERROR() \
    do { Stream.setstate(std::istream::failbit); return Stream; } while (false)
    std::istream::sentry Sentry(Stream);
    if ( !Sentry )
      return Stream;

    char Char = '\0';

    Stream >> std::ws;
    if ( ! Stream.get(Char) || Char != '[' )
      INTERVAL_RET_ERROR();

    T Min = 0;
    Stream >> Min;
    if ( ! Stream )
      INTERVAL_RET_ERROR();

    Stream >> std::ws;
    if ( ! Stream.get(Char) || Char != '.' )
      INTERVAL_RET_ERROR();
    if ( ! Stream.get(Char) || Char != '.' )
      INTERVAL_RET_ERROR();

    T Max = 0;
    Stream >> Max;
    if ( ! Stream )
      INTERVAL_RET_ERROR();
    
    Stream >> std::ws;
    if ( ! Stream.get(Char) || Char != ']' )
      INTERVAL_RET_ERROR();

    Interval.setMinMax( Min, Max );
    return Stream;
#undef INTERVAL_RET_ERROR
  }

  // ============================================================


}

// ============================================================

