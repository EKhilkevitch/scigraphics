
#pragma once

// ============================================================

#include "graphics/graph.h"
#include "graphics/graphviewcollection.h"
#include "graphics/datasequence.h"

// ============================================================

namespace graphics 
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
      graphSequenceSpecified( const std::string &Legend = std::string(), const color &Color = color::White ) : 
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
      graphSequenceVector( const std::string &Legend = std::string(), const color &Color = color::White ) : 
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
      graphAreaVector( const std::string &Legend = std::string(), const color &Color = color::White ) : 
        graphSequenceSpecified< dataSequenceVector, coveredAreaGraphSequenceViewCollection >( Legend, Color ) {}
      
      dataSequenceVector& getDataVector() { return getCastedData(); }
      coveredAreaGraphSequenceViewCollection& getViewsColveredArea() { return getCastedViews(); }
  };

  // ============================================================

}

