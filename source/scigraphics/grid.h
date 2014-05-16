
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

#include "scigraphics/numbers.h"
#include "scigraphics/linestyle.h"

namespace scigraphics
{

  // ============================================================

  class scale;
  class painter;

  // ============================================================

  class grid
  {
    public:
      static const lineStyle DefaultGridLineStyle;

    private:
      lineStyle LineStyle;

    protected:
      virtual void drawMajorGrids( painter &Painter, const scale &Scale, const std::vector<number> &Ticks );
      virtual void drawMajorGridOneLine( painter &Painter, const scale &Scale, const number Value ) = 0;

    public:
      grid() : LineStyle(DefaultGridLineStyle) {}
      virtual ~grid() {}

      const lineStyle& getLineStyle() const { return LineStyle; }
      void setLineStyle( const lineStyle &Style ) { LineStyle = Style; }

      virtual void draw( painter &Painter, const scale &Scale, const std::vector<number> &Ticks );
  };

  class gridX : public grid 
  {
    protected:
      void drawMajorGridOneLine( painter &Painter, const scale &Scale, const number Value );
  };
  
  class gridY : public grid 
  {
    protected:
      void drawMajorGridOneLine( painter &Painter, const scale &Scale, const number Value );
  };

// ============================================================

}

