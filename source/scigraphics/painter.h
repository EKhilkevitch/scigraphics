
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

#include "scigraphics/numbers.h"
#include "scigraphics/geometry.h"

#include <string>
#include <vector>
#include <utility>
  
// ============================================================

namespace scigraphics
{

  // ============================================================
  
  class lineStyle;
  class brushStyle;
  class textStyle;
  class pointStyle;
  class errorBarStyle;
  class drawer;
  
  // ============================================================

  class painter
  {
    public:
      enum textPosition { HLeft = 0x0001,       HRight = 0x0002,        HCenter = 0x0003,
                          VUp   = 0x0100,       VDown = 0x0200,         VCenter = 0x0300 };

    private:
      drawer *Drawer;
      
      indents<wcoord> Indents;
      wrectangle PlotRectangle;

    private:
      painter( const painter& );
      painter& operator=( const painter& );
      
    protected:
      bool ableToDraw() const { return ( Drawer != NULL ); }
      void updatePlotRectangle();

    public:
      explicit painter( drawer *Drawer = NULL );
      ~painter();
      
      void setDrawer( drawer *Drawer );
      drawer* getDrawer() { return Drawer; }
      
      wcoord plotWidth()  const;
      wcoord plotHeight() const;

      void setIndents( const indents<wcoord>& Indents );
      const indents<wcoord> getIndents() const { return Indents; }
      
      inline const wrectangle& plotRectangle() const;
      inline wcoord width() const;
      inline wcoord height() const;

      fcoord wcoord2fcoordX( wcoord X ) const;
      fcoord wcoord2fcoordY( wcoord Y ) const;
      
      inline wcoord fcoord2wcoordX( fcoord X ) const;
      inline wcoord fcoord2wcoordY( fcoord Y ) const;
      
      wpoint fpoint2wpoint( fpoint Pt ) const;
      fpoint wpoint2fpoint( wpoint Pt ) const;

      bool canSeparatePointsF( fpoint Point1, fpoint Ppoint2 ) const;
      bool canSepareteXcoordsF( fcoord X1, fcoord X2 ) const;
     
      void update();
      void flush();
      void clearPlotArea();
      void clearBordersArea();

      void setLineStyle( const lineStyle &Style );
      void drawLineW( wpoint A, wpoint B, const lineStyle &Style );
      void drawLineW( wpoint A, wpoint B );
      void drawLineW( const std::vector<wpoint> &Points, const lineStyle &Style );
      void drawLineW( const std::vector<wpoint> &Points );
      void drawLineF( fpoint A, fpoint B, const lineStyle &Style );
      void drawLineF( fpoint A, fpoint B );

      void drawTextF( const std::string &String, fpoint Point, unsigned PositionFlags, 
        const textStyle &Style, int ShiftLeft = 0, int ShiftUp = 0, double Angle = 0 );
      void drawTextW( const std::string &String, wpoint Point, unsigned Position, 
        const textStyle &Style, int ShiftLeft = 0, int ShiftUp = 0, double Angle = 0 );

      void drawRectangleF( const frectangle &Rect, const brushStyle &BrushStyle );
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

      std::pair<wcoord,wcoord> textWidthHeight( const std::string &Text, const textStyle &Style );
  };

  // ============================================================
      
  const wrectangle& painter::plotRectangle() const 
  { 
    return PlotRectangle; 
  }
  
  // ------------------------------------------------------------
  
  wcoord painter::width() const  
  { 
    return PlotRectangle.width();  
  }
  
  // ------------------------------------------------------------
  
  wcoord painter::height() const 
  { 
    return PlotRectangle.height(); 
  }
  
  // ------------------------------------------------------------

  wcoord painter::fcoord2wcoordX( fcoord X ) const
  {
    return static_cast<wcoord>( X * PlotRectangle.width() ) + Indents.left();
  }

  // ------------------------------------------------------------

  wcoord painter::fcoord2wcoordY( fcoord Y ) const
  {
    return static_cast<wcoord>( ( static_cast<scigraphics::fcoord>(1) - Y ) * PlotRectangle.height() ) + Indents.up();
  }
  
  // ============================================================


}

