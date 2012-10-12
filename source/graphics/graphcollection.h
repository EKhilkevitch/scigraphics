
#pragma once

// ============================================================

#include "graphics/graph.h"
#include "graphics/plotelementcollection.h"

#if _WIN32
  #pragma warning(1 : 4519)
#endif

// ============================================================

namespace graphics 
{

// ============================================================

  class graphCollection : public templatePlotElementsCollection<graph>
  {
    public:

      template < class G > G* create( const std::string &Legend = std::string() );
      template < class G > G* create( const std::string &Legend, const color &Color );
  };

// ============================================================

  template < class G > G* graphCollection::create( const std::string &Legend )
  {
    G *Graph = new G( Legend );
    append( Graph );
    return Graph;
  }

  template < class G > G* graphCollection::create( const std::string &Legend, const color &Color )
  {
    G *Graph = new G( Legend, Color );
    append( Graph );
    return Graph;
  }

// ============================================================

}

