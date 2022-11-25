
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

#include "scigraphics/mouse.h"
#include "scigraphics/plot.h"
#include "scigraphics/painter.h"
#include "scigraphics/floatrect.h"
#include "scigraphics/zoomrect.h"
#include "scigraphics/mousecallback.h"
#include "scigraphics/selection.h"
#include "scigraphics/graphsequence.h"
#include "scigraphics/scale.h"

#include <typeinfo>
#include <algorithm>
#include <cmath>
#include <cassert>

// ============================================================
          
scigraphics::mouse::allowing::allowing( unsigned A ) : 
  AllowedOperations(A) 
{
}
          
// ------------------------------------------------------------
          
unsigned scigraphics::mouse::allowing::value() const 
{ 
  return AllowedOperations; 
}

// ------------------------------------------------------------
          
void scigraphics::mouse::allowing::set( unsigned Op )               
{ 
  AllowedOperations = Op;     
}

// ------------------------------------------------------------
          
void scigraphics::mouse::allowing::allowOperation( unsigned Op )    
{ 
  AllowedOperations |= Op;    
}

// ------------------------------------------------------------
          
void scigraphics::mouse::allowing::disallowOperation( unsigned Op ) 
{ 
  AllowedOperations &= (~Op); 
}  

// ------------------------------------------------------------
          
bool scigraphics::mouse::allowing::isAllowed( unsigned Op ) const 
{ 
  return ( Op & AllowedOperations ) != 0; 
} 

// ============================================================
          
scigraphics::mouse::mouseHandler::mouseHandler( plot &P ) : 
  Plot(P) 
{
}

// ------------------------------------------------------------
          
scigraphics::mouse::mouseHandler::~mouseHandler() 
{
}

// ------------------------------------------------------------

scigraphics::fpoint scigraphics::mouse::mouseHandler::wpoint2fpoint( wpoint Point ) const
{
  return Plot.getPainter().wpoint2fpoint(Point);
}

// ------------------------------------------------------------

double scigraphics::mouse::mouseHandler::plotWidth()  const
{
  return Plot.getPainter().width();
}

// ------------------------------------------------------------

double scigraphics::mouse::mouseHandler::plotHeight() const
{
  return Plot.getPainter().height();
}

// ------------------------------------------------------------

void scigraphics::mouse::mouseHandler::mulZoomX( double Z )
{
  Plot.mulScaleZoom( Z, AxisDirectionX );
}

// ------------------------------------------------------------

void scigraphics::mouse::mouseHandler::mulZoomY( double Z )
{
  Plot.mulScaleZoom( Z, AxisDirectionY );
}

// ------------------------------------------------------------

void scigraphics::mouse::mouseHandler::addShiftX( double S )
{
  Plot.addScaleShift( S, AxisDirectionX );
}

// ------------------------------------------------------------

void scigraphics::mouse::mouseHandler::addShiftY( double S )
{
  Plot.addScaleShift( S, AxisDirectionY );
}

// ------------------------------------------------------------
          
void scigraphics::mouse::mouseHandler::resetScales()
{
  Plot.resetScales( AxisDirectionX );
  Plot.resetScales( AxisDirectionY );
}

// ------------------------------------------------------------
          
unsigned scigraphics::mouse::mouseHandler::requestedAllowing() const 
{ 
  return allowing::Everything; 
}

// ------------------------------------------------------------

bool scigraphics::mouse::mouseHandler::isAllowed( const allowing &Allowing ) const 
{ 
  return Allowing.isAllowed( requestedAllowing() ); 
}

// ============================================================

scigraphics::mouse::mouseActionHandler::mouseActionHandler( plot &P, wpoint Point ) : 
  mouseHandler(P), 
  InitPoint(Point), 
  LastPoint(Point) 
{
}

// ------------------------------------------------------------
          
scigraphics::mouse::mouseActionHandler::~mouseActionHandler() 
{
}

// ------------------------------------------------------------

void scigraphics::mouse::mouseActionHandler::moved( wpoint ) 
{
}

// ------------------------------------------------------------

void scigraphics::mouse::mouseActionHandler::released( wpoint ) 
{
}

// ------------------------------------------------------------

void scigraphics::mouse::mouseActionHandler::setLastPositions( wpoint Pt ) 
{ 
  LastPoint = Pt; 
}

// ------------------------------------------------------------

scigraphics::wpoint scigraphics::mouse::mouseActionHandler::initPoint() const 
{ 
  return InitPoint; 
}

// ------------------------------------------------------------

scigraphics::wpoint scigraphics::mouse::mouseActionHandler::lastPoint() const 
{ 
  return LastPoint; 
}

// ============================================================

scigraphics::mouse::noneAction::noneAction( plot &Plot, wpoint Point ) : 
  mouseActionHandler( Plot, Point )
{
}

// ============================================================

scigraphics::mouse::moveAction::moveAction( plot &Plot, wpoint Point ) : 
  mouseActionHandler(Plot,Point) 
{
}

// ------------------------------------------------------------

void scigraphics::mouse::moveAction::moved( wpoint Point )
{
  const wcoord DeltaX = Point.x() - lastPoint().x();
  const wcoord DeltaY = Point.y() - lastPoint().y();

  addShiftX( - static_cast<double>(DeltaX)/plotWidth() );
  addShiftY( + static_cast<double>(DeltaY)/plotHeight() );

  setLastPositions(Point); 
}

// ============================================================

scigraphics::mouse::zoomAction::zoomAction( plot &P, wpoint Point )
  : mouseActionHandler(P,Point)
{
  Plot.getZoomRectangle().show( Point );
}

// ------------------------------------------------------------

void scigraphics::mouse::zoomAction::moved( wpoint Point )
{
  Plot.getZoomRectangle().pull( Point );
}

// ------------------------------------------------------------

void scigraphics::mouse::zoomAction::released( wpoint Point )
{
  if ( needToApplyChanges(Point) )
  {
    applyShifts(Point);
    applyZooms(Point);
  }

  Plot.getZoomRectangle().hide();
}
          
// ------------------------------------------------------------

bool scigraphics::mouse::zoomAction::needToApplyChanges( wpoint Point )
{
  const wcoord DeltaX = std::abs( Point.x() - initPoint().x() );
  const wcoord DeltaY = std::abs( Point.y() - initPoint().y() );
  const wcoord MinDelta = 5;

  return ( DeltaX > MinDelta ) && 
         ( DeltaY > MinDelta );
}

// ------------------------------------------------------------

void scigraphics::mouse::zoomAction::applyShifts( wpoint Point )
{
  const wpoint XPoint( std::min(Point.x(),initPoint().x()), 
                       std::max(Point.y(),initPoint().y()) );

  const double RelDeltaX =     static_cast<double>( XPoint.x() - Plot.getPainter().getIndents().left() )/plotWidth();
  const double RelDeltaY = 1 - static_cast<double>( XPoint.y() - Plot.getPainter().getIndents().up()   )/plotHeight();

  addShiftX( RelDeltaX );
  addShiftY( RelDeltaY );
}

// ------------------------------------------------------------

void scigraphics::mouse::zoomAction::applyZooms( wpoint Point )
{
  const wcoord DeltaX = std::abs( Point.x() - initPoint().x() );
  const wcoord DeltaY = std::abs( Point.y() - initPoint().y() );
 
  mulZoomX( static_cast<double>(DeltaX)/plotWidth() );
  mulZoomY( static_cast<double>(DeltaY)/plotHeight() );
}

// ============================================================
          
scigraphics::mouse::resetAction::resetAction( plot &Plot, wpoint Point ) : 
  mouseActionHandler(Plot,Point) 
{
}

// ------------------------------------------------------------

void scigraphics::mouse::resetAction::reset()
{
  resetScales();
}

// ------------------------------------------------------------

void scigraphics::mouse::resetAction::released( wpoint )
{
  reset();
}

// ============================================================

scigraphics::mouse::moveFloatAction::moveFloatAction( plot &P, wpoint Point ) : 
  mouseActionHandler(P,Point)
{
  Float = Plot.getFloatRectangle( Point );
}

// ------------------------------------------------------------

void scigraphics::mouse::moveFloatAction::moved( wpoint Point )
{
  if ( Float != NULL )
  {
    const wcoord DeltaX = Point.x() - lastPoint().x();
    const wcoord DeltaY = Point.y() - lastPoint().y();
    setLastPositions( Point );
    
    Float->move( DeltaX, DeltaY );
  }
}

// ============================================================

scigraphics::mouse::selectAction::selectAction( plot &P, wpoint Point ) : 
  mouseActionHandler(P,Point),
  Selection(NULL)
{
}

// ------------------------------------------------------------

void scigraphics::mouse::selectAction::initSelection()
{
  if ( ! Plot.enabledMultipleMouseSelections() )
    Plot.clearSelections();
  Selection = createSelection();
}

// ------------------------------------------------------------

void scigraphics::mouse::selectAction::moved( wpoint Point )
{
  if ( Selection != NULL )
    setSelectionInterval( Point );
}

// ------------------------------------------------------------

void scigraphics::mouse::selectAction::setSelectionInterval( wpoint Point )
{
  assert( Selection != NULL );
  Plot.setSelectionInterval( Selection, Point, initPoint() );
}
          
// ------------------------------------------------------------

void scigraphics::mouse::selectAction::released( wpoint )
{
  if ( Selection == NULL )
    return;

  const number Min = Selection->min();
  const number Max = Selection->max();
  if ( ! isValidNumbers( Min, Max ) )
  {
    Plot.deleteSelection(Selection);
    Selection = NULL;
  }
}

// ============================================================
          
scigraphics::mouse::selectHorizontalAction::selectHorizontalAction( plot &Plot, wpoint Point ) : 
  selectAction(Plot,Point) 
{ 
  initSelection(); 
} 

// ------------------------------------------------------------

scigraphics::selectionStrip* scigraphics::mouse::selectHorizontalAction::createSelection()
{
  return Plot.createSelection<selectionHorizontal>();
}

// ============================================================
          
scigraphics::mouse::selectVerticalAction::selectVerticalAction( plot &Plot, wpoint Point ) : 
  selectAction(Plot,Point) 
{ 
  initSelection(); 
} 

// ------------------------------------------------------------

scigraphics::selectionStrip* scigraphics::mouse::selectVerticalAction::createSelection()
{
  return Plot.createSelection<selectionVertical>();
}

// ============================================================

scigraphics::mouse::moveSelectionAction::moveSelectionAction( plot &Plot, wpoint Point ) : 
  mouseActionHandler( Plot, Point ),
  Selection( getSelection(Point) )
{
}

// ------------------------------------------------------------

scigraphics::selectionStrip* scigraphics::mouse::moveSelectionAction::getSelection( wpoint Point )
{
  selection *SelectionUnderCursor = Plot.getSelection(Point);
  return dynamic_cast<selectionStrip*>( SelectionUnderCursor );
}

// ------------------------------------------------------------

void scigraphics::mouse::moveSelectionAction::moved( wpoint Point )
{
  if ( Selection != NULL )
    changeSelectionInterval( Point );
  setLastPositions( Point );
}

// ------------------------------------------------------------

void scigraphics::mouse::moveSelectionAction::changeSelectionInterval( wpoint Point )
{
  assert( Selection != NULL );
  Plot.shiftSelection( Selection, lastPoint(), Point );
}

// ============================================================
          
scigraphics::mouse::resetSelectionAction::resetSelectionAction( plot &Plot, wpoint Point ) : 
  mouseActionHandler(Plot,Point) 
{
}

// ------------------------------------------------------------

void scigraphics::mouse::resetSelectionAction::released( wpoint Point )
{
  if ( Plot.countOfSelections() == 1 )
  {
    Plot.clearSelections();
  } else {
    selection *SelectionUnderCursor = Plot.getSelection(Point);
    Plot.deleteSelection( SelectionUnderCursor );
  }
}

// ============================================================
          
double scigraphics::mouse::mouseWheelHandler::deltaDumpFactor() const 
{ 
  return 0.25; 
}

// ------------------------------------------------------------

scigraphics::mouse::mouseWheelHandler::mouseWheelHandler( plot &Plot ) : 
  mouseHandler(Plot) 
{
}

// ------------------------------------------------------------
          
void scigraphics::mouse::mouseWheelHandler::wheel( wpoint, wheeldelta ) 
{
}

// ============================================================

scigraphics::mouse::mouseNoneWheel::mouseNoneWheel( plot &Plot ) : 
  mouseWheelHandler(Plot) 
{
}

// ============================================================
          
scigraphics::mouse::horizontalWheel::horizontalWheel( plot &Plot ) : 
  mouseWheelHandler(Plot) 
{
}

// ------------------------------------------------------------

void scigraphics::mouse::horizontalWheel::wheel( wpoint, wheeldelta Delta )
{
  addShiftX( - static_cast<double>( Delta * deltaDumpFactor() )/plotWidth() );
}

// ============================================================
          
scigraphics::mouse::verticalWheel::verticalWheel( plot &Plot ) : 
  mouseWheelHandler(Plot) 
{
}

// ------------------------------------------------------------

void scigraphics::mouse::verticalWheel::wheel( wpoint, wheeldelta Delta )
{
  addShiftY( + static_cast<double>( Delta * deltaDumpFactor() )/plotHeight() );
}

// ============================================================
          
double scigraphics::mouse::zoomWheel::deltaDumpFactor() const 
{ 
  return 0.001; 
}

// ------------------------------------------------------------

scigraphics::mouse::zoomWheel::zoomWheel( plot &Plot ) : 
  mouseWheelHandler(Plot) 
{
}

// ------------------------------------------------------------

void scigraphics::mouse::zoomWheel::wheel( wpoint Point, wheeldelta Delta )
{
  const fpoint FPoint = Plot.getPainter().wpoint2fpoint( Point );
  
  const double Zoom = - Delta * deltaDumpFactor();
  
  addShiftX( - FPoint.x() * Zoom );
  addShiftY( - FPoint.y() * Zoom );
  
  mulZoomX( 1 + Zoom );
  mulZoomY( 1 + Zoom );
}

// ============================================================

scigraphics::mouse::mouseGraphHandler::mouseGraphHandler( plot &P, wpoint Point ) :
  mouseActionHandler( P, Point )
{
}

// ------------------------------------------------------------
          
scigraphics::wcoord scigraphics::mouse::mouseGraphHandler::pointRadius() const
{
  return 10;
}

// ------------------------------------------------------------
          
scigraphics::wcoord scigraphics::mouse::mouseGraphHandler::distance( wpoint Pt1, wpoint Pt2 )
{
  const wcoord DeltaX = Pt1.x() - Pt2.x();
  const wcoord DeltaY = Pt1.y() - Pt2.y();
  const double Distance = std::sqrt( static_cast<double>( DeltaX*DeltaX + DeltaY*DeltaY ) );
  return static_cast<wcoord>( Distance );
}

// ------------------------------------------------------------
          
scigraphics::sequence::graphCreatedByMouseVector* scigraphics::mouse::mouseGraphHandler::findGraph()
{
  for ( graphCollection::reverse_iterator it = Plot.rbeginGraph(); it != Plot.rendGraph(); ++it )
  {
    sequence::graphCreatedByMouseVector *Graph = dynamic_cast< sequence::graphCreatedByMouseVector* >( &*it );
    if ( Graph != NULL )
      return Graph;
  }

  return NULL;
}

// ------------------------------------------------------------
          
scigraphics::sequence::graphCreatedByMouseVector* scigraphics::mouse::mouseGraphHandler::findOrCreateGraph()
{
  sequence::graphCreatedByMouseVector *Graph = findGraph();

  if ( Graph == NULL )
  {
    Graph = new sequence::graphCreatedByMouseVector();
    Plot.appendGraphic( Graph );
    Plot.bindGraphToAxis( Graph, scigraphics::AxisBottom, scigraphics::AxisLeft );
    Graph->setVisiblePoints( true );
    Graph->setPointSize( 5 );
  }

  return Graph;
}

// ------------------------------------------------------------
          
scigraphics::npoint scigraphics::mouse::mouseGraphHandler::wpoint2npoint( wpoint WPoint ) const
{
  const scale *ScaleX = Plot.scaleWithPosition( AxisBottom );
  const scale *ScaleY = Plot.scaleWithPosition( AxisLeft );

  const fpoint FPoint = Plot.getPainter().wpoint2fpoint( WPoint );
  const number RealX = ScaleX->fractionToNumber( FPoint.x() );
  const number RealY = ScaleY->fractionToNumber( FPoint.y() );

  return npoint( RealX, RealY );
}

// ------------------------------------------------------------
          
scigraphics::wpoint scigraphics::mouse::mouseGraphHandler::npoint2wpoint( npoint NPoint ) const
{
  const scale *ScaleX = Plot.scaleWithPosition( AxisBottom );
  const scale *ScaleY = Plot.scaleWithPosition( AxisLeft );

  const fcoord FloatX = ScaleX->numberToFraction( NPoint.x() );
  const fcoord FloatY = ScaleY->numberToFraction( NPoint.y() );

  return Plot.getPainter().fpoint2wpoint( fpoint(FloatX,FloatY) );
}

// ============================================================

scigraphics::mouse::addPointGraphAction::addPointGraphAction( plot &P, wpoint Point ) : 
  mouseGraphHandler( P, Point )
{
}

// ------------------------------------------------------------

void scigraphics::mouse::addPointGraphAction::released( wpoint WPoint )
{
  sequence::graphCreatedByMouseVector *Graph = findOrCreateGraph();
  const npoint Point = wpoint2npoint( WPoint );
  Graph->append( Point.x(), Point.y() );
}

// ============================================================

scigraphics::mouse::movePointGraphAction::movePointGraphAction( plot &P, wpoint WPoint ) : 
  mouseGraphHandler( P, WPoint ),
  Index( std::numeric_limits<size_t>::max() )
{
  const sequence::graphCreatedByMouseVector *Graph = findGraph();
  if ( Graph == NULL )
    return;

  const npoint NPoint = wpoint2npoint( WPoint );
  
  const size_t IndexOfPoint = Graph->indexOfPoint( NPoint.x(), NPoint.y() );
  if ( IndexOfPoint >= Graph->size() )
    return;

  const wpoint GraphWPoint = npoint2wpoint( Graph->at(IndexOfPoint) ); 
  if ( distance( GraphWPoint, WPoint ) > pointRadius() )
    return;

  Index = IndexOfPoint;
}

// ------------------------------------------------------------
          
void scigraphics::mouse::movePointGraphAction::moved( wpoint WPoint )
{
  if ( Index == std::numeric_limits<size_t>::max() )
    return;
  
  sequence::graphCreatedByMouseVector *Graph = findGraph();
  if ( Graph == NULL )
    return;

  assert( Index < Graph->size() );

  const npoint NPoint = wpoint2npoint( WPoint );
  Graph->set( Index, NPoint.x(), NPoint.y() );
}

// ============================================================

scigraphics::mouse::delPointGraphAction::delPointGraphAction( plot &P, wpoint Point ) : 
  mouseGraphHandler( P, Point )
{
}

// ------------------------------------------------------------

void scigraphics::mouse::delPointGraphAction::released( wpoint WPoint )
{
  sequence::graphCreatedByMouseVector *Graph = findGraph();
  if ( Graph == NULL )
    return;

  const npoint NPoint = wpoint2npoint( WPoint );
  
  const size_t IndexOfPoint = Graph->indexOfPoint( NPoint.x(), NPoint.y() );
  if ( IndexOfPoint >= Graph->size() )
    return;

  const wpoint GraphWPoint = npoint2wpoint( Graph->at(IndexOfPoint) ); 
  if ( distance( GraphWPoint, WPoint ) > pointRadius() )
    return;

  Graph->erase( IndexOfPoint );
}

// ============================================================

scigraphics::mouse::mouse( plot &P ) : 
  Plot(P),
  ReplotOnMouseActions( true ),
  EnableMultipleUserSelections( false ),
  ActionHandler( new noneAction(P) ),
  WheelHandler( new mouseNoneWheel(P) ),
  LastPosition(0,0)
{
}

// ------------------------------------------------------------

scigraphics::mouse::~mouse()
{
  delete ActionHandler;
  delete WheelHandler;
}

// ------------------------------------------------------------

void scigraphics::mouse::replaceActionHandler( mouseActionHandler *NewHandler )
{
  if ( NewHandler != NULL && NewHandler->isAllowed(AllowedOperations) )
  {
    delete ActionHandler;
    ActionHandler = NewHandler;
  } else {
    delete NewHandler;
    delete ActionHandler;
    ActionHandler = new noneAction(Plot);
  }
}

// ------------------------------------------------------------

void scigraphics::mouse::replaceActionHandler( wpoint Point, unsigned Buttons )
{
  mouseActionHandler *Handler = createMouseActionHandler(Point,Buttons);
  replaceActionHandler( Handler );
}

// ------------------------------------------------------------

void scigraphics::mouse::setNoneActionHandler()
{
  replaceActionHandler( NULL );
  assert( isNoneHandler() );
}

// ------------------------------------------------------------

void scigraphics::mouse::replaceWheelHandler( mouseWheelHandler *NewHandler )
{
  if ( NewHandler != NULL && NewHandler->isAllowed(AllowedOperations) ) 
  {
    if ( typeid(*WheelHandler) != typeid(*NewHandler) )
    {
      delete WheelHandler;
      WheelHandler = NewHandler;
    } else {
      delete NewHandler;
    }
  } else {
    delete NewHandler;
    WheelHandler = new mouseNoneWheel(Plot);
  }
}

// ------------------------------------------------------------

void scigraphics::mouse::replaceWheelHandler( unsigned Buttons )
{
  mouseWheelHandler *Handler = createMouseWheelHandler(Buttons);
  replaceWheelHandler( Handler );
}

// ------------------------------------------------------------

void scigraphics::mouse::setNoneWheelHandler()
{
  replaceWheelHandler( static_cast<mouseWheelHandler*>(NULL) );
}

// ------------------------------------------------------------

bool scigraphics::mouse::isNoneHandler() const
{
  if ( ActionHandler == NULL )
    return true;
  return typeid(*ActionHandler) == typeid(noneAction);
}

// ------------------------------------------------------------
      
void scigraphics::mouse::mousePressed( wpoint Point, unsigned Buttons )
{ 
  updateLastPosition(Point);
  replaceActionHandler(Point,Buttons); 

  if ( ! isNoneHandler() )
    Plot.getMouseCallBack().onPressed( ActionHandler );

  if ( ! isNoneHandler() )
    replot();
}

// ------------------------------------------------------------

void scigraphics::mouse::mouseMoved( wpoint Point )
{
  updateLastPosition(Point);
  assert( ActionHandler != NULL );
  ActionHandler->moved(Point);

  if ( ! isNoneHandler() )
    Plot.getMouseCallBack().onMoved( ActionHandler );

  if ( ! isNoneHandler() )
    replot();
}

// ------------------------------------------------------------
      
void scigraphics::mouse::mouseReleased( wpoint Point )
{ 
  updateLastPosition(Point);
  assert( ActionHandler != NULL );
  ActionHandler->released(Point); 

  if ( ! isNoneHandler() )
    Plot.getMouseCallBack().onRelease( ActionHandler );

  if ( ! isNoneHandler() )
    replot();

  setNoneActionHandler(); 
}
      
// ------------------------------------------------------------

void scigraphics::mouse::mouseDoubleClicked( wpoint Point, unsigned Buttons ) 
{ 
  mousePressed( Point, Middle|Buttons ); 
  mouseReleased( Point );
  replot();
}

// ------------------------------------------------------------

void scigraphics::mouse::mouseWheel( wpoint Point, wheeldelta Delta, unsigned Buttons )
{
  replaceWheelHandler( Buttons );
  assert( WheelHandler != NULL );
  WheelHandler->wheel( Point, Delta );
  Plot.getMouseCallBack().onWheel( WheelHandler );
  replot();
}

// ------------------------------------------------------------

scigraphics::mouse::mouseActionHandler* scigraphics::mouse::actionHandler() 
{ 
  return ActionHandler; 
}

// ------------------------------------------------------------

void scigraphics::mouse::setAllowedOperations( unsigned Op )    
{ 
  AllowedOperations.allowOperation(Op); 
}

// ------------------------------------------------------------

void scigraphics::mouse::setDisallowedOperations( unsigned Op ) 
{ 
  AllowedOperations.disallowOperation(Op); 
}

// ------------------------------------------------------------

void scigraphics::mouse::setOperations( unsigned Op )           
{ 
  AllowedOperations.set(Op); 
}

// ------------------------------------------------------------

unsigned scigraphics::mouse::allowedOperations() const          
{ 
  return AllowedOperations.value(); 
}

// ------------------------------------------------------------

scigraphics::wpoint scigraphics::mouse::lastPosition() const 
{ 
  return LastPosition; 
}

// ------------------------------------------------------------

void scigraphics::mouse::setReplotOnMouseActions( bool R ) 
{ 
  ReplotOnMouseActions = R; 
}

// ------------------------------------------------------------

bool scigraphics::mouse::replotOnMouseActions() const 
{ 
  return ReplotOnMouseActions; 
}

// ------------------------------------------------------------
      
void scigraphics::mouse::setEnableMultipleMouseSelections( bool E ) 
{
  EnableMultipleUserSelections = E;
}

// ------------------------------------------------------------
 
bool scigraphics::mouse::enabledMultipleMouseSelections() const
{
  return EnableMultipleUserSelections;
}

// ------------------------------------------------------------

void scigraphics::mouse::updateLastPosition( wpoint Point )
{
  LastPosition = Point;
  Plot.replotFloatRectangles();
}

// ------------------------------------------------------------

void scigraphics::mouse::replot()
{
  if ( replotOnMouseActions() )
    Plot.replot();
}

// ------------------------------------------------------------

scigraphics::mouse::mouseActionHandler* scigraphics::mouse::createMouseActionHandler( wpoint Point, unsigned Buttons )
{
  switch ( Buttons )
  {
    case Right:
      if ( Plot.getFloatRectangle(Point) != NULL )
        return new moveFloatAction( Plot, Point );
      return new moveAction( Plot, Point );

    case Left:
      return new zoomAction( Plot, Point );

    case Middle:
      return new resetAction( Plot, Point );

    case (Left|Shift):
      return new selectHorizontalAction( Plot, Point );
    
    case (Left|Ctrl):
      return new selectVerticalAction( Plot, Point );

    case (Right|Ctrl):
      return new moveSelectionAction( Plot, Point );
    
    case (Middle|Ctrl):
      return new resetSelectionAction( Plot, Point );

    case (Left|Alt):
      return new addPointGraphAction( Plot, Point );
    
    case (Right|Alt):
      return new movePointGraphAction( Plot, Point );
    
    case (Middle|Alt):
      return new delPointGraphAction( Plot, Point );

    default:
      return new noneAction( Plot, Point );
  }
}

// ------------------------------------------------------------

scigraphics::mouse::mouseWheelHandler* scigraphics::mouse::createMouseWheelHandler( unsigned Buttons )
{
  switch ( Buttons )
  {
    case None:
      return new horizontalWheel( Plot );

    case Shift:
      return new verticalWheel( Plot );

    case Ctrl:
      return new zoomWheel( Plot );

    default:
      return NULL;
  }
}

// ============================================================

