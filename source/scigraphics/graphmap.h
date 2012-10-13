
#pragma once

// ============================================================

#include "graphics/graph.h"
#include "graphics/graphviewmap.h"
#include "graphics/datamap.h"

#include <string>

// ============================================================

namespace scigraphics 
{

// ============================================================
 
  class graphMap : public graph
  {
    private:
      graphViewMap *View;
      dataMap *Data;

    protected:
      virtual dataMap* createData() = 0;
      virtual graphViewMap* createView() = 0;

      void init();

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

      dataMap::point at( unsigned Index ) const { return getData().at(Index); }
      dataMap::point at( unsigned IndexX, unsigned IndexY ) const { return getCastedData().at(IndexX,IndexY); }
      dataMap::point operator[]( unsigned Index ) const { return at(Index); }
      dataMap::point firstPoint() const { return getData().firstPoint(); }
      dataMap::point lastPoint() const  { return getData().lastPoint();  }
  
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

