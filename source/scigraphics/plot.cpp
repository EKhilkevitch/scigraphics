
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
#include "scigraphics/graphsequence.h"
#include "scigraphics/graphmap.h"

#include <stdexcept>
#include <iostream>
#include <list>

#if _WIN32
#  pragma warning( disable : 4355 )
#endif

// ============================================================

namespace scigraphics
{
  struct plotInternalData
  {
    painter Painter;
    
    plotLimits PlotLimits;

    colorSequence GraphicsColorSequence;

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
  MouseHandler( Plot ), 
  CallBackContainer( Plot ),
  GraphicsColorSequence( colorSequence::defaultColorSequence() )
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
  Plot.Graphics.setDefaultAxisSets( &AxisSets[axisSetCollection::Bottom], &AxisSets[axisSetCollection::Left] );
  Plot.Selections.setDefaultAxisSets( &AxisSets[axisSetCollection::Bottom], &AxisSets[axisSetCollection::Left] );
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
  std::set< const axisSet* > UsedAxisSets = Graphics.setOfGraphAxisSet();
  bool Used = UsedAxisSets.find(&AxisSet) != UsedAxisSets.end();
  return AxisSet.requiredIndent( Used );
}

// ------------------------------------------------------------

void scigraphics::plot::updateIndents()
{
  indents<wcoord> Indents;
  Indents.setLeft(  axisSetIndent(Pimpl->AxisSets[axisSetCollection::Left])   );
  Indents.setRight( axisSetIndent(Pimpl->AxisSets[axisSetCollection::Right])  );
  Indents.setUp(    axisSetIndent(Pimpl->AxisSets[axisSetCollection::Top])    );
  Indents.setDown(  axisSetIndent(Pimpl->AxisSets[axisSetCollection::Bottom]) );

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
  Pimpl->AxisSets.drawAxisTicks(Pimpl->Painter);
}

// ------------------------------------------------------------

void scigraphics::plot::drawAxisLabels()
{
  Pimpl->AxisSets.drawAxisLabels(Pimpl->Painter);
}

// ------------------------------------------------------------

void scigraphics::plot::drawAxisTitles()
{
  Pimpl->AxisSets.drawAxisTitles(Pimpl->Painter);
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
    if ( (*f)->containPoint( Point ) )
      return *f;
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

void scigraphics::plot::bindGraphToAxis( const graph *Graph, axisSetCollection::axisPosition AxisPosX, axisSetCollection::axisPosition AxisPosY )
{
  if ( Graph == NULL )
    throw std::invalid_argument("Zero graphic pointer is ibvalid");

  const axisSet *AxisX = &Pimpl->AxisSets[AxisPosX];
  const axisSet *AxisY = &Pimpl->AxisSets[AxisPosY];

  assert( AxisX != NULL && AxisY != NULL );
  if ( AxisX->getDirection() != axisSet::DirectionX || AxisY->getDirection() != axisSet::DirectionY )
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

scigraphics::graphCollection::iterator scigraphics::plot::eraseGraphic( graphCollection::iterator Iterator )
{
  return Graphics.erase(Iterator);
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
      
void scigraphics::plot::addScalesShift( double Shift, axisSet::direction Direction )
{
  Pimpl->AxisSets.addScalesShift(Shift,Direction);
}

// ------------------------------------------------------------
      
void scigraphics::plot::mulScalesZoom( double Zoom, axisSet::direction Direction )
{
  Pimpl->AxisSets.mulScalesZoom(Zoom,Direction);
}

// ------------------------------------------------------------
      
void scigraphics::plot::resetScales( axisSet::direction Direction )
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
      
const scigraphics::scale* scigraphics::plot::scaleWithPosition( axisSetCollection::axisPosition Position ) const
{
  return Pimpl->AxisSets[Position].getScale();
}

// ------------------------------------------------------------

scigraphics::scale* scigraphics::plot::scaleWithPosition( axisSetCollection::axisPosition Position ) 
{
  return Pimpl->AxisSets[Position].getScale();
}

// ------------------------------------------------------------

void scigraphics::plot::replaceScaleWithPosition( axisSetCollection::axisPosition Position, scale *Scale )
{
  Pimpl->AxisSets[Position].replaceScale(Scale);
}

// ------------------------------------------------------------

void scigraphics::plot::setScaleInterval( axisSetCollection::axisPosition Position, interval<number> Limits )
{
  const axisSet *Set = &Pimpl->AxisSets[Position];
  Pimpl->PlotLimits.setInterval( Set, Limits );
}

// ------------------------------------------------------------
      
scigraphics::interval<scigraphics::number> scigraphics::plot::scaleInterval( axisSetCollection::axisPosition Position ) const
{
  const axisSet *Set = &Pimpl->AxisSets[Position];
  return Pimpl->PlotLimits.getInterval(Set);
}

// ------------------------------------------------------------
      
scigraphics::interval<scigraphics::number> scigraphics::plot::visibleInterval( axisSetCollection::axisPosition Position ) const
{
  const axisSet *Set = &Pimpl->AxisSets[Position];
  return Set->getScale()->getVisivleInterval();
}

// ------------------------------------------------------------

void scigraphics::plot::setScaleLock( axisSetCollection::axisPosition Position, bool Lock )
{
  Pimpl->AxisSets[Position].getScale()->setLock(Lock);
}

// ------------------------------------------------------------

void scigraphics::plot::setAxisTitle( axisSetCollection::axisPosition Position, const std::string &Title )
{
  Pimpl->AxisSets[Position].setAxisTitle( Title );
}

// ------------------------------------------------------------
      
std::string scigraphics::plot::getAxisTitle( axisSetCollection::axisPosition Position ) const
{
  return Pimpl->AxisSets[Position].getAxisTitle();
}

// ------------------------------------------------------------
      
void scigraphics::plot::setBottomLeftAxisTitles( const std::string &TitleX, const std::string &TitleY )
{
  setAxisTitle( axisSetCollection::Bottom, TitleX );
  setAxisTitle( axisSetCollection::Left,   TitleY );
}

// ------------------------------------------------------------

void scigraphics::plot::setAxisNumberStyle( axisSetCollection::axisPosition Position, numberStyle *Style )
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

