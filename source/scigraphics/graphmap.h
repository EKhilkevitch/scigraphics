
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

  class scale;

// ============================================================
 
  class graphMap : public graph
  {
    private:
      graphViewMap *View;
      dataMap *Data;
      scale *ScaleZ;
      interval<number> ForcedIntervalZ;

    protected:
      virtual dataMap* createData() = 0;
      virtual graphViewMap* createView() = 0;

      void init();
      void updateScaleZInterval() const;

    public:
      graphMap( const std::string &Legend );
      ~graphMap();
      
      void draw( painter &Painter, const pairScales& Scales ) const;
      void drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const;
      
      dataMap& getData() { return *Data; }
      const dataMap& getData() const { return *Data; }

      graphViewMap& getView() { return *View; }
      const graphViewMap& getView() const { return *View; }
 
      numberLimits limitsX() const { return getData().limitsX(); }
      numberLimits limitsY( const interval<number> &LimitsX ) const { return getData().limitsY(LimitsX); }
      numberLimits limitsZ() const { return getData().limitsZ(); }

      void setForcedIntervalZ( interval<number> I ) { ForcedIntervalZ = I; }
      void setForcedIntervalZ( number Min, number Max ) { setForcedIntervalZ( interval<number>(Min,Max) ); }
      
      wcoord legendExampleWidth()  const;
      wcoord legendExampleHeight() const;
  };

  template < class D, class V > class graphMapSpecified : public graphMap
  {
    protected:
      dataMap* createData() { return new D(); }
      graphViewMap* createView() { return new V(); }
      
    protected:
      D& getCastedData()  { return dynamic_cast<D&>( getData() ); }
      const D& getCastedData() const { return dynamic_cast<const D&>( getData() ); }
      
      V& getCastedView()  { return dynamic_cast<V&>( getView() ); }
      const V& getCastedView() const { return dynamic_cast<const V&>( getView() ); }

    public:
      graphMapSpecified( const std::string &Legend = std::string() ) : 
        graphMap(Legend) 
        { init(); }

      size_t size() const { return getData().size(); }
      size_t sizeX() const { return getCastedData().sizeX(); }
      size_t sizeY() const { return getCastedData().sizeY(); }
      void resize( size_t SX, size_t SY ) { return getCastedData().resize(SX,SY); }
      bool empty() const  { return getData().empty(); }

      void setIntervalX( number Min, number Max ) { getCastedData().setIntervalX( Min, Max ); }
      void setIntervalY( number Min, number Max ) { getCastedData().setIntervalY( Min, Max ); }

      number coordinateX( unsigned IndexX ) const { return getCastedData().coordinateX(IndexX); }
      number coordinateY( unsigned IndexY ) const { return getCastedData().coordinateY(IndexY); }

      dataMap::point_t at( dataMap::int_t Index ) const { return getData().at(Index); }
      dataMap::point_t at( dataMap::int_t IndexX, unsigned IndexY ) const { return getCastedData().at(IndexX,IndexY); }
      dataMap::point_t operator[]( dataMap::int_t Index ) const { return at(Index); }
      dataMap::point_t firstPoint() const { return getData().firstPoint(); }
      dataMap::point_t lastPoint() const  { return getData().lastPoint();  }
  
      void set( unsigned ix, unsigned iy, number Val ) { getCastedData().set( ix, iy, Val ); }
  };

  class graphMapVector : public graphMapSpecified< dataMapVector, graphViewRectangleMap >
  {
    public:
      graphMapVector( const std::string &Legend = std::string() ) : 
        graphMapSpecified< dataMapVector, graphViewRectangleMap >(Legend) {}

      graphViewRectangleMap& getViewMap() { return getCastedView(); }
  };

// ============================================================

}

