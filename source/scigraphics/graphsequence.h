
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

#include <string>

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
        void setData( data *Data );
        void setViews( graphViewCollection *Views );
 
      public:
        explicit graph( const std::string &Legend = std::string() );
        ~graph();
        
        void draw( painter &Painter, const pairScales& Scales ) const;
        void drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const;

        graphViewCollection& getViews();
        const graphViewCollection& getViews() const;

        data& getData();
        const data& getData() const;
   
        numberLimits limitsX() const;
        numberLimits limitsY( const interval<number> &LimitsX ) const;
        
        wcoord legendExampleWidth()  const; 
        wcoord legendExampleHeight() const;
    };
  
    // ------------------------------------------------------------

    template < class D, class V > class graphSpecified : public graph
    {
      protected:
        virtual data* createData();
        virtual graphViewCollection* createViewCollection();

        void init( const color &Color );
        
      protected:
        D& getCastedData();
        const D& getCastedData() const;

        V& getCastedViews();
        const V& getCastedViews() const;

      public:
        explicit graphSpecified( const color &Color );
        graphSpecified( const std::string &Legend, const color &Color );

        size_t size() const;
        bool empty() const;

        data::point_t at( data::int_t Index ) const;
        data::point_t operator[]( data::int_t Index ) const;
        void clear();
        
        color getColor() const;
        void setColor( const color &Color );
        
        void setLineWidth( unsigned Width );
        void setPointSize( unsigned Size );
        void setLineStyle( lineStyle::style LineStyle );
        void setPointShape( pointStyle::shape PointShape );
        
        void setVisibleLines( bool Show );
        void setVisiblePoints( bool Show );
        void setVisibleErrorBars( bool Show );
        void setVisibleLineHystogram( bool Show);
    };
  
    // ------------------------------------------------------------

    class graphVector : public graphSpecified< dataVector, ordinarGraphViewCollection >
    {
      public:
        explicit graphVector( const color &Color = color() );
        graphVector( const std::string &Legend, const color &Color );

        dataVector& getDataVector();
        ordinarGraphViewCollection& getViews();

        void append( number X, number Y );
        void append( number X, number Y, number ErrY );
        void append( number X, number Y, number ErrX, number ErrY );
        void appendInvalid();
        void appendPolar( number Phi, number R );
    };
  
    // ------------------------------------------------------------
    
    class graphCreatedByMouseVector : public graphSpecified< dataVector, ordinarGraphViewCollection >
    {
      private:
        static number distance( const npoint &Pt1, const npoint &Pt2 ); 

      public:
        static const color DefaultColor;

      public:
        graphCreatedByMouseVector( const color &Color = DefaultColor );

        dataVector& getDataVector();
        ordinarGraphViewCollection& getViews();

        virtual void append( number X, number Y );
        virtual void set( size_t Index, number X, number Y );
        virtual void erase( size_t Index );

        size_t indexOfPoint( number X, number Y ) const;
    };

    // ------------------------------------------------------------
    
    class graphUniformVector : public graphSpecified< dataUniformVector, ordinarGraphViewCollection >
    {
      public:
        explicit graphUniformVector( const color &Color = color() );
        graphUniformVector( const std::string &Legend, const color &Color );

        dataUniformVector& getDataVector();
        const dataUniformVector& getDataVector() const;

        ordinarGraphViewCollection& getViews();

        void setStepX( number Step );
        number stepX() const;

        void setShiftX( number Shift );
        number shiftX() const;
        
        void append( number Y );
        void append( number Y, number ErrY );
    };

    // ------------------------------------------------------------

    class graphAreaVector : public graphSpecified< dataVector, coveredAreaGraphViewCollection >
    {
      public:
        explicit graphAreaVector( const color &Color = color() );
        graphAreaVector( const std::string &Legend, const color &Color );
        
        dataVector& getDataVector();
        coveredAreaGraphViewCollection& getViewsColveredArea();
        
        void append( number X, number Y );
        void append( number X, number Y, number ErrY );
        void append( number X, number Y, number ErrX, number ErrY );
        void appendInvalid();
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
        
    // ------------------------------------------------------------
    
    template < class D, class V > data* graphSpecified<D,V>::createData() 
    { 
      return new D(); 
    }
    
    // ------------------------------------------------------------
    
    template < class D, class V > graphViewCollection* graphSpecified<D,V>::createViewCollection() 
    { 
      return new V(); 
    }
    
    // ------------------------------------------------------------
        
    template < class D, class V > graphSpecified<D,V>::graphSpecified( const color &Color )
    { 
      init(Color); 
    }
    
    // ------------------------------------------------------------
    
    template < class D, class V > graphSpecified<D,V>::graphSpecified( const std::string &Legend, const color &Color ) : 
      graph(Legend) 
    { 
      init(Color); 
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
    
    template < class D, class V > V& graphSpecified<D,V>::getCastedViews() 
    { 
      return dynamic_cast<V&>( getViews() ); 
    }
    
    // ------------------------------------------------------------
    
    template < class D, class V > const V& graphSpecified<D,V>::getCastedViews() const 
    { 
      return dynamic_cast<const V&>( getViews() ); 
    }
    
    // ------------------------------------------------------------

    template < class D, class V > size_t graphSpecified<D,V>::size() const 
    { 
      return getData().size(); 
    }
    
    // ------------------------------------------------------------

    template < class D, class V > bool graphSpecified<D,V>::empty() const  
    { 
      return getData().empty(); 
    }
    
    // ------------------------------------------------------------

    template < class D, class V > data::point_t graphSpecified<D,V>::at( data::int_t Index ) const 
    { 
      return getData().at(Index); 
    }
    
    // ------------------------------------------------------------

    template < class D, class V > data::point_t graphSpecified<D,V>::operator[]( data::int_t Index ) const 
    { 
      return getData()[Index]; 
    }
    
    // ------------------------------------------------------------

    template < class D, class V > void graphSpecified<D,V>::clear()   
    { 
      getCastedData().clear(); 
    }
    
    // ------------------------------------------------------------
    
    template < class D, class V > color graphSpecified<D,V>::getColor() const 
    { 
      return getViews().getColor(); 
    }
    
    // ------------------------------------------------------------

    template < class D, class V > void graphSpecified<D,V>::setColor( const color &Color ) 
    { 
      getViews().setColor(Color); 
    }
    
    // ------------------------------------------------------------
    
    template < class D, class V > void graphSpecified<D,V>::setLineWidth( unsigned Width ) 
    { 
      getCastedViews().setLineWidth(Width); 
    }
    
    // ------------------------------------------------------------

    template < class D, class V > void graphSpecified<D,V>::setPointSize( unsigned Size )  
    { 
      getCastedViews().setPointSize(Size);  
    }
    
    // ------------------------------------------------------------

    template < class D, class V > void graphSpecified<D,V>::setLineStyle( lineStyle::style LineStyle ) 
    { 
      getCastedViews().setLineStyle(LineStyle); 
    }
    
    // ------------------------------------------------------------

    template < class D, class V > void graphSpecified<D,V>::setPointShape( pointStyle::shape PointShape ) 
    { 
      getCastedViews().setPointShape(PointShape); 
    }
        
    // ------------------------------------------------------------
    
    template < class D, class V > void graphSpecified<D,V>::setVisibleLines( bool Show )        
    { 
      getCastedViews().template setViewVisible<graphViewLine>(Show); 
    }
    
    // ------------------------------------------------------------
    
    template < class D, class V > void graphSpecified<D,V>::setVisiblePoints( bool Show )       
    { 
      getCastedViews().template setViewVisible<graphViewPoints>(Show); 
    }
    
    // ------------------------------------------------------------
    
    template < class D, class V > void graphSpecified<D,V>::setVisibleErrorBars( bool Show )    
    { 
      getCastedViews().template setViewVisible<graphViewErrorBars>(Show); 
    }
    
    // ------------------------------------------------------------
    
    template < class D, class V > void graphSpecified<D,V>::setVisibleLineHystogram( bool Show) 
    { 
      getCastedViews().template setViewVisible<graphViewLineHystogram>(Show); 
    }
    
    // ============================================================

  }
}

