
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

  class painter;
  class scale;

// ============================================================

  class axisTicks
  {
    public:
      static const lineStyle DefaultAxisTickStyle;

    private:
      lineStyle TickStyle;
      wcoord Length;
      
    protected:
      virtual void drawOneTick( painter &Painter, const scale &Scale, number Value ) = 0;

    public:
      axisTicks() : TickStyle(DefaultAxisTickStyle), Length(8) {} 
      virtual ~axisTicks() {};

      const lineStyle& getTickStyle() const { return TickStyle; }
      wcoord getTickLength() const { return Length; }

      virtual void draw( painter &Painter, const scale &Scale, const std::vector<number> &Marks ); 
  };

  class axisTicksX : public axisTicks
  {
    private:
      fcoord BaseY;
    protected:
      void drawOneTick( painter &Painter, const scale &Scale, number Value );
    public:
      axisTicksX( fcoord Y ) : BaseY(Y) {}
  };
  
  class axisTicksY : public axisTicks
  {
    private:
      fcoord BaseX;
    protected:
      void drawOneTick( painter &Painter, const scale &Scale, number Value );
    public:
      axisTicksY( fcoord X ) : BaseX(X) {}
  };

// ============================================================

}

