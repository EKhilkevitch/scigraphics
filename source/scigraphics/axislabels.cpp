
// ============================================================

#include "graphics/axislabels.h"

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

