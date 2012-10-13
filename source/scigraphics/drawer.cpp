
// ============================================================

#include "scigraphics/drawer.h"

#include <cassert>

// ============================================================

unsigned scigraphics::drawer::numOfLinesInString( const std::string &String )
{
  unsigned Count = 0;
  for ( std::string::const_iterator c = String.begin(); c != String.end(); ++c ) 
    if ( *c == '\n' )
      Count++;
  return Count + 1;
}

// ------------------------------------------------------------

void scigraphics::drawer::eraseRectangle( const wrectangle& Rectangle )
{
  color Color = color::White;
  brushStyle BrushStyle( Color );
  lineStyle LineStyle( Color );

  drawRectangle( Rectangle, BrushStyle, LineStyle ); 
}

// ------------------------------------------------------------
      
scigraphics::wcoord scigraphics::drawer::textWidth( const std::string &Text, const textStyle &Style )       
{ 
  return Text.length() * Style.getFontSize(); 
}

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::drawer::textHeight( const std::string &Text, const textStyle &Style )
{ 
  return Style.getFontSize() * numOfLinesInString(Text); 
}

// ============================================================
      

