
// ============================================================

#include "graphics/floatrect.h"
#include "graphics/brushstyle.h"

// ============================================================
     
unsigned graphics::floatRectangle::textHeight( painter &Painter, const std::string &Text, const textStyle &Style )
{ 
  return Painter.textHeight( Text, Style ); 
}

// ------------------------------------------------------------

void graphics::floatRectangle::drawBackground( painter &Painter ) const
{      
  brushStyle BackgroundBrush( color::White );
  Painter.drawRectangleW( Rectangle, BackgroundBrush, getBorderStyle() );
}

// ============================================================

