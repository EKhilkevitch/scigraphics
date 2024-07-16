
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

#include "scigraphics/graph.h"

// ============================================================

scigraphics::graph::graph() : 
  Visible(true),
  ShowLegend(true), 
  GridDrawOrder( plotElement::DrawOverGrid )
{
}

// ------------------------------------------------------------
      
scigraphics::graph::~graph()  
{
}

// ------------------------------------------------------------

std::string scigraphics::graph::legend() const 
{ 
  return Legend; 
}

// ------------------------------------------------------------

void scigraphics::graph::setLegend( const std::string &L ) 
{ 
  Legend = L; 
}

// ------------------------------------------------------------
      
bool scigraphics::graph::showLegend() const 
{ 
  return ShowLegend && isVisible(); 
}
      
// ------------------------------------------------------------

void scigraphics::graph::setShowLegend( bool Show ) 
{ 
  ShowLegend = Show; 
}

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::graph::legendExampleWidth() const  
{ 
  return 0; 
}

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::graph::legendExampleHeight() const 
{ 
  return 0; 
}
      
// ------------------------------------------------------------

void scigraphics::graph::setGridDrawOrder( gridDrawOrderType G ) 
{ 
  GridDrawOrder = G;
}

// ------------------------------------------------------------

scigraphics::graph::gridDrawOrderType scigraphics::graph::gridDrawOrder() const 
{ 
  return GridDrawOrder; 
}

// ------------------------------------------------------------

void scigraphics::graph::setVisible( bool V ) 
{ 
  Visible = V; 
}

// ------------------------------------------------------------

bool scigraphics::graph::isVisible() const 
{ 
  return Visible; 
}

// ============================================================


