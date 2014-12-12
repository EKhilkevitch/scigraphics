
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
#include <algorithm>
#include <vector>
#include <cassert>

// ============================================================

namespace scigraphics 
{
  
  // ============================================================

  template <class type, class iterator> iterator find_pointer( iterator Begin, iterator End, const type *Pointer );
  
  // ============================================================

    template < class T, template <class,class> class container = std::vector, class Allocator = std::allocator<T*> > class container_ptr
  {
    public:
      typedef container< T*, Allocator > container_t;

      template < class iterator_t, class U > class iterator_wrapper 
      {
        public:
          typedef iterator_t container_iterator;

          typedef typename container_iterator::difference_type    difference_type;
          typedef typename container_iterator::iterator_category  iterator_category;
          typedef U  value_type;
          typedef U* pointer;
          typedef U& reference;

        private:
          container_iterator Iterator;

        public:
          iterator_wrapper() {}
          iterator_wrapper( const container_iterator &I ) : Iterator( I ) {}
          template <class it> iterator_wrapper( const it &I ) : Iterator( I.get() ) {}

          iterator_wrapper& operator=( const container_iterator &I ) { Iterator = I; return *this; }

          reference operator*() const { return **Iterator; }
          pointer operator->() const { return *Iterator; }
          
          bool operator==( const iterator_wrapper &I ) const { return Iterator == I.Iterator; }
          bool operator!=( const iterator_wrapper &I ) const { return Iterator != I.Iterator; }
          bool operator< ( const iterator_wrapper &I ) const { return Iterator  < I.Iterator; }
          bool operator> ( const iterator_wrapper &I ) const { return Iterator  > I.Iterator; }
          
          iterator_wrapper& operator++()            { ++Iterator; return *this; }
          const iterator_wrapper  operator++(int)   { iterator_wrapper I = *this; ++Iterator; return I; }
          iterator_wrapper& operator--()            { --Iterator; return *this; }
          const iterator_wrapper  operator--(int)   { iterator_wrapper I = *this; --Iterator; return I; }
          iterator_wrapper& operator+=( difference_type N ) { Iterator += N; return *this; }
          iterator_wrapper& operator-=( difference_type N ) { Iterator -= N; return *this; }

          const iterator_wrapper  operator+( difference_type N ) const { return iterator_wrapper( Iterator + N ); }
          const iterator_wrapper  operator-( difference_type N ) const { return iterator_wrapper( Iterator - N ); }
          difference_type operator-( iterator_wrapper I ) const { return Iterator - I.Iterator; }
          
          const container_iterator& get() const { return Iterator; }
          container_iterator& get() { return Iterator; }
          
          bool isNull() const { return (*Iterator) == static_cast<const T*>(0); };
      };

      typedef iterator_wrapper< typename container_t::iterator, T > iterator;
      typedef iterator_wrapper< typename container_t::const_iterator, const T > const_iterator;
      typedef iterator_wrapper< typename container_t::reverse_iterator, T > reverse_iterator;
      typedef iterator_wrapper< typename container_t::const_reverse_iterator, const T > const_reverse_iterator;

      typedef typename container_t::value_type value_type;
      typedef typename container_t::size_type size_type;

    private:
      container_t Container;

    public:
      container_ptr() {}
      container_ptr( const container_ptr &Container );
      container_ptr( const container_t &Container );
      ~container_ptr();
      
      container_ptr& operator=( const container_ptr &Container );

      void push_back( T *Value );
      iterator erase( iterator I );
      iterator erase( iterator First, iterator Last );

      void clear();

      iterator begin() { return Container.begin(); }
      iterator end() { return Container.end(); }

      const_iterator begin() const { return cbegin(); }
      const_iterator end() const { return cend(); }
      
      const_iterator cbegin() const { return Container.begin(); }
      const_iterator cend() const { return Container.end(); }
      
      reverse_iterator rbegin() { return Container.rbegin(); }
      reverse_iterator rend() { return Container.rend(); }
        
      const_reverse_iterator rbegin() const { return crbegin(); }
      const_reverse_iterator rend() const { return crend(); }    
      
      const_reverse_iterator crbegin() const { return Container.rbegin(); }
      const_reverse_iterator crend() const { return Container.rend(); }    

      T& front() { return *Container.front(); }
      T& back()  { return *Container.back(); }
      
      const T& front() const { return *Container.front(); }
      const T& back() const  { return *Container.back(); }

      const T& operator[]( size_t i ) const { return *( Container[i] ); }
      T& operator[]( size_t i ) { return *( Container[i] ); }

      const T& at( size_t i ) const { return *( Container.at(i) ); }
      T& at( size_t i ) { return *( Container.at(i) ); }

      T* release( size_t Index );
      T* release( iterator Iterator );
      T* releaseFront();
      T* releaseBack();
      
      T* set( size_t Index, T *Pointer );

      const container_t& get() const { return Container; }

      bool empty() const { return Container.empty(); }
      size_type size() const { return Container.size(); }

      void resize( size_type Size ) { Container.resize( Size, NULL ); }
      void reserve( size_type Size ) { Container.reserve( Size ); }
  };

  // ============================================================
  
    template < class T, template <class,class> class container, class Allocator > container_ptr<T,container,Allocator>::container_ptr( const container_ptr &Cnt ) 
  { 
    *this = Cnt; 
  }

  // ------------------------------------------------------------

  template < class T, template <class,class> class container, class Allocator > container_ptr<T,container,Allocator>::container_ptr( const container_t &Cnt ) : 
    Container(Cnt) 
  {
  }
  
  // ------------------------------------------------------------
  
  template < class T, template <class,class> class container, class Allocator > container_ptr<T,container,Allocator>::~container_ptr() 
  { 
    clear(); 
  }
  
  // ------------------------------------------------------------

  template < class T, template <class,class> class container, class Allocator > container_ptr<T,container,Allocator>& 
      container_ptr<T,container,Allocator>::operator=( const container_ptr &Cnt )
  {
    if ( &Cnt == this )
      return *this;
      
    clear();

    for ( const_iterator i = Cnt.begin(); i != Cnt.end(); ++i )
    {
      T* Clone = i.isNull() ? static_cast<T*>(NULL) : i->clone();
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

  template < class T, template <class,class> class container, class Allocator > void container_ptr<T,container,Allocator>::clear()
  {
    for ( iterator i = Container.begin(); i != Container.end(); ++i )
      delete &( *i );
    Container.clear();
  }
  
  // ------------------------------------------------------------
  
  template < class T, template <class,class> class container, class Allocator > void container_ptr<T,container,Allocator>::push_back( T *Value )
  {
    Container.push_back( Value );
  }
  
  // ------------------------------------------------------------
  
  template < class T, template <class,class> class container, class Allocator > typename container_ptr<T,container,Allocator>::iterator
     container_ptr<T,container,Allocator>::erase( iterator Iterator )
  {
    T *Pointer = &*Iterator;
    delete Pointer;

    typename container_t::iterator Next = Container.erase( Iterator.get() );
    return iterator(Next);
  }
  
  // ------------------------------------------------------------
      
  template < class T, template <class,class> class container, class Allocator > typename container_ptr<T,container,Allocator>::iterator 
    container_ptr<T,container,Allocator>::erase( iterator First, iterator Last )
  {
    for ( iterator i = First; i != Last; ++i )
    {
      T *Pointer = &*i;
      delete Pointer;
    }
    
    typename container_t::iterator Next = Container.erase( First.get(), Last.get() );
    return iterator(Next);
  }
  
  // ------------------------------------------------------------
      
  template < class T, template <class,class> class container, class Allocator > T* container_ptr<T,container,Allocator>::set( size_t Index, T *Pointer )
  {
    T* ContainedPointer = Container.at(Index);
    if ( ContainedPointer != Pointer )
    {
      delete ContainedPointer;
      Container[Index] = Pointer;
    }
    return Pointer;
  }
  
  // ------------------------------------------------------------
      
  template < class T, template <class,class> class container, class Allocator > T* container_ptr<T,container,Allocator>::release( size_t Index )
  {
    iterator Iterator = begin();
    std::advance( Iterator, Index );
    return release( Iterator );
  }
  
  // ------------------------------------------------------------
  
  template < class T, template <class,class> class container, class Allocator > T* container_ptr<T,container,Allocator>::release( iterator Iterator )
  {
    T* Pointer = &*Iterator;
    Container.erase( Iterator.get() );
    return Pointer;
  }
      
  // ------------------------------------------------------------
  
  template < class T, template <class,class> class container, class Allocator > T* container_ptr<T,container,Allocator>::releaseFront()
  {
    T *Pointer = Container.front();
    Container.pop_front();
    return Pointer;
  }
  
  // ------------------------------------------------------------
  
  template < class T, template <class,class> class container, class Allocator > T* container_ptr<T,container,Allocator>::releaseBack()
  {
    T *Pointer = Container.back();
    Container.pop_back();
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

