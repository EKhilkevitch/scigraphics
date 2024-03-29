
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

      public:
        explicit graph( const std::string &Legend );
        ~graph();
        
        void draw( painter &Painter, const pairScales& Scales ) const;
        void drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const;
        
        data& getData() { return *Data; }
        const data& getData() const { return *Data; }

        graphView& getView() { return *View; }
        const graphView& getView() const { return *View; }
   
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
        data* createData() { return new D(); }
        graphView* createView() { return new V(); }
        
      protected:
        D& getCastedData()  { return dynamic_cast<D&>( getData() ); }
        const D& getCastedData() const { return dynamic_cast<const D&>( getData() ); }
        
        V& getCastedView()  { return dynamic_cast<V&>( getView() ); }
        const V& getCastedView() const { return dynamic_cast<const V&>( getView() ); }

      public:
        explicit graphSpecified( const std::string &Legend = std::string() );

        size_t size() const { return getData().size(); }
        size_t sizeX() const { return getCastedData().sizeX(); }
        size_t sizeY() const { return getCastedData().sizeY(); }
        void resize( size_t SX, size_t SY ) { return getCastedData().resize(SX,SY); }
        bool empty() const  { return getData().empty(); }

        void setIntervalX( number Min, number Max ) { setIntervalX( interval<number>( Min, Max ) ); }
        void setIntervalY( number Min, number Max ) { setIntervalY( interval<number>( Min, Max ) ); }
        void setIntervalX( interval<number> Interval ) { getCastedData().setIntervalX( Interval ); }
        void setIntervalY( interval<number> Interval ) { getCastedData().setIntervalY( Interval ); }

        number coordinateX( size_t IndexX ) const { return getCastedData().coordinateX(IndexX); }
        number coordinateY( size_t IndexY ) const { return getCastedData().coordinateY(IndexY); }
        int nearestIndexX( number X ) const { return getCastedData().nearestIndexX(X); }
        int nearestIndexY( number Y ) const { return getCastedData().nearestIndexY(Y); }

        data::point_t at( data::int_t Index ) const { return getData().at(Index); }
        data::point_t at( data::int_t IndexX, data::int_t IndexY ) const { return getCastedData().at(IndexX,IndexY); }
        data::point_t operator[]( data::int_t Index ) const { return at(Index); }
        data::point_t firstPoint() const { return getData().firstPoint(); }
        data::point_t lastPoint() const  { return getData().lastPoint();  }
    
        void set( data::int_t ix, data::int_t iy, number Val ) { getCastedData().set( ix, iy, Val ); }
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
    
    // ============================================================

  }
}

