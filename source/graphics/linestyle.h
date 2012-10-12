
#pragma once

// ============================================================

#include "graphics/color.h"

namespace scigraphics
{

// ============================================================

  class lineStyle
  {
    public:
      enum style
      {
        None,
        Solid,
        Dash
      };

    private:
      style Style;
      unsigned Width;
      color Color;

    protected:
      void setDefault() 
        { 
          Style = Solid;
          Width = 1;
          Color = color::Black;
        }
    
    public:
      lineStyle() { setDefault(); }
      lineStyle( style S ) { setDefault(); Style = S; }
      lineStyle( unsigned W ) { setDefault(); Width = W; }
      lineStyle( color C )    { setDefault(); Color = C; }
      lineStyle( unsigned W, color C ) { setDefault(); Color = C; Width = W; }

      unsigned width() const { return Style == None ? 0 : Width; }
      void setWidth( unsigned W ) { Width = W; }

      style getStyle() const { return Style; } 
      void setStyle( style S ) { Style = S; }

      color getColor() const { return Color; }
      void setColor( color C ) { Color = C; }
  };
  
// ======================================================

}

