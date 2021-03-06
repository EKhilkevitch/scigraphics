
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

#include "scigraphics/selectioncollection.h"
#include "scigraphics/selection.h"
#include "scigraphics/painter.h"
#include "scigraphics/scale.h"

#include <stdexcept>

// ============================================================
      
scigraphics::selection* scigraphics::selectionCollection::getSelectionOnPoint( const fpoint &Point )
{
  for ( reverse_iterator s = rbegin(); s != rend(); ++s )
  {
    pairScales Scales = getPairScales( &(*s) );
    if ( s->pointInSelection(Point,Scales) )
      return &( *s );
  }

  return NULL;
}

// ------------------------------------------------------------
      
void scigraphics::selectionCollection::setSelectionInterval( selectionStrip *Selection, fpoint Pt1, fpoint Pt2 )
{
  if ( ! exists(Selection) )
    throw std::invalid_argument( "Selection not exists in collection (in setSelectionInterval)" );

  const pairScales Scales = getPairScales( Selection );
  Selection->setIntervalPointsF( Pt1, Pt2, Scales );
}

// ------------------------------------------------------------
      
void scigraphics::selectionCollection::shiftSelection( selectionStrip *Selection, fpoint From, fpoint To )
{
  if ( ! exists(Selection) )
    throw std::invalid_argument( "Selection not exists in collection (in shiftSelection)" );
  
  const pairScales Scales = getPairScales( Selection );
  Selection->shiftPointsF( From, To, Scales );
}

// ============================================================


