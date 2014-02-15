
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

#include "scigraphics/axisset.h"
#include "scigraphics/painter.h"
#include "scigraphics/graphcollection.h"
#include "scigraphics/graphsequence.h"
#include "scigraphics/graphmap.h"
#include "scigraphics/selectioncollection.h"
#include "scigraphics/plotlimits.h"
#include "scigraphics/zoomrect.h"
#include "scigraphics/mouse.h"
#include "scigraphics/mousecallback.h"
#include "scigraphics/legend.h"
#include "scigraphics/cursorpoint.h"
#include "scigraphics/scale.h"

// ============================================================

namespace scigraphics
{

  typedef sequence::graphVector graphSV;
  typedef sequence::graphAreaVector graphAV;
  typedef map::graphVector graphMV;

// ============================================================

  class plot 
  {
    friend class mouse;
    friend class settings;

    private:
      plot( const plot& );
      plot& operator=( const plot& );

    private:
      painter Painter;
      
      plotLimits PlotLimits;

      graphCollection Graphics;
      selectionCollection Selections;

      colorSequence GraphicsColorSequence;

      zoomRectangle ZoomRectangle;

      legend Legend;
      cursorPositionViewer CursorPositionViewer;
      std::list< floatRectangle* > FloatRectangles;

      axisSetCollection AxisSets;
      
      mouse MouseHandler;

      mouseCallBackContainer CallBackContainer;

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
      void flush() { Painter.flush(); }
      
      void replotFloatRectangles();

      void prepareAxisSets();
      void prepareFloatRectangles();

      void preparePainter();
      void prepareForPainting();

    protected:
      void setDrawer( drawer *D ) { Painter.setDrawer(D); }
      drawer* getDrawer() { return Painter.getDrawer(); }

      wcoord axisSetIndent( const axisSet &Set ) const;
      void updateIndents();

      mouseCallBack& getMouseCallBack() { return CallBackContainer.get(); }
      void setMouseCallBack( mouseCallBack *C ) { CallBackContainer.set(C); }
      
      mouse& mouseHandler() { return MouseHandler; }
      const mouse& mouseHandler() const { return MouseHandler; }
     
#if _MSC_VER
    public:
#else
    protected:
#endif
      painter& getPainter() { return Painter; }
      const painter& getPainter() const { return Painter; }
      
      zoomRectangle&  getZoomRectangle();
      floatRectangle* getFloatRectangle( wpoint Point );
      selection*      getSelection( wpoint Point );
      
      void setSelectionInterval( selectionStrip *Selection, wpoint Pt1, wpoint Pt2 );
      void shiftSelection( selectionStrip *Selection, wpoint From, wpoint To );
      
    public:
      plot();
      virtual ~plot();

      virtual void replot();

      template < class G > G* createGraph( const std::string &Legend = std::string() ) { return Graphics.create<G>(Legend); }
      template < class G > G* createGraph( const std::string &Legend, const color &Color ) { return Graphics.create<G>(Legend,Color); }
      graphSV* createGraphSV( const std::string &Legend = std::string(), const color &Color = color::White );
      graphAV* createGraphAV( const std::string &Legend = std::string(), const color &Color = color::White );
      graphMV* createGraphMV( const std::string &Legend = std::string() );

      // deprecated
      sequence::graphVector* createGraphSequenceVector( const std::string &Legend = std::string(), const color &Color = color::White );

      void appendGraphic( graph *Graph );
      void bindGraphToAxis( const graph *Graph, axisSetCollection::axisPosition AxisX, axisSetCollection::axisPosition AxisY );
      void clearGraphics() { Graphics.clear(); GraphicsColorSequence.reset(); }
      void removeGraphic( graph *Graph ) { Graphics.erase(Graph); }
      graphCollection::iterator beginGraph() { return Graphics.begin(); }
      graphCollection::iterator endGraph()   { return Graphics.end(); }
      graphCollection::const_iterator beginGraph() const { return Graphics.begin(); }
      graphCollection::const_iterator endGraph()   const { return Graphics.end(); }

      template <class S> S* createSelection() { return Selections.create<S>(); }
      void deleteSelection( selection *S ) { Selections.erase( S ); }
      void clearSelections() { Selections.clear(); }
      selectionCollection::iterator beginSelection() { return Selections.begin(); }
      selectionCollection::iterator endSelection()   { return Selections.end(); }
      selectionCollection::const_iterator beginSelection() const { return Selections.begin(); }
      selectionCollection::const_iterator endSelection()   const { return Selections.end(); }

      pairScales getBottomLeftPairScales();

      color nextGraphColor() { return GraphicsColorSequence.next(); }
      color currentGraphColor() const { return GraphicsColorSequence.current(); }
      color selectNextGraphColor( const color &Color = color::White );
      void resetGraphColor() { GraphicsColorSequence.reset(); }
      
      const scale* scaleWithPosition( axisSetCollection::axisPosition Position ) const;
      scale* scaleWithPosition( axisSetCollection::axisPosition Position );
      void replaceScaleWithPosition( axisSetCollection::axisPosition Position, scale *Scale );

      void setScaleInterval( axisSetCollection::axisPosition Position, interval<number> Limits );
      interval<number> scaleInterval( axisSetCollection::axisPosition Position ) const;
      
      void setScaleIntervalX( interval<number> L )     { setScaleInterval(axisSetCollection::Bottom,L); }
      void setScaleIntervalX( number Min, number Max ) { setScaleIntervalX(interval<number>(Min,Max)); }
      interval<number> scaleIntervalX() const          { return scaleInterval(axisSetCollection::Bottom); }

      void setScaleIntervalY( interval<number> L )     { setScaleInterval(axisSetCollection::Left,L); }
      void setScaleIntervalY( number Min, number Max ) { setScaleIntervalY(interval<number>(Min,Max)); }
      interval<number> scaleIntervalY() const          { return scaleInterval(axisSetCollection::Left); }
      
      void updateScaleLimits();

      interval<number> visibleInterval( axisSetCollection::axisPosition Position ) const;
      interval<number> visibleIntervalX() const { return visibleInterval(axisSetCollection::Bottom); }
      interval<number> visibleIntervalY() const { return visibleInterval(axisSetCollection::Left); }
      
      void addScalesShift( double Shift, axisSet::direction Direction );
      void mulScalesZoom( double Zoom, axisSet::direction Direction );
      void resetScales( axisSet::direction Direction );
      void resetAllScales();
      void setScalesTo1x1( bool SetTo1x1 );

      void setStretchFactors( double SX, double SY );

      void setScaleLock( axisSetCollection::axisPosition Position, bool Lock );
      void setScaleLockX( bool Lock )   { setScaleLock( axisSetCollection::Bottom, Lock ); }
      void setScaleLockY( bool Lock )   { setScaleLock( axisSetCollection::Left,   Lock ); }

      void setAxisTitle( axisSetCollection::axisPosition Position, const std::string &Title );
      std::string getAxisTitle( axisSetCollection::axisPosition Position ) const;
      void setBottomLeftAxisTitles( const std::string &TitleX, const std::string &TitleY );

      void setAxisNumberStyle( axisSetCollection::axisPosition Position, numberStyle *Style );

      void setDisallowedMouseOperations( unsigned Operation ) { mouseHandler().setDisallowedOperations(Operation); }
      void setAllowedMouseOperations( unsigned Operation )    { mouseHandler().setAllowedOperations(Operation);    }
      void setMouseOperations( unsigned Operations )          { mouseHandler().setOperations(Operations);          }
      unsigned allowedOperations() const                      { return mouseHandler().allowedOperations(); }
     
      void setVisibleLegend( bool V ) { Legend.setVisible(V); }
      void setVisibleCursorPositionViewer( bool V ) { CursorPositionViewer.setVisible(V); }

      void setReplotOnMouseActions( bool R ) { mouseHandler().setReplotOnMouseActions(R); }
  };

// ============================================================

}


