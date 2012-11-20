
// ============================================================

#include "scigraphics/graphsequence.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

// ============================================================

void scigraphics::graphSequence::draw( painter &Painter, const pairScales& Scales ) const
{
  assert( Views != NULL );
  assert( Data != NULL );

  Views->draw( Painter, Scales, *Data );
}

// ------------------------------------------------------------

void scigraphics::graphSequence::drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const
{
  assert( Views != NULL );
  Views->drawLegendExample( Painter, Rectangle );
}
      
// ------------------------------------------------------------

scigraphics::wcoord scigraphics::graphSequence::legendExampleWidth()  const
{
  assert( Views != NULL );
  return Views->legendExampleWidth();
}

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::graphSequence::legendExampleHeight() const
{
  assert( Views != NULL );
  return Views->legendExampleHeight();
}

// ------------------------------------------------------------

scigraphics::graphSequence::graphSequence( const std::string &Lgnd ) : Views(NULL), Data(NULL)
{
  setLegend( Lgnd );
}

// ------------------------------------------------------------

void scigraphics::graphSequence::init( const color &Color )
{
  assert( Data == NULL );
  assert( Views == NULL );

  Data = createData();
  if ( Data == NULL )
    throw std::runtime_error( "createData() must return valid pointer" );

  Views = createViewCollection();
  if ( Views == NULL )
    throw std::runtime_error( "createGraphViewCollection() must return valid pointer" );
  Views->setColor( Color );
}

// ------------------------------------------------------------

scigraphics::graphSequence::~graphSequence()
{
  delete Views;
  delete Data;
}

// ============================================================


