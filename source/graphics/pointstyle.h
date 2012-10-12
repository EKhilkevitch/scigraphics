
#pragma once

#include "graphics/color.h"

#include <cstdio>
#include <string>

namespace graphics
{

// ============================================================

  class pointStyle
  {
    public:
      enum shape
      {
        None,
        Square,
        Cyrcle
      };

    private:
      shape Shape;
      unsigned Width;
      color Color;

    public:

      pointStyle( const color& C = color::Black ) : Shape(Square), Width(4), Color(C) {}

      shape getShape() const { return Shape; } 
      void setShape( shape S ) { Shape = S; }

      unsigned width()   const { return Width; }
      void setWidth( unsigned W ) { Width = W; }

      const color& getColor() const { return Color; }
      void setColor( color C ) { Color = C; }
  };
  
// ======================================================

}

