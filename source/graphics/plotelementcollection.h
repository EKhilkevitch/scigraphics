
#pragma once

// ============================================================

#include "graphics/plotelement.h"
#include "graphics/container_ptr.h"
#include "graphics/scale.h"

#include <list>
#include <map>
#include <set>
#include <stdexcept>
#include <algorithm>
#include <cassert>

// ============================================================

namespace scigraphics 
{

// ============================================================

  class axisSet;

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
      virtual void erase( plotElement *PlotElement );
      virtual void clear();

      bool exist( const plotElement *PlotElement ) const;

      void setDefaultAxisSets( const axisSet *X, const axisSet *Y );

      virtual void draw( painter &Painter ) const;

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
      typedef typename std::list<T*>::iterator iterator;
      typedef typename templatePlotElementList::const_iterator const_iterator;
      typedef typename templatePlotElementList::const_reverse_iterator const_reverse_iterator;

    private:
      template <class R, class I> static R castIterator( const I& Iterator )
      {
        assert( sizeof(R) == sizeof(I) );
        return *reinterpret_cast<const R*>( &Iterator );
      }

    public:
      void append( plotElement *PlotElement ) 
      {
        if ( PlotElement != NULL )
          if ( dynamic_cast<T*>( PlotElement ) == NULL )
            throw std::invalid_argument("Invalid type of appended argument");
        plotElementsCollection::append( PlotElement );
      }
      
      iterator begin() { return castIterator<iterator>(getPlotElementsList().begin()); }
      iterator end()   { return castIterator<iterator>(getPlotElementsList().end());   }

      const_iterator begin() const { return castIterator<const_iterator>(getPlotElementsList().begin()); }
      const_iterator end()   const { return castIterator<const_iterator>(getPlotElementsList().end()); }
      
      const_reverse_iterator rbegin() const { return castIterator<const_reverse_iterator>(getPlotElementsList().rbegin()); }
      const_reverse_iterator rend()   const { return castIterator<const_reverse_iterator>(getPlotElementsList().rend()); }

      const T* front() const { return dynamic_cast<const T*>( getPlotElementsList().front() ); }
      const T* back() const  { return dynamic_cast<const T*>( getPlotElementsList().back() ); }
  };

// ============================================================

}



