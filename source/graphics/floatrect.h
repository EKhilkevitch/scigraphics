
#pragma once

// ============================================================

#include "graphics/geometry.h"
#include "graphics/painter.h"
#include "graphics/linestyle.h"
#include "graphics/textstyle.h"

// ============================================================

namespace scigraphics
{

// ============================================================

  class floatRectangle
  {
    private:
      wrectangle Rectangle;
      lineStyle BorderStyle;

      bool Visible;

    protected:
      static unsigned textHeight( painter &Painter, const std::string &Text, const textStyle &Style );
      void drawBackground( painter &Painter ) const;

    public:
      floatRectangle( const wrectangle &Rect = wrectangle() ) : Rectangle(Rect), Visible(true) {}
      virtual ~floatRectangle() {}

      const lineStyle& getBorderStyle() const { return BorderStyle; }
      void setBorderStyle( const lineStyle &S ) { BorderStyle = S; }

      const wrectangle& getRectangle() const { return Rectangle; }
      void setRectangle( const wrectangle &Rect ) { Rectangle = Rect; }
      void setRectangle( const wpoint &A, const wpoint &B ) { setRectangle( wrectangle(A,B) ); }

      virtual void move( const wcoord DX, const wcoord DY ) {  Rectangle.moveXY( DX, DY ); }
      void moveX( const wcoord DX ) { move(DX,0); }
      void moveY( const wcoord DY ) { move(0,DY); }
      bool containPoint( wpoint Point ) const { return Rectangle.contain(Point); }
      
      void setVisible( bool V ) { Visible = V; }
      bool isVisible() const { return Visible; }
  };


// ============================================================

}



