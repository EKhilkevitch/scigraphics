
// ============================================================

#include "graphics/graphmap.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

// ============================================================

graphics::graphMap::graphMap( const std::string &Legend ) : 
  View(NULL),
  Data(NULL)
{
  setLegend(Legend);
}

// ------------------------------------------------------------

graphics::graphMap::~graphMap()
{
  delete Data;
}

// ------------------------------------------------------------

void graphics::graphMap::init()
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

void graphics::graphMap::draw( painter &Painter, const pairScales& Scales ) const
{
  assert( View != NULL );
  assert( Data != NULL );

  View->draw( Painter, Scales, *Data );
}

// ------------------------------------------------------------

void graphics::graphMap::drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const
{
  assert( View != NULL );
  assert( Data != NULL );

  View->drawLegendExample( Painter, Rectangle, Data->limitsZ() );
}
      
// ------------------------------------------------------------

graphics::wcoord graphics::graphMap::legendExampleWidth()  const
{
  assert( View != NULL );
  return View->legendExampleWidth();
}

// ------------------------------------------------------------

graphics::wcoord graphics::graphMap::legendExampleHeight() const
{
  assert( View != NULL );
  return View->legendExampleHeight();
}

// ============================================================

