
// ============================================================

#include "scigraphics/graphmap.h"
#include "scigraphics/scale.h"
#include "scigraphics/plotlimits.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

// ============================================================

scigraphics::graphMap::graphMap( const std::string &Legend ) : 
  View(NULL),
  Data(NULL),
  ScaleZ(NULL),
  ForcedIntervalZ( plotLimits::autoScaleInterval() )
{
  ScaleZ = new scaleLinear();
  ScaleZ->setNumberOfMarks( 7, 15 );

  setDrawOverGrid( false );
  setLegend(Legend);
}

// ------------------------------------------------------------

scigraphics::graphMap::~graphMap()
{
  delete Data;
  delete ScaleZ;
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

void scigraphics::graphMap::updateScaleZInterval() const
{
  assert( Data != NULL );
  assert( ScaleZ != NULL );

  numberLimits LimitsZ = plotLimits::applyForcedLimitsMinMax( limitsZ(), ForcedIntervalZ );
  ScaleZ->setNumberLimits( LimitsZ );
}

// ------------------------------------------------------------

void scigraphics::graphMap::draw( painter &Painter, const pairScales& Scales ) const
{
  assert( View != NULL );
  assert( Data != NULL );
  assert( ScaleZ != NULL );

  updateScaleZInterval();

  View->draw( Painter, Scales, *Data, *ScaleZ );
}

// ------------------------------------------------------------

void scigraphics::graphMap::drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const
{
  assert( View != NULL );
  assert( Data != NULL );
  assert( ScaleZ != NULL );

  updateScaleZInterval();

  View->drawLegendExample( Painter, Rectangle, *ScaleZ );
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

