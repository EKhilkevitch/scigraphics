
#pragma once

// ============================================================

#include "textstyle.h"
#include "floatrect.h"

// ============================================================

namespace scigraphics
{

// ============================================================

  class cursorPositionViewer : public floatRectangle
  {
    private:
      static const wrectangle InitCursorRectangle;
      textStyle TextStyle;

    protected:
      void updateRectangle( const std::string &Text, drawer &Drawer );
      
      unsigned textVerticalIndent() const { return std::max( getTextStyle().getFontSize()/3, 2U ); }
      unsigned textHorizontalIndent() const { return 5; }
  
      void drawText( painter &Painter, const std::string &Text );
      void updateRectangle( painter &Painter, const std::string &Text );
      std::string pointText( painter &Painter, const pairScales &Scales, const wpoint &Point ) const;

    public:
      cursorPositionViewer();
      
      const textStyle& getTextStyle() const { return TextStyle; }
      void setTextStyle( const textStyle &S ) { TextStyle = S; }

      void draw( painter &Painter, const pairScales &Scales, const wpoint &Point );
  };

// ============================================================

}


