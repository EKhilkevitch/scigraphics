
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

#include "scigraphics/numlimits.h"

#include <iostream>
#include <algorithm>

namespace scigraphics 
{

// ============================================================

  template <class data> class iteratorData
  {
    public:
      typedef typename data::point point;

      typedef int difference_type;
      typedef std::random_access_iterator_tag iterator_category;
      typedef point value_type;
      typedef point* pointer;
      typedef point& reference;

    private:
      int Index;
      const data *Data;
      int DataSize;
      mutable point CurrPoint;

    private:
      void selfConstraintIndex() 
      { 
        Index = std::min<int>( Index, DataSize ); 
        Index = std::max<int>( Index, 0 ); 
      } 

    public:
      iteratorData() : Index(0), Data(NULL), DataSize(0)  {}
      iteratorData( int I, const data *D ) : Index(I), Data(D), DataSize(D->size()) {}
      iteratorData operator++()                         { Index++; selfConstraintIndex(); return *this; }
      iteratorData operator++(int)                      { iteratorData I = *this; ++(*this); return I; }
      int operator-( const iteratorData &I ) const      { return Index - I.Index; }
      iteratorData& operator+=( int N )                 { Index += N; selfConstraintIndex(); return *this; }
      iteratorData& operator-=( int N )                 { *this += (-N); return *this; }
      iteratorData operator+( int N ) const             { iteratorData I = *this; I += N; return I; }
      iteratorData operator-( int N ) const             { return *this + (-N); }
      bool operator==( const iteratorData& I ) const    { return I.Data == Data && I.Index == Index; }
      bool operator!=( const iteratorData& I ) const    { return !(*this==I); }
      bool operator<(  const iteratorData& I ) const    { return Index < I.Index; }
      point operator *() const                          { return Data->at(Index); }
      const point* operator->() const                   { CurrPoint = Data->at(Index); return &CurrPoint; }
      ~iteratorData() {}
  };


  template <class pnt> class dataPoint
  {
    public:
      typedef pnt point;
      typedef iteratorData< dataPoint<point> > iterator;

    public:
      virtual ~dataPoint() {}

      virtual size_t size() const = 0;
      bool empty() const { return size() == 0; }

      virtual point at( int i ) const = 0;
      point operator[]( int i ) const { return at(i); }
      
      point firstPoint() const { return empty() ? point() : at(0); }
      point lastPoint()  const { return empty() ? point() : at(size()-1); }

      virtual iterator begin() const { return iterator( 0, this ); }
      virtual iterator end()   const { return iterator( size(), this ); }

      virtual numberLimits limitsX() const = 0;
      virtual numberLimits limitsY( const interval<number> &LimitsX ) const = 0;
  };

// ============================================================

}

