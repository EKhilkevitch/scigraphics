
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

  template < class DT > class data_iterator
  {
    public:
      typedef DT data_t;
      typedef typename data_t::point_t point_t;
      typedef typename data_t::int_t int_t;

      typedef int_t difference_type;
      typedef std::random_access_iterator_tag iterator_category;
      typedef const point_t value_type;
      typedef const point_t* pointer;
      typedef const point_t& reference;

    private:
      const data_t *Data;
      int_t Index;
      int_t DataSize;
      mutable point_t CurrPoint;

    public:
      data_iterator();
      data_iterator( const data_t &D, int_t I );
      ~data_iterator() {}

      data_iterator operator++()                                { Index++; return *this; }
      data_iterator operator++(int)                             { data_iterator I = *this; ++(*this); return I; }
      difference_type operator-( const data_iterator &I ) const { return Index - I.Index; }
      data_iterator& operator+=( difference_type N )            { Index += N; return *this; }
      data_iterator& operator-=( difference_type N )            { *this += (-N); return *this; }
      data_iterator operator+( difference_type N ) const        { data_iterator I = *this; I += N; return I; }
      data_iterator operator-( difference_type N ) const        { return *this + (-N); }
      bool operator==( const data_iterator& I ) const           { return I.Index == Index; }
      bool operator!=( const data_iterator& I ) const           { return !(*this==I); }
      bool operator<(  const data_iterator& I ) const           { return Index < I.Index; }

      value_type operator *() const                             { return Data->at(Index); }
      pointer operator->() const                                { CurrPoint = Data->at(Index); return &CurrPoint; }

  };

  // ============================================================
 
  template <class DT> data_iterator<DT>::data_iterator() :
    Data( NULL ),
    Index( 0 ),
    DataSize( 0 ) 
  {
  }

  // ------------------------------------------------------------
  
  template <class DT> data_iterator<DT>::data_iterator( const data_t &D, int_t I ) :
    Data( &D ),
    Index( I ),
    DataSize( D.size() )
  {
  }
  
  // ============================================================
}

