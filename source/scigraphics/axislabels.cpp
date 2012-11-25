
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

#include "scigraphics/axislabels.h"

#include <cstdio>
#include <iostream>

// ============================================================

const scigraphics::textStyle scigraphics::axisLabels::DefaultAxisTextStyle( 10 );

// ------------------------------------------------------------

void scigraphics::axisLabels::draw( painter &Painter, const scale &Scale, const std::vector<number> &Marks )
{
  for ( std::vector<number>::const_iterator m = Marks.begin(); m != Marks.end(); ++m )
    drawOneLabel( Painter, Scale, *m );
}

// ------------------------------------------------------------

void scigraphics::axisLabelsX::drawOneLabel( painter &Painter, const scale &Scale, number Value )
{
  fcoord X = Scale.numberToFraction( Value );
  std::string Text = getNumberStyle().numberText( Value );
  unsigned Flags = ( BaseY < 0.5 ) ? drawer::HCenter|drawer::VUp : drawer::HCenter|drawer::VDown;
  int Shift = ( BaseY < 0.5 ? +1 : -1 ) * (-3);

//  std::cout << "axisTicksX: draw " << Text << " crd " << Coord << " clr " << getTextStyle().getColor().name() << std::endl;
  Painter.drawTextF( Text, fpoint(X,BaseY), Flags, getTextStyle(), 0, Shift );
}

// ------------------------------------------------------------

void scigraphics::axisLabelsY::drawOneLabel( painter &Painter, const scale &Scale, number Value )
{
  fcoord Y = Scale.numberToFraction( Value );
  std::string Text = getNumberStyle().numberText( Value );
  unsigned Flags = ( BaseX < 0.5 ) ? drawer::HRight|drawer::VCenter : drawer::HLeft|drawer::VCenter;
  int Shift = ( BaseX < 0.5 ? +1 : -1 ) * 6; 

  Painter.drawTextF( Text, fpoint(BaseX,Y), Flags, getTextStyle(), Shift, 0 );
}

// ============================================================

