
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

#include "scigraphics/datasequence.h"
#include "scigraphics/numbers.h"
#include "scigraphics/geometry.h"

#include "scigraphics/linestyle.h"
#include "scigraphics/pointstyle.h"
#include "scigraphics/errorbarstyle.h"
#include "scigraphics/brushstyle.h"
    
// ============================================================

namespace scigraphics 
{
    
  // ============================================================

  class color;
  class painter;
  class pairScales;
    
  // ============================================================

  namespace sequence
  {
    
    // ============================================================
    
    class graphView
    {
      private:
        bool Visible;
      
      public:
        graphView() : Visible(true) {}
        virtual ~graphView() {}

        void setVisible( bool V ) { Visible = V; }
        bool isVisible() const { return Visible; }

        virtual void draw( painter &Painter, const pairScales& Scales, const sequence::data &Data ) const = 0;
        virtual void drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const = 0;

        virtual color getColor() const = 0;
        virtual void setColor( const color &C ) = 0;
        
        virtual wcoord legendExampleWidth() const  { return 35; }
        virtual wcoord legendExampleHeight() const { return 5;  }
    };

    class graphViewOrdered : public graphView
    {
      protected:
        virtual void drawUnorderedByX( painter &Painter, const pairScales& Scales, sequence::data::iterator Begin, sequence::data::iterator End ) const = 0;
        virtual void drawOrderedByX( painter &Painter, const pairScales& Scales, sequence::data::iterator Begin, sequence::data::iterator End ) const;

      public:
        void draw( painter &Painter, const pairScales& Scales, const sequence::data &Data ) const;
    };

    // ============================================================
    
    template < class styleClass, class parentClass > class graphViewStyle : public parentClass
    {
      public:
        typedef styleClass style;
      
      private:
        style Style;
      
      public:
        graphViewStyle( style S = style() ) : Style(S) {}

        void setStyle( style S ) { Style = S; }
        const style& getStyle() const { return Style; }

        color getColor() const { return Style.getColor(); }
        void setColor( const color &C ) { Style.setColor(C); }
    };
    
    // ============================================================

    class graphViewGeneralLine : public graphViewStyle<lineStyle,graphViewOrdered>
    {
      protected:
        virtual void drawLineBetweenPoints( painter &Painter, std::vector<wpoint> *Points ) const = 0;
        void drawUnorderedByX( painter &Painter, const pairScales& Scales, sequence::data::iterator Begin, sequence::data::iterator End ) const;
        
      public:
        graphViewGeneralLine( style Style );
        void drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const;
    };
    
    // ------------------------------------------------------------

    class graphViewLine : public graphViewGeneralLine
    {
      protected:
        void drawLineBetweenPoints( painter &Painter, std::vector<wpoint> *Points ) const;
      public:
        graphViewLine( style S ) : graphViewGeneralLine(S) {}
    };

    // ------------------------------------------------------------
    
    class graphViewPoints : public graphViewStyle<pointStyle,graphViewOrdered>
    {
      protected:
        void drawUnorderedByX( painter &Painter, const pairScales& Scales, sequence::data::iterator Begin, sequence::data::iterator End ) const;
      public:
        graphViewPoints( style S ) : graphViewStyle<pointStyle,graphViewOrdered>(S) {}

        void drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const;
    };

    // ------------------------------------------------------------
    
    class graphViewErrorBars : public graphViewStyle<errorBarStyle,graphViewOrdered>
    {
      protected:
        void drawUnorderedByX( painter &Painter, const pairScales& Scales, sequence::data::iterator Begin, sequence::data::iterator End ) const;
        void drawHorizontalErrorBar( painter &Painter, const pairScales& Scales, const npoint &Point, number ErrX ) const;
        void drawVerticalErrorBar( painter &Painter, const pairScales& Scales, const npoint &Point, number ErrY ) const;
      public:
        graphViewErrorBars( style S ) : graphViewStyle<errorBarStyle,graphViewOrdered>(S) {}

        void drawLegendExample( painter &, const wrectangle & ) const {}
    };

    // ------------------------------------------------------------
    
    class graphViewLineHystogram : public graphViewGeneralLine
    {
      protected:
        void drawLineBetweenPoints( painter &Painter, std::vector<wpoint> *Points ) const;
      public:
        graphViewLineHystogram( style S ) : graphViewGeneralLine(S) {}
    };

    // ------------------------------------------------------------
    
    class graphViewCoveredArea : public graphViewStyle<brushStyle,graphView>
    {
      private:
        static sequence::data::iterator fillPolygonVector( sequence::data::iterator Begin, sequence::data::iterator End, const pairScales& Scales, std::vector<fpoint> *Polygon );
        
      protected:
        void drawUnorderedByX( painter &Painter, const pairScales& Scales, sequence::data::iterator Begin, sequence::data::iterator End ) const;
        void drawOrderedByX( painter &Painter, const pairScales& Scales, sequence::data::iterator Begin, sequence::data::iterator End ) const;

      public:
        graphViewCoveredArea( style S ) : graphViewStyle<brushStyle,graphView>(S) {}
       
        void draw( painter &Painter, const pairScales& Scales, const sequence::data &Data ) const;
        void drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const;
    };
    
    // ============================================================

  }
}

