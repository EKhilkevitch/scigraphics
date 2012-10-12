
// ============================================================

#include "graphics/painter.h"

#include <iostream>
#include <cassert>
#include <cmath>

#if _WIN32
#  pragma warning( disable : 4244 ) 
#endif


// ============================================================
      
void graphics::painter::flush() 
{ 
  if ( ableToDraw() ) 
    Drawer->flush(); 
}

// ------------------------------------------------------------
      
void graphics::painter::updatePlotRectangle()
{
  if ( ableToDraw() )
    PlotRectangle = Indents.apply( Drawer->plotRectangle() );
  else 
    PlotRectangle = wrectangle();
}

// ------------------------------------------------------------

graphics::wcoord graphics::painter::fcoord2wcoordX( fcoord X ) const
{
  return X * PlotRectangle.width() + Indents.left();
}

// ------------------------------------------------------------

graphics::wcoord graphics::painter::fcoord2wcoordY( fcoord Y ) const
{
  return ( 1.0 - Y ) * PlotRectangle.height() + Indents.up();
}

// ------------------------------------------------------------

graphics::wpoint graphics::painter::fpoint2wpoint( fpoint Pt ) const
{
  wcoord X = fcoord2wcoordX(Pt.x()); 
  wcoord Y = fcoord2wcoordY(Pt.y()); 
  return wpoint( X, Y );
}
      
// ------------------------------------------------------------
      
graphics::fcoord graphics::painter::wcoord2fcoordX( wcoord X ) const
{
  if ( PlotRectangle.width() <= 0 || PlotRectangle.height() <= 0 )
    return 0;
  return (double)( X - Indents.left() )/PlotRectangle.width();
}

// ------------------------------------------------------------
 
graphics::fcoord graphics::painter::wcoord2fcoordY( wcoord Y ) const
{
  if ( PlotRectangle.width() <= 0 || PlotRectangle.height() <= 0 )
    return 0;
  return 1 - (double)( Y - Indents.up()   )/PlotRectangle.height();
}

// ------------------------------------------------------------

graphics::fpoint graphics::painter::wpoint2fpoint( wpoint Pt ) const
{
  if ( PlotRectangle.width() <= 0 || PlotRectangle.height() <= 0 )
    return fpoint(0,0);

  fcoord X = wcoord2fcoordX( Pt.x() ); 
  fcoord Y = wcoord2fcoordY( Pt.y() ); 

  return fpoint( X, Y );
}
      
// ------------------------------------------------------------

bool graphics::painter::canSeparatePointsF( fpoint Pt1, fpoint Pt2 ) const
{
  wpoint WPt1 = fpoint2wpoint( Pt1 );
  wpoint WPt2 = fpoint2wpoint( Pt2 );

  return ( std::abs( WPt1.x() - WPt2.x() ) >= 1 ) || 
         ( std::abs( WPt1.y() - WPt2.y() ) >= 1 );
}

// ------------------------------------------------------------

bool graphics::painter::canSepareteXcoordsF( fcoord X1, fcoord X2 ) const
{
  double ApproxStepX = 1.0 / (double)PlotRectangle.width();
  if ( std::fabs(X1-X2) < ApproxStepX/3.0 )
    return true;
  return canSeparatePointsF( fpoint(X1,0), fpoint(X2,0) );
}

// ------------------------------------------------------------

void graphics::painter::clearPlotArea()
{
  if ( ableToDraw() )
    Drawer->eraseAll();
}
      
// ------------------------------------------------------------

void graphics::painter::clearBordersArea()
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

void graphics::painter::drawLineW( wpoint A, wpoint B, const lineStyle &Style ) 
{ 
  //std::cout << "painter::drawCoordLine()" << A.x() << " " << A.y() << " " << Style.getColor().name() << std::endl;
  if ( ableToDraw() )
    Drawer->drawLine( A, B, Style ); 
}

// ------------------------------------------------------------

void graphics::painter::drawLineF( fpoint A, fpoint B, const lineStyle &Style )
{
  if ( ! ableToDraw() )
    return;
  wpoint WA = fpoint2wpoint(A);
  wpoint WB = fpoint2wpoint(B);
  drawLineW( WA, WB, Style );
}
      
// ------------------------------------------------------------

void graphics::painter::drawTextW( const std::string &String, wpoint Point, unsigned Position, 
        const textStyle &Style, int ShiftLeft, int ShiftUp, double Angle )
{
  if ( ! ableToDraw() )
    return;

  int Width  = Drawer->textWidth( String, Style );
  int Height = Drawer->textHeight( String, Style );

  switch ( Position & 0x00FF )
  {
    case drawer::HLeft:        ShiftLeft += 0;          break;
    case drawer::HRight:       ShiftLeft += Width;      break;
    case drawer::HCenter:      ShiftLeft += Width/2;    break;
  }

  switch ( Position & 0xFF00 )
  {
    case drawer::VUp:          ShiftUp += 0;            break;
    case drawer::VDown:        ShiftUp += Height;       break;
    case drawer::VCenter:      ShiftUp += Height/2;     break;
  }

  wpoint LU( Point.x() - ShiftLeft, Point.y() - ShiftUp );
  wpoint RD( LU.x() + Width, LU.y() + Height );

  Drawer->drawText( String, wrectangle(LU,RD), Style, Angle );
}

// ------------------------------------------------------------

void graphics::painter::drawTextF( const std::string &String, fpoint Point, unsigned PositionFlags, 
        const textStyle &Style, int ShiftLeft, int ShiftUp, double Angle )
{
  wpoint WPoint = fpoint2wpoint( Point );
  drawTextW( String, WPoint, PositionFlags, Style, ShiftLeft, ShiftUp, Angle );
}
      
// ------------------------------------------------------------

void graphics::painter::drawRectangleF( const frectangle &Rect, const brushStyle &BrushStyle )
{
  lineStyle LineStyle( BrushStyle.getColor() );
  LineStyle.setWidth(0);
  drawRectangleF( Rect, BrushStyle, LineStyle );
}
      
// ------------------------------------------------------------

void graphics::painter::drawRectangleF( const frectangle &Rect, const brushStyle &BrushStyle, const lineStyle &LineStyle )
{
  wpoint LU = fpoint2wpoint( Rect.leftUp() );
  wpoint RD = fpoint2wpoint( Rect.rightDown() );
  wrectangle WRect( LU, RD );

  drawRectangleW( WRect, BrushStyle, LineStyle );
}

// ------------------------------------------------------------

void graphics::painter::drawRectangleW( const wrectangle &Rect, const brushStyle &BrushStyle, const lineStyle &LineStyle )
{
  if ( ableToDraw() )
    Drawer->drawRectangle( Rect, BrushStyle, LineStyle );
}
      
// ------------------------------------------------------------

void graphics::painter::drawPointF( const fpoint &Point, const pointStyle &Style )
{
  wpoint WPoint = fpoint2wpoint(Point);
  drawPointW( WPoint, Style );
}

// ------------------------------------------------------------

void graphics::painter::drawPointW( const wpoint &Point, const pointStyle &Style )
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
        Drawer->drawRectangle( wrectangle(LU,RD), BrushStyle, lineStyle(BrushStyle.getColor()) );
      }
      break;

    default:
      abort();
  }
}

// ------------------------------------------------------------
      
void graphics::painter::drawVerticalErrorBarF( const fpoint &Min, const fpoint &Max, const errorBarStyle &Style )
{
  wpoint WMin = fpoint2wpoint(Min);
  wpoint WMax = fpoint2wpoint(Max);
  drawVecticalErrorBarW( WMin, WMax, Style );
}

// ------------------------------------------------------------

void graphics::painter::drawHorizontalErrorBarF( const fpoint &Min, const fpoint &Max, const errorBarStyle &Style )
{
  wpoint WMin = fpoint2wpoint(Min);
  wpoint WMax = fpoint2wpoint(Max);
  drawHorizontalErrorBarW( WMin, WMax, Style );
}

// ------------------------------------------------------------

void graphics::painter::drawVecticalErrorBarW( const wpoint &Min, const wpoint &Max, const errorBarStyle &Style )
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

void graphics::painter::drawHorizontalErrorBarW( const wpoint &Min, const wpoint &Max, const errorBarStyle &Style )
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

void graphics::painter::drawPolygonF( const std::vector<fpoint> &FPoints, const brushStyle &Style )
{
  std::vector<wpoint> WPoints;
  WPoints.reserve( FPoints.size() );

  for ( unsigned i = 0; i < FPoints.size(); i++ )
    WPoints.push_back( fpoint2wpoint(FPoints[i]) );

  assert( WPoints.size() == FPoints.size() );
  drawPolygonW( WPoints, Style );
}

// ------------------------------------------------------------

void graphics::painter::drawPolygonW( const std::vector<wpoint> &Points, const brushStyle &Style )
{
  if ( ! ableToDraw() )
    return;

  if ( Points.size() < 3 )
    return;
  
  Drawer->drawPolygon( Points, Style );
}

// ------------------------------------------------------------

graphics::wcoord graphics::painter::textWidth( const std::string &Text, const textStyle &Style )
{
  if ( ableToDraw() )
    return Drawer->textWidth( Text, Style );
  return 0;
}

// ------------------------------------------------------------

graphics::wcoord graphics::painter::textHeight( const std::string &Text, const textStyle &Style )
{
  if ( ableToDraw() )
    return Drawer->textHeight( Text, Style);
  return 0;
}

// ============================================================

