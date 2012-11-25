
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

#include "scigraphics/floatrect.h"
#include "scigraphics/textstyle.h"
#include "scigraphics/graphcollection.h"

#include <list>
#include <string>

// ============================================================

namespace scigraphics
{

// ============================================================

  class legend : public floatRectangle
  {
    private:
      static const wrectangle InitLegendRectangle;

      textStyle TextStyle;

    protected:
      class legendSize 
      { 
        private:
          wcoord Width, Height; 
        public:
          legendSize() : Width(0), Height(0) {}
          legendSize( wcoord W, wcoord H ) : Width(W), Height(H) {}

          void updateWidth( wcoord W )  { Width = std::max( W, Width ); }
          void updateHeight( wcoord H ) { Height += H; }
          void addToWidth( wcoord W )   { Width += W; }
          void addToHeight( wcoord H )  { Height += H; }

          wcoord width()  const { return Width;  }
          wcoord height() const { return Height; }
      };

      static bool shouldDrawGraphLegend( const graph &Graph );
      static std::list<std::string> legendsList( const graphCollection &Graphics );
      static bool shouldDrawLegend( const graphCollection &Graphics );

      textStyle updateLegendRectangle( painter &Painter, const graphCollection &Graphics );
      static wrectangle createInitialRectangle( painter &Painter );
      static legendSize sizesForLegendRectangle( painter &Painter, const textStyle &Style, const graphCollection &Graphics );
      void setRectangleFromLegendSize( painter &Painter, const legendSize &LegendSize );

      static wcoord interTextVerticalDistance( const textStyle &Style ) { return std::max<wcoord>( Style.getFontSize()/3, 2 ); }
      static wcoord textHorizontalIndent() { return 5; }
      static unsigned minFontSize() { return 5; }

      wcoord drawGraphLegend( painter &Painter, wcoord y, const graph &Graph, const textStyle &Style );
      void drawAllLegends( painter &Painter, const graphCollection &Graphics, const textStyle &Style );

    public:
      legend() : floatRectangle(InitLegendRectangle) {}

      const textStyle& getLegendTextStyle() const { return TextStyle; }
      void draw( painter &Painter, const graphCollection &Graphics );
  };

// ============================================================

}

