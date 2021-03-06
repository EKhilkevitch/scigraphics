
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

#include "scigraphics/geometry.h"

namespace scigraphics
{

  // ============================================================

  class painter;

  // ============================================================

  class zoomRectangle
  {
    private:
      wpoint InitPoint, CursorPoint;
      bool Visible;

    public:
      zoomRectangle();
      ~zoomRectangle() {}

      void show( wpoint Point );
      void pull( wpoint Point );
      void hide();

      void draw( painter &Painter );
  };

// ============================================================

}

