
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

#include <iterator>
#include <vector>
#include <limits>

namespace scigraphics 
{
  // ============================================================

  template < class DT > class data_iterator
  {
    public:
      typedef DT data_t;
      typedef typename data_t::point_t point_t;
      typedef typename data_t::int_t int_t;

      typedef long long int difference_type;
      typedef std::random_access_iterator_tag iterator_category;
      typedef const point_t value_type;
      typedef const point_t* pointer;
      typedef const point_t& reference;

    private:
      const data_t *Data;
      int_t Index;
      int_t DataSize;

      struct currentPoint
      {
        point_t Point;
        int_t Index;
        currentPoint();
      } mutable CurrentPoint;

    public:
      data_iterator();
      data_iterator( const data_t &D, int_t I );

      data_iterator operator++();
      data_iterator operator++(int);
      data_iterator operator--();
      data_iterator operator--(int);
      difference_type operator-( const data_iterator &I ) const;
      data_iterator& operator+=( difference_type N );
      data_iterator& operator-=( difference_type N );
      data_iterator operator+( difference_type N ) const;
      data_iterator operator-( difference_type N ) const;

      value_type operator *() const;
      pointer operator->() const;

      data_iterator& fill( size_t Size, std::vector<point_t> *Vector );
      int_t index() const;
  };
      
  template <class DT> inline bool operator==( const data_iterator<DT>& A, const data_iterator<DT>& B );
  template <class DT> inline bool operator!=( const data_iterator<DT>& A, const data_iterator<DT>& B );
  template <class DT> inline bool operator<(  const data_iterator<DT>& A, const data_iterator<DT>& B );
  template <class DT> inline bool operator<=( const data_iterator<DT>& A, const data_iterator<DT>& B );

  // ============================================================
  
  template <class DT> data_iterator<DT>::currentPoint::currentPoint() :
    Index( std::numeric_limits<int_t>::max() )
  {
  }
  
  // ------------------------------------------------------------
 
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
  
  // ------------------------------------------------------------
  
  template <class DT> data_iterator<DT> data_iterator<DT>::operator++()
  {
    ++Index;
    return *this;
  }
  
  // ------------------------------------------------------------
  
  template <class DT> data_iterator<DT> data_iterator<DT>::operator++(int)
  {
    data_iterator Iterator = *this; 
    ++(*this); 
    return Iterator;
  }
  
  // ------------------------------------------------------------
  
  template <class DT> data_iterator<DT> data_iterator<DT>::operator--()
  {
    --Index;
    return *this;
  }
  
  // ------------------------------------------------------------
  
  template <class DT> data_iterator<DT> data_iterator<DT>::operator--(int)
  {
    data_iterator Iterator = *this; 
    --(*this);
    return Iterator;
  }
  
  // ------------------------------------------------------------
      
  template <class DT> data_iterator<DT> data_iterator<DT>::operator+( difference_type N ) const
  { 
    data_iterator Iterator = *this; 
    Iterator += N; 
    return Iterator;
  }
  
  // ------------------------------------------------------------

  template <class DT> data_iterator<DT> data_iterator<DT>::operator-( difference_type N ) const        
  { 
    return (*this) + (-N); 
  }
  
  // ------------------------------------------------------------
  
  template <class DT> typename data_iterator<DT>::difference_type data_iterator<DT>::operator-( const data_iterator &I ) const 
  { 
    return Index - I.Index; 
  }
  
  // ------------------------------------------------------------
  
  template <class DT> data_iterator<DT>& data_iterator<DT>::operator+=( difference_type N )            
  { 
    Index += N; 
    return *this; 
  }
  
  // ------------------------------------------------------------
  
  template <class DT> data_iterator<DT>& data_iterator<DT>::operator-=( difference_type N )            
  { 
    *this += (-N); 
    return *this; 
  }
  
  // ------------------------------------------------------------
      
  template <class DT> const typename data_iterator<DT>::value_type data_iterator<DT>::operator*() const
  { 
    if ( CurrentPoint.Index != Index )
    {
      CurrentPoint.Point = (*Data)[Index];
      CurrentPoint.Index = Index;
    }
    return CurrentPoint.Point;
  }
  
  // ------------------------------------------------------------
  
  template <class DT> typename data_iterator<DT>::pointer data_iterator<DT>::operator->() const                                
  { 
    if ( CurrentPoint.Index != Index )
    {
      CurrentPoint.Point = (*Data)[Index];
      CurrentPoint.Index = Index;
    }
    return &CurrentPoint.Point;
  }
  
  // ------------------------------------------------------------
      
  template <class DT> data_iterator<DT>& data_iterator<DT>::fill( size_t Size, std::vector<point_t> *Vector )
  {
    if ( Vector == NULL || Size <= 0 ) 
      return *this;

    const size_t ReadedSize = Data->fill( Index, Size, Vector );
    Index += ReadedSize;
    Vector->resize( ReadedSize );

    return *this;
  }
  
  // ------------------------------------------------------------
      
  template <class DT> typename data_iterator<DT>::int_t data_iterator<DT>::index() const
  {
    return Index;
  }

  // ============================================================
  
  template <class DT> bool operator==( const data_iterator<DT>& A, const data_iterator<DT>& B ) { return A.index() == B.index(); }
  template <class DT> bool operator!=( const data_iterator<DT>& A, const data_iterator<DT>& B ) { return ! ( A == B ); }
  template <class DT> bool operator<(  const data_iterator<DT>& A, const data_iterator<DT>& B ) { return A.index() < B.index(); }
  template <class DT> bool operator<=( const data_iterator<DT>& A, const data_iterator<DT>& B ) { return A.index() <= B.index(); }
  
  // ============================================================

}

