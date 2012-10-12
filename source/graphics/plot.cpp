
// ============================================================

#include "graphics/plot.h"

#include <stdexcept>
#include <iostream>

#if _WIN32
#  pragma warning( disable : 4355 )
#endif

// ============================================================

graphics::plot::plot() : 
  MouseHandler(*this), 
  CallBackContainer(*this)
{
  prepareAxisSets();
  prepareFloatRectangles();
}

// ------------------------------------------------------------

graphics::plot::~plot()
{
}

// ------------------------------------------------------------

graphics::graphSequenceVector* graphics::plot::createGraphSequenceVector( const std::string &Legend, const color &Color )
{
  return createGraph<graphSequenceVector>( Legend, Color );
}

// ------------------------------------------------------------

void graphics::plot::prepareAxisSets()
{
  Graphics.setDefaultAxisSets( &AxisSets[axisSetCollection::Bottom], &AxisSets[axisSetCollection::Left] );
  Selections.setDefaultAxisSets( &AxisSets[axisSetCollection::Bottom], &AxisSets[axisSetCollection::Left] );
}

// ------------------------------------------------------------

void graphics::plot::prepareFloatRectangles()
{
  FloatRectangles.push_back( &Legend );
  FloatRectangles.push_back( &CursorPositionViewer );
}

// ------------------------------------------------------------

void graphics::plot::replot()
{
  if ( getDrawer() == NULL )
    throw std::runtime_error( "Drawer is not initialized" );

  prepareForPainting();

  clearPlotArea();
  drawGrid();
  drawGraphics();
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

void graphics::plot::prepareForPainting()
{
  updateScaleLimits();
  preparePainter();
  updateIndents();
}

// ------------------------------------------------------------

void graphics::plot::replotFloatRectangles()
{
  if ( getDrawer() == NULL )
    throw std::runtime_error( "Drawer is not initialized" );

  drawFloatRectangles();
  flush();
}

// ------------------------------------------------------------

void graphics::plot::preparePainter()
{
  Painter.update();
}

// ------------------------------------------------------------

void graphics::plot::updateScaleLimits()
{
  plotLimits::limitsXY Limits = PlotLimits.limitsForGraphics( Graphics );
  AxisSets.setAxisNumberLimits( &Limits );
}

// ------------------------------------------------------------

graphics::wcoord graphics::plot::axisSetIndent( const axisSet &AxisSet ) const
{
  std::set< const axisSet* > UsedAxisSets = Graphics.setOfGraphAxisSet();
  bool Used = UsedAxisSets.find(&AxisSet) != UsedAxisSets.end();
  return AxisSet.requiredIndent( Used );
}

// ------------------------------------------------------------

void graphics::plot::updateIndents()
{
  indents<wcoord> Indents;
  Indents.setLeft(  axisSetIndent(AxisSets[axisSetCollection::Left])   );
  Indents.setRight( axisSetIndent(AxisSets[axisSetCollection::Right])  );
  Indents.setUp(    axisSetIndent(AxisSets[axisSetCollection::Top])    );
  Indents.setDown(  axisSetIndent(AxisSets[axisSetCollection::Bottom]) );

  Painter.setIndents( Indents );
}

// ------------------------------------------------------------
      
void graphics::plot::clearPlotArea()
{
  Painter.clearPlotArea();
}

// ------------------------------------------------------------

void graphics::plot::clearBorders()
{
  Painter.clearBordersArea();
}

// ------------------------------------------------------------

void graphics::plot::drawGrid()
{
  AxisSets.drawGrid(Painter);
}

// ------------------------------------------------------------

void graphics::plot::drawAxis()
{
  AxisSets.drawAxis(Painter);
}

// ------------------------------------------------------------

void graphics::plot::drawFloatRectangles()
{
  Legend.draw( Painter, Graphics );
  CursorPositionViewer.draw( Painter, getBottomLeftPairScales(), MouseHandler.lastPosition() );
}

// ------------------------------------------------------------

void graphics::plot::drawAxisTicks()
{
  AxisSets.drawAxisTicks(Painter);
}

// ------------------------------------------------------------

void graphics::plot::drawAxisLabels()
{
  AxisSets.drawAxisLabels(Painter);
}

// ------------------------------------------------------------

void graphics::plot::drawAxisTitles()
{
  AxisSets.drawAxisTitles(Painter);
}

// ------------------------------------------------------------

void graphics::plot::drawGraphics()
{
  Graphics.draw( Painter );
}

// ------------------------------------------------------------

void graphics::plot::drawSelections()
{
  Selections.draw( Painter );
}

// ------------------------------------------------------------

void graphics::plot::drawZoomRectangle()
{
  ZoomRectangle.draw( Painter );
}

// ------------------------------------------------------------

graphics::floatRectangle* graphics::plot::getFloatRectangle( wpoint Point )
{
  for ( std::list< floatRectangle* >::reverse_iterator f = FloatRectangles.rbegin(); f != FloatRectangles.rend(); ++f )
    if ( (*f)->containPoint( Point ) )
      return *f;
  return NULL;
}
      
// ------------------------------------------------------------

graphics::selection* graphics::plot::getSelection( wpoint Point )
{
  fpoint FPoint = Painter.wpoint2fpoint(Point);
  return Selections.getSelectionOnPoint( FPoint );
}
      
// ------------------------------------------------------------

void graphics::plot::setSelectionInterval( selectionStrip *Selection, wpoint Pt1, wpoint Pt2 )
{
  fpoint FPoint1 = Painter.wpoint2fpoint(Pt1);
  fpoint FPoint2 = Painter.wpoint2fpoint(Pt2);
  Selections.setSelectionInterval( Selection, FPoint1, FPoint2 );
}

// ------------------------------------------------------------
      
void graphics::plot::shiftSelection( selectionStrip *Selection, wpoint From, wpoint To )
{
  fpoint FFrom = Painter.wpoint2fpoint(From);
  fpoint FTo   = Painter.wpoint2fpoint(To);
  Selections.shiftSelection( Selection, FFrom, FTo );
}

// ------------------------------------------------------------

graphics::pairScales graphics::plot::getBottomLeftPairScales() 
{
  return AxisSets.getBottomLeftPairScales();
}
      
// ------------------------------------------------------------

void graphics::plot::appendGraphic( graph *Graph )
{
  if ( Graph == NULL )
    throw std::invalid_argument("Zero graphic pointer is ibvalid");
  Graphics.append(Graph); 
}

// ------------------------------------------------------------

void graphics::plot::bindGraphToAxis( const graph *Graph, axisSetCollection::axisPosition AxisPosX, axisSetCollection::axisPosition AxisPosY )
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
      
void graphics::plot::addScalesShift( double Shift, axisSet::direction Direction )
{
  AxisSets.addScalesShift(Shift,Direction);
}

// ------------------------------------------------------------
      
void graphics::plot::mulScalesZoom( double Zoom, axisSet::direction Direction )
{
  AxisSets.mulScalesZoom(Zoom,Direction);
}

// ------------------------------------------------------------
      
void graphics::plot::resetScales( axisSet::direction Direction )
{
  AxisSets.resetScales(Direction);
}
      
// ------------------------------------------------------------

void graphics::plot::resetScalesTo1x1()
{
  if ( getDrawer() == NULL )
    throw std::runtime_error( "Drawer is not initialized" );
  
  prepareForPainting();
  AxisSets.resetScalesTo1x1(Painter);
}

// ------------------------------------------------------------
      
const graphics::scale* graphics::plot::scaleWithPosition( axisSetCollection::axisPosition Position ) const
{
  return AxisSets[Position].getScale();
}

// ------------------------------------------------------------

graphics::scale* graphics::plot::scaleWithPosition( axisSetCollection::axisPosition Position ) 
{
  return AxisSets[Position].getScale();
}

// ------------------------------------------------------------

void graphics::plot::replaceScaleWithPosition( axisSetCollection::axisPosition Position, scale *Scale )
{
  AxisSets[Position].replaceScale(Scale);
}

// ------------------------------------------------------------

void graphics::plot::setScaleInterval( axisSetCollection::axisPosition Position, interval<number> Limits )
{
  PlotLimits.setInterval( &AxisSets[Position], Limits );
}

// ------------------------------------------------------------

void graphics::plot::setScaleLock( axisSetCollection::axisPosition Position, bool Lock )
{
  AxisSets[Position].getScale()->setLock(Lock);
}

// ------------------------------------------------------------

void graphics::plot::setAxisTitle( axisSetCollection::axisPosition Position, const std::string &Title )
{
  AxisSets[Position].setAxisTitle( Title );
}

// ------------------------------------------------------------
      
std::string graphics::plot::getAxisTitle( axisSetCollection::axisPosition Position ) const
{
  return AxisSets[Position].getAxisTitle();
}

// ------------------------------------------------------------
      
void graphics::plot::setBottomLeftAxisTitles( const std::string &TitleX, const std::string &TitleY )
{
  setAxisTitle( axisSetCollection::Bottom, TitleX );
  setAxisTitle( axisSetCollection::Left,   TitleY );
}

// ------------------------------------------------------------
      
void graphics::plot::setStretchFactors( double SX, double SY )
{
  PlotLimits.setStretchFactorX(SX);
  PlotLimits.setStretchFactorY(SY);
}

// ============================================================

