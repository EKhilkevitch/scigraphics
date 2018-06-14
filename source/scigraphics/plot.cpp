
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


// ============================================================

#include "scigraphics/plot.h"
#include "scigraphics/painter.h"
#include "scigraphics/scale.h"
#include "scigraphics/legend.h"
#include "scigraphics/cursorpoint.h"
#include "scigraphics/zoomrect.h"
#include "scigraphics/mouse.h"
#include "scigraphics/mousecallback.h"
#include "scigraphics/plotlimits.h"
#include "scigraphics/selection.h"
#include "scigraphics/graph.h"
#include "scigraphics/graphsequence.h"
#include "scigraphics/graphmap.h"
#include "scigraphics/axisset.h"
#include "scigraphics/interval.h"

#include <stdexcept>
#include <iostream>
#include <list>

#if _MSC_VER
#  pragma warning( disable : 4355 )
#endif

// ============================================================

namespace scigraphics
{
  struct plotInternalData
  {
    colorSequence GraphicsColorSequence;

    painter Painter;
    
    plotLimits PlotLimits;

    zoomRectangle ZoomRectangle;

    legend Legend;
    cursorPositionViewer CursorPositionViewer;
    std::list< floatRectangle* > FloatRectangles;

    axisSetCollection AxisSets;
    
    mouse MouseHandler;
    mouseCallBackContainer CallBackContainer;

    plotInternalData( plot &Plot );
    ~plotInternalData();
      
    void prepareAxisSets( plot &Plot );
    void prepareFloatRectangles();
  };
}

// ------------------------------------------------------------

scigraphics::plotInternalData::plotInternalData( plot &Plot ) :
  GraphicsColorSequence( colorSequence::defaultColorSequence() ),
  MouseHandler( Plot ), 
  CallBackContainer( Plot )
{
  prepareAxisSets(Plot);
  prepareFloatRectangles();
}

// ------------------------------------------------------------

scigraphics::plotInternalData::~plotInternalData()
{
}

// ------------------------------------------------------------

void scigraphics::plotInternalData::prepareAxisSets( plot &Plot )
{
  Plot.Graphics.setDefaultAxisSets( &AxisSets[AxisBottom], &AxisSets[AxisLeft] );
  Plot.Selections.setDefaultAxisSets( &AxisSets[AxisBottom], &AxisSets[AxisLeft] );
}

// ------------------------------------------------------------

void scigraphics::plotInternalData::prepareFloatRectangles()
{
  FloatRectangles.push_back( &Legend );
  FloatRectangles.push_back( &CursorPositionViewer );
}

// ============================================================

scigraphics::plot::plot() : 
  Pimpl( new plotInternalData(*this) )
{
}

// ------------------------------------------------------------

scigraphics::plot::~plot()
{
  delete Pimpl;
}

// ------------------------------------------------------------

scigraphics::sequence::graphVector* scigraphics::plot::createGraphSequenceVector( const std::string &Legend, const color &Color )
{
  return createGraphSV( Legend, Color );
}

// ------------------------------------------------------------

scigraphics::graphSV* scigraphics::plot::createGraphSV( const std::string &Legend, const color &Color ) 
{ 
  return createGraph<graphSV>( Legend, selectNextGraphColor(Color) ); 
}

// ------------------------------------------------------------

scigraphics::graphAV* scigraphics::plot::createGraphAV( const std::string &Legend, const color &Color ) 
{
  return createGraph<graphAV>( Legend, selectNextGraphColor(Color) ); 
}

// ------------------------------------------------------------

scigraphics::graphMV* scigraphics::plot::createGraphMV( const std::string &Legend ) 
{ 
  return createGraph<graphMV>(Legend); 
}

// ------------------------------------------------------------

void scigraphics::plot::replot()
{
  if ( getDrawer() == NULL )
    throw std::runtime_error( "Drawer is not initialized" );

  prepareForPainting();

  clearPlotArea();
  drawGraphicsUnderGrid();
  drawGrid();
  drawGraphicsOverGrid();
  drawSelections();
  
  clearBorders();
  drawAxis();
  drawAxisTicks();
  drawAxisLabels();
  drawAxisTitles();
  drawFloatRectangles();
  drawZoomRectangle();

  flush();
}

// ------------------------------------------------------------

void scigraphics::plot::prepareForPainting()
{
  preparePainter();
  updateIndents();
  updateScaleLimits();
}

// ------------------------------------------------------------

void scigraphics::plot::replotFloatRectangles()
{
  if ( getDrawer() == NULL )
    throw std::runtime_error( "Drawer is not initialized" );

  drawFloatRectangles();
  flush();
}

// ------------------------------------------------------------

void scigraphics::plot::preparePainter()
{
  Pimpl->Painter.update();
}

// ------------------------------------------------------------

void scigraphics::plot::updateScaleLimits()
{
  plotLimits::limitsXY Limits = Pimpl->PlotLimits.limitsForGraphics( Graphics );
  Pimpl->AxisSets.setAxisNumberLimits( &Limits );
  Pimpl->AxisSets.setScalesTo1x1ifNeeded( Pimpl->Painter );
}

// ------------------------------------------------------------

void scigraphics::plot::setDrawer( drawer *Drawer ) 
{ 
  Pimpl->Painter.setDrawer(Drawer); 
}

// ------------------------------------------------------------

scigraphics::drawer* scigraphics::plot::getDrawer() 
{ 
  return Pimpl->Painter.getDrawer(); 
}

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::plot::axisSetIndent( const axisSet &AxisSet ) const
{
  std::set< const axisSet* > UsedAxisSets = Graphics.setOfVisibleAxisSet();
  bool Used = UsedAxisSets.find(&AxisSet) != UsedAxisSets.end();
  return AxisSet.requiredIndent( Used );
}

// ------------------------------------------------------------

void scigraphics::plot::updateIndents()
{
  indents<wcoord> Indents;
  Indents.setLeft(  axisSetIndent(Pimpl->AxisSets[AxisLeft])   );
  Indents.setRight( axisSetIndent(Pimpl->AxisSets[AxisRight])  );
  Indents.setUp(    axisSetIndent(Pimpl->AxisSets[AxisTop])    );
  Indents.setDown(  axisSetIndent(Pimpl->AxisSets[AxisBottom]) );

  Pimpl->Painter.setIndents( Indents );
}

// ------------------------------------------------------------

scigraphics::mouseCallBack& scigraphics::plot::getMouseCallBack() 
{ 
  return Pimpl->CallBackContainer.get(); 
}

// ------------------------------------------------------------

void scigraphics::plot::setMouseCallBack( mouseCallBack *CallBack ) 
{ 
  Pimpl->CallBackContainer.set(CallBack); 
}

// ------------------------------------------------------------
      
scigraphics::mouse& scigraphics::plot::mouseHandler() 
{ 
  return Pimpl->MouseHandler; 
}

// ------------------------------------------------------------

const scigraphics::mouse& scigraphics::plot::mouseHandler() const 
{ 
  return Pimpl->MouseHandler; 
}
      
// ------------------------------------------------------------

scigraphics::painter& scigraphics::plot::getPainter() 
{ 
  return Pimpl->Painter; 
}

// ------------------------------------------------------------

const scigraphics::painter& scigraphics::plot::getPainter() const 
{ 
  return Pimpl->Painter; 
}

// ------------------------------------------------------------
      
void scigraphics::plot::clearPlotArea()
{
  Pimpl->Painter.clearPlotArea();
}

// ------------------------------------------------------------
      
void scigraphics::plot::flush() 
{ 
  Pimpl->Painter.flush(); 
}

// ------------------------------------------------------------

void scigraphics::plot::clearBorders()
{
  Pimpl->Painter.clearBordersArea();
}

// ------------------------------------------------------------

void scigraphics::plot::drawGrid()
{
  Pimpl->AxisSets.drawGrid(Pimpl->Painter);
}

// ------------------------------------------------------------

void scigraphics::plot::drawAxis()
{
  Pimpl->AxisSets.drawAxis(Pimpl->Painter);
}

// ------------------------------------------------------------

void scigraphics::plot::drawFloatRectangles()
{
  Pimpl->Legend.draw( Pimpl->Painter, Graphics );
  Pimpl->CursorPositionViewer.draw( Pimpl->Painter, getBottomLeftPairScales(), Pimpl->MouseHandler.lastPosition() );
}

// ------------------------------------------------------------

void scigraphics::plot::drawAxisTicks()
{
  std::set< const axisSet* > UsedAxisSets = Graphics.setOfVisibleAxisSet();
  for ( std::set< const axisSet* >::const_iterator i = UsedAxisSets.begin(); i != UsedAxisSets.end(); ++i )
    (*i)->drawAxisTicks( Pimpl->Painter );
}

// ------------------------------------------------------------

void scigraphics::plot::drawAxisLabels()
{
  std::set< const axisSet* > UsedAxisSets = Graphics.setOfVisibleAxisSet();
  for ( std::set< const axisSet* >::const_iterator i = UsedAxisSets.begin(); i != UsedAxisSets.end(); ++i )
    (*i)->drawAxisLabels( Pimpl->Painter );
}

// ------------------------------------------------------------

void scigraphics::plot::drawAxisTitles()
{
  std::set< const axisSet* > UsedAxisSets = Graphics.setOfVisibleAxisSet();
  for ( std::set< const axisSet* >::const_iterator i = UsedAxisSets.begin(); i != UsedAxisSets.end(); ++i )
    (*i)->drawAxisTitle( Pimpl->Painter );
}

// ------------------------------------------------------------

void scigraphics::plot::drawGraphicsOverGrid()
{
  Graphics.draw( Pimpl->Painter, true );
}

// ------------------------------------------------------------

void scigraphics::plot::drawGraphicsUnderGrid()
{
  Graphics.draw( Pimpl->Painter, false );
}

// ------------------------------------------------------------

void scigraphics::plot::drawSelections()
{
  Selections.draw( Pimpl->Painter, true );
}

// ------------------------------------------------------------

void scigraphics::plot::drawZoomRectangle()
{
  Pimpl->ZoomRectangle.draw( Pimpl->Painter );
}

// ------------------------------------------------------------

scigraphics::floatRectangle* scigraphics::plot::getFloatRectangle( wpoint Point )
{
  for ( std::list< floatRectangle* >::reverse_iterator f = Pimpl->FloatRectangles.rbegin(); f != Pimpl->FloatRectangles.rend(); ++f )
  {
    if ( (*f)->containPoint( Point ) )
      return *f;
  }
  return NULL;
}

// ------------------------------------------------------------
      
scigraphics::zoomRectangle&  scigraphics::plot::getZoomRectangle() 
{ 
  return Pimpl->ZoomRectangle; 
}
      
// ------------------------------------------------------------

scigraphics::selection* scigraphics::plot::getSelection( wpoint Point )
{
  fpoint FPoint = Pimpl->Painter.wpoint2fpoint(Point);
  return Selections.getSelectionOnPoint( FPoint );
}
      
// ------------------------------------------------------------

void scigraphics::plot::setSelectionInterval( selectionStrip *Selection, wpoint Pt1, wpoint Pt2 )
{
  fpoint FPoint1 = Pimpl->Painter.wpoint2fpoint(Pt1);
  fpoint FPoint2 = Pimpl->Painter.wpoint2fpoint(Pt2);
  Selections.setSelectionInterval( Selection, FPoint1, FPoint2 );
}

// ------------------------------------------------------------
      
void scigraphics::plot::shiftSelection( selectionStrip *Selection, wpoint From, wpoint To )
{
  fpoint FFrom = Pimpl->Painter.wpoint2fpoint(From);
  fpoint FTo   = Pimpl->Painter.wpoint2fpoint(To);
  Selections.shiftSelection( Selection, FFrom, FTo );
}

// ------------------------------------------------------------

scigraphics::pairScales scigraphics::plot::getBottomLeftPairScales() 
{
  return Pimpl->AxisSets.getBottomLeftPairScales();
}
      
// ------------------------------------------------------------

scigraphics::color scigraphics::plot::nextGraphColor() 
{ 
  return Pimpl->GraphicsColorSequence.next(); 
}

// ------------------------------------------------------------

scigraphics::color scigraphics::plot::currentGraphColor() const 
{ 
  return Pimpl->GraphicsColorSequence.current(); 
}

// ------------------------------------------------------------

scigraphics::color scigraphics::plot::selectNextGraphColor( const color &Color )
{
  if ( Color == color::White )
    return nextGraphColor();
  return Color;
}

// ------------------------------------------------------------

void scigraphics::plot::resetGraphColor() 
{ 
  Pimpl->GraphicsColorSequence.reset(); 
}

// ------------------------------------------------------------

void scigraphics::plot::appendGraphic( graph *Graph )
{
  if ( Graph == NULL )
    throw std::invalid_argument("Zero graphic pointer is ibvalid");
  Graphics.append(Graph); 
}

// ------------------------------------------------------------

void scigraphics::plot::bindGraphToAxis( const graph *Graph, axisPosition AxisPosX, axisPosition AxisPosY )
{
  if ( Graph == NULL )
    throw std::invalid_argument("Zero graphic pointer is ibvalid");

  const axisSet *AxisX = &Pimpl->AxisSets[AxisPosX];
  const axisSet *AxisY = &Pimpl->AxisSets[AxisPosY];

  assert( AxisX != NULL && AxisY != NULL );
  if ( AxisX->getDirection() != AxisDirectionX || AxisY->getDirection() != AxisDirectionY )
    throw std::invalid_argument("Invalid axis X or Y position");

  Graphics.bindGraphToAxisSet( Graph, AxisX, AxisY );
}

// ------------------------------------------------------------

void scigraphics::plot::clearGraphics() 
{ 
  Graphics.clear(); 
  Pimpl->GraphicsColorSequence.reset(); 
}

// ------------------------------------------------------------

void scigraphics::plot::removeGraphic( graph *Graph ) 
{ 
  Graphics.remove(Graph);
}
      
// ------------------------------------------------------------

void scigraphics::plot::releaseGraphic( graph *Graph )
{
  Graphics.release(Graph);
}

// ------------------------------------------------------------

scigraphics::graphCollection::iterator scigraphics::plot::eraseGraphic( graphCollection::iterator Iterator )
{
  return Graphics.erase(Iterator);
}

// ------------------------------------------------------------

size_t scigraphics::plot::countOfGraphics() const
{
  return Graphics.size();
}

// ------------------------------------------------------------

void scigraphics::plot::deleteSelection( selection *Selection ) 
{ 
  Selections.remove( Selection ); 
}

// ------------------------------------------------------------

void scigraphics::plot::clearSelections() 
{ 
  Selections.clear(); 
}

// ------------------------------------------------------------

size_t scigraphics::plot::countOfSelections() const
{
  return Selections.size();
}

// ------------------------------------------------------------
      
bool scigraphics::plot::enabledMultipleMouseSelections() const
{
  return Pimpl->MouseHandler.enabledMultipleMouseSelections();
}

// ------------------------------------------------------------

void scigraphics::plot::setEnableMultipleMouseSelections( bool Enable )
{
  Pimpl->MouseHandler.setEnableMultipleMouseSelections( Enable );
}

// ------------------------------------------------------------
      
void scigraphics::plot::addScalesShift( double Shift, axisDirection Direction )
{
  Pimpl->AxisSets.addScalesShift(Shift,Direction);
}

// ------------------------------------------------------------
      
void scigraphics::plot::mulScalesZoom( double Zoom, axisDirection Direction )
{
  Pimpl->AxisSets.mulScalesZoom(Zoom,Direction);
}

// ------------------------------------------------------------
      
void scigraphics::plot::resetScales( axisDirection Direction )
{
  Pimpl->AxisSets.resetScales(Direction);
}
      
// ------------------------------------------------------------

void scigraphics::plot::resetAllScales()
{
  Pimpl->AxisSets.resetAllScales();
}

// ------------------------------------------------------------

void scigraphics::plot::setScalesTo1x1( bool SetTo1x1 )
{
  Pimpl->AxisSets.setFixedScalesTo1x1( SetTo1x1 );
}

// ------------------------------------------------------------
      
const scigraphics::scale* scigraphics::plot::scaleWithPosition( axisPosition Position ) const
{
  return Pimpl->AxisSets[Position].getScale();
}

// ------------------------------------------------------------

scigraphics::scale* scigraphics::plot::scaleWithPosition( axisPosition Position ) 
{
  return Pimpl->AxisSets[Position].getScale();
}

// ------------------------------------------------------------

void scigraphics::plot::replaceScaleWithPosition( axisPosition Position, scale *Scale )
{
  Pimpl->AxisSets[Position].replaceScale(Scale);
}

// ------------------------------------------------------------

void scigraphics::plot::setScaleInterval( axisPosition Position, interval<number> Limits )
{
  const axisSet *Set = &Pimpl->AxisSets[Position];
  Pimpl->PlotLimits.setInterval( Set, Limits );
}

// ------------------------------------------------------------

void scigraphics::plot::setScaleIntervalX( interval<number> Interval )     
{ 
  setScaleInterval( AxisBottom, Interval ); 
}

// ------------------------------------------------------------

void scigraphics::plot::setScaleIntervalX( number Min, number Max ) 
{ 
  setScaleIntervalX( interval<number>(Min,Max) ); 
}

// ------------------------------------------------------------

scigraphics::interval<scigraphics::number> scigraphics::plot::scaleIntervalX() const          
{ 
  return scaleInterval( AxisBottom ); 
}

// ------------------------------------------------------------

void scigraphics::plot::setScaleIntervalY( interval<number> Interval )     
{ 
  setScaleInterval( AxisLeft, Interval ); 
}

// ------------------------------------------------------------

void scigraphics::plot::setScaleIntervalY( number Min, number Max ) 
{ 
  setScaleIntervalY( interval<number>(Min,Max) ); 
}

// ------------------------------------------------------------

scigraphics::interval<scigraphics::number> scigraphics::plot::scaleIntervalY() const          
{ 
  return scaleInterval( AxisLeft ); 
}

// ------------------------------------------------------------
      
scigraphics::interval<scigraphics::number> scigraphics::plot::scaleInterval( axisPosition Position ) const
{
  const axisSet *Set = &Pimpl->AxisSets[Position];
  return Pimpl->PlotLimits.getInterval(Set);
}

// ------------------------------------------------------------
      
scigraphics::interval<scigraphics::number> scigraphics::plot::visibleInterval( axisPosition Position ) const
{
  const axisSet *Set = &Pimpl->AxisSets[Position];
  return Set->getScale()->getVisivleInterval();
}

// ------------------------------------------------------------
      
scigraphics::interval<scigraphics::number> scigraphics::plot::visibleIntervalX() const 
{ 
  return visibleInterval(AxisBottom); 
}

// ------------------------------------------------------------

scigraphics::interval<scigraphics::number> scigraphics::plot::visibleIntervalY() const 
{ 
  return visibleInterval(AxisLeft); 
}

// ------------------------------------------------------------

void scigraphics::plot::setScaleLock( axisPosition Position, bool Lock )
{
  Pimpl->AxisSets[Position].getScale()->setLock(Lock);
}

// ------------------------------------------------------------
      
void scigraphics::plot::setScaleLockX( bool Lock )   
{ 
  setScaleLock( AxisBottom, Lock ); 
}

// ------------------------------------------------------------

void scigraphics::plot::setScaleLockY( bool Lock )   
{ 
  setScaleLock( AxisLeft, Lock ); 
}

// ------------------------------------------------------------

void scigraphics::plot::setAxisTitle( axisPosition Position, const std::string &Title )
{
  Pimpl->AxisSets[Position].setAxisTitle( Title );
}

// ------------------------------------------------------------
      
std::string scigraphics::plot::getAxisTitle( axisPosition Position ) const
{
  return Pimpl->AxisSets[Position].getAxisTitle();
}

// ------------------------------------------------------------
      
void scigraphics::plot::setBottomLeftAxisTitles( const std::string &TitleX, const std::string &TitleY )
{
  setAxisTitle( AxisBottom, TitleX );
  setAxisTitle( AxisLeft,   TitleY );
}

// ------------------------------------------------------------

void scigraphics::plot::setAxisNumberStyle( axisPosition Position, const numberStyle &Style )
{
  Pimpl->AxisSets[Position].setNumberStyle(Style);
}

// ------------------------------------------------------------

void scigraphics::plot::setDisallowedMouseOperations( unsigned Operation ) 
{ 
  mouseHandler().setDisallowedOperations(Operation); 
}

// ------------------------------------------------------------

void scigraphics::plot::setAllowedMouseOperations( unsigned Operation )    
{ 
  mouseHandler().setAllowedOperations(Operation);    
}

// ------------------------------------------------------------

void scigraphics::plot::setMouseOperations( unsigned Operations )          
{ 
  mouseHandler().setOperations(Operations);          
}

// ------------------------------------------------------------

unsigned scigraphics::plot::allowedOperations() const                      
{ 
  return mouseHandler().allowedOperations(); 
}
      
// ------------------------------------------------------------

void scigraphics::plot::setReplotOnMouseActions( bool Replot ) 
{ 
  mouseHandler().setReplotOnMouseActions(Replot); 
}

// ------------------------------------------------------------

void scigraphics::plot::setVisibleLegend( bool Visible ) 
{ 
  Pimpl->Legend.setVisible(Visible); 
}

// ------------------------------------------------------------

void scigraphics::plot::setVisibleCursorPositionViewer( bool Visible ) 
{ 
  Pimpl->CursorPositionViewer.setVisible(Visible); 
}

// ------------------------------------------------------------
      
void scigraphics::plot::setStretchFactors( double SX, double SY )
{
  Pimpl->PlotLimits.setStretchFactorX(SX);
  Pimpl->PlotLimits.setStretchFactorY(SY);
}

// ============================================================

