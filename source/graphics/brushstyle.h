
#pragma once

// ============================================================

#include "graphics/color.h"

// ============================================================

namespace scigraphics
{

// ============================================================

  class brushStyle 
  {
    public:
      enum style
      {
        None,
        Solid
      };

    private:
      style Style;
      color Color;
      int Transperancy;

    public:
      brushStyle( color C = color::Black, style S = Solid ) : Style(S), Color(C) {}
      
      style getStyle() const { return Style; }
      void setStyle( style S ) { Style = S; }

      const color& getColor() const { return Color; }
      void setColor( color C ) { Color = C; }
  };

// ======================================================

}

