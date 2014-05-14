
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


#pragma once

// ============================================================

#include "scigraphics/graph.h"
#include "scigraphics/plotelementcollection.h"

#if _WIN32
  #pragma warning(1 : 4519)
#endif

// ============================================================

namespace scigraphics 
{

// ============================================================

  class color;

// ============================================================

  class graphCollection : public templatePlotElementsCollection<graph>
  {
    public:

      template < class G > G* create();
      template < class G > G* create( const color &Color );
      template < class G > G* create( const std::string &Legend );
      template < class G > G* create( const std::string &Legend, const color &Color );
  };

  // ============================================================
  
  template < class G > G* graphCollection::create()
  {
    G *Graph = new G();
    append( Graph );
    return Graph;
  }
  
  // ------------------------------------------------------------

  template < class G > G* graphCollection::create( const color &Color )
  {
    G *Graph = new G( Color );
    append( Graph );
    return Graph;
  }
  
  // ------------------------------------------------------------

  template < class G > G* graphCollection::create( const std::string &Legend )
  {
    G *Graph = new G( Legend );
    append( Graph );
    return Graph;
  }
  
  // ------------------------------------------------------------

  template < class G > G* graphCollection::create( const std::string &Legend, const color &Color )
  {
    G *Graph = new G( Legend, Color );
    append( Graph );
    return Graph;
  }

  // ============================================================

}

