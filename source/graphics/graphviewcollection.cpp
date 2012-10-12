
// ============================================================

#include "graphics/graphviewcollection.h"

#include <iostream>
#include <cmath>
#include <limits>
#include <cassert>

// ============================================================

void graphics::graphViewSequencesCollection::erase( graphViewSequence *View ) 
{  
  viewsList::iterator Iterator = Views.find(View);
  Views.erase( Iterator ); 
}

// ------------------------------------------------------------
      
void graphics::graphViewSequencesCollection::addView( graphViewSequence *View, bool Show ) 
{ 
  if ( View == NULL )
    return;

  Views.push_back( View ); 
  View->setColor( DefaultColor );
  View->setVisible( Show );
}

// ------------------------------------------------------------
      
void graphics::graphViewSequencesCollection::draw( painter &Painter, const pairScales& Scales, const dataSequence &Data ) const
{
  for ( viewsList::const_iterator View = Views.begin(); View != Views.end(); ++View )
    if ( (*View)->isVisible() )
      (*View)->draw( Painter, Scales, Data );
}

// ------------------------------------------------------------

void graphics::graphViewSequencesCollection::drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const
{
  for ( viewsList::const_iterator View = Views.begin(); View != Views.end(); ++View )
    if ( (*View)->isVisible() )
      (*View)->drawLegendExample( Painter, Rectangle );
}
      
// ------------------------------------------------------------
      
graphics::color graphics::graphViewSequencesCollection::getColor() const 
{ 
  return DefaultColor; 
}

// ------------------------------------------------------------

void graphics::graphViewSequencesCollection::setDefaultColor( const color &Color ) 
{ 
  DefaultColor = Color; 
}

// ------------------------------------------------------------

void graphics::graphViewSequencesCollection::setColor( const color &Color )
{
  setDefaultColor( Color );
  for ( viewsList::const_iterator View = Views.begin(); View != Views.end(); ++View )
    (*View)->setColor(Color);
}

// ------------------------------------------------------------

graphics::wcoord graphics::graphViewSequencesCollection::legendExampleWidth() const
{
  wcoord Width = 0;
  for ( viewsList::const_iterator View = Views.begin(); View != Views.end(); ++View )
    Width = std::max( Width, (*View)->legendExampleWidth() );
  return Width;
}

// ------------------------------------------------------------

graphics::wcoord graphics::graphViewSequencesCollection::legendExampleHeight() const
{
  wcoord Height = 0;
  for ( viewsList::const_iterator View = Views.begin(); View != Views.end(); ++View )
    Height = std::max( Height, (*View)->legendExampleHeight() );
  return Height;
}

// ============================================================

graphics::ordinarGraphSequenceViewCollection::ordinarGraphSequenceViewCollection()
{
  color Color = getColor();
  addView<graphViewLine>( lineStyle(Color), true );
  addView<graphViewPoints>( pointStyle(Color), false );
  addView<graphViewErrorBars>( errorBarStyle(Color), false );
  addView<graphViewLineHystogram>( lineStyle(Color), false );
}

// ------------------------------------------------------------

void graphics::ordinarGraphSequenceViewCollection::setLineWidth( unsigned Width )
{
  lineStyle Style = getViewStyle<graphViewLine>();
  Style.setWidth( Width );
  setViewStyle<graphViewLine>( Style );
}

// ------------------------------------------------------------

void graphics::ordinarGraphSequenceViewCollection::setPointSize( unsigned Size )
{
  pointStyle Style = getViewStyle<graphViewPoints>();
  Style.setWidth( Size );
  setViewStyle<graphViewPoints>( Style );
}

// ============================================================

graphics::coveredAreaGraphSequenceViewCollection::coveredAreaGraphSequenceViewCollection()
{
  color Color = getColor();
  addView<graphViewCoveredArea>( brushStyle(Color), true );
}

// ============================================================

