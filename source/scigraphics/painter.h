
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

#include "scigraphics/drawer.h"
#include "scigraphics/geometry.h"
#include "scigraphics/scale.h"
#include "scigraphics/linestyle.h"
#include "scigraphics/brushstyle.h"
#include "scigraphics/textstyle.h"
#include "scigraphics/pointstyle.h"
#include "scigraphics/errorbarstyle.h"

#include <string>
#include <vector>
#include <cstdio>

namespace scigraphics
{

// ============================================================

  typedef point<fcoord> fpoint;
  typedef rectangle<fcoord> frectangle;
  
  typedef point<number> npoint;
  typedef rectangle<number> nrectangle;

  class painter
  {
    private:
      drawer *Drawer;
      
      indents<wcoord> Indents;
      wrectangle PlotRectangle;

      painter( const painter& );
      painter& operator=( const painter& );
      
    protected:
      bool ableToDraw() const { return ( Drawer != NULL ); }
      void updatePlotRectangle();

    public:
      painter( drawer *D = NULL ) : Drawer(D) {}
      virtual ~painter() { delete Drawer; }
      
      void setDrawer( drawer *D ) { delete Drawer; Drawer = D; } 
      drawer* getDrawer() { return Drawer; }
      
      wcoord plotWidth()  const { return ableToDraw() ? Drawer->width()  : 100; }
      wcoord plotHeight() const { return ableToDraw() ? Drawer->height() : 100; }

      void setIndents( const indents<wcoord>& Ids ) { Indents = Ids; update(); }
      const indents<wcoord> getIndents() const { return Indents; }
      
      const wrectangle& plotRectangle() const { return PlotRectangle; }
      wcoord width() const  { return PlotRectangle.width();  }
      wcoord height() const { return PlotRectangle.height(); }

      fcoord wcoord2fcoordX( wcoord X ) const;
      fcoord wcoord2fcoordY( wcoord Y ) const;
      
      wcoord fcoord2wcoordX( fcoord X ) const;
      wcoord fcoord2wcoordY( fcoord Y ) const;
      
      wpoint fpoint2wpoint( fpoint Pt ) const;
      fpoint wpoint2fpoint( wpoint Pt ) const;

      bool canSeparatePointsF( fpoint Pt1, fpoint Pt2 ) const;
      bool canSepareteXcoordsF( fcoord X1, fcoord X2 ) const;
     
      void update() { updatePlotRectangle(); }
      void flush();
      void clearPlotArea();
      void clearBordersArea();

      void drawLineW( wpoint A, wpoint B, const lineStyle &Style );
      void drawLineF( fpoint A, fpoint B, const lineStyle &Style );

      void drawTextW( const std::string &String, wpoint Point, unsigned Position, 
        const textStyle &Style = textStyle(), int ShiftLeft = 0, int ShiftUp = 0, double Angle = 0 );
      void drawTextF( const std::string &String, fpoint Point, unsigned PositionFlags, 
        const textStyle &Style = textStyle(), int ShiftLeft = 0, int ShiftUp = 0, double Angle = 0 );

      void drawRectangleF( const frectangle &Rect, const brushStyle &BrushStyle = brushStyle() );
      void drawRectangleF( const frectangle &Rect, const brushStyle &BrushStyle, const lineStyle &LineStyle );
      void drawRectangleW( const wrectangle &Rect, const brushStyle &BrushStyle, const lineStyle &LineStyle );

      void drawPointF( const fpoint &Point, const pointStyle &Style );
      void drawPointW( const wpoint &Point, const pointStyle &Style );

      void drawVerticalErrorBarF( const fpoint &Min, const fpoint &Max, const errorBarStyle &Style );
      void drawHorizontalErrorBarF( const fpoint &Min, const fpoint &Max, const errorBarStyle &Style );
      void drawVecticalErrorBarW( const wpoint &Min, const wpoint &Max, const errorBarStyle &Style );
      void drawHorizontalErrorBarW( const wpoint &Min, const wpoint &Max, const errorBarStyle &Style );

      void drawPolygonF( const std::vector<fpoint> &Points, const brushStyle &Style );
      void drawPolygonW( const std::vector<wpoint> &Points, const brushStyle &Style );

      wcoord textWidth( const std::string &Text, const textStyle &Style = textStyle() );
      wcoord textHeight( const std::string &Text, const textStyle &Style = textStyle() );
  };

// ============================================================

}

