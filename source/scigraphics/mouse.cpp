
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

#include <typeinfo>
#include <algorithm>
#include <cmath>
#include <cassert>

// ============================================================
          
scigraphics::mouse::allowing::allowing( unsigned A ) : 
  AllowedOperations(A) 
{
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
  Plot.mulScalesZoom( Z, AxisDirectionX );
}

// ------------------------------------------------------------

void scigraphics::mouse::mouseHandler::mulZoomY( double Z )
{
  Plot.mulScalesZoom( Z, AxisDirectionY );
}

// ------------------------------------------------------------

void scigraphics::mouse::mouseHandler::addShiftX( double S )
{
  Plot.addScalesShift( S, AxisDirectionX );
}

// ------------------------------------------------------------

void scigraphics::mouse::mouseHandler::addShiftY( double S )
{
  Plot.addScalesShift( S, AxisDirectionY );
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
  const double DeltaX = Point.x() - lastPoint().x();
  const double DeltaY = Point.y() - lastPoint().y();

  addShiftX( - DeltaX/plotWidth() );
  addShiftY( + DeltaY/plotHeight() );

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
  const double DeltaX = std::fabs(static_cast<double>(Point.x()-initPoint().x()));
  const double DeltaY = std::fabs(static_cast<double>(Point.y()-initPoint().y()));
  const double MinDelta = 5;

  return ( DeltaX > MinDelta ) && 
         ( DeltaY > MinDelta );
}

// ------------------------------------------------------------

void scigraphics::mouse::zoomAction::applyShifts( wpoint Point )
{
  wpoint XPoint( std::min(Point.x(),initPoint().x()), 
                 std::max(Point.y(),initPoint().y()) );

  const double RelDeltaX =     ( XPoint.x() - Plot.getPainter().getIndents().left() )/plotWidth();
  const double RelDeltaY = 1 - ( XPoint.y() - Plot.getPainter().getIndents().up()   )/plotHeight();

  addShiftX( RelDeltaX );
  addShiftY( RelDeltaY );
}

// ------------------------------------------------------------

void scigraphics::mouse::zoomAction::applyZooms( wpoint Point )
{
  const double DeltaX = std::fabs(static_cast<double>(Point.x()-initPoint().x()));
  const double DeltaY = std::fabs(static_cast<double>(Point.y()-initPoint().y()));
 
  mulZoomX( DeltaX/plotWidth() );
  mulZoomY( DeltaY/plotHeight() );
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

scigraphics::mouse::moveFloatAction::moveFloatAction( plot &P, wpoint Point )
  : mouseActionHandler(P,Point)
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

scigraphics::mouse::selectAction::selectAction( plot &P, wpoint Point ) 
  : mouseActionHandler(P,Point),
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
  selection *SelectionUnderCursor = Plot.getSelection(Point);
  Plot.deleteSelection( SelectionUnderCursor );
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

// ============================================================

scigraphics::mouse::mouseNoneWheel::mouseNoneWheel( plot &Plot ) : 
  mouseWheelHandler(Plot) 
{
}

// ============================================================
          
scigraphics::mouse::mouseHorizontalWheel::mouseHorizontalWheel( plot &Plot ) : 
  mouseWheelHandler(Plot) 
{
}

// ------------------------------------------------------------

void scigraphics::mouse::mouseHorizontalWheel::wheel( wpoint, wheeldelta Delta )
{
  addShiftX( - static_cast<double>( Delta * deltaDumpFactor() )/plotWidth() );
}

// ============================================================
          
scigraphics::mouse::mouseVerticalWheel::mouseVerticalWheel( plot &Plot ) : 
  mouseWheelHandler(Plot) 
{
}

// ------------------------------------------------------------

void scigraphics::mouse::mouseVerticalWheel::wheel( wpoint, wheeldelta Delta )
{
  addShiftY( + static_cast<double>( Delta * deltaDumpFactor() )/plotHeight() );
}

// ============================================================
          
double scigraphics::mouse::mouseZoomWheel::deltaDumpFactor() const 
{ 
  return 0.001; 
}

// ------------------------------------------------------------

scigraphics::mouse::mouseZoomWheel::mouseZoomWheel( plot &Plot ) : 
  mouseWheelHandler(Plot) 
{
}

// ------------------------------------------------------------

void scigraphics::mouse::mouseZoomWheel::wheel( wpoint Point, wheeldelta Delta )
{
  const fpoint FPoint = Plot.getPainter().wpoint2fpoint( Point );
  
  const double Zoom = - Delta * deltaDumpFactor();
  
  addShiftX( - FPoint.x() * Zoom );
  addShiftY( - FPoint.y() * Zoom );
  
  mulZoomX( 1 + Zoom );
  mulZoomY( 1 + Zoom );
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
  replaceWheelHandler( (mouseWheelHandler*)(0) );
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

void scigraphics::mouse::mouseDoubleClicked( wpoint Point ) 
{ 
  mousePressed( Point, Middle ); 
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
      return new mouseHorizontalWheel( Plot );

    case Shift:
      return new mouseVerticalWheel( Plot );

    case Ctrl:
      return new mouseZoomWheel( Plot );

    default:
      return NULL;
  }
}

// ============================================================

