
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

#include "scigraphics/imagemagick/drawer.h"
#include "scigraphics/color.h"
#include "scigraphics/linestyle.h"
#include "scigraphics/textstyle.h"
#include "scigraphics/brushstyle.h"

#include <Magick++.h>
typedef Magick::Quantum Quantum;

#include <cassert>

// ================================================================

Magick::Color scigraphics::imdrawer::colorIm( const color& Color )
{
  Magick::Color ImColor; 
  ImColor.redQuantum( (double)Color.red()/0xFF * MaxRGB );
  ImColor.greenQuantum( (double)Color.green()/0xFF * MaxRGB );
  ImColor.blueQuantum( (double)Color.blue()/0xFF * MaxRGB );
  ImColor.alphaQuantum( (double)Color.transparency()/0xFF * MaxRGB );
  return ImColor;
}

// ----------------------------------------------------------------
    
Magick::Coordinate scigraphics::imdrawer::coordinateIm( const wpoint &Point )
{
  return Magick::Coordinate( Point.x(), Point.y() );
}

// ----------------------------------------------------------------
    
std::list< Magick::Coordinate > scigraphics::imdrawer::polygonIm( const std::vector<wpoint> &Points )
{
  std::list< Magick::Coordinate > Polygon;
  for ( unsigned i = 0; i < Points.size(); i++ )
    Polygon.push_back( coordinateIm( Points[i] ) );
  return Polygon;
}

// ----------------------------------------------------------------

void scigraphics::imdrawer::setLineStyle( const lineStyle &Style )
{
  if ( Image == NULL )
    return;

//  Image->fillColor( colorIm( scigraphics::color::Transparency ) );
  Image->strokeColor( colorIm( Style.getColor() ) );
  Image->strokeWidth( 0 );
  Image->lineWidth( Style.width() );

  double DashPattern[] = { 3, 3, 0 };

  switch ( Style.getStyle() )
  {
    case lineStyle::Solid:
      Image->strokeDashArray( NULL );
      break;
    case lineStyle::Dash:
      Image->strokeDashArray( DashPattern );
      break;
    default:
      Image->strokeDashArray( NULL );
      Image->lineWidth( 0 );
      break;
  }  
}

// ----------------------------------------------------------------

void scigraphics::imdrawer::setBrushStyle( const brushStyle &Style )
{
  if ( Image == NULL )
    return;

  color Color = Style.getColor();
  switch ( Style.getStyle() )
  {
    case brushStyle::Solid:
      break;

    default:
    case brushStyle::None:
      Color.setTransparency( 1 );
      break;
  }
  
  Image->fillColor( colorIm( Color ) );
  Image->strokeWidth( 0 );
  Image->strokeColor( colorIm( Color ) );
  //Image->strokeColor( Magick::Color(0, 0, 0, MaxRGB) );
}

// ----------------------------------------------------------------

void scigraphics::imdrawer::setTextStyle( const textStyle &Style )
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
    
void scigraphics::imdrawer::drawLine( const wpoint &A, const wpoint &B )
{
  if ( Image == NULL )
    return;

  Image->draw( Magick::DrawableLine( A.x(), A.y(), B.x(), B.y() ) );
}

// ----------------------------------------------------------------

void scigraphics::imdrawer::drawRectangle( const wrectangle& Rectangle )
{
  if ( Image == NULL )
    return;

  Image->draw( Magick::DrawableRectangle( Rectangle.left(), Rectangle.up(), Rectangle.right(), Rectangle.down() ) );
}
    
// ----------------------------------------------------------------
    
void scigraphics::imdrawer::drawPolygon( const std::vector<wpoint> &Points ) 
{
  if ( Points.size() < 3 )
    return;
  
  if ( Image == NULL )
    return;
  
  std::list< Magick::Coordinate > Polygon = polygonIm( Points );

  Image->strokeColor( Magick::Color(0, 0, 0, MaxRGB) );
  Image->draw( Magick::DrawablePolygon( Polygon ) );
}

// ----------------------------------------------------------------

void scigraphics::imdrawer::drawText( const std::string &Text, const wrectangle& Rectangle, double RotAngle )
{
  if ( Image == NULL )
    return;
 
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

scigraphics::wcoord scigraphics::imdrawer::textWidth( const std::string &Text, const textStyle &Style )
{
  if ( Image == NULL )
    return drawer::textWidth( Text, Style );
  
  setTextStyle( Style );

  Magick::TypeMetric Metric;
  Image->fontTypeMetrics( Text, &Metric );
  return Metric.textWidth();
}

// ----------------------------------------------------------------

scigraphics::wcoord scigraphics::imdrawer::textHeight( const std::string &Text, const textStyle &Style )
{
  if ( Image == NULL )
    return drawer::textHeight( Text, Style );
  
  setTextStyle( Style );

  Magick::TypeMetric Metric;
  Image->fontTypeMetrics( Text, &Metric );
  return Metric.textHeight();
}

// ----------------------------------------------------------------

scigraphics::wcoord scigraphics::imdrawer::width()  const 
{ 
  return Image == NULL ? 0 : Image->columns(); 
};

// ----------------------------------------------------------------

scigraphics::wcoord scigraphics::imdrawer::height() const 
{ 
  return Image == NULL ? 0 : Image->rows(); 
};

// ----------------------------------------------------------------

scigraphics::imdrawer::imdrawer( size_t SizeX, size_t SizeY ) :
  Image( NULL )  
{
  Image = new Magick::Image( Magick::Geometry(SizeX,SizeY), Magick::Color("white") );
}

// ----------------------------------------------------------------
    
void scigraphics::imdrawer::write( const std::string &FileName )
{
  if ( Image == NULL )
    return;
  Image->write( FileName );
}

// ----------------------------------------------------------------

void scigraphics::imdrawer::display()
{
  if ( Image == NULL )
    return;
  Image->display();
}

// ================================================================


