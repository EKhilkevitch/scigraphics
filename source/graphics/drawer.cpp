
// ============================================================

#include "graphics/drawer.h"

#include <cassert>

// ============================================================

unsigned graphics::drawer::numOfLinesInString( const std::string &String )
{
  unsigned Count = 0;
  for ( std::string::const_iterator c = String.begin(); c != String.end(); ++c ) 
    if ( *c == '\n' )
      Count++;
  return Count + 1;
}

// ------------------------------------------------------------

void graphics::drawer::eraseRectangle( const wrectangle& Rectangle )
{
  color Color = color::White;
  brushStyle BrushStyle( Color );
  lineStyle LineStyle( Color );

  drawRectangle( Rectangle, BrushStyle, LineStyle ); 
}

// ------------------------------------------------------------
      
graphics::wcoord graphics::drawer::textWidth( const std::string &Text, const textStyle &Style )       
{ 
  return Text.length() * Style.getFontSize(); 
}

// ------------------------------------------------------------

graphics::wcoord graphics::drawer::textHeight( const std::string &Text, const textStyle &Style )
{ 
  return Style.getFontSize() * numOfLinesInString(Text); 
}

// ============================================================
      

