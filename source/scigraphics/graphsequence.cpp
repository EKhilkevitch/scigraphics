
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


