
#pragma once

// ============================================================

#include "graphics/color.h"
#include <string>

// ============================================================

namespace graphics
{

// ============================================================

  class textStyle 
  {
    private:
      std::string Font;
      unsigned FontSize;
      color Color;

    public:
      textStyle( unsigned FS = 12 ) : Font(), FontSize(FS), Color() {}

      const color& getColor() const { return Color; }
      void setColor( color C ) { Color = C; }

      unsigned getFontSize() const { return FontSize; }
      void setFontSize( unsigned S ) { FontSize = S; }

      void setFontName( const std::string &F ) { Font = F; }
      const std::string& getFontName() const { return Font; }
  };
  
// ======================================================

}

