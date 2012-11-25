
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

#include "scigraphics/plotelement.h"
#include "scigraphics/numlimits.h"
#include "scigraphics/painter.h"

#include <string>

// ============================================================

namespace scigraphics 
{

// ============================================================

  class graph : public plotElement
  {
    private:
      bool ShowLegend;
      bool DrawOverGrid;
      std::string Legend;

    public:
      graph() : ShowLegend(true), DrawOverGrid(true) {}
      virtual ~graph()  {}

      virtual numberLimits limitsX() const = 0;
      virtual numberLimits limitsY( const interval<number> &LimitsX ) const = 0;

      virtual std::string legend() const { return Legend; }
      virtual void setLegend( const std::string &L ) { Legend = L; }
      virtual bool showLegend() const { return ShowLegend; }
      virtual void setShowLegend( bool Enable ) { ShowLegend = Enable; }
      virtual wcoord legendExampleWidth() const  { return 0; }
      virtual wcoord legendExampleHeight() const { return 0; }

      virtual void draw( painter &Painter, const pairScales& Scales ) const = 0;
      virtual void drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const = 0;

      void setDrawOverGrid( bool D ) { DrawOverGrid = D; }
      virtual bool isDrawOverGrid() const { return DrawOverGrid; }
  };

// ============================================================

}

