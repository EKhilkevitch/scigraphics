
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

#include "scigraphics/qt4/drawer.h"
#include "scigraphics/qt4/plotview.h"

#include "scigraphics/color.h"
#include "scigraphics/linestyle.h"
#include "scigraphics/textstyle.h"
#include "scigraphics/brushstyle.h"

#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QWidget>
#include <QApplication>
#include <QDesktopWidget>
#include <QFile>

#if QT_VERSION >= QT_VERSION_CHECK( 5, 0, 0 )
#  include <QScreen>
#endif

// ================================================================

QColor scigraphics::qt4drawer::colorQt( color Color )         
{ 
  return QColor( Color.red(), Color.green(), Color.blue(), 0xFF - Color.transparency() ); 
}

// ----------------------------------------------------------------

QPoint scigraphics::qt4drawer::pointQt( const wpoint& Point )        
{ 
  return QPoint( Point.x(), Point.y() ); 
}

// ----------------------------------------------------------------
    
QRect scigraphics::qt4drawer::rectangleQt( const wrectangle& Rect ) 
{ 
  return QRect( Rect.left(), Rect.down(), Rect.width(), Rect.height() );
}

// ----------------------------------------------------------------

QPen scigraphics::qt4drawer::penQt( const lineStyle& Style )
{
  QPen Pen;
  Pen.setColor( colorQt(Style.getColor()) );
  Qt::PenStyle PenStyle = Qt::NoPen;
  switch ( Style.getStyle() )
  {
    case scigraphics::lineStyle::Solid:
      PenStyle = Qt::SolidLine;
      break;

    case scigraphics::lineStyle::Dash:
      PenStyle = Qt::DashLine;
      break;

    case scigraphics::lineStyle::None:
    default:
      PenStyle = Qt::NoPen;
      break;
  }
  Pen.setStyle( PenStyle );
  Pen.setWidth( Style.width() );
  return Pen;
}

// ----------------------------------------------------------------

QBrush scigraphics::qt4drawer::brushQt( const brushStyle &Style )
{
  QBrush Brush;
  Brush.setColor( colorQt( Style.getColor() ) );
  Qt::BrushStyle BrushStyle = Qt::SolidPattern;
  switch ( Style.getStyle() )
  {
    case scigraphics::brushStyle::Solid:
      BrushStyle = Qt::SolidPattern;
      break;

    case scigraphics::brushStyle::None:
    default:
      BrushStyle = Qt::NoBrush;
      break;
  }
  Brush.setStyle( BrushStyle );
  return Brush;
}

// ----------------------------------------------------------------

QFont scigraphics::qt4drawer::fontQt( const textStyle &Style ) 
{
  const char *DefaultFontName = "Times";

  QString FontName = QString::fromStdString( Style.getFontName() ); 
  if ( FontName.isEmpty() )
    FontName = DefaultFontName;
  return QFont( FontName, Style.getFontSize() ); 
}

// ----------------------------------------------------------------
    
QPolygon scigraphics::qt4drawer::polygonQt( const std::vector<wpoint> &Points )
{
  QPolygon Polygon;
  for ( std::vector<wpoint>::const_iterator i = Points.begin(); i != Points.end(); ++i )
    Polygon.append( pointQt( *i ) );
  return Polygon;
}

// ----------------------------------------------------------------
      
QString scigraphics::qt4drawer::stringQt( const std::string &String )
{
  return QString::fromUtf8( String.c_str() );
}

// ----------------------------------------------------------------

void scigraphics::qt4drawer::eraseAll()
{
  if ( Painter == NULL )
    return;
  Painter->eraseRect( 0, 0, Painter->viewport().width(), Painter->viewport().height() );
}

// ----------------------------------------------------------------

void scigraphics::qt4drawer::eraseRectangle( const wrectangle& Rectangle )
{
  if ( Painter == NULL )
    return;
  Painter->eraseRect( rectangleQt(Rectangle) ); 
}

// ----------------------------------------------------------------

void scigraphics::qt4drawer::setLineStyle( const lineStyle &Style )
{
  Painter->setPen( penQt(Style) );
}

// ----------------------------------------------------------------

void scigraphics::qt4drawer::setBrushStyle( const brushStyle &Style )
{
  Painter->setBrush( brushQt(Style) );
}

// ----------------------------------------------------------------

void scigraphics::qt4drawer::setTextStyle( const textStyle &Style )
{
  Painter->setFont( fontQt(Style) );
  Painter->setPen( penQt( scigraphics::lineStyle( Style.getColor() ) ) );
}

// ----------------------------------------------------------------

void scigraphics::qt4drawer::drawLine( const wpoint &A, const wpoint &B )
{
  if ( Painter == NULL )
    return;
  Painter->drawLine( pointQt(A), pointQt(B) );
}

// ----------------------------------------------------------------

void scigraphics::qt4drawer::drawPolyline( const std::vector<wpoint> &Points )
{
  if ( Painter == NULL )
    return;
  if ( Points.size() <= 1 )
    return;

  QVector<QLine> Lines;
  Lines.reserve( static_cast<int>(Points.size()) - 1 );

  for ( size_t i = 1; i < Points.size(); i++ )
  {
    QPoint Point1 = pointQt(Points[i-1]);
    QPoint Point2 = pointQt(Points[i]);
    Lines.append( QLine( Point1, Point2 ) );
  }
  Painter->drawLines( Lines );
}

// ----------------------------------------------------------------

void scigraphics::qt4drawer::drawRectangle( const wrectangle& Rectangle )
{
  if ( Painter == NULL )
    return;

  QRect RectangleQt( rectangleQt(Rectangle) );
  
  // Win32 bug on 4.8.5
  if ( RectangleQt.top() < 0 )
    RectangleQt.setTop( -1 );

  Painter->drawRect( RectangleQt );
}

// ----------------------------------------------------------------
    
void scigraphics::qt4drawer::drawPolygon( const std::vector<wpoint> &Points )
{
  if ( Painter == NULL )
    return;

  QPolygon Polygon = polygonQt( Points );
  Painter->drawPolygon( Polygon ); 
}

// ----------------------------------------------------------------
    
void scigraphics::qt4drawer::drawText( const std::string &Text, const wrectangle& Rectangle, double Angle )
{
  const bool DrawDebugRectangle = false;
  if ( Painter == NULL )
    return;

  const QString QtText = stringQt(Text);

  Painter->save();
  Painter->rotate( Angle );
  QPoint Center = Painter->transform().inverted().map( QPoint(Rectangle.center().x(),Rectangle.center().y()) );
  QRect TextRectangle = QRect( QPoint( Center.x()-Rectangle.width()/2,Center.y()-Rectangle.height()/2) , QSize( Rectangle.width(), Rectangle.height() ) );
  Painter->drawText( TextRectangle, Qt::AlignHCenter|Qt::AlignVCenter, QtText );
  if ( DrawDebugRectangle )
    Painter->drawRect( TextRectangle );
  Painter->restore();
}

// ----------------------------------------------------------------

std::pair<scigraphics::wcoord,scigraphics::wcoord> scigraphics::qt4drawer::textWidthHeight( const std::string &Text, const textStyle &Style )
{
  const QString QtText = stringQt(Text);
  const QFontMetrics Metrics( fontQt(Style) );
  const QSize Sizes = Metrics.size( 0, QtText );
  return std::make_pair<wcoord,wcoord>( Sizes.width(), Sizes.height() );
}

// ----------------------------------------------------------------

scigraphics::wcoord scigraphics::qt4drawer::width()  const 
{ 
  return PlotPixmap->width();
}

// ----------------------------------------------------------------

scigraphics::wcoord scigraphics::qt4drawer::height() const 
{ 
  return PlotPixmap->height(); 
}

// ----------------------------------------------------------------

void scigraphics::qt4drawer::flush()
{
}

// ----------------------------------------------------------------
      
const QPixmap& scigraphics::qt4drawer::pixmap() const
{
  return *PlotPixmap;
}

// ----------------------------------------------------------------

scigraphics::qt4drawer::qt4drawer( QSize Size ) 
{
  PlotPixmap = new QPixmap( Size );
  Painter = new QPainter( PlotPixmap );
}

// ----------------------------------------------------------------

scigraphics::qt4drawer::~qt4drawer()
{
  delete Painter;
  delete PlotPixmap;
}

// ================================================================

scigraphics::qt4drawerOnWidget::qt4drawerOnWidget( QWidget *Widget ) :
  qt4drawer( screenGeometrySize() ),
  Parent( Widget )
{
  PixmapItem = new QGraphicsPixmapItem( pixmap(), NULL );
  PixmapItem->setOffset( 0, 0 );
  
  Scene = new QGraphicsScene();
  Scene->addItem(PixmapItem);
  
  View  = new qt4plotView( Parent );
  View->setScene( Scene );
  View->move( Parent->x(), Parent->y() );
  View->resize( Scene->width(), Scene->height() );
  View->show();
}

// ----------------------------------------------------------------

scigraphics::qt4drawerOnWidget::~qt4drawerOnWidget()
{
  delete View;
  delete PixmapItem;
  delete Scene;
}

// ----------------------------------------------------------------
      
QSize scigraphics::qt4drawerOnWidget::screenGeometrySize()
{
#if QT_VERSION >= QT_VERSION_CHECK( 5, 0, 0 )
  QList<QScreen*> Screens = QGuiApplication::screens();

  if ( Screens.isEmpty() )
    return QSize( 1000, 1000 );

  QSize Result = Screens[0]->size();
  for ( int i = 1; i < Screens.size(); i++ )
  {
    if ( Screens[i]->size().width() > Result.width() )
      Result = Screens[i]->size();
  }

  return Result;

#else
  return QApplication::desktop()->screenGeometry().size();
#endif
}

// ----------------------------------------------------------------

scigraphics::wcoord scigraphics::qt4drawerOnWidget::width()  const 
{ 
  return Scene->width();
}

// ----------------------------------------------------------------

scigraphics::wcoord scigraphics::qt4drawerOnWidget::height() const 
{ 
  return Scene->height(); 
}

// ----------------------------------------------------------------

void scigraphics::qt4drawerOnWidget::flush()
{
  PixmapItem->setPixmap( pixmap() ); 
  View->update();
}

// ----------------------------------------------------------------
      
QGraphicsScene* scigraphics::qt4drawerOnWidget::scene() 
{ 
  return Scene; 
}

// ----------------------------------------------------------------

scigraphics::qt4plotView* scigraphics::qt4drawerOnWidget::view() 
{ 
  return View; 
}

// ================================================================

scigraphics::qt4drawerOnImage::qt4drawerOnImage( QSize Size ) :
  qt4drawer( Size )
{
}

// ----------------------------------------------------------------

scigraphics::qt4drawerOnImage::~qt4drawerOnImage()
{
}

// ----------------------------------------------------------------
      
bool scigraphics::qt4drawerOnImage::write( const QString &FileName, int Quality )
{
  QFile File( FileName );
  
  bool Ok = File.open( QIODevice::WriteOnly );
  if ( !Ok )
    return false;

  Ok = pixmap().save( &File, NULL, Quality );
  if ( !Ok )
    return false;

  return true;
}

// ================================================================



