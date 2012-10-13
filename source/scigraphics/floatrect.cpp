
// ============================================================

#include "scigraphics/floatrect.h"
#include "scigraphics/brushstyle.h"

// ============================================================
     
unsigned scigraphics::floatRectangle::textHeight( painter &Painter, const std::string &Text, const textStyle &Style )
{ 
  return Painter.textHeight( Text, Style ); 
}

// ------------------------------------------------------------

void scigraphics::floatRectangle::drawBackground( painter &Painter ) const
{      
  brushStyle BackgroundBrush( color::White );
  Painter.drawRectangleW( Rectangle, BackgroundBrush, getBorderStyle() );
}

// ============================================================

