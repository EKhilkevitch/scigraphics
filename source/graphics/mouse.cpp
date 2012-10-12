
// ============================================================

#include "graphics/mouse.h"
#include "graphics/plot.h"

#include <cassert>

// ============================================================

graphics::fpoint graphics::mouse::mouseHandler::wpoint2fpoint( wpoint Point ) const
{
  return Plot.getPainter().wpoint2fpoint(Point);
}

// ------------------------------------------------------------

double graphics::mouse::mouseHandler::plotWidth()  const
{
  return Plot.getPainter().width();
}

// ------------------------------------------------------------

double graphics::mouse::mouseHandler::plotHeight() const
{
  return Plot.getPainter().height();
}

// ------------------------------------------------------------

void graphics::mouse::mouseHandler::mulZoomX( double Z )
{
  Plot.mulScalesZoom( Z, axisSet::DirectionX );
}

// ------------------------------------------------------------

void graphics::mouse::mouseHandler::mulZoomY( double Z )
{
  Plot.mulScalesZoom( Z, axisSet::DirectionY );
}

// ------------------------------------------------------------

void graphics::mouse::mouseHandler::addShiftX( double S )
{
  Plot.addScalesShift( S, axisSet::DirectionX );
}

// ------------------------------------------------------------

void graphics::mouse::mouseHandler::addShiftY( double S )
{
  Plot.addScalesShift( S, axisSet::DirectionY );
}

// ------------------------------------------------------------
          
void graphics::mouse::mouseHandler::resetScales()
{
  Plot.resetScales( axisSet::DirectionX );
  Plot.resetScales( axisSet::DirectionY );
}

// ============================================================

void graphics::mouse::moveAction::moved( wpoint Point )
{
  double DeltaX = Point.x() - lastPoint().x();
  double DeltaY = Point.y() - lastPoint().y();

  addShiftX( - DeltaX/plotWidth() );
  addShiftY( + DeltaY/plotHeight() );

  setLastPositions(Point); 
}

// ============================================================

graphics::mouse::zoomAction::zoomAction( plot &P, wpoint Point )
  : mouseActionHandler(P,Point)
{
  Plot.getZoomRectangle().show( Point );
}

// ------------------------------------------------------------

void graphics::mouse::zoomAction::moved( wpoint Point )
{
  Plot.getZoomRectangle().pull( Point );
}

// ------------------------------------------------------------

void graphics::mouse::zoomAction::released( wpoint Point )
{
  if ( needToApplyChanges(Point) )
  {
    applyShifts(Point);
    applyZooms(Point);
  }

  Plot.getZoomRectangle().hide();
}
          
// ------------------------------------------------------------

bool graphics::mouse::zoomAction::needToApplyChanges( wpoint Point )
{
  double DeltaX = std::fabs((double)(Point.x()-initPoint().x()));
  double DeltaY = std::fabs((double)(Point.y()-initPoint().y()));
  const double MinDelta = 5;

  return ( DeltaX > MinDelta ) && 
         ( DeltaY > MinDelta );
}

// ------------------------------------------------------------

void graphics::mouse::zoomAction::applyShifts( wpoint Point )
{
  wpoint XPoint( std::min(Point.x(),initPoint().x()), 
                 std::max(Point.y(),initPoint().y()) );

  double RelDeltaX =     ( XPoint.x() - Plot.getPainter().getIndents().left() )/plotWidth();
  double RelDeltaY = 1 - ( XPoint.y() - Plot.getPainter().getIndents().up()   )/plotHeight();

  addShiftX( RelDeltaX );
  addShiftY( RelDeltaY );
}

// ------------------------------------------------------------

void graphics::mouse::zoomAction::applyZooms( wpoint Point )
{
  double DeltaX = std::fabs((double)(Point.x()-initPoint().x()));
  double DeltaY = std::fabs((double)(Point.y()-initPoint().y()));
 
  mulZoomX( DeltaX/plotWidth() );
  mulZoomY( DeltaY/plotHeight() );
}

// ============================================================

void graphics::mouse::resetAction::reset()
{
  resetScales();
}

// ------------------------------------------------------------

void graphics::mouse::resetAction::released( wpoint )
{
  reset();
}

// ============================================================

graphics::mouse::moveFloatAction::moveFloatAction( plot &P, wpoint Point )
  : mouseActionHandler(P,Point)
{
  Float = Plot.getFloatRectangle( Point );
}

// ------------------------------------------------------------

void graphics::mouse::moveFloatAction::moved( wpoint Point )
{
  if ( Float != NULL )
  {
    wcoord DeltaX = Point.x() - lastPoint().x();
    wcoord DeltaY = Point.y() - lastPoint().y();
    setLastPositions( Point );
    
    Float->move( DeltaX, DeltaY );
  }
}

// ============================================================

graphics::mouse::selectAction::selectAction( plot &P, wpoint Point ) 
  : mouseActionHandler(P,Point),
  Selection(NULL)
{
}

// ------------------------------------------------------------

void graphics::mouse::selectAction::initSelection()
{
  Plot.clearSelections();
  Selection = createSelection();
}

// ------------------------------------------------------------

void graphics::mouse::selectAction::moved( wpoint Point )
{
  if ( Selection != NULL )
    setSelectionInterval( Point );
}

// ------------------------------------------------------------

void graphics::mouse::selectAction::setSelectionInterval( wpoint Point )
{
  assert( Selection != NULL );
  Plot.setSelectionInterval( Selection, Point, initPoint() );
}
          
// ------------------------------------------------------------

void graphics::mouse::selectAction::released( wpoint )
{
  if ( Selection == NULL )
    return;
  number Min = Selection->min();
  number Max = Selection->max();
  if ( ! isValidNumbers( Min, Max ) )
  {
    Plot.deleteSelection(Selection);
    Selection = NULL;
  }
}

// ============================================================

graphics::selectionStrip* graphics::mouse::selectHorizontalAction::createSelection()
{
  return Plot.createSelection<selectionHorizontal>();
}

// ============================================================

graphics::selectionStrip* graphics::mouse::selectVerticalAction::createSelection()
{
  return Plot.createSelection<selectionVertical>();
}

// ============================================================

graphics::mouse::moveSelectionAction::moveSelectionAction( plot &P, wpoint Point )
  : mouseActionHandler( P, Point ),
  Selection( getSelection(Point) )
{
}

// ------------------------------------------------------------

graphics::selectionStrip* graphics::mouse::moveSelectionAction::getSelection( wpoint Point )
{
  selection *S = Plot.getSelection(Point);
  return dynamic_cast<selectionStrip*>( S );
}

// ------------------------------------------------------------

void graphics::mouse::moveSelectionAction::moved( wpoint Point )
{
  if ( Selection != NULL )
    changeSelectionInterval( Point );
  setLastPositions( Point );
}

// ------------------------------------------------------------

void graphics::mouse::moveSelectionAction::changeSelectionInterval( wpoint Point )
{
  assert( Selection != NULL );
  Plot.shiftSelection( Selection, lastPoint(), Point );
}

// ============================================================

void graphics::mouse::resetSelectionAction::released( wpoint )
{
  Plot.clearSelections();
}

// ============================================================

void graphics::mouse::mouseHorizontalWheel::wheel( wpoint, wheeldelta Delta )
{
  addShiftX( - (double)( Delta * deltaDumpFactor() )/plotWidth() );
}

// ============================================================

void graphics::mouse::mouseVerticalWheel::wheel( wpoint, wheeldelta Delta )
{
  addShiftY( + (double)( Delta * deltaDumpFactor() )/plotHeight() );
}

// ============================================================

void graphics::mouse::mouseZoomWheel::wheel( wpoint Point, wheeldelta Delta )
{
  fpoint FPoint = Plot.getPainter().wpoint2fpoint( Point );
  
  double Zoom = Delta * deltaDumpFactor();
  
  addShiftX( - FPoint.x() * Zoom );
  addShiftY( - FPoint.y() * Zoom );
  
  mulZoomX( 1 + Zoom );
  mulZoomY( 1 + Zoom );
}

// ============================================================

graphics::mouse::mouse( plot &P ) : 
  Plot(P),
  ReplotOnMouseActions( true ),
  ActionHandler( new noneAction(P) ),
  WheelHandler( new mouseNoneWheel(P) ),
  LastPosition(0,0)
{
}

// ------------------------------------------------------------

graphics::mouse::~mouse()
{
  delete ActionHandler;
  delete WheelHandler;
}

// ------------------------------------------------------------

void graphics::mouse::replaceActionHandler( mouseActionHandler *NewHandler )
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

void graphics::mouse::replaceActionHandler( wpoint Point, unsigned Buttons )
{
  mouseActionHandler *Handler = createMouseActionHandler(Point,Buttons);
  replaceActionHandler( Handler );
}

// ------------------------------------------------------------

void graphics::mouse::setNoneActionHandler()
{
  replaceActionHandler( NULL );
  assert( isNoneHandler() );
}

// ------------------------------------------------------------

void graphics::mouse::replaceWheelHandler( mouseWheelHandler *NewHandler )
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

void graphics::mouse::replaceWheelHandler( unsigned Buttons )
{
  mouseWheelHandler *Handler = createMouseWheelHandler(Buttons);
  replaceWheelHandler( Handler );
}

// ------------------------------------------------------------

void graphics::mouse::setNoneWheelHandler()
{
  replaceWheelHandler( (mouseWheelHandler*)(0) );
}

// ------------------------------------------------------------

bool graphics::mouse::isNoneHandler() const
{
  if ( ActionHandler == NULL )
    return true;
  return typeid(*ActionHandler) == typeid(noneAction);
}

// ------------------------------------------------------------
      
void graphics::mouse::mousePressed( wpoint Point, unsigned Buttons )
{ 
  updateLastPosition(Point);
  replaceActionHandler(Point,Buttons); 
  if ( ! isNoneHandler() )
    Plot.getMouseCallBack().onPressed( ActionHandler );
  if ( ! isNoneHandler() )
    replot();
}

// ------------------------------------------------------------

void graphics::mouse::mouseMoved( wpoint Point )
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
      
void graphics::mouse::mouseReleased( wpoint Point )
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

void graphics::mouse::mouseDoubleClicked( wpoint Point ) 
{ 
  mousePressed(Point,Middle); 
  replot();
}

// ------------------------------------------------------------

void graphics::mouse::mouseWheel( wpoint Point, wheeldelta Delta, unsigned Buttons )
{
  replaceWheelHandler( Buttons );
  assert( WheelHandler != NULL );
  WheelHandler->wheel(Point,Delta);  
  Plot.getMouseCallBack().onWheel( WheelHandler );
  replot();
}

// ------------------------------------------------------------

void graphics::mouse::updateLastPosition( wpoint Point )
{
  LastPosition = Point;
  Plot.replotFloatRectangles();
}

// ------------------------------------------------------------

void graphics::mouse::replot()
{
  if ( replotOnMouseActions() )
    Plot.replot();
}

// ------------------------------------------------------------

graphics::mouse::mouseActionHandler* graphics::mouse::createMouseActionHandler( wpoint Point, unsigned Buttons )
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

graphics::mouse::mouseWheelHandler* graphics::mouse::createMouseWheelHandler( unsigned Buttons )
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

