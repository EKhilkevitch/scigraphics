
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

#include "scigraphics/painter.h"
#include "scigraphics/drawer.h"
#include "scigraphics/linestyle.h"
#include "scigraphics/brushstyle.h"
#include "scigraphics/textstyle.h"
#include "scigraphics/pointstyle.h"
#include "scigraphics/errorbarstyle.h"

#include <iostream>
#include <cassert>
#include <cmath>

#if _MSC_VER
#  pragma warning( disable : 4244 ) 
#endif

// ============================================================
      
scigraphics::painter::~painter() 
{ 
  delete Drawer; 
  Drawer = NULL;
}
      
// ------------------------------------------------------------

void scigraphics::painter::setDrawer( drawer *D ) 
{ 
  if ( D != Drawer )
  {
    delete Drawer; 
    Drawer = D; 
  }
} 

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::painter::plotWidth()  const 
{ 
  if ( ! ableToDraw() )
    return 100;
  return Drawer->width(); 
}

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::painter::plotHeight() const 
{ 
  if ( ! ableToDraw() )
    return 100;
  return Drawer->height(); 
}

// ------------------------------------------------------------

void scigraphics::painter::setIndents( const indents<wcoord>& Ids ) 
{ 
  Indents = Ids; 
  update(); 
}

// ------------------------------------------------------------

void scigraphics::painter::flush() 
{ 
  if ( ableToDraw() ) 
    Drawer->flush(); 
}

// ------------------------------------------------------------

void scigraphics::painter::update() 
{ 
  updatePlotRectangle(); 
}

// ------------------------------------------------------------
      
void scigraphics::painter::updatePlotRectangle()
{
  if ( ableToDraw() )
    PlotRectangle = Indents.apply( Drawer->plotRectangle() );
  else 
    PlotRectangle = wrectangle();
}

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::painter::fcoord2wcoordX( fcoord X ) const
{
  return X * PlotRectangle.width() + Indents.left();
}

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::painter::fcoord2wcoordY( fcoord Y ) const
{
  return ( 1.0 - Y ) * PlotRectangle.height() + Indents.up();
}

// ------------------------------------------------------------

scigraphics::wpoint scigraphics::painter::fpoint2wpoint( fpoint Pt ) const
{
  wcoord X = fcoord2wcoordX(Pt.x()); 
  wcoord Y = fcoord2wcoordY(Pt.y()); 
  return wpoint( X, Y );
}
      
// ------------------------------------------------------------
      
scigraphics::fcoord scigraphics::painter::wcoord2fcoordX( wcoord X ) const
{
  if ( PlotRectangle.width() <= 0 || PlotRectangle.height() <= 0 )
    return 0;
  return (double)( X - Indents.left() )/PlotRectangle.width();
}

// ------------------------------------------------------------
 
scigraphics::fcoord scigraphics::painter::wcoord2fcoordY( wcoord Y ) const
{
  if ( PlotRectangle.width() <= 0 || PlotRectangle.height() <= 0 )
    return 0;
  return 1 - (double)( Y - Indents.up()   )/PlotRectangle.height();
}

// ------------------------------------------------------------

scigraphics::fpoint scigraphics::painter::wpoint2fpoint( wpoint Pt ) const
{
  if ( PlotRectangle.width() <= 0 || PlotRectangle.height() <= 0 )
    return fpoint(0,0);

  fcoord X = wcoord2fcoordX( Pt.x() ); 
  fcoord Y = wcoord2fcoordY( Pt.y() ); 

  return fpoint( X, Y );
}
      
// ------------------------------------------------------------

bool scigraphics::painter::canSeparatePointsF( fpoint Pt1, fpoint Pt2 ) const
{
  wpoint WPt1 = fpoint2wpoint( Pt1 );
  wpoint WPt2 = fpoint2wpoint( Pt2 );

  return ( std::abs( WPt1.x() - WPt2.x() ) >= 1 ) || 
         ( std::abs( WPt1.y() - WPt2.y() ) >= 1 );
}

// ------------------------------------------------------------

bool scigraphics::painter::canSepareteXcoordsF( fcoord X1, fcoord X2 ) const
{
  double ApproxStepX = 1.0 / (double)PlotRectangle.width();
  if ( std::fabs(X1-X2) < ApproxStepX/3.0 )
    return true;
  return canSeparatePointsF( fpoint(X1,0), fpoint(X2,0) );
}

// ------------------------------------------------------------

void scigraphics::painter::clearPlotArea()
{
  if ( ableToDraw() )
    Drawer->eraseAll();
}
      
// ------------------------------------------------------------

void scigraphics::painter::clearBordersArea()
{
  if ( ! ableToDraw() )
    return;

  const int Shift = 10;

  wpoint Point1( -Shift, -Shift );
  wpoint Point2( Indents.left(), Drawer->height() + Shift );
  Drawer->eraseRectangle( wrectangle(Point1,Point2) );

  wpoint Point3( Drawer->width() + Shift, Drawer->height() - Indents.down() );
  Drawer->eraseRectangle( wrectangle(Point2,Point3) );
  
  wpoint Point4( Drawer->width() - Indents.right(), -Shift );
  Drawer->eraseRectangle( wrectangle(Point3,Point4) );

  wpoint Point5( -Shift, Indents.up() );
  Drawer->eraseRectangle( wrectangle(Point4,Point5) );
}

// ------------------------------------------------------------

void scigraphics::painter::setLineStyle( const lineStyle &Style )
{
  if ( ableToDraw() )
    Drawer->setLineStyle( Style );
}

// ------------------------------------------------------------

void scigraphics::painter::drawLineW( wpoint A, wpoint B, const lineStyle &Style ) 
{ 
  //std::cout << "painter::drawCoordLine()" << A.x() << " " << A.y() << " " << Style.getColor().name() << std::endl;
  setLineStyle( Style );
  drawLineW( A, B );
}

// ------------------------------------------------------------

void scigraphics::painter::drawLineW( wpoint A, wpoint B )
{
  if ( ableToDraw() )
    Drawer->drawLine( A, B ); 
}

// ------------------------------------------------------------

void scigraphics::painter::drawLineW( const std::vector<wpoint> &Points, const lineStyle &Style )
{
  setLineStyle( Style );
  drawLineW( Points );
}

// ------------------------------------------------------------

void scigraphics::painter::drawLineW( const std::vector<wpoint> &Points )
{
  if ( ableToDraw() )
    Drawer->drawPolyline( Points );
}

// ------------------------------------------------------------

void scigraphics::painter::drawLineF( fpoint A, fpoint B, const lineStyle &Style )
{
  setLineStyle( Style );
  drawLineF( A, B );
}

// ------------------------------------------------------------

void scigraphics::painter::drawLineF( fpoint A, fpoint B )
{
  if ( ! ableToDraw() )
    return;
  wpoint WA = fpoint2wpoint(A);
  wpoint WB = fpoint2wpoint(B);
  drawLineW( WA, WB );
}
      
// ------------------------------------------------------------

void scigraphics::painter::drawTextW( const std::string &String, wpoint Point, unsigned Position, 
        const textStyle &Style, int ShiftLeft, int ShiftUp, double Angle )
{
  if ( ! ableToDraw() )
    return;

  int Width  = Drawer->textWidth( String, Style );
  int Height = Drawer->textHeight( String, Style );

  switch ( Position & 0x00FF )
  {
    case HLeft:        ShiftLeft += 0;          break;
    case HRight:       ShiftLeft += Width;      break;
    case HCenter:      ShiftLeft += Width/2;    break;
  }

  switch ( Position & 0xFF00 )
  {
    case VUp:          ShiftUp += 0;            break;
    case VDown:        ShiftUp += Height;       break;
    case VCenter:      ShiftUp += Height/2;     break;
  }

  wpoint LU( Point.x() - ShiftLeft, Point.y() - ShiftUp );
  wpoint RD( LU.x() + Width, LU.y() + Height );

  Drawer->setTextStyle( Style );
  Drawer->drawText( String, wrectangle(LU,RD), Angle );
}

// ------------------------------------------------------------

void scigraphics::painter::drawTextF( const std::string &String, fpoint Point, unsigned PositionFlags, 
        const textStyle &Style, int ShiftLeft, int ShiftUp, double Angle )
{
  wpoint WPoint = fpoint2wpoint( Point );
  drawTextW( String, WPoint, PositionFlags, Style, ShiftLeft, ShiftUp, Angle );
}
      
// ------------------------------------------------------------

void scigraphics::painter::drawRectangleF( const frectangle &Rect, const brushStyle &BrushStyle )
{
  lineStyle LineStyle( BrushStyle.getColor() );
  LineStyle.setWidth(0);
  drawRectangleF( Rect, BrushStyle, LineStyle );
}
      
// ------------------------------------------------------------

void scigraphics::painter::drawRectangleF( const frectangle &Rect, const brushStyle &BrushStyle, const lineStyle &LineStyle )
{
  wpoint LU = fpoint2wpoint( Rect.leftUp() );
  wpoint RD = fpoint2wpoint( Rect.rightDown() );
  wrectangle WRect( LU, RD );

  drawRectangleW( WRect, BrushStyle, LineStyle );
}

// ------------------------------------------------------------

void scigraphics::painter::drawRectangleW( const wrectangle &Rect, const brushStyle &BrushStyle, const lineStyle &LineStyle )
{
  if ( ableToDraw() )
  {
    Drawer->setBrushStyle( BrushStyle );
    Drawer->setLineStyle( LineStyle );
    Drawer->drawRectangle( Rect );
  }
}
      
// ------------------------------------------------------------

void scigraphics::painter::drawPointF( const fpoint &Point, const pointStyle &Style )
{
  wpoint WPoint = fpoint2wpoint(Point);
  drawPointW( WPoint, Style );
}

// ------------------------------------------------------------

void scigraphics::painter::drawPointW( const wpoint &Point, const pointStyle &Style )
{
  if ( ! ableToDraw() )
    return;

  brushStyle BrushStyle( Style.getColor() );

  switch ( Style.getShape() )
  {
    case pointStyle::None:
      break;

    case pointStyle::Square:
      {
        wpoint LU( Point.x() - Style.width()/2, Point.y() - Style.width()/2 );
        wpoint RD( LU.x() + Style.width(), LU.y() + Style.width() );
        Drawer->setBrushStyle( BrushStyle );
        Drawer->setLineStyle( lineStyle(BrushStyle.getColor()) );
        Drawer->drawRectangle( wrectangle(LU,RD) );
      }
      break;

    default:
      abort();
  }
}

// ------------------------------------------------------------
      
void scigraphics::painter::drawVerticalErrorBarF( const fpoint &Min, const fpoint &Max, const errorBarStyle &Style )
{
  wpoint WMin = fpoint2wpoint(Min);
  wpoint WMax = fpoint2wpoint(Max);
  drawVecticalErrorBarW( WMin, WMax, Style );
}

// ------------------------------------------------------------

void scigraphics::painter::drawHorizontalErrorBarF( const fpoint &Min, const fpoint &Max, const errorBarStyle &Style )
{
  wpoint WMin = fpoint2wpoint(Min);
  wpoint WMax = fpoint2wpoint(Max);
  drawHorizontalErrorBarW( WMin, WMax, Style );
}

// ------------------------------------------------------------

void scigraphics::painter::drawVecticalErrorBarW( const wpoint &Min, const wpoint &Max, const errorBarStyle &Style )
{
  assert( Min.x() == Max.x() );
  
  lineStyle LineStyle = Style.getLineStyle();
  wcoord HatWidth = Style.hatWidth();
  wcoord X = Min.x();

  drawLineW( Min, Max, LineStyle );
  drawLineW( wpoint(X-HatWidth,Min.y()), wpoint(X+HatWidth,Min.y()), LineStyle );
  drawLineW( wpoint(X-HatWidth,Max.y()), wpoint(X+HatWidth,Max.y()), LineStyle );
}

// ------------------------------------------------------------

void scigraphics::painter::drawHorizontalErrorBarW( const wpoint &Min, const wpoint &Max, const errorBarStyle &Style )
{
  assert( Min.y() == Max.y() );
  
  lineStyle LineStyle = Style.getLineStyle();
  wcoord HatWidth = Style.hatWidth();
  wcoord Y = Min.y();

  drawLineW( Min, Max, LineStyle );
  drawLineW( wpoint(Min.x(),Y-HatWidth), wpoint(Min.x(),Y+HatWidth), LineStyle );
  drawLineW( wpoint(Max.x(),Y-HatWidth), wpoint(Max.x(),Y+HatWidth), LineStyle );
}
      
// ------------------------------------------------------------

void scigraphics::painter::drawPolygonF( const std::vector<fpoint> &FPoints, const brushStyle &Style )
{
  std::vector<wpoint> WPoints;
  WPoints.reserve( FPoints.size() );

  for ( size_t i = 0; i < FPoints.size(); i++ )
    WPoints.push_back( fpoint2wpoint(FPoints[i]) );

  assert( WPoints.size() == FPoints.size() );
  drawPolygonW( WPoints, Style );
}

// ------------------------------------------------------------

void scigraphics::painter::drawPolygonW( const std::vector<wpoint> &Points, const brushStyle &Style )
{
  if ( ! ableToDraw() )
    return;

  if ( Points.size() < 3 )
    return;
  
  Drawer->setBrushStyle( Style );
  Drawer->setLineStyle( lineStyle(lineStyle::None) );
  Drawer->drawPolygon( Points );
}

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::painter::textWidth( const std::string &Text, const textStyle &Style )
{
  if ( ! ableToDraw() )
    return 0;
  return Drawer->textWidth( Text, Style );
}

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::painter::textHeight( const std::string &Text, const textStyle &Style )
{
  if ( ! ableToDraw() )
    return 0;
  return Drawer->textHeight( Text, Style);
}

// ============================================================

