
#pragma once

#include "scigraphics/numbers.h"
#include "scigraphics/geometry.h"
#include "scigraphics/linestyle.h"
#include "scigraphics/brushstyle.h"
#include "scigraphics/textstyle.h"

#include <string>
#include <vector>

namespace scigraphics
{

// ============================================================

  typedef point<wcoord> wpoint;
  typedef rectangle<wcoord> wrectangle;

  class drawer
  {
    public:
      enum textPosition { HLeft = 0x0001,       HRight = 0x0002,        HCenter = 0x0003,
                          VUp   = 0x0100,       VDown = 0x0200,         VCenter = 0x0300 };

    protected:
      static unsigned numOfLinesInString( const std::string &String );

    public:
      virtual wcoord width() const  = 0;
      virtual wcoord height() const = 0;
      wrectangle plotRectangle() const { return wrectangle( wpoint(0,0), wpoint(width(),height()) ); }

      virtual void eraseAll() { eraseRectangle( plotRectangle() ); }
      
      virtual void drawLine( const wpoint &A, const wpoint &B, const lineStyle& Style = lineStyle() ) = 0;
      virtual void drawRectangle( const wrectangle& Rectangle, const brushStyle& BrushStyle = brushStyle(), const lineStyle &LineStyle = lineStyle() ) = 0;
      virtual void drawPolygon( const std::vector<wpoint> &Points, const brushStyle& BrushStyle = brushStyle() ) = 0;
      virtual void drawText( const std::string &Text, const wrectangle& Rectangle, const textStyle &Style = textStyle(), double RotAngle = 0 ) = 0;
      virtual void eraseRectangle( const wrectangle& Rectangle );
      virtual void flush() {}

      virtual wcoord textWidth( const std::string &Text, const textStyle &Style = textStyle() );
      virtual wcoord textHeight( const std::string &Text, const textStyle &Style = textStyle() );
      
      virtual ~drawer() {}
  };

// ============================================================

}

