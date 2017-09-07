
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

#if _WIN32
#  define _CRT_SECURE_NO_WARNINGS 1
#endif

#include "scigraphics/cursorpoint.h"
#include "scigraphics/painter.h"
#include "scigraphics/scale.h"

#include <cstdio>
#include <algorithm>
#include <iostream>

// ============================================================

const scigraphics::wrectangle scigraphics::cursorPositionViewer::InitCursorRectangle( wpoint(30,30), wpoint(30,30) );

// ============================================================

struct scigraphics::cursorPositionViewer::textSizesCache
{
  std::string Text;
  wcoord Width;
  wcoord Height;

  textSizesCache() :
    Width(0),
    Height(0) {}
};

// ============================================================
      
scigraphics::cursorPositionViewer::cursorPositionViewer() : 
  floatRectangle( InitCursorRectangle ),
  TextSizesCache( NULL )
{
}

// ------------------------------------------------------------

scigraphics::cursorPositionViewer::~cursorPositionViewer()
{
  delete TextSizesCache;
}

// ------------------------------------------------------------

unsigned scigraphics::cursorPositionViewer::textVerticalIndent() const 
{ 
  return std::max( getTextStyle().getFontSize()/3, 2U ); 
}

// ------------------------------------------------------------

unsigned scigraphics::cursorPositionViewer::textHorizontalIndent() const 
{ 
  return 5; 
}

// ------------------------------------------------------------

void scigraphics::cursorPositionViewer::draw( painter &Painter, const pairScales &Scales, const wpoint &Point )
{
  if ( ! isVisible() )
    return;

  const std::string Text = pointText( Painter, Scales, Point );

  updateRectangle( Painter, Text );
  drawBackground( Painter );
  drawText( Painter, Text );
}

// ------------------------------------------------------------

void scigraphics::cursorPositionViewer::updateTextSizesCacheIfNeed( painter &Painter, const std::string &Text )
{
  if ( TextSizesCache == NULL )
    TextSizesCache = new textSizesCache();

  if ( TextSizesCache->Text == Text )
  {
    //std::cout << "cache OK!" << std::endl;
    return;
  }

  {
    //std::cout << "cache fault" << std::endl;
    TextSizesCache->Text = Text;
    TextSizesCache->Width = Painter.textWidth( Text, getTextStyle() );
    TextSizesCache->Height = Painter.textHeight( Text, getTextStyle() );
  }
}

// ------------------------------------------------------------

void scigraphics::cursorPositionViewer::updateRectangle( painter &Painter, const std::string &Text )
{
  updateTextSizesCacheIfNeed( Painter, Text );
  
  const wcoord TextWidth  = TextSizesCache->Width;
  const wcoord TextHeight = TextSizesCache->Height;
  
  const wpoint LegendRightDownCorner = wpoint( std::max<wcoord>( getRectangle().left() + TextWidth + 2*textHorizontalIndent(), getRectangle().right() ), 
                                               std::min<wcoord>( getRectangle().up() - TextHeight - 2*textVerticalIndent(),  getRectangle().down()  ) );
  setRectangle( getRectangle().leftUp(), LegendRightDownCorner );
}

// ------------------------------------------------------------

void scigraphics::cursorPositionViewer::drawText( painter &Painter, const std::string &Text )
{
  wpoint Point = getRectangle().leftUp();
  Point.moveY( -static_cast<int>(textVerticalIndent()) );
  Point.moveX( +static_cast<int>(textHorizontalIndent()) );
  Painter.drawTextW( Text, Point, painter::HLeft|painter::VDown, getTextStyle() );
}

// ------------------------------------------------------------
      
std::string scigraphics::cursorPositionViewer::pointText( painter &Painter, const pairScales &Scales, const wpoint &Point ) const
{
#if _WIN32 && _MSC_VER
  int (__cdecl *snprintf)( char *buffer, size_t count, const char *format, ... ) = _snprintf;
#endif
  
  npoint NPoint = Scales.wpoint2npoint( Point, Painter );

  char Buffer[ 256 ];
  Buffer[ 0 ] = Buffer[ sizeof(Buffer)-1 ] = '\0';
  snprintf( Buffer, sizeof(Buffer)-1, "( %g : %g )", NPoint.x(), NPoint.y() );

  return Buffer;
}

// ============================================================



