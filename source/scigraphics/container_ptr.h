
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

#include <typeinfo>
#include <algorithm>

// ============================================================

namespace scigraphics 
{
  
  // ============================================================

  template <class type, class iterator> iterator find_pointer( iterator Begin, iterator End, const type *Pointer );
  
  // ============================================================

  template < template <class,class> class container, class T, class Allocator = std::allocator<T*> > class container_ptr
  {
    public:
      typedef container< T*, Allocator > container_t;

      template < class iterator_t, class pointer_t, class reference_t > class iterator_wrapper 
      {
        public:
          typedef iterator_t container_iterator;

          typedef typename container_iterator::difference_type    difference_type;
          typedef typename container_iterator::iterator_category  iterator_category;
          typedef T  value_type;
          typedef pointer_t pointer;
          typedef reference_t reference;

        private:
          container_iterator Iterator;

        public:
          iterator_wrapper() {}
          iterator_wrapper( const container_iterator &I ) : Iterator(I) {}
          iterator_wrapper( const iterator_wrapper &I ) : Iterator( I.get() ) {}
          iterator_wrapper& operator=( const container_iterator &I ) { Iterator = I; return *this; }
          reference operator*() const { return **Iterator; }
          pointer operator->() const { return *Iterator; }
          bool operator==( const iterator_wrapper &I ) const { return Iterator == I.Iterator; }
          bool operator!=( const iterator_wrapper &I ) const { return Iterator != I.Iterator; }
          bool operator< ( const iterator_wrapper &I ) const { return Iterator  < I.Iterator; }
          bool operator> ( const iterator_wrapper &I ) const { return Iterator  > I.Iterator; }
          iterator_wrapper& operator++()      { ++Iterator; return *this; }
          iterator_wrapper  operator++( int ) { iterator_wrapper I = *this; ++Iterator; return I; }
          iterator_wrapper& operator--()      { --Iterator; return *this; }
          iterator_wrapper  operator--(int)   { iterator_wrapper I = *this; --Iterator; return I; }
          iterator_wrapper& operator+=( typename container_iterator::difference_type N ) { Iterator += N; return *this; }
          iterator_wrapper& operator-=( typename container_iterator::difference_type N ) { Iterator -= N; return *this; }
          iterator_wrapper  operator+( typename container_iterator::difference_type N ) const { return iterator_wrapper( Iterator + N ); }
          iterator_wrapper  operator-( typename container_iterator::difference_type N ) const { return iterator_wrapper( Iterator - N ); }
          const container_iterator& get() const { return Iterator; }
          container_iterator& get() { return Iterator; }
          bool isNull() const { return (*Iterator) == static_cast<const T*>(0); };
      };

      typedef iterator_wrapper< typename container_t::iterator, T*, T& > iterator;
      typedef iterator_wrapper< typename container_t::const_iterator, const T*, const T& > const_iterator;
      typedef iterator_wrapper< typename container_t::reverse_iterator, T*, T& > reverse_iterator;
      typedef iterator_wrapper< typename container_t::const_reverse_iterator, const T*, const T& > const_reverse_iterator;
      
      typedef typename container_t::value_type value_type;
      typedef typename container_t::size_type size_type;

    private:
      container_t Container;

    public:
      container_ptr() {}
      container_ptr( const container_ptr &Cnt ) { *this = Cnt; }
      container_ptr( const container_t &Cnt ) : Container(Cnt) {}
      ~container_ptr() { clear(); }
      
      container_ptr& operator=( const container_ptr &Cnt );

      void push_back( T *Value );
      iterator erase( iterator I );

      void clear();

      iterator begin() { return Container.begin(); }
      iterator end() { return Container.end(); }

      const_iterator begin() const { return Container.begin(); }
      const_iterator end() const { return Container.end(); }
      
      reverse_iterator rbegin() { return Container.rbegin(); }
      reverse_iterator rend() { return Container.rend(); }
        
      const_reverse_iterator rbegin() const { return Container.rbegin(); }
      const_reverse_iterator rend() const { return Container.rend(); }    

      T& front() { return *Container.front(); }
      T& back()  { return *Container.back(); }
      
      const T& front() const { return *Container.front(); }
      const T& back() const  { return *Container.back(); }

      const T& operator[]( size_t i ) const { return *( Container[i] ); }
      T& operator[]( size_t i ) { return *( Container[i] ); }

      const T& at( size_t i ) const { return *( Container.at(i) ); }
      T& at( size_t i ) { return *( Container.at(i) ); }

      T* set( size_t Index, T *Pointer );

      const container_t& get() const { return Container; }

      bool empty() const { return Container.empty(); }
      size_type size() const { return Container.size(); }

      void resize( size_type Size ) { Container.resize( Size, NULL ); }
      void reserve( size_type Size ) { Container.reserve( Size ); }
  };

  // ============================================================

  template < template <class,class> class container, class T, class Allocator > container_ptr<container,T,Allocator>& 
      container_ptr<container,T,Allocator>::operator=( const container_ptr &Container )
  {
    if ( &Container == this )
      return *this;
      
    clear();

    for ( const_iterator i = Container.begin(); i != Container.end(); ++i )
    {
      T* Clone = i.isNull() ? static_cast<T*>(0) : i->clone();
      try
      {
        push_back( Clone );
      } catch ( ... )
      {
        delete Clone;
        throw;
      }
    }

    return *this;
  }

  // ------------------------------------------------------------
  
  template < template <class,class> class container, class T, class Allocator > void container_ptr<container,T,Allocator>::push_back( T *Value ) 
  { 
    Container.push_back( Value ); 
  }
  
  // ------------------------------------------------------------
  
  template < template <class,class> class container, class T, class Allocator > typename container_ptr<container,T,Allocator>::iterator container_ptr<container,T,Allocator>::erase( iterator I ) 
  { 
    delete &*I; 
    typename container_t::iterator Next = Container.erase(I.get()); 
    return iterator(Next);
  }
  
  // ------------------------------------------------------------

  template < template <class,class> class container, class T, class Allocator > void container_ptr<container,T,Allocator>::clear()
  {
    for ( iterator i = Container.begin(); i != Container.end(); ++i )
      delete &( *i );
    Container.clear();
  } 

  // ------------------------------------------------------------
      
  template < template <class,class> class container, class T, class Allocator > T* container_ptr<container,T,Allocator>::set( size_t Index, T *Pointer )
  {
    T* ContainedPointer = Container.at(Index);
    if ( ContainedPointer != Pointer )
    {
      delete ContainedPointer;
      Container[Index] = Pointer;
    }
    return Pointer;
  }

  // ============================================================
  
  template <class type, class iterator> iterator find_pointer( iterator Begin, iterator End, const type *Pointer )
  {
    for ( iterator i = Begin; i != End; ++i )
      if ( &( *i ) == Pointer )
        return i;
    return End;
  }

  // ============================================================

}

// ============================================================

