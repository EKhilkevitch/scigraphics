
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

#include "scigraphics/graph.h"
#include "scigraphics/graphviewmap.h"
#include "scigraphics/datamap.h"

#include <string>

// ============================================================

namespace scigraphics 
{

  // ============================================================
  
  class scale;
    
  // ============================================================

  namespace map
  {

    // ============================================================
   
    class graph : public ::scigraphics::graph
    {
      private:
        graphView *View;
        data *Data;
        scale *ScaleZ;
        interval<number> ForcedIntervalZ;

      protected:
        virtual data* createData() = 0;
        virtual graphView* createView() = 0;

        void init();
        void updateScaleZInterval() const;

      private:
        graph( const graph& );
        graph& operator=( const graph& );

      public:
        explicit graph( const std::string &Legend );
        ~graph();
        
        void draw( painter &Painter, const pairScales& Scales ) const;
        void drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const;
        
        data& getData();
        const data& getData() const;

        graphView& getView();
        const graphView& getView() const;
   
        numberLimits limitsX() const;
        numberLimits limitsY( const interval<number> &LimitsX ) const;
        numberLimits limitsZ() const;

        void setForcedIntervalZ( interval<number> Interval );
        void setForcedIntervalZ( number MinZ, number MaxZ );
        
        wcoord legendExampleWidth()  const;
        wcoord legendExampleHeight() const;
    };
    
    // ------------------------------------------------------------

    template < class D, class V > class graphSpecified : public graph
    {
      protected:
        data* createData();
        graphView* createView();
        
        D& getCastedData();
        const D& getCastedData() const;
        
        V& getCastedView();
        const V& getCastedView() const;

      public:
        explicit graphSpecified( const std::string &Legend = std::string() );

        data::int_t size() const;
        data::int_t sizeX() const;
        data::int_t sizeY() const;
        void resize( data::int_t SX, data::int_t SY );
        bool empty() const;

        void setIntervalX( number Min, number Max );
        void setIntervalY( number Min, number Max );
        void setIntervalX( interval<number> Interval );
        void setIntervalY( interval<number> Interval );

        number coordinateX( data::int_t IndexX ) const;
        number coordinateY( data::int_t IndexY ) const;
        data::int_t nearestIndexX( number X ) const;
        data::int_t nearestIndexY( number Y ) const;

        data::point_t operator[]( data::int_t Index ) const;
        data::point_t at( data::int_t Index ) const;
        data::int_t index( data::int_t IndexX, data::int_t IndexY ) const;
        void set( data::int_t ix, data::int_t iy, number Val );
    };
    
    // ------------------------------------------------------------

    class graphVector : public graphSpecified< dataVector, graphViewRectangle >
    {
      public:
        explicit graphVector( const std::string &Legend = std::string() );
        graphViewRectangle& getView() { return getCastedView(); }
    };

    // ============================================================
    
    template < class D, class V > graphSpecified<D,V>::graphSpecified( const std::string &Legend ) : 
      graph(Legend) 
    {
      init();
    }
    
    // ------------------------------------------------------------
        
    template < class D, class V > data* graphSpecified<D,V>::createData() 
    { 
      return new D(); 
    }
    
    // ------------------------------------------------------------
    
    template < class D, class V > graphView* graphSpecified<D,V>::createView() 
    { 
      return new V(); 
    }
    
    // ------------------------------------------------------------
        
    template < class D, class V > D& graphSpecified<D,V>::getCastedData()  
    { 
      return dynamic_cast<D&>( getData() ); 
    }
    
    // ------------------------------------------------------------
    
    template < class D, class V > const D& graphSpecified<D,V>::getCastedData() const 
    { 
      return dynamic_cast<const D&>( getData() ); 
    }
    
    // ------------------------------------------------------------
    
    template < class D, class V > V& graphSpecified<D,V>::getCastedView()  
    { 
      return dynamic_cast<V&>( getView() ); 
    }
    
    // ------------------------------------------------------------
    
    template < class D, class V > const V& graphSpecified<D,V>::getCastedView() const 
    { 
      return dynamic_cast<const V&>( getView() ); 
    }
    
    // ------------------------------------------------------------
        
    template < class D, class V > data::int_t graphSpecified<D,V>::size() const 
    { 
      return getData().size(); 
    }
    
    // ------------------------------------------------------------
    
    template < class D, class V > data::int_t graphSpecified<D,V>::sizeX() const 
    { 
      return getCastedData().sizeX(); 
    }
    
    // ------------------------------------------------------------
    
    template < class D, class V > data::int_t graphSpecified<D,V>::sizeY() const 
    { 
      return getCastedData().sizeY(); 
    }
    
    // ------------------------------------------------------------
    
    template < class D, class V > void graphSpecified<D,V>::resize( data::int_t SX, data::int_t SY ) 
    { 
      return getCastedData().resize(SX,SY); 
    }
    
    // ------------------------------------------------------------
     
    template < class D, class V > bool graphSpecified<D,V>::empty() const  
    { 
      return getData().empty(); 
    }
    
    // ------------------------------------------------------------
        
    template < class D, class V > void graphSpecified<D,V>::setIntervalX( number Min, number Max ) 
    { 
      setIntervalX( interval<number>( Min, Max ) ); 
    }
    
    // ------------------------------------------------------------
    
    template < class D, class V > void graphSpecified<D,V>::setIntervalY( number Min, number Max ) 
    { 
      setIntervalY( interval<number>( Min, Max ) ); 
    }
    
    // ------------------------------------------------------------
    
    template < class D, class V > void graphSpecified<D,V>::setIntervalX( interval<number> Interval ) 
    { 
      getCastedData().setIntervalX( Interval ); 
    }
    
    // ------------------------------------------------------------
    
    template < class D, class V > void graphSpecified<D,V>::setIntervalY( interval<number> Interval ) 
    { 
      getCastedData().setIntervalY( Interval ); 
    }
    
    // ------------------------------------------------------------
        
    template < class D, class V > number graphSpecified<D,V>::coordinateX( data::int_t IndexX ) const 
    { 
      return getCastedData().coordinateX(IndexX); 
    }
    
    // ------------------------------------------------------------
    
    template < class D, class V > number graphSpecified<D,V>::coordinateY( data::int_t IndexY ) const 
    { 
      return getCastedData().coordinateY(IndexY); 
    }
    
    // ------------------------------------------------------------
    
    template < class D, class V > data::int_t graphSpecified<D,V>::nearestIndexX( number X ) const 
    { 
      return getCastedData().nearestIndexX(X); 
    }
    
    // ------------------------------------------------------------
    
    template < class D, class V > data::int_t graphSpecified<D,V>::nearestIndexY( number Y ) const 
    { 
      return getCastedData().nearestIndexY(Y); 
    }
    
    // ------------------------------------------------------------

    template < class D, class V > data::point_t graphSpecified<D,V>::at( data::int_t Index ) const 
    { 
      return getData().at(Index); 
    }
    
    // ------------------------------------------------------------

    template < class D, class V > data::point_t graphSpecified<D,V>::operator[]( data::int_t Index ) const 
    { 
      return at(Index); 
    }
    
    // ------------------------------------------------------------

    template < class D, class V > data::int_t graphSpecified<D,V>::index( data::int_t IndexX, data::int_t IndexY ) const 
    { 
      return getCastedData().at(IndexX,IndexY); 
    }

    // ------------------------------------------------------------

    template < class D, class V > void graphSpecified<D,V>::set( data::int_t ix, data::int_t iy, number Val ) 
    { 
      getCastedData().set( ix, iy, Val ); 
    }
    
    // ============================================================

  }
}

