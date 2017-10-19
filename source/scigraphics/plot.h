
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

#include "scigraphics/axisposition.h"
#include "scigraphics/color.h"
#include "scigraphics/graphcollection.h"
#include "scigraphics/selectioncollection.h"

#include <string>

// ============================================================

namespace scigraphics
{
  
  // ============================================================

  namespace sequence
  {
    class graphVector;
    class graphAreaVector;
  }

  namespace map
  {
    class graphVector;
  }

  template <class T> class interval;

  typedef sequence::graphVector graphSV;
  typedef sequence::graphAreaVector graphAV;
  typedef map::graphVector graphMV;

  struct plotInternalData;

  class mouse;
  class painter;
  class drawer;

  class floatRectangle;
  class zoomRectangle;
  class mouseCallBack;

  class numberStyle;

  // ============================================================

  class plot 
  {
    friend class mouse;
    friend class settings;
    friend struct plotInternalData;

    private:
      plot( const plot& );
      plot& operator=( const plot& );

    private:
      graphCollection Graphics;
      selectionCollection Selections;

      plotInternalData *Pimpl;

    private:
      void drawAxis();
      void drawAxisTicks();
      void drawAxisLabels();
      void drawAxisTitles();
      void drawGrid();
      void drawGraphicsUnderGrid();
      void drawGraphicsOverGrid();
      void drawSelections();
      void drawFloatRectangles();
      void drawZoomRectangle();
      void clearBorders();
      void clearPlotArea();
      void flush();
      
      void replotFloatRectangles();

      void prepareForPainting();
      
      void preparePainter();
      void updateIndents();
      void updateScaleLimits();

    protected:
      void setDrawer( drawer *Drawer );
      drawer* getDrawer();

      wcoord axisSetIndent( const axisSet &Set ) const;

      mouseCallBack& getMouseCallBack();
      void setMouseCallBack( mouseCallBack *CallBack );
      
      mouse& mouseHandler();
      const mouse& mouseHandler() const;
     
#if _MSC_VER <= 1400
    public:
#else
    protected:
#endif
      painter& getPainter();
      const painter& getPainter() const;
      
      zoomRectangle&  getZoomRectangle();
      floatRectangle* getFloatRectangle( wpoint Point );
      selection*      getSelection( wpoint Point );
      
      void setSelectionInterval( selectionStrip *Selection, wpoint Pt1, wpoint Pt2 );
      void shiftSelection( selectionStrip *Selection, wpoint From, wpoint To );
      
    public:
      plot();
      virtual ~plot();

      virtual void replot();

      template < class G > G* createGraph( const std::string &Legend = std::string() );
      template < class G > G* createGraph( const std::string &Legend, const color &Color );
      graphSV* createGraphSV( const std::string &Legend = std::string(), const color &Color = color::White );
      graphAV* createGraphAV( const std::string &Legend = std::string(), const color &Color = color::White );
      graphMV* createGraphMV( const std::string &Legend = std::string() );

      // deprecated
      sequence::graphVector* createGraphSequenceVector( const std::string &Legend = std::string(), const color &Color = color::White );

      void appendGraphic( graph *Graph );
      void bindGraphToAxis( const graph *Graph, axisPosition AxisX, axisPosition AxisY );
      void clearGraphics();
      void removeGraphic( graph *Graph );
      void releaseGraphic( graph *Graph );
      graphCollection::iterator eraseGraphic( graphCollection::iterator Iterator );
      graphCollection::iterator beginGraph() { return Graphics.begin(); }
      graphCollection::iterator endGraph()   { return Graphics.end(); }
      graphCollection::const_iterator beginGraph() const { return Graphics.begin(); }
      graphCollection::const_iterator endGraph()   const { return Graphics.end(); }
      size_t countOfGraphics() const;

      template <class S> S* createSelection();
      void deleteSelection( selection *Selection );
      void clearSelections();
      selectionCollection::iterator beginSelection() { return Selections.begin(); }
      selectionCollection::iterator endSelection()   { return Selections.end(); }
      selectionCollection::const_iterator beginSelection() const { return Selections.begin(); }
      selectionCollection::const_iterator endSelection()   const { return Selections.end(); }
      selectionCollection::const_reverse_iterator rbeginSelection() const { return Selections.rbegin(); }
      selectionCollection::const_reverse_iterator rendSelection()   const { return Selections.rend(); }
      size_t countOfSelections() const;
      bool enabledMultipleMouseSelections() const;
      void setEnableMultipleMouseSelections( bool Enable );

      pairScales getBottomLeftPairScales();

      color nextGraphColor();
      color currentGraphColor() const;
      color selectNextGraphColor( const color &Color = color::White );
      void resetGraphColor();
      
      const scale* scaleWithPosition( axisPosition Position ) const;
      scale* scaleWithPosition( axisPosition Position );
      void replaceScaleWithPosition( axisPosition Position, scale *Scale );

      void setScaleInterval( axisPosition Position, interval<number> Limits );
      interval<number> scaleInterval( axisPosition Position ) const;
      
      void setScaleIntervalX( interval<number> Interval );
      void setScaleIntervalX( number Min, number Max );
      interval<number> scaleIntervalX() const;

      void setScaleIntervalY( interval<number> Interval );
      void setScaleIntervalY( number Min, number Max );
      interval<number> scaleIntervalY() const;
      
      interval<number> visibleInterval( axisPosition Position ) const;
      interval<number> visibleIntervalX() const;
      interval<number> visibleIntervalY() const;
      
      void addScalesShift( double Shift, axisDirection Direction );
      void mulScalesZoom( double Zoom, axisDirection Direction );
      void resetScales( axisDirection Direction );
      void resetAllScales();
      void setScalesTo1x1( bool SetTo1x1 );

      void setStretchFactors( double SX, double SY );

      void setScaleLock( axisPosition Position, bool Lock );
      void setScaleLockX( bool Lock );
      void setScaleLockY( bool Lock );

      void setAxisTitle( axisPosition Position, const std::string &Title );
      std::string getAxisTitle( axisPosition Position ) const;
      void setBottomLeftAxisTitles( const std::string &TitleX, const std::string &TitleY );

      void setAxisNumberStyle( axisPosition Position, const numberStyle &Style );

      void setDisallowedMouseOperations( unsigned Operation );
      void setAllowedMouseOperations( unsigned Operation );
      void setMouseOperations( unsigned Operations );
      unsigned allowedOperations() const;
      void setReplotOnMouseActions( bool Relpot );
     
      void setVisibleLegend( bool Visible );
      void setVisibleCursorPositionViewer( bool Visible );
  };

  // ============================================================
      
  template < class G > G* plot::createGraph( const std::string &Legend ) 
  { 
    return Graphics.create<G>(Legend); 
  }
  
  // ------------------------------------------------------------
  
  template < class G > G* plot::createGraph( const std::string &Legend, const color &Color ) 
  { 
    return Graphics.create<G>(Legend,Color); 
  }
  
  // ------------------------------------------------------------
      
  template <class S> S* plot::createSelection() 
  { 
    return Selections.create<S>(); 
  }
  
  // ============================================================

}


