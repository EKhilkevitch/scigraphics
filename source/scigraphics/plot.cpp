
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
#include "scigraphics/scale.h"

#include <stdexcept>
#include <iostream>

#if _WIN32
#  pragma warning( disable : 4355 )
#endif

// ============================================================

scigraphics::plot::plot() : 
  MouseHandler(*this), 
  CallBackContainer(*this),
  GraphicsColorSequence( colorSequence::defaultColorSequence() )
{
  prepareAxisSets();
  prepareFloatRectangles();
}

// ------------------------------------------------------------

scigraphics::plot::~plot()
{
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

scigraphics::color scigraphics::plot::selectNextGraphColor( const color &Color )
{
  if ( Color == color::White )
    return nextGraphColor();
  return Color;
}

// ------------------------------------------------------------

void scigraphics::plot::prepareAxisSets()
{
  Graphics.setDefaultAxisSets( &AxisSets[axisSetCollection::Bottom], &AxisSets[axisSetCollection::Left] );
  Selections.setDefaultAxisSets( &AxisSets[axisSetCollection::Bottom], &AxisSets[axisSetCollection::Left] );
}

// ------------------------------------------------------------

void scigraphics::plot::prepareFloatRectangles()
{
  FloatRectangles.push_back( &Legend );
  FloatRectangles.push_back( &CursorPositionViewer );
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
  Painter.update();
}

// ------------------------------------------------------------

void scigraphics::plot::updateScaleLimits()
{
  plotLimits::limitsXY Limits = PlotLimits.limitsForGraphics( Graphics );
  AxisSets.setAxisNumberLimits( &Limits );
  AxisSets.setScalesTo1x1ifNeeded( Painter );
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
  Indents.setLeft(  axisSetIndent(AxisSets[axisSetCollection::Left])   );
  Indents.setRight( axisSetIndent(AxisSets[axisSetCollection::Right])  );
  Indents.setUp(    axisSetIndent(AxisSets[axisSetCollection::Top])    );
  Indents.setDown(  axisSetIndent(AxisSets[axisSetCollection::Bottom]) );

  Painter.setIndents( Indents );
}

// ------------------------------------------------------------
      
void scigraphics::plot::clearPlotArea()
{
  Painter.clearPlotArea();
}

// ------------------------------------------------------------

void scigraphics::plot::clearBorders()
{
  Painter.clearBordersArea();
}

// ------------------------------------------------------------

void scigraphics::plot::drawGrid()
{
  AxisSets.drawGrid(Painter);
}

// ------------------------------------------------------------

void scigraphics::plot::drawAxis()
{
  AxisSets.drawAxis(Painter);
}

// ------------------------------------------------------------

void scigraphics::plot::drawFloatRectangles()
{
  Legend.draw( Painter, Graphics );
  CursorPositionViewer.draw( Painter, getBottomLeftPairScales(), MouseHandler.lastPosition() );
}

// ------------------------------------------------------------

void scigraphics::plot::drawAxisTicks()
{
  AxisSets.drawAxisTicks(Painter);
}

// ------------------------------------------------------------

void scigraphics::plot::drawAxisLabels()
{
  AxisSets.drawAxisLabels(Painter);
}

// ------------------------------------------------------------

void scigraphics::plot::drawAxisTitles()
{
  AxisSets.drawAxisTitles(Painter);
}

// ------------------------------------------------------------

void scigraphics::plot::drawGraphicsOverGrid()
{
  Graphics.draw( Painter, true );
}

// ------------------------------------------------------------

void scigraphics::plot::drawGraphicsUnderGrid()
{
  Graphics.draw( Painter, false );
}

// ------------------------------------------------------------

void scigraphics::plot::drawSelections()
{
  Selections.draw( Painter, true );
}

// ------------------------------------------------------------

void scigraphics::plot::drawZoomRectangle()
{
  ZoomRectangle.draw( Painter );
}

// ------------------------------------------------------------

scigraphics::floatRectangle* scigraphics::plot::getFloatRectangle( wpoint Point )
{
  for ( std::list< floatRectangle* >::reverse_iterator f = FloatRectangles.rbegin(); f != FloatRectangles.rend(); ++f )
    if ( (*f)->containPoint( Point ) )
      return *f;
  return NULL;
}

// ------------------------------------------------------------
      
scigraphics::zoomRectangle&  scigraphics::plot::getZoomRectangle() 
{ 
  return ZoomRectangle; 
}
      
// ------------------------------------------------------------

scigraphics::selection* scigraphics::plot::getSelection( wpoint Point )
{
  fpoint FPoint = Painter.wpoint2fpoint(Point);
  return Selections.getSelectionOnPoint( FPoint );
}
      
// ------------------------------------------------------------

void scigraphics::plot::setSelectionInterval( selectionStrip *Selection, wpoint Pt1, wpoint Pt2 )
{
  fpoint FPoint1 = Painter.wpoint2fpoint(Pt1);
  fpoint FPoint2 = Painter.wpoint2fpoint(Pt2);
  Selections.setSelectionInterval( Selection, FPoint1, FPoint2 );
}

// ------------------------------------------------------------
      
void scigraphics::plot::shiftSelection( selectionStrip *Selection, wpoint From, wpoint To )
{
  fpoint FFrom = Painter.wpoint2fpoint(From);
  fpoint FTo   = Painter.wpoint2fpoint(To);
  Selections.shiftSelection( Selection, FFrom, FTo );
}

// ------------------------------------------------------------

scigraphics::pairScales scigraphics::plot::getBottomLeftPairScales() 
{
  return AxisSets.getBottomLeftPairScales();
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

  const axisSet *AxisX = &AxisSets[AxisPosX];
  const axisSet *AxisY = &AxisSets[AxisPosY];

  assert( AxisX != NULL && AxisY != NULL );
  if ( AxisX->getDirection() != axisSet::DirectionX || AxisY->getDirection() != axisSet::DirectionY )
    throw std::invalid_argument("Invalid axis X or Y position");

  Graphics.bindGraphToAxisSet( Graph, AxisX, AxisY );
}

// ------------------------------------------------------------

void scigraphics::plot::clearGraphics() 
{ 
  Graphics.clear(); 
  GraphicsColorSequence.reset(); 
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
  AxisSets.addScalesShift(Shift,Direction);
}

// ------------------------------------------------------------
      
void scigraphics::plot::mulScalesZoom( double Zoom, axisSet::direction Direction )
{
  AxisSets.mulScalesZoom(Zoom,Direction);
}

// ------------------------------------------------------------
      
void scigraphics::plot::resetScales( axisSet::direction Direction )
{
  AxisSets.resetScales(Direction);
}
      
// ------------------------------------------------------------

void scigraphics::plot::resetAllScales()
{
  AxisSets.resetAllScales();
}

// ------------------------------------------------------------

void scigraphics::plot::setScalesTo1x1( bool SetTo1x1 )
{
  AxisSets.setFixedScalesTo1x1( SetTo1x1 );
}

// ------------------------------------------------------------
      
const scigraphics::scale* scigraphics::plot::scaleWithPosition( axisSetCollection::axisPosition Position ) const
{
  return AxisSets[Position].getScale();
}

// ------------------------------------------------------------

scigraphics::scale* scigraphics::plot::scaleWithPosition( axisSetCollection::axisPosition Position ) 
{
  return AxisSets[Position].getScale();
}

// ------------------------------------------------------------

void scigraphics::plot::replaceScaleWithPosition( axisSetCollection::axisPosition Position, scale *Scale )
{
  AxisSets[Position].replaceScale(Scale);
}

// ------------------------------------------------------------

void scigraphics::plot::setScaleInterval( axisSetCollection::axisPosition Position, interval<number> Limits )
{
  const axisSet *Set = &AxisSets[Position];
  PlotLimits.setInterval( Set, Limits );
}

// ------------------------------------------------------------
      
scigraphics::interval<scigraphics::number> scigraphics::plot::scaleInterval( axisSetCollection::axisPosition Position ) const
{
  const axisSet *Set = &AxisSets[Position];
  return PlotLimits.getInterval(Set);
}

// ------------------------------------------------------------
      
scigraphics::interval<scigraphics::number> scigraphics::plot::visibleInterval( axisSetCollection::axisPosition Position ) const
{
  const axisSet *Set = &AxisSets[Position];
  return Set->getScale()->getVisivleInterval();
}

// ------------------------------------------------------------

void scigraphics::plot::setScaleLock( axisSetCollection::axisPosition Position, bool Lock )
{
  AxisSets[Position].getScale()->setLock(Lock);
}

// ------------------------------------------------------------

void scigraphics::plot::setAxisTitle( axisSetCollection::axisPosition Position, const std::string &Title )
{
  AxisSets[Position].setAxisTitle( Title );
}

// ------------------------------------------------------------
      
std::string scigraphics::plot::getAxisTitle( axisSetCollection::axisPosition Position ) const
{
  return AxisSets[Position].getAxisTitle();
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
  AxisSets[Position].setNumberStyle(Style);
}

// ------------------------------------------------------------
      
void scigraphics::plot::setStretchFactors( double SX, double SY )
{
  PlotLimits.setStretchFactorX(SX);
  PlotLimits.setStretchFactorY(SY);
}

// ============================================================

