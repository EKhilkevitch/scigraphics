
#pragma once

#include "scigraphics/color.h"

namespace scigraphics
{

// ============================================================

  class selectionStyle
  {
    private:
      color BrushColor;
      color LineColor;

    public:
      selectionStyle( color BC = color::Yellow, color LC = color::Magenta ) : 
        BrushColor(BC), 
        LineColor(LC) 
      { 
        BrushColor.setTransparency(0.5);
      }

      const color& getBrushColor() const { return BrushColor; }
      void setBrushColor( color C ) { BrushColor = C; }

      const color& getLineColor() const { return LineColor; }
      void setLineColor( color C ) { LineColor = C; }
  };
  

// ======================================================

}

