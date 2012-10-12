
#pragma once

#include "graphics/color.h"
#include "graphics/linestyle.h"

namespace graphics
{

// ============================================================

  class errorBarStyle
  {
    private:
      color Color;
      int HatWidth;
      int LineWidth;

    public:
      errorBarStyle( color C = color::Black ) : Color(C), HatWidth(8), LineWidth(1) {}

      int hatWidth() const { return HatWidth; }
      int lineWidth() const { return LineWidth; }

      const color& getColor() const { return Color; }
      void setColor( color C ) { Color = C; } 

      lineStyle getLineStyle() const { return lineStyle( lineWidth(), getColor() ); }
  };
  
// ======================================================

}

