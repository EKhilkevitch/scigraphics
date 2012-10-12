
#pragma once

#include "graphics/numbers.h"
#include "graphics/painter.h"

namespace graphics
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

