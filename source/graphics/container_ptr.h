
#pragma once

#include <typeinfo>
#include <algorithm>

// ============================================================

namespace graphics {

// ============================================================

  template < template <class,class> class container, class T > class container_ptr
  {
    public:
      typedef container< T*, std::allocator<T*> > container_t;
      typedef typename container_t::iterator iterator;
      typedef typename container_t::const_iterator const_iterator;
      typedef typename container_t::const_reverse_iterator const_reverse_iterator;

      class hasTheSameTypeChecker
      {
        private:
          const T *Base;
        public:
          hasTheSameTypeChecker( const T *Ptr ) : Base(Ptr) {}
          bool operator()( const T *Ptr ) const { return typeid(*Base) == typeid(*Ptr); }
      };

    private:
      container_t Container;

    public:
      template <class X> static bool hasTheSameType( const T *Pointer )  { return typeid(*Pointer) == typeid(const X); }
      static bool hasTheSameType( const T *Pointer1, const T *Pointer2 ) { return hasTheSameTypeChecker(Pointer1)(Pointer2); }

      container_ptr() {}
      container_ptr( const container_ptr &Cnt ) { *this = Cnt; }
      container_ptr( const container_t &Cnt ) : Container(Cnt) {}
      
      container_ptr& operator=( const container_ptr &Cnt )
      {
        clear();
        for ( const_iterator i = Cnt.begin(); i != Cnt.end(); ++i )
          push_back( (*i)->clone() );
        return *this;
      }

      void push_back( T *Value ) { Container.push_back( Value ); }
      void erase( iterator I ) { delete *I; Container.erase(I); }

      void clear()
      {
        for ( iterator i = Container.begin(); i != Container.end(); ++i )
          delete *i;
        Container.clear();
      }

      template <class X> iterator find_type() 
        { return std::find_if( Container.begin(), Container.end(), hasTheSameType<X> ); }

      template <class X> const_iterator find_type() const
        { return std::find_if( Container.begin(), Container.end(), hasTheSameType<X> ); }

      const_iterator find_same_type( const T *Pointer ) const
        { return std::find_if( Container.begin(), Container.end(), hasTheSameTypeChecker(Pointer) ); }
      
      iterator find_same_type( const T *Pointer ) 
        { return std::find_if( Container.begin(), Container.end(), hasTheSameTypeChecker(Pointer) ); }
    
      iterator find( const T *Pointer )
        { return std::find( Container.begin(), Container.end(), Pointer ); } 

      iterator begin() { return Container.begin(); }
      iterator end() { return Container.end(); }

      const_iterator begin() const { return Container.begin(); }
      const_iterator end() const { return Container.end(); }
      
      const_reverse_iterator rbegin() const { return Container.rbegin(); }
      const_reverse_iterator rend() const { return Container.rend(); }

      T* front() { return Container.front(); }
      T* back()  { return Container.back(); }
      
      const T* front() const { return Container.front(); }
      const T* back() const  { return Container.back(); }

      const T* operator[]( unsigned i ) const { return Container[i]; }
      T*& operator[]( unsigned i ) { return Container[i]; }
    
      const T* at( unsigned i ) const { return Container.at(i); }
      T*& at( unsigned i ) { return Container.at(i); }

      bool empty() const { return Container.empty(); }
      size_t size() const { return Container.size(); }
      void resize( size_t S ) { Container.resize(S); }

      virtual ~container_ptr() { clear(); }
  };

  // ------------------------------------------------------------

  template < template <class,class> class container, class base_type > class container_type_ptr
  {
    public:
      typedef typename container_ptr<container,base_type>::container_t container_t;
      typedef typename container_ptr<container,base_type>::iterator iterator;
      typedef typename container_ptr<container,base_type>::const_iterator const_iterator;

    private:
      container_ptr<container,base_type> Container;

    public:
      container_type_ptr() {}

      container_type_ptr( const container_ptr<container,base_type> &Cnt ) 
      { 
        for ( const_iterator i = Cnt.begin(); i != Cnt.end(); ++i ) 
        {
          base_type *Clone = (*i)->clone();
          bool Pushed = push_back( Clone, false );
          if ( ! Pushed ) delete Clone;
        }
      }

      template <class T> bool exist() const { return find_type<T>() != end(); }
      bool exist_same_type( const base_type *Pointer ) const { return find_same_type(Pointer) != end(); }
      
      bool push_back( base_type *Value, bool Replace = true ) 
      { 
        if ( Value == NULL ) return false;
        if ( Replace ) erase_same_type(Value);
        if ( exist_same_type(Value) ) return false;
        Container.push_back(Value); 
        return true;
      }
      template <class T> bool push_back( bool Replace = false ) 
      { 
        T *NewElement = new T();
        bool Pushed = push_back( NewElement, Replace ); 
        if ( ! Pushed ) delete NewElement;
        return Pushed;
      }
      
      template <class T> iterator find_type() { return Container.find_type<T>(); }
      template <class T> const_iterator find_type() const { return Container.find_type<T>(); }

      const_iterator find_same_type( const base_type *Pointer ) const { return Container.find_same_type(Pointer); }
      iterator find_same_type( const base_type *Pointer ) { return Container.find_same_type(Pointer); }

      template <class T> void erase() { iterator i = find_type<T>(); erase(i); }
      void erase_same_type( const base_type *Pointer ) { iterator i = find_same_type(Pointer); erase(i); }
      void erase( iterator i ) { if ( i != Container.end() ) Container.erase(i); }

      template <class T> T* get() { iterator i = find_type<T>(); return ( i==end() ) ? NULL : dynamic_cast<T*>(*i); }
      template <class T> const T* get() const { const_iterator i = find_type<T>(); return ( i==end() ) ? NULL : dynamic_cast<const T*>(*i); }

      void clear() { Container.clear(); }

      iterator begin() { return Container.begin(); }
      iterator end() { return Container.end(); }

      const_iterator begin() const { return Container.begin(); }
      const_iterator end() const { return Container.end(); }

      base_type* front() { return Container.front(); }
      base_type* back()  { return Container.back(); }
      
      const base_type* front() const { return Container.front(); }
      const base_type* back() const  { return Container.back(); }

      bool empty() const { return Container.empty(); }
      size_t size() const { return Container.size(); }

      virtual ~container_type_ptr() { clear(); }
  };


// ============================================================

}

// ============================================================

