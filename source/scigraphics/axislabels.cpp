
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
#include "scigraphics/painter.h"
#include "scigraphics/scale.h"

#include <stdexcept>
#include <string>
#include <iostream>

// ============================================================

const scigraphics::textStyle scigraphics::axisLabels::DefaultAxisTextStyle( 10 );

// ------------------------------------------------------------

scigraphics::axisLabels::axisLabels() :
  TextStyle(DefaultAxisTextStyle), 
  NumberStyle(numberStyle::General) 
{
}

// ------------------------------------------------------------

scigraphics::axisLabels::~axisLabels()
{
}

// ------------------------------------------------------------

const scigraphics::textStyle& scigraphics::axisLabels::getTextStyle() const 
{ 
  return TextStyle; 
}

// ------------------------------------------------------------

void scigraphics::axisLabels::setTextStyle( const textStyle &S ) 
{ 
  TextStyle = S; 
}

// ------------------------------------------------------------

const scigraphics::numberStyle& scigraphics::axisLabels::getNumberStyle() const 
{ 
  return NumberStyle; 
}

// ------------------------------------------------------------

void scigraphics::axisLabels::setNumberStyle( const numberStyle &S ) 
{ 
  NumberStyle = S; 
}

// ------------------------------------------------------------

void scigraphics::axisLabels::draw( painter &Painter, const scale &Scale, const std::vector<number> &Marks ) const
{
  for ( std::vector<number>::const_iterator m = Marks.begin(); m != Marks.end(); ++m )
    drawOneLabel( Painter, Scale, *m );
}

// ============================================================

scigraphics::axisLabelsX::axisLabelsX( fcoord Y ) : 
  BaseY(Y) 
{
}

// ------------------------------------------------------------

void scigraphics::axisLabelsX::drawOneLabel( painter &Painter, const scale &Scale, number Value ) const
{
  const fcoord X = Scale.numberToFraction( Value );
  const std::string Text = getNumberStyle().numberText( Value );
  
  const unsigned Flags = ( BaseY < static_cast<fcoord>(0.5) ) ? painter::HCenter|painter::VUp : painter::HCenter|painter::VDown;
  const int Shift = ( BaseY < static_cast<fcoord>(0.5) ? +1 : -1 ) * (-3);

//  std::cout << "axisTicksX: draw " << Text << " crd " << Coord << " clr " << getTextStyle().getColor().name() << std::endl;
  Painter.drawTextF( Text, fpoint(X,BaseY), Flags, getTextStyle(), 0, Shift );
}

// ============================================================
      
scigraphics::axisLabelsY::axisLabelsY( fcoord X ) : 
  BaseX(X) 
{
}

// ------------------------------------------------------------

void scigraphics::axisLabelsY::drawOneLabel( painter &Painter, const scale &Scale, number Value ) const
{
  const fcoord Y = Scale.numberToFraction( Value );
  const std::string Text = getNumberStyle().numberText( Value );
  const unsigned Flags = ( BaseX < static_cast<fcoord>(0.5) ) ? painter::HRight|painter::VCenter : painter::HLeft|painter::VCenter;
  const int Shift = ( BaseX < static_cast<fcoord>(0.5) ? +1 : -1 ) * 6; 

  Painter.drawTextF( Text, fpoint(BaseX,Y), Flags, getTextStyle(), Shift, 0 );
//  std::cout << "axisLabelsY::drawOneLabel: " << Text << " - " << BaseX << " " << Y << " " << Shift << std::endl;
}

// ============================================================

