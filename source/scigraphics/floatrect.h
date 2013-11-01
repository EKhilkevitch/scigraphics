
/*
 * Copyright 2011,2012 Evgeniy Khilkevitch 
 * 
 * This file is part of scigraphics.
 * 
 * Scigraphics is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Scigraphics is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with scigraphics.  If not, see <http://www.gnu.org/licenses/>.
 */


#pragma once

// ============================================================

#include "scigraphics/geometry.h"
#include "scigraphics/linestyle.h"

// ============================================================

namespace scigraphics
{

// ============================================================

  class painter;
  class textStyle;
  class pairScales;

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



