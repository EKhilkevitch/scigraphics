
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

#include <map>
#include <set>
#include <cassert>

// ============================================================

namespace scigraphics 
{

  // ============================================================

  class plotElement;
  class pairScales;
  class axisSet;
  class scale;
  class painter;

  // ============================================================

  class plotElementsCollection 
  {
    friend class plotElement;

    public:
      typedef container_ptr< plotElement > plotElementList;

      enum gridDrawStatusType
      {
        GridNotDrawnYet,
        GridAlreadyDrawn
      };
      
    private:
      class axisSetsPair
      {
        private:
          const axisSet *AxisX, *AxisY;

        public:
          axisSetsPair();
          axisSetsPair( const axisSet *AxisX, const axisSet *AxisY );

          bool isZero() const;
          pairScales createPairScales() const;

          const scale* scaleX() const; 
          const scale* scaleY() const;

          const axisSet* axisSetX() const { return AxisX; }
          const axisSet* axisSetY() const { return AxisY; }
      };
      
      typedef std::map< const plotElement*, axisSetsPair > axisBindMap;
      
    private:
      axisSetsPair DefaultAxisSets;

      axisBindMap AxisBindMap;
      plotElementList PlotElementsList;

    protected:
      const plotElementList& getPlotElementsList() const { return PlotElementsList; }
      plotElementList& getPlotElementsList() { return PlotElementsList; }
      
      const axisSetsPair *getAxisSetsPair( const plotElement *PlotElement ) const;
      pairScales getPairScales( const plotElement *PlotElement ) const;

      void appendPlotElement( plotElement *PlotElement );
      void removePlotElement( plotElement *PlotElement );
      void releasePlotElement( plotElement *PlotElement );
      bool existsPlotElement( const plotElement *PlotElement ) const;
      plotElementList::iterator erasePlotElementIterator( plotElementList::iterator Iterator );
      plotElementList::iterator releasePlotElementIterator( plotElementList::iterator Iterator );
      void clearAllElements();
      
    public:
      plotElementsCollection();
      virtual ~plotElementsCollection() = 0;

      size_t size() const;
      bool empty() const;

      void setDefaultAxisSets( const axisSet *X, const axisSet *Y );

      void draw( painter &Painter, gridDrawStatusType GridDrawStatus ) const;

      const scale* graphScaleX( const plotElement *PlotElement ) const;
      const scale* graphScaleY( const plotElement *PlotElement ) const;
          
      const axisSet* graphAxisSetX( const plotElement *PlotElement ) const;
      const axisSet* graphAxisSetY( const plotElement *PlotElement ) const;

      std::set< const axisSet* > setOfVisibleAxisSet() const;

      void bindGraphToAxisSet( const plotElement *PlotElement, const axisSet *AxisX, const axisSet *AxisY );
  };

  // ============================================================

  template <class T> class templatePlotElementsCollection : public plotElementsCollection
  {
    public:
      typedef container_ptr< T > templatePlotElementList;
      typedef typename templatePlotElementList::iterator iterator;
      typedef typename templatePlotElementList::const_iterator const_iterator;
      typedef typename templatePlotElementList::reverse_iterator reverse_iterator;
      typedef typename templatePlotElementList::const_reverse_iterator const_reverse_iterator;

    private:
      template <class R, class I> static R castIterator( const I& Iterator );

    public:
      void append( T *PlotElement );
      void remove( T *PlotElement );
      void release( T *PlotElement );
      bool exists( const T *PlotElement ) const;
      void clear();
      
      iterator begin() { return castIterator<iterator>(getPlotElementsList().begin()); }
      iterator end()   { return castIterator<iterator>(getPlotElementsList().end());   }

      const_iterator begin() const { return castIterator<const_iterator>(getPlotElementsList().begin()); }
      const_iterator end()   const { return castIterator<const_iterator>(getPlotElementsList().end()); }
      
      reverse_iterator rbegin() { return castIterator<reverse_iterator>(getPlotElementsList().rbegin()); }
      reverse_iterator rend()   { return castIterator<reverse_iterator>(getPlotElementsList().rend()); }
      
      const_reverse_iterator rbegin() const { return castIterator<const_reverse_iterator>(getPlotElementsList().rbegin()); }
      const_reverse_iterator rend()   const { return castIterator<const_reverse_iterator>(getPlotElementsList().rend()); }

      const T* front() const { return dynamic_cast<const T*>( &getPlotElementsList().front() ); }
      const T* back() const  { return dynamic_cast<const T*>( &getPlotElementsList().back() ); }

      iterator erase( iterator Iterator );
  };

  // ============================================================
  
  template <class T> template <class R, class I> R templatePlotElementsCollection<T>::castIterator( const I& Iterator )
  {
    assert( sizeof(R) == sizeof(I) );
    return *reinterpret_cast<const R*>( &Iterator );
  }

  // ------------------------------------------------------------
 
  template <class T> void templatePlotElementsCollection<T>::append( T *PlotElement ) 
  {
    // reinterpret_cast for MSVC becouse of C2664 error
    appendPlotElement( reinterpret_cast<plotElement*>(PlotElement) );
  }
  
  // ------------------------------------------------------------
  
  template <class T> void templatePlotElementsCollection<T>::remove( T *PlotElement ) 
  {
    removePlotElement( reinterpret_cast<plotElement*>(PlotElement) );
  }
  
  // ------------------------------------------------------------
  
  template <class T> void templatePlotElementsCollection<T>::release( T *PlotElement ) 
  {
    releasePlotElement( reinterpret_cast<plotElement*>(PlotElement) );
  }
  
  // ------------------------------------------------------------
  
  template <class T> bool templatePlotElementsCollection<T>::exists( const T *PlotElement ) const
  {
    return existsPlotElement( reinterpret_cast<const plotElement*>(PlotElement) );
  }
  
  // ------------------------------------------------------------
  
  template <class T> void templatePlotElementsCollection<T>::clear() 
  {
    clearAllElements();
  }
  
  // ------------------------------------------------------------
      
  template <class T> typename templatePlotElementsCollection<T>::iterator templatePlotElementsCollection<T>::erase( iterator Iterator ) 
  {
    return castIterator<iterator>( erasePlotElementIterator( castIterator<plotElementList::iterator>(Iterator) ) );
  }

  // ============================================================

}



