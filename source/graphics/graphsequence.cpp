
// ============================================================

#include "graphics/graphsequence.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

// ============================================================

void graphics::graphSequence::draw( painter &Painter, const pairScales& Scales ) const
{
  assert( Views != NULL );
  assert( Data != NULL );

  Views->draw( Painter, Scales, *Data );
}

// ------------------------------------------------------------

void graphics::graphSequence::drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const
{
  assert( Views != NULL );
  Views->drawLegendExample( Painter, Rectangle );
}
      
// ------------------------------------------------------------

graphics::wcoord graphics::graphSequence::legendExampleWidth()  const
{
  assert( Views != NULL );
  return Views->legendExampleWidth();
}

// ------------------------------------------------------------

graphics::wcoord graphics::graphSequence::legendExampleHeight() const
{
  assert( Views != NULL );
  return Views->legendExampleHeight();
}

// ------------------------------------------------------------

graphics::graphSequence::graphSequence( const std::string &Lgnd ) : Views(NULL), Data(NULL)
{
  setLegend( Lgnd );
}

// ------------------------------------------------------------

void graphics::graphSequence::init( const color &Color )
{
  assert( Data == NULL );
  assert( Views == NULL );

  Data = createData();
  if ( Data == NULL )
    throw std::runtime_error( "createData() must return valid pointer" );

  Views = createViewCollection();
  if ( Views == NULL )
    throw std::runtime_error( "createGraphViewCollection() must return valid pointer" );
  Views->setColor( Color == color::White ? color::nextColor() : Color );
}

// ------------------------------------------------------------

graphics::graphSequence::~graphSequence()
{
  delete Views;
  delete Data;
}

// ============================================================


