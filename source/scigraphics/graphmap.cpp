
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

#include "scigraphics/graphmap.h"
#include "scigraphics/scale.h"
#include "scigraphics/plotlimits.h"

#include <cassert>
#include <iostream>
#include <stdexcept>

// ============================================================

scigraphics::map::graph::graph( const std::string &Legend ) : 
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

scigraphics::map::graph::~graph()
{
  delete Data;
  delete ScaleZ;
}

// ------------------------------------------------------------

void scigraphics::map::graph::init()
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

void scigraphics::map::graph::updateScaleZInterval() const
{
  assert( Data != NULL );
  assert( ScaleZ != NULL );

  numberLimits LimitsZ = plotLimits::applyForcedLimitsMinMax( limitsZ(), ForcedIntervalZ );
  ScaleZ->setNumberLimits( LimitsZ );
}

// ------------------------------------------------------------

void scigraphics::map::graph::draw( painter &Painter, const pairScales& Scales ) const
{
  assert( View != NULL );
  assert( Data != NULL );
  assert( ScaleZ != NULL );

  updateScaleZInterval();

  View->draw( Painter, Scales, *Data, *ScaleZ );
}

// ------------------------------------------------------------

void scigraphics::map::graph::drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const
{
  assert( View != NULL );
  assert( Data != NULL );
  assert( ScaleZ != NULL );

  updateScaleZInterval();

  View->drawLegendExample( Painter, Rectangle, *ScaleZ );
}
      
// ------------------------------------------------------------

scigraphics::wcoord scigraphics::map::graph::legendExampleWidth()  const
{
  assert( View != NULL );
  return View->legendExampleWidth();
}

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::map::graph::legendExampleHeight() const
{
  assert( View != NULL );
  return View->legendExampleHeight();
}

// ============================================================

