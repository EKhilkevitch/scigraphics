
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

// ============================================================

namespace scigraphics
{

  typedef graphSequenceVector graphSV;
  typedef graphAreaVector graphAV;
  typedef graphMapVector graphMV;

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

    protected:
      void setDrawer( drawer *D ) { Painter.setDrawer(D); }
      drawer* getDrawer() { return Painter.getDrawer(); }

      void preparePainter();
      void updateScaleLimits();
      void prepareForPainting();

      wcoord axisSetIndent( const axisSet &Set ) const;
      void updateIndents();

      mouseCallBack& getMouseCallBack() { return CallBackContainer.get(); }
      void setMouseCallBack( mouseCallBack *C ) { CallBackContainer.set(C); }
      
    public:
      painter& getPainter() { return Painter; }
      const painter& getPainter() const { return Painter; }

      void addScalesShift( double Shift, axisSet::direction Direction );
      void mulScalesZoom( double Zoom, axisSet::direction Direction );
      void resetScales( axisSet::direction Direction );
      void resetAllScales();
      void resetScalesTo1x1();
     
      void setSelectionInterval( selectionStrip *Selection, wpoint Pt1, wpoint Pt2 );
      void shiftSelection( selectionStrip *Selection, wpoint From, wpoint To );

      zoomRectangle&  getZoomRectangle() { return ZoomRectangle; }
      floatRectangle* getFloatRectangle( wpoint Point );
      selection*      getSelection( wpoint Point );

      mouse& mouseHandler() { return MouseHandler; }


    public:
      plot();
      virtual ~plot();

      virtual void replot();

      template < class G > G* createGraph( const std::string &Legend = std::string() ) { return Graphics.create<G>(Legend); }
      template < class G > G* createGraph( const std::string &Legend, const color &Color ) { return Graphics.create<G>(Legend,Color); }
      graphSV* createGraphSV( const std::string &Legend = std::string(), const color &Color = color::White ) { return createGraph<graphSV>(Legend,Color); }
      graphAV* createGraphAV( const std::string &Legend = std::string(), const color &Color = color::White ) { return createGraph<graphAV>(Legend,Color); }
      graphMV* createGraphMV( const std::string &Legend = std::string() ) { return createGraph<graphMV>(Legend); }
      graphSequenceVector* createGraphSequenceVector( const std::string &Legend = std::string(), const color &Color = color::White );

      void appendGraphic( graph *Graph );
      void bindGraphToAxis( const graph *Graph, axisSetCollection::axisPosition AxisX, axisSetCollection::axisPosition AxisY );
      void clearGraphics() { Graphics.clear(); }
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

      void setStretchFactors( double SX, double SY );

      void setScaleLock( axisSetCollection::axisPosition Position, bool Lock );
      void setScaleLockX( bool Lock )   { setScaleLock( axisSetCollection::Bottom, Lock ); }
      void setScaleLockY( bool Lock )   { setScaleLock( axisSetCollection::Left,   Lock ); }

      void setAxisTitle( axisSetCollection::axisPosition Position, const std::string &Title );
      std::string getAxisTitle( axisSetCollection::axisPosition Position ) const;

      void setBottomLeftAxisTitles( const std::string &TitleX, const std::string &TitleY );

      void setDisallowedMouseOperations( unsigned Operation ) { mouseHandler().setDisallowedOperations(Operation); }
      void setAllowedMouseOperations( unsigned Operation )    { mouseHandler().setAllowedOperations(Operation);    }
     
      void setVisibleLegend( bool V ) { Legend.setVisible(V); }
      void setVisibleCursorPositionViewer( bool V ) { CursorPositionViewer.setVisible(V); }

      void setReplotOnMouseActions( bool R ) { mouseHandler().setReplotOnMouseActions(R); }
  };

// ============================================================

}


