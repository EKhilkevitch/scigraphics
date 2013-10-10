
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

void scigraphics::graphViewSequencesCollection::erase( graphViewSequence *View ) 
{  
  viewsList::iterator Iterator = find_pointer( Views.begin(), Views.end(), View );
  Views.erase( Iterator ); 
}

// ------------------------------------------------------------
      
void scigraphics::graphViewSequencesCollection::addView( graphViewSequence *View, bool Show ) 
{ 
  if ( View == NULL )
    return;

  Views.push_back( View ); 
  View->setColor( DefaultColor );
  View->setVisible( Show );
}

// ------------------------------------------------------------
      
void scigraphics::graphViewSequencesCollection::draw( painter &Painter, const pairScales& Scales, const dataSequence &Data ) const
{
  for ( viewsList::const_iterator View = Views.begin(); View != Views.end(); ++View )
    if ( View->isVisible() )
      View->draw( Painter, Scales, Data );
}

// ------------------------------------------------------------

void scigraphics::graphViewSequencesCollection::drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const
{
  for ( viewsList::const_iterator View = Views.begin(); View != Views.end(); ++View )
    if ( View->isVisible() )
      View->drawLegendExample( Painter, Rectangle );
}
      
// ------------------------------------------------------------
      
scigraphics::color scigraphics::graphViewSequencesCollection::getColor() const 
{ 
  return DefaultColor; 
}

// ------------------------------------------------------------

void scigraphics::graphViewSequencesCollection::setDefaultColor( const color &Color ) 
{ 
  DefaultColor = Color; 
}

// ------------------------------------------------------------

void scigraphics::graphViewSequencesCollection::setColor( const color &Color )
{
  setDefaultColor( Color );
  for ( viewsList::iterator View = Views.begin(); View != Views.end(); ++View )
    View->setColor(Color);
}

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::graphViewSequencesCollection::legendExampleWidth() const
{
  wcoord Width = 0;
  for ( viewsList::const_iterator View = Views.begin(); View != Views.end(); ++View )
    Width = std::max( Width, View->legendExampleWidth() );
  return Width;
}

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::graphViewSequencesCollection::legendExampleHeight() const
{
  wcoord Height = 0;
  for ( viewsList::const_iterator View = Views.begin(); View != Views.end(); ++View )
    Height = std::max( Height, View->legendExampleHeight() );
  return Height;
}

// ============================================================

scigraphics::ordinarGraphSequenceViewCollection::ordinarGraphSequenceViewCollection()
{
  color Color = getColor();
  addView<graphViewLine>( lineStyle(Color), true );
  addView<graphViewPoints>( pointStyle(Color), false );
  addView<graphViewErrorBars>( errorBarStyle(Color), false );
  addView<graphViewLineHystogram>( lineStyle(Color), false );
}

// ------------------------------------------------------------

void scigraphics::ordinarGraphSequenceViewCollection::setLineWidth( unsigned Width )
{
  lineStyle Style = getViewStyle<graphViewLine>();
  Style.setWidth( Width );
  setViewStyle<graphViewLine>( Style );
}

// ------------------------------------------------------------

void scigraphics::ordinarGraphSequenceViewCollection::setPointSize( unsigned Size )
{
  pointStyle Style = getViewStyle<graphViewPoints>();
  Style.setWidth( Size );
  setViewStyle<graphViewPoints>( Style );
}

// ============================================================

scigraphics::coveredAreaGraphSequenceViewCollection::coveredAreaGraphSequenceViewCollection()
{
  color Color = getColor();
  addView<graphViewCoveredArea>( brushStyle(Color), true );
}

// ============================================================

