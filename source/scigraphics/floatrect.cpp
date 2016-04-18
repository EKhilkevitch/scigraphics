
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


// ============================================================

#include "scigraphics/floatrect.h"
#include "scigraphics/painter.h"
#include "scigraphics/textstyle.h"
#include "scigraphics/brushstyle.h"

// ============================================================
      
scigraphics::floatRectangle::floatRectangle( const wrectangle &Rect ) : 
  Rectangle(Rect), 
  Visible(true) 
{
}

// ------------------------------------------------------------
      
scigraphics::floatRectangle::~floatRectangle() 
{
}

// ------------------------------------------------------------

const scigraphics::lineStyle& scigraphics::floatRectangle::getBorderStyle() const 
{ 
  return BorderStyle; 
}

// ------------------------------------------------------------

void scigraphics::floatRectangle::setBorderStyle( const lineStyle &S ) 
{ 
  BorderStyle = S; 
}

// ------------------------------------------------------------

const scigraphics::wrectangle& scigraphics::floatRectangle::getRectangle() const 
{ 
  return Rectangle; 
}

// ------------------------------------------------------------

void scigraphics::floatRectangle::setRectangle( const wrectangle &Rect ) 
{ 
  Rectangle = Rect; 
}

// ------------------------------------------------------------
      
void scigraphics::floatRectangle::setRectangle( const wpoint &A, const wpoint &B ) 
{ 
  setRectangle( wrectangle(A,B) ); 
}

// ------------------------------------------------------------
     
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
      
// ------------------------------------------------------------

void scigraphics::floatRectangle::move( const wcoord DX, const wcoord DY ) 
{ 
  Rectangle.moveXY( DX, DY ); 
}

// ------------------------------------------------------------

void scigraphics::floatRectangle::moveX( const wcoord DX ) 
{ 
  move(DX,0); 
}

// ------------------------------------------------------------

void scigraphics::floatRectangle::moveY( const wcoord DY ) 
{ 
  move(0,DY); 
}

// ------------------------------------------------------------

bool scigraphics::floatRectangle::containPoint( wpoint Point ) const 
{ 
  return Rectangle.contain(Point); 
}
      
// ------------------------------------------------------------

void scigraphics::floatRectangle::setVisible( bool V ) 
{ 
  Visible = V; 
}

// ------------------------------------------------------------

bool scigraphics::floatRectangle::isVisible() const 
{ 
  return Visible; 
}

// ============================================================

