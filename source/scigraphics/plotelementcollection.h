
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

#include "scigraphics/plotelement.h"
#include "scigraphics/container_ptr.h"

#include <list>
#include <map>
#include <set>
#include <stdexcept>
#include <cassert>

// ============================================================

namespace scigraphics 
{

  // ============================================================

  class axisSet;
  class scale;

  // ============================================================

  class plotElementsCollection 
  {
    public:
      typedef container_ptr< std::list, plotElement > plotElementList;
      
    protected:
      class axisSetsPair
      {
        private:
          const axisSet *AxisX, *AxisY;

        public:
          axisSetsPair() : AxisX(NULL), AxisY(NULL) {}
          axisSetsPair( const axisSet *X, const axisSet *Y ) : AxisX(X), AxisY(Y) {}

          bool isZero() const { return AxisX == NULL || AxisY == NULL; }
          pairScales createPairScales() const;

          const scale* scaleX() const; 
          const scale* scaleY() const;

          const axisSet* axisSetX() const { return AxisX; }
          const axisSet* axisSetY() const { return AxisY; }
      };
      
      axisSetsPair DefaultAxisSets;

      typedef std::map< const plotElement*, axisSetsPair > axisBindMap;
      axisBindMap AxisBindMap;

    private:
      plotElementList PlotElementsList;

    protected:
      const plotElementList& getPlotElementsList() const { return PlotElementsList; }
      plotElementList& getPlotElementsList() { return PlotElementsList; }
      
      const axisSetsPair *getAxisSetsPair( const plotElement *PlotElement ) const;
      pairScales getPairScales( const plotElement *PlotElement ) const;
      
    public:
      plotElementsCollection() {}
      virtual ~plotElementsCollection() {}

      size_t size() const { return PlotElementsList.size(); }
      bool  empty() const { return PlotElementsList.empty(); }

      virtual void append( plotElement *PlotElement );
      virtual void remove( plotElement *PlotElement );
      virtual void clear();

      bool exist( const plotElement *PlotElement ) const;

      void setDefaultAxisSets( const axisSet *X, const axisSet *Y );

      virtual void draw( painter &Painter, bool isGridDrawn ) const;

      const scale* graphScaleX( const plotElement *PlotElement ) const;
      const scale* graphScaleY( const plotElement *PlotElement ) const;
          
      const axisSet* graphAxisSetX( const plotElement *PlotElement ) const;
      const axisSet* graphAxisSetY( const plotElement *PlotElement ) const;

      std::set< const axisSet* > setOfGraphAxisSet() const;

      void bindGraphToAxisSet( const plotElement *PlotElement, const axisSet *X, const axisSet *Y );
  };

  // ============================================================

  template <class T> class templatePlotElementsCollection : public plotElementsCollection
  {
    public:
      typedef container_ptr< std::list, T > templatePlotElementList;
      typedef typename templatePlotElementList::iterator iterator;
      typedef typename templatePlotElementList::const_iterator const_iterator;
      typedef typename templatePlotElementList::const_reverse_iterator const_reverse_iterator;

    private:
      template <class R, class I> static R castIterator( const I& Iterator );

    public:
      void append( T *PlotElement );
      
      iterator begin() { return castIterator<iterator>(getPlotElementsList().begin()); }
      iterator end()   { return castIterator<iterator>(getPlotElementsList().end());   }

      const_iterator begin() const { return castIterator<const_iterator>(getPlotElementsList().begin()); }
      const_iterator end()   const { return castIterator<const_iterator>(getPlotElementsList().end()); }
      
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
    plotElementsCollection::append( PlotElement );
  }
  
  // ------------------------------------------------------------
      
  template <class T> typename templatePlotElementsCollection<T>::iterator templatePlotElementsCollection<T>::erase( iterator Iterator ) 
  { 
    return castIterator<iterator>( getPlotElementsList().erase( castIterator<plotElementList::iterator>(Iterator) ) ); 
  }

  // ============================================================

}



