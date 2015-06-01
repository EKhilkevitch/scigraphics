
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

#include "scigraphics/numbers.h"
#include "scigraphics/linestyle.h"

// ============================================================

namespace scigraphics
{

  // ============================================================

  class painter;

  // ============================================================

  class axis
  {
    public:
      static const lineStyle DefaultAxisLineStyle;

    private:
      lineStyle LineStyle;
      
    public:
      axis();
      virtual ~axis();

      const lineStyle& getLineStyle() const { return LineStyle; }
      void setLineStyle( const lineStyle &S ) { LineStyle = S; }

      virtual void draw( painter &Painter ) = 0;
  };

  class axisX : public axis
  {
    private:
      fcoord BaseY;

    public:
      explicit axisX( fcoord Y );
      void draw( painter &Painter );
  };

  class axisY : public axis
  {
    private:
      fcoord BaseX;

    public:
      explicit axisY( fcoord X );
      void draw( painter &Painter );
  };

  // ============================================================

}

