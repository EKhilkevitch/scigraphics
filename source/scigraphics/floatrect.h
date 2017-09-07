
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
      void drawBackground( painter &Painter ) const;

    public:
      explicit floatRectangle( const wrectangle &Rectangle = wrectangle() );
      virtual ~floatRectangle();

      const lineStyle& getBorderStyle() const;
      void setBorderStyle( const lineStyle &Style );

      const wrectangle& getRectangle() const;
      void setRectangle( const wrectangle &Rectangle );
      void setRectangle( const wpoint &A, const wpoint &B );

      virtual void move( const wcoord DX, const wcoord DY );
      void moveX( const wcoord DX );
      void moveY( const wcoord DY );
      bool containPoint( wpoint Point ) const;
      
      void setVisible( bool Visible );
      bool isVisible() const;
  };

  // ============================================================

}



