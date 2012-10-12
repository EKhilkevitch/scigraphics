
#pragma once

// ============================================================

#include "graphics/selection.h"
#include "graphics/plotelementcollection.h"
#include "graphics/painter.h"

// ============================================================

namespace graphics 
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

