
// ============================================================

#include "scigraphics/graphmap.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

// ============================================================

scigraphics::graphMap::graphMap( const std::string &Legend ) : 
  View(NULL),
  Data(NULL)
{
  setLegend(Legend);
}

// ------------------------------------------------------------

scigraphics::graphMap::~graphMap()
{
  delete Data;
}

// ------------------------------------------------------------

void scigraphics::graphMap::init()
{
  assert( Data == NULL );
  assert( View == NULL );

  Data = createData();
  if ( Data == NULL )
    throw std::runtime_error( "createData() must return valid pointer" );

  View = createView();
  if ( View == NULL )
    throw std::runtime_error( "createView() must return valid pointer" );
}

// ------------------------------------------------------------

void scigraphics::graphMap::draw( painter &Painter, const pairScales& Scales ) const
{
  assert( View != NULL );
  assert( Data != NULL );

  View->draw( Painter, Scales, *Data );
}

// ------------------------------------------------------------

void scigraphics::graphMap::drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const
{
  assert( View != NULL );
  assert( Data != NULL );

  View->drawLegendExample( Painter, Rectangle, Data->limitsZ() );
}
      
// ------------------------------------------------------------

scigraphics::wcoord scigraphics::graphMap::legendExampleWidth()  const
{
  assert( View != NULL );
  return View->legendExampleWidth();
}

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::graphMap::legendExampleHeight() const
{
  assert( View != NULL );
  return View->legendExampleHeight();
}

// ============================================================

