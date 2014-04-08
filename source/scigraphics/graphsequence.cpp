
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

// ============================================================

void scigraphics::sequence::graph::draw( painter &Painter, const pairScales& Scales ) const
{
  assert( Views != NULL );
  assert( Data != NULL );

  Views->draw( Painter, Scales, *Data );
}

// ------------------------------------------------------------

void scigraphics::sequence::graph::drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const
{
  assert( Views != NULL );
  Views->drawLegendExample( Painter, Rectangle );
}
      
// ------------------------------------------------------------

scigraphics::wcoord scigraphics::sequence::graph::legendExampleWidth()  const
{
  assert( Views != NULL );
  return Views->legendExampleWidth();
}

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::sequence::graph::legendExampleHeight() const
{
  assert( Views != NULL );
  return Views->legendExampleHeight();
}

// ------------------------------------------------------------

scigraphics::sequence::graph::graph( const std::string &Lgnd ) : 
  Views(NULL), 
  Data(NULL)
{
  setLegend( Lgnd );
}

// ------------------------------------------------------------

void scigraphics::sequence::graph::setData( data *D )
{
  if ( D != Data )
  {
    delete Data;
    Data = D;
  }
}

// ------------------------------------------------------------

void scigraphics::sequence::graph::setViews( graphViewCollection *V )
{
  if ( V != Views )
  {
    delete Views;
    Views = V;
  }
}

// ------------------------------------------------------------

scigraphics::sequence::graph::~graph()
{
  delete Views;
  delete Data;
}

// ============================================================


