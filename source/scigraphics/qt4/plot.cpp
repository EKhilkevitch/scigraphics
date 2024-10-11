
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


// ================================================================

#include "scigraphics/qt4/plot.h"
#include "scigraphics/qt4/drawer.h"
#include "scigraphics/qt4/mousecallback.h"
#include "scigraphics/qt4/plotview.h"
#include "scigraphics/qt4/settings.h"
#include "scigraphics/scale.h"
#include "scigraphics/linestyle.h"
#include "scigraphics/brushstyle.h"

#include <QMouseEvent>
#include <QBitmap>

// ================================================================

scigraphics::qt4plot::qt4plot( QWidget* Parent, Qt::WindowFlags Flags ) : 
  QWidget(Parent,Flags), 
  plot()
{
  init();
}

// ----------------------------------------------------------------

scigraphics::qt4plot::qt4plot( QWidget* Parent ) : 
  QWidget(Parent), 
  plot()
{
  init();
}

// ----------------------------------------------------------------
      
void scigraphics::qt4plot::init()
{
  setMouseCallBack( new qt4mouseCallBack(*this) );

  qt4drawerOnWidget *Drawer = new qt4drawerOnWidget(this); 
  setDrawer( Drawer );

  qt4plotView *View = Drawer->view();

  connect( View, SIGNAL(resized()), SLOT(resizePlot()) );
  connect( View, SIGNAL(dropped(QString)), SIGNAL(dropped(QString)) );
 
  connect( View, SIGNAL(mousePressed(QMouseEvent*)),  SLOT(mousePressed(QMouseEvent*)) );
  connect( View, SIGNAL(mouseMoved(QMouseEvent*)),    SLOT(mouseMoved(QMouseEvent*)) );
  connect( View, SIGNAL(mouseReleased(QMouseEvent*)), SLOT(mouseReleased(QMouseEvent*)) );
  connect( View, SIGNAL(mouseDoubleClick(QMouseEvent*)), SLOT(mouseDoubleClicked(QMouseEvent*)) );
  connect( View, SIGNAL(mouseWheel(QWheelEvent*)),    SLOT(mouseWheel(QWheelEvent*)) );
  //connect( View, SIGNAL(keyReleased(QKeyEvent*)),     SLOT(keyReleased(QKeyEvent*)) );

  resize( 700, 800 );

  //printTestCornerRectangles();

}

// ----------------------------------------------------------------

scigraphics::qt4plot::~qt4plot()
{
}

// ----------------------------------------------------------------
    
scigraphics::qt4drawerOnWidget* scigraphics::qt4plot::getDrawerQt() 
{ 
  return dynamic_cast<qt4drawerOnWidget*>( getDrawer() ); 
}

// ----------------------------------------------------------------

void scigraphics::qt4plot::resizeEvent( QResizeEvent* ) 
{ 
  resizePlot(); 
  //printTestCornerRectangles();
}

// ----------------------------------------------------------------

unsigned scigraphics::qt4plot::plotMouseModifiers( Qt::KeyboardModifiers Modifiers )
{
  unsigned Result = 0;

  if ( Modifiers & Qt::ShiftModifier )    Result |= scigraphics::mouse::Shift;
  if ( Modifiers & Qt::ControlModifier )  Result |= scigraphics::mouse::Ctrl;
  if ( Modifiers & Qt::AltModifier )      Result |= scigraphics::mouse::Alt;

  return Result;
}

// ----------------------------------------------------------------

unsigned scigraphics::qt4plot::plotMouseButtons( const QMouseEvent *Event ) 
{
  Q_ASSERT( Event != NULL );
  
  unsigned Result = 0;

  if ( Event->button() & Qt::LeftButton )    Result |= scigraphics::mouse::Left;
  if ( Event->button() & Qt::RightButton )   Result |= scigraphics::mouse::Right;
  if ( Event->button() & Qt::MiddleButton )  Result |= scigraphics::mouse::Middle;

  Result |= plotMouseModifiers( Event->modifiers() );

  return Result;
}

// ----------------------------------------------------------------

unsigned scigraphics::qt4plot::plotMouseButtons( QWheelEvent *Event ) 
{ 
  Q_ASSERT( Event != NULL );
  return plotMouseModifiers( Event->modifiers() ); 
}

// ----------------------------------------------------------------

scigraphics::wpoint scigraphics::qt4plot::plotMousePoisition( const QMouseEvent *Event )
{
  Q_ASSERT( Event != NULL );
  return scigraphics::wpoint( Event->x(), Event->y() );
}

// ----------------------------------------------------------------

scigraphics::wpoint scigraphics::qt4plot::plotMousePoisition( const QWheelEvent *Event )
{
  Q_ASSERT( Event != NULL );

#if QT_VERSION >= QT_VERSION_CHECK( 5, 14, 0 )
  const QPointF Point = Event->position();
#else
  const QPoint Point = Event->pos();
#endif
  return scigraphics::wpoint( static_cast<wcoord>(Point.x()), static_cast<wcoord>(Point.y()) );
}

// ----------------------------------------------------------------

int scigraphics::qt4plot::sceneShift() const 
{ 
  return 10; 
}

// ----------------------------------------------------------------

void scigraphics::qt4plot::printTestCornerRectangles()
{
  const unsigned W = getDrawerQt()->width();
  const unsigned H = getDrawerQt()->height();

  getDrawerQt()->eraseAll();
  getDrawerQt()->setLineStyle( lineStyle(1,color::Red) );
  getDrawerQt()->setBrushStyle( brushStyle(color::Black) );
  getDrawerQt()->drawRectangle( wrectangle( wpoint(0,0), wpoint(10,10) ) );
  getDrawerQt()->setLineStyle( lineStyle(1,color::Green) );
  getDrawerQt()->setBrushStyle( brushStyle(color::White) );
  getDrawerQt()->drawRectangle( wrectangle( wpoint(W-10,H-10), wpoint(W,H) ) );
  getDrawerQt()->flush();
}

// ----------------------------------------------------------------

void scigraphics::qt4plot::resize( int Width, int Height ) 
{ 
  QWidget::resize( Width, Height ); 
  resizePlot(); 
}

// ----------------------------------------------------------------

void scigraphics::qt4plot::enableDrop( bool Enable )
{ 
  getDrawerQt()->view()->enableDrop(Enable); 
}

// ----------------------------------------------------------------
      
void scigraphics::qt4plot::emitSelectionChanged() 
{ 
  emit selectionChanged(); 
}

// ----------------------------------------------------------------

void scigraphics::qt4plot::emitSelectionChangingFinished() 
{ 
  emit selectionChangingFinished(); 
}

// ----------------------------------------------------------------

void scigraphics::qt4plot::resizePlot()
{
  getDrawerQt()->view()->resize( width(), height() );
  getDrawerQt()->scene()->setSceneRect( 0, 0, width()-sceneShift(), height()-sceneShift() );
  replot();
}

// ----------------------------------------------------------------

void scigraphics::qt4plot::mousePressed( QMouseEvent *Event )   
{ 
  scigraphics::wpoint Point = plotMousePoisition( Event );
  unsigned Buttons = plotMouseButtons(Event);
  mouseHandler().mousePressed( Point, Buttons ); 
  emitPositionObtained(Event); 
}

// ----------------------------------------------------------------

void scigraphics::qt4plot::mouseMoved( QMouseEvent *Event )     
{ 
  scigraphics::wpoint Point = plotMousePoisition( Event );
  mouseHandler().mouseMoved( Point ); 
}

// ----------------------------------------------------------------

void scigraphics::qt4plot::mouseReleased( QMouseEvent *Event )  
{ 
  const scigraphics::wpoint Point = plotMousePoisition( Event );
  mouseHandler().mouseReleased( Point ); 
}

// ----------------------------------------------------------------

void scigraphics::qt4plot::mouseDoubleClicked( QMouseEvent *Event )  
{
  const scigraphics::wpoint Point = plotMousePoisition( Event );
  const unsigned Buttons = plotMouseButtons( Event );
  mouseHandler().mouseDoubleClicked( Point, Buttons ); 
}

// ----------------------------------------------------------------

void scigraphics::qt4plot::mouseWheel( QWheelEvent *Event )     
{ 
#if QT_VERSION >= QT_VERSION_CHECK( 5, 0, 0 )
  const int Delta = Event->angleDelta().y();
#else
  const int Delta = Event->delta();
#endif
  const unsigned Buttons = plotMouseButtons(Event);
  const scigraphics::wpoint Point = plotMousePoisition( Event );
  mouseHandler().mouseWheel( Point, Delta, Buttons ); 
}

// ----------------------------------------------------------------

QPixmap scigraphics::qt4plot::createTranspPixmap( const int Width, const int Height )
{
  QPixmap Pixmap( Width, Height );
  Pixmap.fill(Qt::transparent);
  QBitmap Mask( Pixmap.size() );
  Mask.fill( Qt::color1 );
  Pixmap.setMask( Mask );
  return Pixmap;
}

// ----------------------------------------------------------------

void scigraphics::qt4plot::replot()
{
  plot::replot();
}

// ----------------------------------------------------------------

void scigraphics::qt4plot::setCrossCursor( bool Set )
{
  if ( Set )
  {
    const int MaxCursorLength = 61;
    const QColor CursorColor  = Qt::black;
    QPixmap XYCursor = createTranspPixmap( MaxCursorLength, MaxCursorLength );
    QPainter Painter( &XYCursor );
    Painter.fillRect( 0, XYCursor.height()/2, XYCursor.width(), 1, CursorColor );
    Painter.fillRect( XYCursor.width()/2, 0,  1, XYCursor.height(), CursorColor );
    setCursor( QCursor(XYCursor) );
  } else {
    unsetCursor();
  }
}

// ----------------------------------------------------------------
    
void scigraphics::qt4plot::updatePlotSettings( qt4settings *Settings )
{
  if ( Settings != NULL )
    Settings->apply( this );
}

// ----------------------------------------------------------------

void scigraphics::qt4plot::updatePlotSettings()
{
  emit settingsUpdateRequired();
}

// ----------------------------------------------------------------

void scigraphics::qt4plot::emitPositionObtained( const QMouseEvent *Event )
{
  assert( Event != NULL );
  if ( plotMouseButtons(Event) == scigraphics::mouse::Left )
  {
    scigraphics::wpoint MousePos = plotMousePoisition(Event);
    scigraphics::npoint Point = getBottomLeftPairScales().wpoint2npoint( MousePos, getPainter() );
    emit positionObtained( Point.x(), Point.y() );
  }
}

// ----------------------------------------------------------------

void scigraphics::qt4plot::emitPlotChangedByMouse()
{
  emit plotChangedByMouse();
}

// ================================================================

