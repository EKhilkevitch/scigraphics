
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

// ============================================================

#include "scigraphics/container_ptr.h"

#include <algorithm>
#include <typeinfo>

// ============================================================

namespace scigraphics
{

  // ============================================================

  template <class type, class iterator> iterator find_same_type( iterator Begin, iterator End, const type *Pointer );
  template <class type, class iterator> iterator find_type( iterator Begin, iterator End );

  template < template <class,class> class container, class T, class Allocator = std::allocator<T*> > class container_type_ptr
  {
    public:
      typedef typename container_ptr<container,T>::container_t container_t;
      typedef typename container_ptr<container,T>::iterator iterator;
      typedef typename container_ptr<container,T>::const_iterator const_iterator;

    private:
      container_ptr<container,T> Container;

    public:
      container_type_ptr() {}
      container_type_ptr( const container_ptr<container,T> &Cnt );

      bool push_back( T *Value, bool Replace = true );
      template <class U> bool push_back( bool Replace = false );

      void erase( iterator i ) { Container.erase(i); }
      void clear() { Container.clear(); }

      iterator begin() { return Container.begin(); }
      iterator end() { return Container.end(); }

      const_iterator begin() const { return Container.begin(); }
      const_iterator end() const { return Container.end(); }

      T& front() { return *Container.front(); }
      T& back()  { return *Container.back(); }
      
      const T& front() const { return *Container.front(); }
      const T& back() const  { return *Container.back(); }

      bool empty() const { return Container.empty(); }
      size_t size() const { return Container.size(); }

  };

  // ============================================================

  template < template <class,class> class container, class T, class Allocator > container_type_ptr<container,T,Allocator>::container_type_ptr( const container_ptr<container,T> &Cnt ) 
  { 
    for ( const_iterator i = Cnt.begin(); i != Cnt.end(); ++i ) 
    {
      T *Clone = i->clone();

      bool Pushed = false;
      
      try
      {
        Pushed = push_back( Clone, false );
      } catch ( ... )
      {
        delete Clone;
        throw;
      }

      if ( ! Pushed ) 
        delete Clone;
    }
  }

  // ------------------------------------------------------------

  template < template <class,class> class container, class T, class Allocator > bool container_type_ptr<container,T,Allocator>::push_back( T *Value, bool Replace ) 
  { 
    if ( Value == NULL ) 
      return false;

    iterator i = find_same_type( begin(), end(), *Value );

    if ( i != end() )
    {
      if ( Replace )
      {
        erase(i);
      } else {
        return false;
      }
    }
    
    Container.push_back(Value); 

    return true;
  }

  // ------------------------------------------------------------

  template < template <class,class> class container, class T, class Allocator > template <class U> bool container_type_ptr<container,T,Allocator>::push_back( bool Replace ) 
  { 
    T *NewElement = new U();
    bool Pushed = false;
    
    try
    {
      Pushed = push_back( NewElement, Replace ); 
    } catch ( ... )
    {
      delete NewElement;
      throw;
    }

    if ( ! Pushed ) 
      delete NewElement;

    return Pushed;
  }

  // ============================================================

  template <class T> class has_same_type_checker
  {
    private:
      const std::type_info *TypeInfo;
    public:
      has_same_type_checker() : TypeInfo( &typeid(T) ) {}
      has_same_type_checker( const T &Value ) : TypeInfo( &typeid(Value) ) {}
      template <class V> bool operator()( const V &Value ) const { return *TypeInfo == typeid(Value); }
  };

  // ------------------------------------------------------------

  template <class T> has_same_type_checker<T> has_same_type() 
  { 
    return has_same_type_checker<T>();
  }

  // ------------------------------------------------------------

  template <class T> has_same_type_checker<T> has_same_type( const T &Pointer ) 
  { 
    return has_same_type_checker<T>(Pointer);
  }

  // ------------------------------------------------------------

  template <class type, class iterator> iterator find_same_type( iterator Begin, iterator End, const type &Pointer )
  {
    return std::find_if( Begin, End, has_same_type<type>(Pointer) );
  }

  // ------------------------------------------------------------

  template <class type, class iterator> iterator find_type( iterator Begin, iterator End )
  {
    return std::find_if( Begin, End, has_same_type<type>() );
  }

  // ============================================================

}

