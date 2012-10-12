
// ================================================================

#include "graphics/imagemagick/implot.h"

#include <Magick++.h>

#include <cassert>
#include <iostream>
#include <list>

// ================================================================

Magick::Color drawerIm::colorIm( const graphics::color& Color )
{
  Magick::Color ImColor; 
  ImColor.redQuantum( (double)Color.red()/0xFF * MaxRGB );
  ImColor.greenQuantum( (double)Color.green()/0xFF * MaxRGB );
  ImColor.blueQuantum( (double)Color.blue()/0xFF * MaxRGB );
  ImColor.alphaQuantum( (double)Color.transparency()/0xFF * MaxRGB );
  return ImColor;
}

// ----------------------------------------------------------------
    
Magick::Coordinate drawerIm::coordinateIm( const graphics::wpoint &Point )
{
  return Magick::Coordinate( Point.x(), Point.y() );
}

// ----------------------------------------------------------------
    
std::list< Magick::Coordinate > drawerIm::polygonIm( const std::vector<graphics::wpoint> &Points )
{
  std::list< Magick::Coordinate > Polygon;
  for ( unsigned i = 0; i < Points.size(); i++ )
    Polygon.push_back( coordinateIm( Points[i] ) );
  return Polygon;
}

// ----------------------------------------------------------------
    
void drawerIm::prepareForLineStyle( const graphics::lineStyle &Style )
{
  if ( Image == NULL )
    return;

  Image->fillColor( colorIm( graphics::color::Transparency ) );
  Image->strokeColor( colorIm( Style.getColor() ) );
  Image->strokeWidth( 0 );
  Image->lineWidth( Style.width() );

  double DashPattern[] = { 3, 3, 0 };

  switch ( Style.getStyle() )
  {
    case graphics::lineStyle::Solid:
      Image->strokeDashArray( NULL );
      break;
    case graphics::lineStyle::Dash:
      Image->strokeDashArray( DashPattern );
      break;
    default:
      Image->strokeDashArray( NULL );
      Image->lineWidth( 0 );
      break;
  }  
}

// ----------------------------------------------------------------

void drawerIm::prepareForBrushStyle( const graphics::brushStyle& Style )
{
  if ( Image == NULL )
    return;

  graphics::color Color = Style.getColor();
  switch ( Style.getStyle() )
  {
    case graphics::brushStyle::Solid:
      break;

    default:
    case graphics::brushStyle::None:
      Color.setTransparency( 1 );
      break;
  }
  
  Image->strokeWidth( 0 );
  Image->fillColor( colorIm( Color ) );
}

// ----------------------------------------------------------------

void drawerIm::prepareForTextStyle( const graphics::textStyle &Style )
{
  if ( Image == NULL )
    return;
  
  Image->strokeColor( Magick::Color(0, 0, 0, MaxRGB) );
  Image->fillColor( colorIm( Style.getColor() ) );
  Image->font( Style.getFontName().empty() ? "Times-Roman" : Style.getFontName() ); //Style.getFontName() );
  Image->fontPointsize( Style.getFontSize() );
  Image->lineWidth(1);
  Image->strokeDashArray( NULL );
}

// ----------------------------------------------------------------
    
void drawerIm::drawLine( const graphics::wpoint &A, const graphics::wpoint &B, const graphics::lineStyle &Style )
{
  if ( Image == NULL )
    return;

  prepareForLineStyle( Style );
  Image->draw( Magick::DrawableLine( A.x(), A.y(), B.x(), B.y() ) );
}

// ----------------------------------------------------------------

void drawerIm::drawRectangle( const graphics::wrectangle& Rectangle, const graphics::brushStyle& BrushStyle, const graphics::lineStyle &LineStyle )
{
  if ( Image == NULL )
    return;

  prepareForLineStyle( LineStyle );
  prepareForBrushStyle( BrushStyle );

  Image->draw( Magick::DrawableRectangle( Rectangle.left(), Rectangle.up(), Rectangle.right(), Rectangle.down() ) );
}
    
// ----------------------------------------------------------------
    
void drawerIm::drawPolygon( const std::vector<graphics::wpoint> &Points, const graphics::brushStyle& BrushStyle ) 
{
  if ( Points.size() < 3 )
    return;
  
  if ( Image == NULL )
    return;
  
  std::list< Magick::Coordinate > Polygon = polygonIm( Points );

  prepareForBrushStyle( BrushStyle );
  Image->strokeColor( Magick::Color(0, 0, 0, MaxRGB) );
  Image->draw( Magick::DrawablePolygon( Polygon ) );
}

// ----------------------------------------------------------------

void drawerIm::drawText( const std::string &Text, const graphics::wrectangle& Rectangle, const graphics::textStyle &Style, double RotAngle )
{
  if ( Image == NULL )
    return;
 
  prepareForTextStyle( Style );

  std::list<Magick::Drawable> DrawList;
  DrawList.push_back( Magick::DrawableTranslation( Rectangle.center().x(), Rectangle.center().y() ) );
  DrawList.push_back( Magick::DrawableRotation( RotAngle ) );
  /* I don't understand, why is +Rectangle.height()/4 in next string */
  DrawList.push_back( Magick::DrawableText( -Rectangle.width()/2, +Rectangle.height()/4, Text, "UTF-8" ) ); 
  DrawList.push_back( Magick::DrawableRotation( -RotAngle ) );
  DrawList.push_back( Magick::DrawableTranslation( -Rectangle.center().x(), -Rectangle.center().y() ) );
  Image->draw( DrawList );
}

// ----------------------------------------------------------------

graphics::wcoord drawerIm::textWidth( const std::string &Text, const graphics::textStyle &Style )
{
  if ( Image == NULL )
    return drawer::textWidth( Text, Style );
  
  prepareForTextStyle( Style );

  Magick::TypeMetric Metric;
  Image->fontTypeMetrics( Text, &Metric );
  return Metric.textWidth();
}

// ----------------------------------------------------------------

graphics::wcoord drawerIm::textHeight( const std::string &Text, const graphics::textStyle &Style )
{
  if ( Image == NULL )
    return drawer::textHeight( Text, Style );
  
  prepareForTextStyle( Style );

  Magick::TypeMetric Metric;
  Image->fontTypeMetrics( Text, &Metric );
  return Metric.textHeight();
}

// ----------------------------------------------------------------

graphics::wcoord drawerIm::width()  const 
{ 
  return Image == NULL ? 0 : Image->columns(); 
};

// ----------------------------------------------------------------

graphics::wcoord drawerIm::height() const 
{ 
  return Image == NULL ? 0 : Image->rows(); 
};

// ----------------------------------------------------------------

drawerIm::drawerIm( size_t SizeX, size_t SizeY ) :
  Image( NULL )  
{
  Image = new Magick::Image( Magick::Geometry(SizeX,SizeY), Magick::Color("white") );
}

// ----------------------------------------------------------------
    
void drawerIm::write( const std::string &FileName )
{
  if ( Image == NULL )
    return;
  Image->write( FileName );
}

// ----------------------------------------------------------------

void drawerIm::display()
{
  if ( Image == NULL )
    return;
  Image->display();
}

// ================================================================

implot::implot( size_t SizeX, size_t SizeY )
{
  drawerIm *Drawer = new drawerIm(SizeX,SizeY);
  setDrawer( Drawer );
}

// ----------------------------------------------------------------
    
void implot::write( const std::string &FileName )
{
  drawerIm *Drawer = getDrawerIm();
  if ( Drawer == NULL )
    return;

  replot();
  Drawer->write( FileName );
}

// ----------------------------------------------------------------
    
void implot::display()
{
  drawerIm *Drawer = getDrawerIm();
  if ( Drawer == NULL )
    return;

  replot();
  Drawer->display();
}

// ================================================================

