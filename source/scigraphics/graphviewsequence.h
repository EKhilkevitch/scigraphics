
#pragma once

#include "scigraphics/datasequence.h"
#include "scigraphics/numbers.h"
#include "scigraphics/painter.h"

#include "scigraphics/linestyle.h"
#include "scigraphics/pointstyle.h"
#include "scigraphics/errorbarstyle.h"
#include "scigraphics/brushstyle.h"

namespace scigraphics 
{
 
  // ============================================================
  
  class graphViewSequence
  {
    private:
      bool Visible;
    
    public:
      graphViewSequence() : Visible(true) {}
      virtual ~graphViewSequence() {}

      void setVisible( bool V ) { Visible = V; }
      bool isVisible() const { return Visible; }

      virtual void draw( painter &Painter, const pairScales& Scales, const dataSequence &Data ) const = 0;
      virtual void drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const = 0;

      virtual color getColor() const = 0;
      virtual void setColor( const color &C ) = 0;
      
      virtual wcoord legendExampleWidth() const  { return 35; }
      virtual wcoord legendExampleHeight() const { return 5;  }
  };

  class graphViewSequenceOrderOptimized : public graphViewSequence
  {
    protected:
      virtual void drawUnorderedByX( painter &Painter, const pairScales& Scales, dataSequence::iterator Begin, dataSequence::iterator End ) const = 0;
      virtual void drawOrderedByX( painter &Painter, const pairScales& Scales, dataSequence::iterator Begin, dataSequence::iterator End ) const;

    public:
      void draw( painter &Painter, const pairScales& Scales, const dataSequence &Data ) const;
  };

  // ============================================================
  
  template < class styleClass > class graphViewSequenceStyle : public graphViewSequenceOrderOptimized
  {
    public:
      typedef styleClass style;
    
    private:
      style Style;
    
    public:
      graphViewSequenceStyle( style S = style() ) : Style(S) {}

      void setStyle( style S ) { Style = S; }
      const style& getStyle() const { return Style; }

      color getColor() const { return Style.getColor(); }
      void setColor( const color &C ) { Style.setColor(C); }
  };

  class graphViewGeneralLine : public graphViewSequenceStyle<lineStyle>
  {
    protected:
      virtual void drawLineBetweenPoints( painter &Painter, const fpoint Pt1, const fpoint &Pt2 ) const = 0;
      void drawUnorderedByX( painter &Painter, const pairScales& Scales, dataSequence::iterator Begin, dataSequence::iterator End ) const;
      
    public:
      graphViewGeneralLine( style S ) : graphViewSequenceStyle<lineStyle>(S) {}

      void drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const;
  };

  class graphViewLine : public graphViewGeneralLine
  {
    protected:
      void drawLineBetweenPoints( painter &Painter, const fpoint Pt1, const fpoint &Pt2 ) const;
    public:
      graphViewLine( style S ) : graphViewGeneralLine(S) {}
  };

  class graphViewPoints : public graphViewSequenceStyle<pointStyle>
  {
    protected:
      void drawUnorderedByX( painter &Painter, const pairScales& Scales, dataSequence::iterator Begin, dataSequence::iterator End ) const;
    public:
      graphViewPoints( style S ) : graphViewSequenceStyle<pointStyle>(S) {}

      void drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const;
  };

  class graphViewErrorBars : public graphViewSequenceStyle<errorBarStyle>
  {
    protected:
      void drawUnorderedByX( painter &Painter, const pairScales& Scales, dataSequence::iterator Begin, dataSequence::iterator End ) const;
      void drawHorizontalErrorBar( painter &Painter, const pairScales& Scales, const npoint &Point, number ErrX ) const;
      void drawVerticalErrorBar( painter &Painter, const pairScales& Scales, const npoint &Point, number ErrY ) const;
    public:
      graphViewErrorBars( style S ) : graphViewSequenceStyle<errorBarStyle>(S) {}

      void drawLegendExample( painter &, const wrectangle & ) const {}
  };

  class graphViewLineHystogram : public graphViewGeneralLine
  {
    protected:
      void drawLineBetweenPoints( painter &Painter, const fpoint Pt1, const fpoint &Pt2 ) const;
    public:
      graphViewLineHystogram( style S ) : graphViewGeneralLine(S) {}
  };

  class graphViewCoveredArea : public graphViewSequenceStyle<brushStyle>
  {
    private:
      static dataSequence::iterator fillPolygonVector( dataSequence::iterator Begin, dataSequence::iterator End, const pairScales& Scales, std::vector<fpoint> *Polygon );
      
    protected:
      void drawUnorderedByX( painter &Painter, const pairScales& Scales, dataSequence::iterator Begin, dataSequence::iterator End ) const;
      void drawOrderedByX( painter &Painter, const pairScales& Scales, dataSequence::iterator Begin, dataSequence::iterator End ) const;

    public:
      graphViewCoveredArea( style S ) : graphViewSequenceStyle<brushStyle>(S) {}
     
      void draw( painter &Painter, const pairScales& Scales, const dataSequence &Data ) const;
      void drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const;
  };
  
  // ============================================================

}

