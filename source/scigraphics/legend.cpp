
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
#include <iostream>
#include <cassert>

// ============================================================

const scigraphics::wrectangle scigraphics::legend::InitLegendRectangle( wpoint(0,0), wpoint(0,0) );

// ------------------------------------------------------------

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

  do
  {
    LegendSizes = sizesForLegendRectangle( Painter, Style, Graphics );
    if ( LegendSizes.height() < Painter.height() )
      break;
    Style.setFontSize( Style.getFontSize() - 1 );
  } while ( Style.getFontSize() > minFontSize() );

  setRectangleFromLegendSize( Painter, LegendSizes );
  return Style;
}

// ------------------------------------------------------------

scigraphics::wrectangle scigraphics::legend::createInitialRectangle( painter &Painter ) 
{
  wpoint Point( Painter.width() - 30, 90 );
  return wrectangle( Point, Point );
}

// ------------------------------------------------------------
      
scigraphics::legend::legendSize scigraphics::legend::sizesForLegendRectangle( painter &Painter, const textStyle &Style, const graphCollection &Graphics )
{
  const wcoord VerticalDistance = interTextVerticalDistance(Style);
  
  legendSize Size;
  for ( graphCollection::const_iterator g = Graphics.begin(); g != Graphics.end(); ++g )
  {
    const graph *Graph = *g;
    assert( Graph != NULL );

    if ( shouldDrawGraphLegend(*Graph) )
    {
      wcoord Width  = Graph->legendExampleWidth() + Painter.textWidth(  Graph->legend(), Style );
      wcoord Height = std::max( Graph->legendExampleHeight(), Painter.textHeight( Graph->legend(), Style ) );
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
  wpoint LeftUp = getRectangle().leftUp();
  wpoint RightDown( LeftUp.x() + LegendSize.width(), LeftUp.y() - LegendSize.height() );

  wrectangle LegendRectangle( LeftUp, RightDown );

  if ( LegendRectangle.width() < Painter.plotWidth() )
    while ( LegendRectangle.right() > Painter.plotRectangle().right() ) 
      LegendRectangle.moveX( -1 );

  if ( LegendRectangle.height() < Painter.plotHeight() )
    while ( LegendRectangle.down() < 0 )
      LegendRectangle.moveY( +1 );

  setRectangle( LegendRectangle );
}

// ------------------------------------------------------------

std::list<std::string> scigraphics::legend::legendsList( const graphCollection &Graphics )
{
  std::list< std::string > Legends;
  for ( graphCollection::const_iterator g = Graphics.begin(); g != Graphics.end(); ++g )
    if ( shouldDrawGraphLegend(**g) )
      Legends.push_back( (*g)->legend() );

  return Legends;
}

// ------------------------------------------------------------

bool scigraphics::legend::shouldDrawLegend( const graphCollection &Graphics )
{
  for ( graphCollection::const_iterator g = Graphics.begin(); g != Graphics.end(); ++g )
    if ( shouldDrawGraphLegend(**g) )
      return true;
  return false;
}

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::legend::drawGraphLegend( painter &Painter, wcoord y, const graph &Graph, const textStyle &Style )
{
  const wcoord TextHeight = textHeight( Painter, Graph.legend(), Style );

  wcoord x = getRectangle().left() + textHorizontalIndent();
  wcoord ExampleWidth = Graph.legendExampleWidth();
  wcoord GraphLegendHeight = std::max( Graph.legendExampleHeight(), TextHeight );

  Graph.drawLegendExample( Painter, wrectangle( wpoint(x,y-GraphLegendHeight), wpoint(x+ExampleWidth,y) ) );
  
  x += ExampleWidth + textHorizontalIndent();
  Painter.drawTextW( Graph.legend(), wpoint(x,y-GraphLegendHeight/2), drawer::HLeft|drawer::VCenter, Style );

  return GraphLegendHeight;
}
      
// ------------------------------------------------------------

void scigraphics::legend::drawAllLegends( painter &Painter, const graphCollection &Graphics, const textStyle &Style )
{
  wcoord y = getRectangle().up() - interTextVerticalDistance(Style);
  for ( graphCollection::const_reverse_iterator Graph = Graphics.rbegin(); Graph != Graphics.rend(); ++Graph )
  {
    if ( shouldDrawGraphLegend( **Graph ) )
    {
      wcoord LegendHeight = drawGraphLegend( Painter, y, **Graph, Style );
      y -=  LegendHeight + interTextVerticalDistance(Style);
    }
  }
}

// ------------------------------------------------------------

void scigraphics::legend::draw( painter &Painter, const graphCollection &Graphics ) 
{
  if ( ! isVisible() )
    return;

  if ( ! shouldDrawLegend(Graphics) )
    return;

  textStyle CurrTextStyle = updateLegendRectangle( Painter, Graphics );
  if ( getRectangle().width() <= 1 )
    return;
  
  drawBackground( Painter );
  drawAllLegends( Painter, Graphics, CurrTextStyle );
}

// ============================================================

