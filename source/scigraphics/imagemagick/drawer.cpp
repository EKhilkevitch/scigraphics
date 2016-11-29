
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

#include <list>
#include <vector>
#include <cassert>

// ================================================================

Magick::Color scigraphics::imdrawer::colorIm( color Color )
{
  Magick::Color ImColor; 
  ImColor.redQuantum( static_cast<double>(Color.red())/0xFF * MaxRGB );
  ImColor.greenQuantum( static_cast<double>(Color.green())/0xFF * MaxRGB );
  ImColor.blueQuantum( static_cast<double>(Color.blue())/0xFF * MaxRGB );
  ImColor.alphaQuantum( static_cast<double>(Color.transparency())/0xFF * MaxRGB );
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
  for ( std::vector<wpoint>::const_iterator i = Points.begin(); i != Points.end(); ++i )
    Polygon.push_back( coordinateIm( *i ) );
  return Polygon;
}

// ----------------------------------------------------------------

void scigraphics::imdrawer::applyCurrentLineStyle()
{
  if ( LastStyleUsed == LastLineStyle )
    return;

  LastStyleUsed = LastLineStyle;

  Image->fillColor( colorIm( scigraphics::color::Transparency ) );
  Image->strokeColor( colorIm( LineStyle->getColor() ) );
  Image->strokeWidth( 0 );
  Image->lineWidth( LineStyle->width() );

  const double DashPattern[] = { 3, 3, 0 };

  switch ( LineStyle->getStyle() )
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

void scigraphics::imdrawer::setLineStyle( const lineStyle &Style )
{
  *LineStyle = Style;
  LastStyleUsed = LastNoneStyle;
}

// ----------------------------------------------------------------

void scigraphics::imdrawer::applyCurrentBrushStyle()
{
  if ( LastStyleUsed == LastBrushStyle )
    return;

  LastStyleUsed = LastBrushStyle;

  color Color = BrushStyle->getColor();
  switch ( BrushStyle->getStyle() )
  {
    case brushStyle::Solid:
      break;

    default:
    case brushStyle::None:
      Color.setTransparency( 1 );
      break;
  }
  
  Image->fillColor( colorIm( Color ) );
  Image->strokeColor( colorIm( Color ) );
  Image->strokeWidth( 0 );
  //Image->strokeColor( Magick::Color(0, 0, 0, MaxRGB) );
}

// ----------------------------------------------------------------

void scigraphics::imdrawer::setBrushStyle( const brushStyle &Style )
{
  *BrushStyle = Style;
  LastStyleUsed = LastNoneStyle;
}

// ----------------------------------------------------------------

void scigraphics::imdrawer::applyCurrentTextStyle()
{
  const char *DefaultFontName = "Times-Roman";
  
  if ( LastStyleUsed == LastTextStyle )
    return;

  LastStyleUsed = LastTextStyle;

  std::string FontName = TextStyle->getFontName();
  if ( FontName.empty() )
    FontName = DefaultFontName;

  Image->strokeColor( Magick::Color(0, 0, 0, MaxRGB) );
  Image->fillColor( colorIm( TextStyle->getColor() ) );
  Image->font( FontName ); 
  Image->fontPointsize( TextStyle->getFontSize() );
  Image->lineWidth(1);
  Image->strokeDashArray( NULL );
}

// ----------------------------------------------------------------

void scigraphics::imdrawer::setTextStyle( const textStyle &Style )
{
  *TextStyle = Style;
  LastStyleUsed = LastNoneStyle;
}

// ----------------------------------------------------------------
    
void scigraphics::imdrawer::drawLine( const wpoint &A, const wpoint &B )
{
  if ( Image == NULL )
    return;
  applyCurrentLineStyle();
  Image->draw( Magick::DrawableLine( A.x(), A.y(), B.x(), B.y() ) );
}

// ----------------------------------------------------------------

void scigraphics::imdrawer::drawRectangle( const wrectangle& Rectangle )
{
  if ( Image == NULL )
    return;

  Magick::DrawableRectangle MagickRectange( Rectangle.left(), Rectangle.up(), Rectangle.right(), Rectangle.down() );

  applyCurrentBrushStyle();
  Image->draw( MagickRectange );

  applyCurrentLineStyle();
  Image->draw( MagickRectange );
}
    
// ----------------------------------------------------------------
    
void scigraphics::imdrawer::drawPolygon( const std::vector<wpoint> &Points ) 
{
  if ( Points.size() < 3 )
    return;
  
  if ( Image == NULL )
    return;
  
  std::list< Magick::Coordinate > Polygon = polygonIm( Points );

  applyCurrentBrushStyle();
  Image->draw( Magick::DrawablePolygon( Polygon ) );
}

// ----------------------------------------------------------------

void scigraphics::imdrawer::drawText( const std::string &Text, const wrectangle& Rectangle, double RotAngle )
{
  if ( Image == NULL )
    return;

  applyCurrentTextStyle();

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
  applyCurrentTextStyle();

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
  applyCurrentTextStyle();

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

  LineStyle = new lineStyle();
  BrushStyle = new brushStyle();
  TextStyle = new textStyle();
  LastStyleUsed = LastNoneStyle;
}

// ----------------------------------------------------------------

scigraphics::imdrawer::~imdrawer()
{
  delete TextStyle;
  delete LineStyle;
  delete BrushStyle;
  delete Image;
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


