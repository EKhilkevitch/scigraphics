
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

// ================================================================

QColor scigraphics::qt4drawer::colorQt( const color& Color )         
{ 
  return QColor( Color.red(), Color.green(), Color.blue(), 0xFF - Color.transparency() ); 
}

// ----------------------------------------------------------------

QPoint scigraphics::qt4drawer::pointQt( const wpoint& Point )        
{ 
  return QPoint( Point.x(), Point.y() ); 
}

// ----------------------------------------------------------------
    
QRect  scigraphics::qt4drawer::rectangleQt( const wrectangle& Rect ) 
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
  QString FontName = Style.getFontName().empty() ? "Times" : Style.getFontName().c_str();
  return QFont( FontName, Style.getFontSize() ); 
}

// ----------------------------------------------------------------
    
QPolygon scigraphics::qt4drawer::polygonQt( const std::vector<wpoint> &Points )
{
  QPolygon Polygon;
  for ( unsigned i = 0; i < Points.size(); i++ )
    Polygon.append( pointQt( Points[i] ) );
  return Polygon;
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
  Painter->drawRect( rectangleQt(Rectangle) );
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

  Painter->save();
  Painter->rotate( Angle );
  QPoint Center = Painter->transform().inverted().map( QPoint(Rectangle.center().x(),Rectangle.center().y()) );
  QRect TextRectangle = QRect( QPoint( Center.x()-Rectangle.width()/2,Center.y()-Rectangle.height()/2) , QSize( Rectangle.width(), Rectangle.height() ) );
  Painter->drawText( TextRectangle, Qt::AlignHCenter|Qt::AlignVCenter, QString::fromUtf8(Text.c_str()) );
  if ( DrawDebugRectangle )
    Painter->drawRect( TextRectangle );
  Painter->restore();
}

// ----------------------------------------------------------------

scigraphics::wcoord scigraphics::qt4drawer::textWidth( const std::string &Text, const textStyle &Style )
{
  QFontMetrics Metrics( fontQt(Style) );
  return Metrics.size( 0, Text.c_str() ).width();
}

// ----------------------------------------------------------------

scigraphics::wcoord scigraphics::qt4drawer::textHeight( const std::string &Text, const textStyle &Style )
{
  QFontMetrics Metrics( fontQt(Style) );
  return Metrics.size( 0, Text.c_str() ).height();
}

// ----------------------------------------------------------------

scigraphics::wcoord scigraphics::qt4drawer::width()  const 
{ 
  return Scene == NULL ? 0 : Scene->width();
}

// ----------------------------------------------------------------

scigraphics::wcoord scigraphics::qt4drawer::height() const 
{ 
  return Scene == NULL ? 0 : Scene->height(); 
}

// ----------------------------------------------------------------

void scigraphics::qt4drawer::flush()
{
  PixmapItem->setPixmap( *PlotPixmap ); 
  View->update();
}

// ----------------------------------------------------------------

scigraphics::qt4drawer::qt4drawer( QWidget *Prnt ) : 
  Parent(Prnt)
{
  QSize ScreenSize = QApplication::desktop()->screenGeometry().size();

  PlotPixmap = new QPixmap( ScreenSize );
  Painter = new QPainter( PlotPixmap );

  Scene = new QGraphicsScene();
  PixmapItem = new QGraphicsPixmapItem( 0, Scene );
  PixmapItem->setOffset( 0, 0 );
  
  View  = new qt4plotView( Parent );
  View->setScene( Scene );
  View->move( Parent->x(), Parent->y() );
  View->resize( width(), height() );
  View->show();
}

// ----------------------------------------------------------------

scigraphics::qt4drawer::~qt4drawer()
{
  delete View;
  delete PixmapItem;
  delete Scene;
  delete Painter;
  delete PlotPixmap;
}

// ================================================================



