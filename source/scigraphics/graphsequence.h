
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
        void init( const color &Color );

        virtual data* createData() = 0;
        virtual graphViewCollection* createViewCollection() = 0;
        
      public:
        graph( const std::string &Legend = std::string() );
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
        data* createData() { return new D(); }
        graphViewCollection* createViewCollection() { return new V(); }
        
      protected:
        D& getCastedData()  { return dynamic_cast<D&>( getData() ); }
        V& getCastedViews() { return dynamic_cast<V&>( getViews() ); }

      public:
        graphSpecified( const color &Color ) { init(Color); }
        graphSpecified( const std::string &Legend, const color &Color ) : 
          graph(Legend) 
          { init(Color); }

        size_t size() const { return getData().size(); }
        bool empty() const  { return getData().empty(); }

        data::point_t at( data::int_t Index ) const { return getData().at(Index); }
        data::point_t operator[]( data::int_t Index ) const { return at(Index); }
        
        color getColor() const { return getViews().getColor(); }
        void setColor( const color &Color ) { getViews().setColor(Color); }

        void append( number X, number Y )                           { getCastedData().append(X,Y); }
        void append( number X, number Y, number ErrY )              { getCastedData().append(X,Y,ErrY); }
        void append( number X, number Y, number ErrX, number ErrY ) { getCastedData().append(X,Y,ErrX,ErrY); }
        void appendInvalid()                                        { append( invalidNumber(), invalidNumber() ); }
        void appendPolar( number Phi, number R )                    { append( R*std::cos(Phi), R*std::sin(Phi) ); } 
        void clear()                                                { getCastedData().clear(); }
    };
  
    // ------------------------------------------------------------

    class graphVector : public graphSpecified< dataVector, ordinarGraphViewCollection >
    {
      public:
        graphVector( const color &Color = color() ) : 
          graphSpecified< dataVector, ordinarGraphViewCollection >( Color ) {}
        graphVector( const std::string &Legend, const color &Color ) : 
          graphSpecified< dataVector, ordinarGraphViewCollection >( Legend, Color ) {}

        dataVector& getDataVector() { return getCastedData(); }
        ordinarGraphViewCollection& getViews() { return getCastedViews(); }
        
        void setLineWidth( unsigned Width ) { getViews().setLineWidth(Width); }
        void setPointSize( unsigned Size )  { getViews().setPointSize(Size);  }
        
        void setVisibleLines( bool Show )        { getViews().setViewVisible<graphViewLine>(Show); }
        void setVisiblePoints( bool Show )       { getViews().setViewVisible<graphViewPoints>(Show); }
        void setVisibleErrorBars( bool Show )    { getViews().setViewVisible<graphViewErrorBars>(Show); }
        void setVisibleLineHystogram( bool Show) { getViews().setViewVisible<graphViewLineHystogram>(Show); }
    };
  
    // ------------------------------------------------------------

    class graphAreaVector : public graphSpecified< dataVector, coveredAreaGraphViewCollection >
    {
      public:
        graphAreaVector( const color &Color = color() ) : 
          graphSpecified< dataVector, coveredAreaGraphViewCollection >(Color) {}
        graphAreaVector( const std::string &Legend, const color &Color ) : 
          graphSpecified< dataVector, coveredAreaGraphViewCollection >( Legend, Color ) {}
        
        dataVector& getDataVector() { return getCastedData(); }
        coveredAreaGraphViewCollection& getViewsColveredArea() { return getCastedViews(); }
    };

    // ============================================================

  }
}

