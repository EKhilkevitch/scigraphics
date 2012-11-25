
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

#include "scigraphics/axistitle.h"

// ============================================================

const scigraphics::textStyle scigraphics::axisTitle::DefaultAxisTextStyle( 12 );

// ============================================================

void scigraphics::axisTitleX::draw( painter &Painter ) const 
{
  if ( title().empty() )
    return;

  unsigned Flags = ( BaseY < 0.5 ) ? drawer::HCenter|drawer::VUp : drawer::HCenter|drawer::VDown;
  int Shift = - ( BaseY < 0.5 ? +1 : -1 ) * (20);

  Painter.drawTextW( title(), Painter.fpoint2wpoint(fpoint(0.5,BaseY)), Flags, getTextStyle(), 0, Shift );
}

// ============================================================

void scigraphics::axisTitleY::draw( painter &Painter ) const 
{
  if ( title().empty() )
    return;

  wcoord Y = Painter.fcoord2wcoordY(0.5);
  wcoord X = BaseX < 0.5 ? 0 : Painter.plotWidth();

  unsigned Flags = ( BaseX < 0.5 ) ? drawer::HCenter|drawer::VCenter : drawer::HCenter|drawer::VCenter;
  int Shift = - ( BaseX < 0.5 ? +1 : -1 ) * ( 5 + getTextStyle().getFontSize());
  double Angle = ( BaseX < 0.5 ? -1 : +1 ) * 90;

  Painter.drawTextW( title(), wpoint(X,Y), Flags, getTextStyle(), Shift, 0, Angle );
}

// ============================================================

