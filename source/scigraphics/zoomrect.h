
#pragma once

#include "scigraphics/numbers.h"
#include "scigraphics/painter.h"

namespace scigraphics
{

// ============================================================

  class zoomRectangle
  {
    private:
      wpoint InitPoint, CursorPoint;
      bool Visible;
    public:
      zoomRectangle();
      virtual ~zoomRectangle() {}

      void show( wpoint Point );
      void pull( wpoint Point );
      void hide();

      void draw( painter &Painter );
  };

// ============================================================

}

