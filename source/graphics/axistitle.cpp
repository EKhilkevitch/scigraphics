
// ============================================================

#include "graphics/axistitle.h"

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

