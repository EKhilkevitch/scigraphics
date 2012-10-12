
// ============================================================

#include "graphics/selectioncollection.h"

// ============================================================
      
scigraphics::selection* scigraphics::selectionCollection::getSelectionOnPoint( const fpoint &Point )
{
  for ( iterator s = begin(); s != end(); ++s )
  {
    pairScales Scales = getPairScales( *s );
    if ( (*s)->pointInSelection(Point,Scales) )
      return *s;
  }

  return NULL;
}

// ------------------------------------------------------------
      
void scigraphics::selectionCollection::setSelectionInterval( selectionStrip *Selection, fpoint Pt1, fpoint Pt2 )
{
  if ( ! exist(Selection) )
    throw std::invalid_argument( "Selection not exist in collection (in setSelectionInterval)" );

  pairScales Scales = getPairScales( Selection );
  Selection->setIntervalPointsF( Pt1, Pt2, Scales );
}

// ------------------------------------------------------------
      
void scigraphics::selectionCollection::shiftSelection( selectionStrip *Selection, fpoint From, fpoint To )
{
  if ( ! exist(Selection) )
    throw std::invalid_argument( "Selection not exist in collection (in shiftSelection)" );
  
  pairScales Scales = getPairScales( Selection );
  Selection->shiftPointsF( From, To, Scales );
}

// ============================================================


