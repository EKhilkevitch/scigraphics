
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

#include "scigraphics/graphviewcollection.h"

#include <iostream>
#include <cmath>
#include <limits>
#include <cassert>

// ============================================================
        
scigraphics::sequence::graphViewCollection::graphViewCollection() 
{
}

// ------------------------------------------------------------

scigraphics::sequence::graphViewCollection::~graphViewCollection() 
{
}

// ------------------------------------------------------------

void scigraphics::sequence::graphViewCollection::erase( graphView *View ) 
{  
  viewsList::iterator Iterator = find_pointer( Views.begin(), Views.end(), View );
  Views.erase( Iterator ); 
}

// ------------------------------------------------------------

void scigraphics::sequence::graphViewCollection::setAllViewsVisible( bool Show )
{
  for ( viewsList::iterator v = Views.begin(); v != Views.end(); ++v )
    v->setVisible( Show );
}

// ------------------------------------------------------------

void scigraphics::sequence::graphViewCollection::clearViews() 
{ 
  Views.clear(); 
}

// ------------------------------------------------------------
      
void scigraphics::sequence::graphViewCollection::addView( graphView *View, bool Show ) 
{ 
  if ( View == NULL )
    return;

  Views.push_back( View ); 
  View->setColor( DefaultColor );
  View->setVisible( Show );
}

// ------------------------------------------------------------
      
void scigraphics::sequence::graphViewCollection::draw( painter &Painter, const pairScales& Scales, const sequence::data &Data ) const
{
  for ( viewsList::const_iterator View = Views.begin(); View != Views.end(); ++View )
    if ( View->isVisible() )
      View->draw( Painter, Scales, Data );
}

// ------------------------------------------------------------

void scigraphics::sequence::graphViewCollection::drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const
{
  for ( viewsList::const_iterator View = Views.begin(); View != Views.end(); ++View )
    if ( View->isVisible() )
      View->drawLegendExample( Painter, Rectangle );
}
      
// ------------------------------------------------------------
      
scigraphics::color scigraphics::sequence::graphViewCollection::getColor() const 
{ 
  return DefaultColor; 
}
        
// ------------------------------------------------------------

void scigraphics::sequence::graphViewCollection::setDefaultColor( const color &Color ) 
{ 
  DefaultColor = Color; 
}

// ------------------------------------------------------------

void scigraphics::sequence::graphViewCollection::setColor( const color &Color )
{
  setDefaultColor( Color );
  for ( viewsList::iterator View = Views.begin(); View != Views.end(); ++View )
    View->setColor(Color);
}

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::sequence::graphViewCollection::legendExampleWidth() const
{
  wcoord Width = 0;
  for ( viewsList::const_iterator View = Views.begin(); View != Views.end(); ++View )
    Width = std::max( Width, View->legendExampleWidth() );
  return Width;
}

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::sequence::graphViewCollection::legendExampleHeight() const
{
  wcoord Height = 0;
  for ( viewsList::const_iterator View = Views.begin(); View != Views.end(); ++View )
    Height = std::max( Height, View->legendExampleHeight() );
  return Height;
}

// ============================================================

scigraphics::sequence::ordinarGraphViewCollection::ordinarGraphViewCollection()
{
  color Color = getColor();
  addView<graphViewLine>( lineStyle(Color), true );
  addView<graphViewPoints>( pointStyle(Color), false );
  addView<graphViewErrorBars>( errorBarStyle(Color), false );
  addView<graphViewLineHystogram>( lineStyle(Color), false );
}

// ------------------------------------------------------------

void scigraphics::sequence::ordinarGraphViewCollection::setLineStyle( lineStyle::style LineStyle )
{
  lineStyle Style = getViewStyle<graphViewLine>();
  Style.setStyle( LineStyle );
  setViewStyle<graphViewLine>( Style );
  
  Style = getViewStyle<graphViewLineHystogram>();
  Style.setStyle( LineStyle );
  setViewStyle<graphViewLineHystogram>( Style );
}

// ------------------------------------------------------------

void scigraphics::sequence::ordinarGraphViewCollection::setLineWidth( unsigned Width )
{
  graphViewGeneralLine *View = getView<graphViewLine>();
  if ( View != NULL )
    View->setLineWidth( Width );

  View = getView<graphViewLineHystogram>();
  if ( View != NULL )
    View->setLineWidth( Width );
}

// ------------------------------------------------------------

void scigraphics::sequence::ordinarGraphViewCollection::setPointSize( unsigned Size )
{
  graphViewPoints *View = getView<graphViewPoints>();
  if ( View != NULL )
    View->setPointSize( Size );
}

// ============================================================

scigraphics::sequence::coveredAreaGraphViewCollection::coveredAreaGraphViewCollection()
{
  color Color = getColor();
  addView<graphViewCoveredArea>( brushStyle(Color), true );
}

// ============================================================

