
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

#include "scigraphics/graphviewmap.h"
#include "scigraphics/painter.h"
#include "scigraphics/scale.h"
#include "scigraphics/numberstyle.h"
#include "scigraphics/color.h"
#include "scigraphics/brushstyle.h"
#include "scigraphics/linestyle.h"
#include "scigraphics/textstyle.h"

#include <iostream>
#include <cmath>
#include <cassert>
#include <stdexcept>

#if _MSC_VER
#  pragma warning( disable : 4244 )
#endif

// ============================================================
        
scigraphics::map::graphView::graphView() : 
  Visible(true) 
{
}

// ------------------------------------------------------------

scigraphics::map::graphView::~graphView() 
{
}

// ------------------------------------------------------------

void scigraphics::map::graphView::setVisible( bool V ) 
{ 
  Visible = V; 
}

// ------------------------------------------------------------

bool scigraphics::map::graphView::isVisible() const 
{ 
  return Visible; 
}

// ------------------------------------------------------------
        
scigraphics::wcoord scigraphics::map::graphView::legendExampleWidth() const  
{ 
  return 80; 
}

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::map::graphView::legendExampleHeight() const 
{ 
  return 200; 
}

// ============================================================

double scigraphics::map::graphViewRectangle::pointColorStrategy::relativePointValue( const map::data::point_t &Point, const scale &Scale )
{
  if ( ! Point.isValid() )
    return invalidNumber();

  double RelValue = Scale.numberToFraction(Point.z());
  RelValue = relativeValueToUnitInterval( RelValue );
  return RelValue;
}

// ------------------------------------------------------------

double scigraphics::map::graphViewRectangle::pointColorStrategy::relativeValueToUnitInterval( double Value )
{
  if ( ! isValidNumber(Value) )
    return 0;
  Value = std::max<double>( Value, 0 );
  Value = std::min<double>( Value, 1 );
  return Value;
}

// ------------------------------------------------------------

scigraphics::color scigraphics::map::graphViewRectangle::pointColorStrategy::invalidValueColor() const
{
  return color( 0xFF, 0xFF, 0xFF, 0xFF );
}

// ------------------------------------------------------------

scigraphics::color scigraphics::map::graphViewRectangle::pointColorStrategy::pointColor( const map::data::point_t &Point, const scale &Scale ) const
{
  if ( ! Point.isValid() )
    return invalidValueColor();

  double RelativeValue = relativePointValue(Point,Scale);
  return relativeValueColor( RelativeValue );
}

// ------------------------------------------------------------

scigraphics::map::graphViewRectangle::pointColorStrategy::~pointColorStrategy() 
{
}

// ------------------------------------------------------------

scigraphics::color scigraphics::map::graphViewRectangle::grayscalePointColorStrategy::relativeValueColor( double Value ) const
{
  if ( Value < 0.0 || 1.0 < Value )
    return invalidValueColor();
  
  const int ColorValue = static_cast<int>( Value * 0xFF );
  return color( ColorValue, ColorValue, ColorValue );
}

// ------------------------------------------------------------

scigraphics::color scigraphics::map::graphViewRectangle::redYellowBluePointColorStrategy::relativeValueColor( double Value ) const
{
  if ( Value < 0.0 || 1.0 < Value )
    return invalidValueColor();

  const int HsvHue = static_cast<int>( ( 1.0 - Value )*240 ); 
  const int HsvSaturation = 100;
  const int HsvValue = 80;

  return color::fromHSV( HsvHue, HsvSaturation, HsvValue );
}

// ------------------------------------------------------------

scigraphics::color scigraphics::map::graphViewRectangle::yellowRedBluePointColorStrategy::relativeValueColor( double Value ) const
{
  if ( Value < 0 || 1 < Value )
    return invalidValueColor();

  const int HsvHue = 360 + 60 - static_cast<int>( ( 1.0 - Value )*180 ); 
  const int HsvSaturation = 100;
  const int HsvValue = 80;

  return color::fromHSV( HsvHue % 360, HsvSaturation, HsvValue );
}

// ------------------------------------------------------------
      
scigraphics::map::graphViewRectangle::graphViewRectangle() : 
  PointColorStrategy(NULL)
{
  PointColorStrategy = new redYellowBluePointColorStrategy();
}

// ------------------------------------------------------------

scigraphics::map::graphViewRectangle::~graphViewRectangle()
{
  delete PointColorStrategy;
}

// ------------------------------------------------------------

void scigraphics::map::graphViewRectangle::setColorStrategy( pointColorStrategy *Strategy )
{
  if ( Strategy == NULL )
    throw std::invalid_argument( "Strategy argument must be not NULL" );

  delete PointColorStrategy;
  PointColorStrategy = Strategy;
}

// ------------------------------------------------------------

void scigraphics::map::graphViewRectangle::draw( painter &Painter, const pairScales& Scales, const map::data &Data, const scale &ScaleZ ) const
{
  assert( PointColorStrategy != NULL );

  data::iterator Begin = Data.begin();
  data::iterator End   = Data.end();

  for ( data::iterator Point = Begin; Point != End; ++Point )
    drawPoint( Painter, Scales, *Point, ScaleZ );
}

// ------------------------------------------------------------
      
void scigraphics::map::graphViewRectangle::drawPoint( painter &Painter, const pairScales& Scales, const map::data::point_t &Point, const scale &ScaleZ ) const
{
  if ( ! Point.isValid() )
    return;

  const color Color = PointColorStrategy->pointColor( Point, ScaleZ );

  const fpoint Point0 = Scales.npoint2fpoint( npoint( Point.x0(), Point.y0() ) );
  const fpoint Point1 = Scales.npoint2fpoint( npoint( Point.x1(), Point.y1() ) );

  Painter.drawRectangleF( frectangle(Point0,Point1), brushStyle(Color) );
}

// ------------------------------------------------------------

void scigraphics::map::graphViewRectangle::drawLegendExample( painter &Painter, const wrectangle &Rectangle, const scale &ScaleZ ) const
{
  wpoint LeftUp = Rectangle.leftUp();
  wpoint RightDown = Rectangle.rightDown();

  LeftUp.moveXY( 5, -5 );
  RightDown.moveXY( -5, 5 );

  const wrectangle EffRectangle = wrectangle( LeftUp, RightDown );

  drawRainbowRectangle( Painter, EffRectangle );
  drawRainbowRectangleBorder( Painter, EffRectangle );
  drawRainbowMarkers( Painter, EffRectangle, ScaleZ );
}

// ------------------------------------------------------------
      
void scigraphics::map::graphViewRectangle::drawRainbowRectangleBorder( painter &Painter, const wrectangle &Rectangle ) const
{
  const lineStyle BorderLineStyle( color::Black );
  const brushStyle BrushStyle( color::Yellow, brushStyle::None );
  const wpoint LeftUp = Rectangle.leftUp();
  const wpoint RightDown = wpoint( Rectangle.left() + rainbowRectangleWidth(), Rectangle.down() );
  Painter.drawRectangleW( wrectangle( LeftUp, RightDown ), BrushStyle, BorderLineStyle );
}

// ------------------------------------------------------------

void scigraphics::map::graphViewRectangle::drawRainbowRectangle( painter &Painter, const wrectangle &Rectangle ) const
{
  assert( PointColorStrategy != NULL );
  assert( Rectangle.up() >= Rectangle.down() );
  
  const unsigned NumOfRainbowLines = std::min<unsigned>( 200, Rectangle.height() );
  lineStyle NoneLineStyle( lineStyle::None );

  const wcoord RainbowRectangleWidth = rainbowRectangleWidth();

  for ( unsigned i = 0; i < NumOfRainbowLines; i++ )
  {
    const double Part0 = static_cast<double>(i)/(NumOfRainbowLines);
    const double Part1 = static_cast<double>(i+1)/(NumOfRainbowLines);
    
    const wcoord Y0 = static_cast<wcoord>( Rectangle.up() - Rectangle.height() * Part0 );
    const wcoord Y1 = static_cast<wcoord>( Rectangle.up() - Rectangle.height() * Part1 );
    const wcoord X0 = Rectangle.left();
    const wcoord X1 = Rectangle.left() + RainbowRectangleWidth;

    const color Color = PointColorStrategy->relativeValueColor( Part0 );
    const brushStyle BrushStyle( Color );
    NoneLineStyle.setColor( Color );
    Painter.drawRectangleW( wrectangle( wpoint(X0,Y0), wpoint(X1,Y1) ), BrushStyle, NoneLineStyle );
  }
}

// ------------------------------------------------------------
      
void scigraphics::map::graphViewRectangle::drawRainbowMarkers( painter &Painter, const wrectangle &Rectangle, const scale &ScaleZ ) const
{
  const std::vector<number> Marks = ScaleZ.marks();

  const wcoord RainbowRectangleWidth = rainbowRectangleWidth();

  const numberStyle NumberStyle( numberStyle::General );
  const lineStyle LineStyle( color::Black );
  const textStyle TextStyle( 13 );

  for ( size_t i = 0; i < Marks.size(); i++ )
  {
    const double Part = ScaleZ.numberToFraction( Marks[i] );
    
    const wcoord X0 = Rectangle.left();
    const wcoord X1 = Rectangle.left() + RainbowRectangleWidth;
    const wcoord Y  = static_cast<wcoord>( Rectangle.up() - Rectangle.height() * Part );
    
    Painter.drawLineW( wpoint(X0,Y), wpoint(X1,Y), LineStyle );
    Painter.drawTextW( NumberStyle.numberText(Marks[i]), wpoint( X1 + 4, Y ), painter::HLeft|painter::VCenter, TextStyle );
  }
}

// ------------------------------------------------------------
        
scigraphics::wcoord scigraphics::map::graphViewRectangle::rainbowRectangleWidth() const 
{ 
  return legendExampleWidth()/3; 
}

// ============================================================


