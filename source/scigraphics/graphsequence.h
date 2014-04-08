
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
#include "scigraphics/graphviewcollection.h"
#include "scigraphics/datasequence.h"

#include <cmath>

// ============================================================

namespace scigraphics 
{
  namespace sequence
  {

    // ============================================================
    
    class graph : public ::scigraphics::graph 
    {
      private:
        graphViewCollection *Views;
        data *Data;

      private:
        graph( graph& );
        graph& operator=( const graph& );

      protected:
        void setData( data *D );
        void setViews( graphViewCollection *V );
 
      public:
        explicit graph( const std::string &Legend = std::string() );
        ~graph();
        
        void draw( painter &Painter, const pairScales& Scales ) const;
        void drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const;

        graphViewCollection& getViews() { return *Views; }
        const graphViewCollection& getViews() const { return *Views; }

        data& getData() { return *Data; }
        const data& getData() const { return *Data; }
   
        numberLimits limitsX() const { return getData().limitsX(); }
        numberLimits limitsY( const interval<number> &LimitsX ) const { return getData().limitsY(LimitsX); }
        
        wcoord legendExampleWidth()  const; 
        wcoord legendExampleHeight() const;
    };
  
    // ------------------------------------------------------------

    template < class D, class V > class graphSpecified : public graph
    {
      protected:
        virtual data* createData() { return new D(); }
        virtual graphViewCollection* createViewCollection() { return new V(); }

        void init( const color &Color );
        
      protected:
        D& getCastedData()  { return dynamic_cast<D&>( getData() ); }
        const D& getCastedData() const { return dynamic_cast<const D&>( getData() ); }

        V& getCastedViews() { return dynamic_cast<V&>( getViews() ); }
        const V& getCastedViews() const { return dynamic_cast<const V&>( getViews() ); }

      public:
        explicit graphSpecified( const color &Color ) 
          { init(Color); }
        explicit graphSpecified( const std::string &Legend, const color &Color ) : 
          graph(Legend) 
          { init(Color); }

        size_t size() const { return getData().size(); }
        bool empty() const  { return getData().empty(); }

        data::point_t at( data::int_t Index ) const { return getData().at(Index); }
        data::point_t operator[]( data::int_t Index ) const { return at(Index); }
        void clear()   { getCastedData().clear(); }
        
        color getColor() const { return getViews().getColor(); }
        void setColor( const color &Color ) { getViews().setColor(Color); }
        
        void setLineWidth( unsigned Width ) { getCastedViews().setLineWidth(Width); }
        void setPointSize( unsigned Size )  { getCastedViews().setPointSize(Size);  }
        
        void setVisibleLines( bool Show )        { getCastedViews().template setViewVisible<graphViewLine>(Show); }
        void setVisiblePoints( bool Show )       { getCastedViews().template setViewVisible<graphViewPoints>(Show); }
        void setVisibleErrorBars( bool Show )    { getCastedViews().template setViewVisible<graphViewErrorBars>(Show); }
        void setVisibleLineHystogram( bool Show) { getCastedViews().template setViewVisible<graphViewLineHystogram>(Show); }
    };
  
    // ------------------------------------------------------------

    class graphVector : public graphSpecified< dataVector, ordinarGraphViewCollection >
    {
      public:
        explicit graphVector( const color &Color = color() ) : 
          graphSpecified< dataVector, ordinarGraphViewCollection >( Color ) {}
        explicit graphVector( const std::string &Legend, const color &Color ) : 
          graphSpecified< dataVector, ordinarGraphViewCollection >( Legend, Color ) {}

        dataVector& getDataVector() { return getCastedData(); }
        ordinarGraphViewCollection& getViews() { return getCastedViews(); }

        void append( number X, number Y )                           { getCastedData().append(X,Y); }
        void append( number X, number Y, number ErrY )              { getCastedData().append(X,Y,ErrY); }
        void append( number X, number Y, number ErrX, number ErrY ) { getCastedData().append(X,Y,ErrX,ErrY); }
        void appendInvalid()                                        { append( invalidNumber(), invalidNumber() ); }
        void appendPolar( number Phi, number R )                    { append( R*std::cos(Phi), R*std::sin(Phi) ); } 
    };
  
    // ------------------------------------------------------------
    
    class graphUniformVector : public graphSpecified< dataUniformVector, ordinarGraphViewCollection >
    {
      public:
        explicit graphUniformVector( const color &Color = color() ) : 
          graphSpecified< dataUniformVector, ordinarGraphViewCollection >( Color ) {}
        explicit graphUniformVector( const std::string &Legend, const color &Color ) : 
          graphSpecified< dataUniformVector, ordinarGraphViewCollection >( Legend, Color ) {}

        dataUniformVector& getDataVector() { return getCastedData(); }
        const dataUniformVector& getDataVector() const { return getCastedData(); }

        ordinarGraphViewCollection& getViews() { return getCastedViews(); }

        void setStepX( number Step ) { getDataVector().setStepX( Step ); } 
        number stepX() const { return getDataVector().stepX(); }
        
        void append( number Y )                           { getCastedData().append(Y); }
        void append( number Y, number ErrY )              { getCastedData().append(Y,ErrY); }
    };

    // ------------------------------------------------------------

    class graphAreaVector : public graphSpecified< dataVector, coveredAreaGraphViewCollection >
    {
      public:
        explicit graphAreaVector( const color &Color = color() ) : 
          graphSpecified< dataVector, coveredAreaGraphViewCollection >(Color) {}
        explicit graphAreaVector( const std::string &Legend, const color &Color ) : 
          graphSpecified< dataVector, coveredAreaGraphViewCollection >( Legend, Color ) {}
        
        dataVector& getDataVector() { return getCastedData(); }
        coveredAreaGraphViewCollection& getViewsColveredArea() { return getCastedViews(); }
        
        void append( number X, number Y )                           { getCastedData().append(X,Y); }
        void append( number X, number Y, number ErrY )              { getCastedData().append(X,Y,ErrY); }
        void append( number X, number Y, number ErrX, number ErrY ) { getCastedData().append(X,Y,ErrX,ErrY); }
        void appendInvalid()                                        { append( invalidNumber(), invalidNumber() ); }
    };

    // ============================================================
    
    template < class D, class V > void graphSpecified<D,V>::init( const color &Color )
    {
      data *Data = createData();
      setData( Data );

      graphViewCollection *Views = createViewCollection();
      Views->setColor( Color );
      setViews( Views );
    }
    
    // ============================================================

  }
}

