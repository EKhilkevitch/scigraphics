
#pragma once

// ============================================================

#include "scigraphics/selection.h"
#include "scigraphics/plotelementcollection.h"
#include "scigraphics/painter.h"

// ============================================================

namespace scigraphics 
{

// ============================================================

  class selectionCollection : public templatePlotElementsCollection<selection>
  {
    public:
      template <class S> S* create();

      selection* getSelectionOnPoint( const fpoint &Point ); 
      void setSelectionInterval( selectionStrip *Selection, fpoint Pt1, fpoint Pt2 );
      void shiftSelection( selectionStrip *Selection, fpoint From, fpoint To );
  };

// ============================================================

  template <class S> S* selectionCollection::create()
  {
    S *Selection = new S();
    append(Selection);
    return Selection;
  }

// ============================================================

}

