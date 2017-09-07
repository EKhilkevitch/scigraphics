
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

#include "scigraphics/legend.h"
#include "scigraphics/textstyle.h"
#include "scigraphics/graphcollection.h"
#include "scigraphics/graph.h"
#include "scigraphics/painter.h"

#include <iostream>
#include <algorithm>
#include <cassert>

// ============================================================

const scigraphics::wrectangle scigraphics::legend::InitLegendRectangle( wpoint(0,0), wpoint(0,0) );

// ============================================================

scigraphics::legend::legendSize::legendSize() :
  Width(0), 
  Height(0) 
{
}

// ------------------------------------------------------------
          
scigraphics::legend::legendSize::legendSize( wcoord W, wcoord H ) : 
  Width(W), 
  Height(H) 
{
}

// ------------------------------------------------------------
          
void scigraphics::legend::legendSize::updateWidth( wcoord W )  
{ 
  Width = std::max( W, Width ); 
}

// ------------------------------------------------------------
          
void scigraphics::legend::legendSize::updateHeight( wcoord H ) 
{ 
  Height += H; 
}

// ============================================================


bool scigraphics::legend::shouldDrawGraphLegend( const graph &Graph ) 
{ 
  if ( ! Graph.showLegend() )
    return false;
  if ( Graph.legend().empty() )
    return false;
  return true;
} 

// ------------------------------------------------------------

scigraphics::textStyle scigraphics::legend::updateLegendRectangle( painter &Painter, const graphCollection &Graphics )
{
  if ( getRectangle() == InitLegendRectangle )
    setRectangle( createInitialRectangle(Painter) );

  std::list<std::string> Legends = legendsList(Graphics);
  if ( Legends.empty() )
  {
    setRectangle( getRectangle().leftUp(), getRectangle().leftUp() );
    return getLegendTextStyle();
  }

  textStyle Style = getLegendTextStyle();
  legendSize LegendSizes;

  while ( true )
  {
    LegendSizes = sizesForLegendRectangle( Painter, Style, Graphics );
    if ( Style.getFontSize() <= minFontSize() )
      break;
    if ( LegendSizes.height() < Painter.height() )
      break;
    if ( Style.getFontSize() <= minFontSize() )
      break;
    Style.setFontSize( Style.getFontSize() - 1 );
  }

  setRectangleFromLegendSize( Painter, LegendSizes );
  return Style;
}

// ------------------------------------------------------------

scigraphics::wrectangle scigraphics::legend::createInitialRectangle( painter &Painter ) 
{
  const wpoint Point( Painter.width() - 30, 90 );
  return wrectangle( Point, Point );
}

// ------------------------------------------------------------
      
scigraphics::legend::legendSize scigraphics::legend::sizesForLegendRectangle( painter &Painter, const textStyle &Style, const graphCollection &Graphics )
{
  const wcoord VerticalDistance = interTextVerticalDistance(Style);
  
  legendSize Size;
  for ( graphCollection::const_iterator Graph = Graphics.begin(); Graph != Graphics.end(); ++Graph )
  {
    assert( &*Graph != NULL );

    if ( shouldDrawGraphLegend(*Graph) )
    {
      const wcoord Width  = Graph->legendExampleWidth() + Painter.textWidth(  Graph->legend(), Style );
      const wcoord Height = std::max( Graph->legendExampleHeight(), Painter.textHeight( Graph->legend(), Style ) );
      Size.updateWidth( Width + 3*textHorizontalIndent() );
      Size.updateHeight( Height + VerticalDistance );
    }
  }

  if ( Size.height() > 0 )
    Size.addToHeight( VerticalDistance );
      
  return Size;
}

// ------------------------------------------------------------

void scigraphics::legend::setRectangleFromLegendSize( painter &Painter, const legendSize &LegendSize )
{
  const wpoint LeftUp = getRectangle().leftUp();
  const wpoint RightDown( LeftUp.x() + LegendSize.width(), LeftUp.y() - LegendSize.height() );

  wrectangle LegendRectangle( LeftUp, RightDown );

  if ( LegendRectangle.width() < Painter.plotWidth() )
  {
    while ( LegendRectangle.right() > Painter.plotWidth() ) 
      LegendRectangle.moveX( -1 );
  }

  if ( LegendRectangle.height() < Painter.plotHeight() )
  {
    while ( LegendRectangle.down() < 0 )
      LegendRectangle.moveY( +1 );
  }

  setRectangle( LegendRectangle );
}

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::legend::interTextVerticalDistance( const textStyle &Style ) 
{ 
  return std::max<wcoord>( Style.getFontSize()/3, 2 ); 
}
      
// ------------------------------------------------------------

scigraphics::wcoord scigraphics::legend::textHorizontalIndent() 
{ 
  return 5; 
}

// ------------------------------------------------------------

unsigned scigraphics::legend::minFontSize() 
{ 
  return 5; 
}

// ------------------------------------------------------------

std::list<std::string> scigraphics::legend::legendsList( const graphCollection &Graphics )
{
  std::list< std::string > Legends;
  
  for ( graphCollection::const_iterator g = Graphics.begin(); g != Graphics.end(); ++g )
  {
    if ( shouldDrawGraphLegend(*g) )
      Legends.push_back( g->legend() );
  }

  return Legends;
}

// ------------------------------------------------------------

bool scigraphics::legend::shouldDrawLegend( const graphCollection &Graphics )
{
  for ( graphCollection::const_iterator g = Graphics.begin(); g != Graphics.end(); ++g )
  {
    if ( shouldDrawGraphLegend(*g) )
      return true;
  }
  return false;
}

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::legend::drawGraphLegend( painter &Painter, wcoord y, const graph &Graph, const textStyle &Style )
{
  const wcoord TextHeight = Painter.textHeight( Graph.legend(), Style );

  const wcoord XForExample = getRectangle().left() + textHorizontalIndent();
  const wcoord ExampleWidth = Graph.legendExampleWidth();
  const wcoord GraphLegendHeight = std::max( Graph.legendExampleHeight(), TextHeight );

  Graph.drawLegendExample( Painter, wrectangle( wpoint( XForExample, y-GraphLegendHeight ), wpoint(XForExample+ExampleWidth,y) ) );
  
  const wcoord XForText = XForExample + ExampleWidth + textHorizontalIndent();
  Painter.drawTextW( Graph.legend(), wpoint( XForText, y-GraphLegendHeight/2 ), painter::HLeft|painter::VCenter, Style );

  return GraphLegendHeight;
}
      
// ------------------------------------------------------------

void scigraphics::legend::drawAllLegends( painter &Painter, const graphCollection &Graphics, const textStyle &Style )
{
  wcoord y = getRectangle().up() - interTextVerticalDistance(Style);

  for ( graphCollection::const_reverse_iterator Graph = Graphics.rbegin(); Graph != Graphics.rend(); ++Graph )
  {
    if ( shouldDrawGraphLegend( *Graph ) )
    {
      const wcoord LegendHeight = drawGraphLegend( Painter, y, *Graph, Style );
      y -=  LegendHeight + interTextVerticalDistance(Style);
    }
  }
}

// ------------------------------------------------------------

scigraphics::legend::legend() : 
  floatRectangle(InitLegendRectangle) 
{
}

// ------------------------------------------------------------

void scigraphics::legend::draw( painter &Painter, const graphCollection &Graphics ) 
{
  if ( ! isVisible() )
    return;

  if ( ! shouldDrawLegend(Graphics) )
    return;

  const textStyle CurrTextStyle = updateLegendRectangle( Painter, Graphics );
  if ( getRectangle().width() <= 1 )
    return;
  
  drawBackground( Painter );
  drawAllLegends( Painter, Graphics, CurrTextStyle );
}

// ============================================================

