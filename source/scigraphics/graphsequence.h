
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

// ============================================================

  class graphSequence : public graph 
  {
    private:
      graphViewSequencesCollection *Views;
      dataSequence *Data;

    protected:
      void init( const color &Color );

      virtual dataSequence* createData() = 0;
      virtual graphViewSequencesCollection* createViewCollection() = 0;
      
    public:
      graphSequence( const std::string &Legend = std::string() );
      ~graphSequence();
      
      void draw( painter &Painter, const pairScales& Scales ) const;
      void drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const;

      graphViewSequencesCollection& getViews() { return *Views; }
      const graphViewSequencesCollection& getViews() const { return *Views; }

      dataSequence& getData() { return *Data; }
      const dataSequence& getData() const { return *Data; }
 
      numberLimits limitsX() const { return getData().limitsX(); }
      numberLimits limitsY( const interval<number> &LimitsX ) const { return getData().limitsY(LimitsX); }
      
      wcoord legendExampleWidth()  const; 
      wcoord legendExampleHeight() const;
  };

  template < class D, class V > class graphSequenceSpecified : public graphSequence
  {
    protected:
      dataSequence* createData() { return new D(); }
      graphViewSequencesCollection* createViewCollection() { return new V(); }
      
    protected:
      D& getCastedData()  { return dynamic_cast<D&>( getData() ); }
      V& getCastedViews() { return dynamic_cast<V&>( getViews() ); }

    public:
      graphSequenceSpecified( const color &Color ) { init(Color); }
      graphSequenceSpecified( const std::string &Legend, const color &Color ) : 
        graphSequence(Legend) 
        { init(Color); }

      size_t size() const { return getData().size(); }
      bool empty() const  { return getData().empty(); }

      dataSequence::point at( unsigned Index ) const { return getData().at(Index); }
      dataSequence::point operator[]( unsigned Index ) const { return at(Index); }
      dataSequence::point firstPoint() const { return getData().firstPoint(); }
      dataSequence::point lastPoint() const  { return getData().lastPoint();  }
      
      color getColor() const { return getViews().getColor(); }
      void setColor( const color &Color ) { getViews().setColor(Color); }

      void append( number X, number Y )                           { getCastedData().append(X,Y); }
      void append( number X, number Y, number ErrY )              { getCastedData().append(X,Y,ErrY); }
      void append( number X, number Y, number ErrX, number ErrY ) { getCastedData().append(X,Y,ErrX,ErrY); }
      void appendInvalid()                                        { append( invalidNumber(), invalidNumber() ); }
      void appendPolar( number Phi, number R )                    { append( R*std::cos(Phi), R*std::sin(Phi) ); } 
      void clear()                                                { getCastedData().clear(); }
  };

  class graphSequenceVector : public graphSequenceSpecified< dataSequenceVector, ordinarGraphSequenceViewCollection >
  {
    public:
      graphSequenceVector( const color &Color = color() ) : 
        graphSequenceSpecified< dataSequenceVector, ordinarGraphSequenceViewCollection >( Color ) {}
      graphSequenceVector( const std::string &Legend, const color &Color ) : 
        graphSequenceSpecified< dataSequenceVector, ordinarGraphSequenceViewCollection >( Legend, Color ) {}

      dataSequenceVector& getDataVector() { return getCastedData(); }
      ordinarGraphSequenceViewCollection& getViewsSequence() { return getCastedViews(); }
      
      void setLineWidth( unsigned Width ) { getViewsSequence().setLineWidth(Width); }
      void setPointSize( unsigned Size )  { getViewsSequence().setPointSize(Size);  }
      
      void setVisibleLines( bool Show )        { getViews().setViewVisible<graphViewLine>(Show); }
      void setVisiblePoints( bool Show )       { getViews().setViewVisible<graphViewPoints>(Show); }
      void setVisibleErrorBars( bool Show )    { getViews().setViewVisible<graphViewErrorBars>(Show); }
      void setVisibleLineHystogram( bool Show) { getViews().setViewVisible<graphViewLineHystogram>(Show); }
  };

  class graphAreaVector : public graphSequenceSpecified< dataSequenceVector, coveredAreaGraphSequenceViewCollection >
  {
    public:
      graphAreaVector( const color &Color = color() ) : 
        graphSequenceSpecified< dataSequenceVector, coveredAreaGraphSequenceViewCollection >(Color) {}
      graphAreaVector( const std::string &Legend, const color &Color ) : 
        graphSequenceSpecified< dataSequenceVector, coveredAreaGraphSequenceViewCollection >( Legend, Color ) {}
      
      dataSequenceVector& getDataVector() { return getCastedData(); }
      coveredAreaGraphSequenceViewCollection& getViewsColveredArea() { return getCastedViews(); }
  };

  // ============================================================

}

