
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


// =========================================================

#include "mockpainter.h"
#include "painter.h"

// =========================================================

void scigraphics::mockDrawer::drawLine( const wpoint &A, const wpoint &B )
{
  action Act;
  Act.Type = action::DrawLine;
  Act.Points.push_back(A);
  Act.Points.push_back(B);
 // Act.Color = Style.getColor();
  Actions.push_back(Act);
}

// ---------------------------------------------------------

void scigraphics::mockDrawer::drawRectangle( const wrectangle& Rectangle )
{
  action Act;
  Act.Type = action::DrawRectangle;
  Act.Points.push_back(Rectangle.leftUp());
  Act.Points.push_back(Rectangle.rightDown());
 // Act.Color = BrushStyle.getColor();
  Actions.push_back(Act);
}

// ---------------------------------------------------------

void scigraphics::mockDrawer::drawText( const std::string &, const wrectangle& , double  )
{
}

// =========================================================

std::ostream& scigraphics::operator<<( std::ostream &Stream, mockDrawer::action::type Type )
{

#define CASE_RET( Value ) \
  case mockDrawer::action::Value: return Stream << #Value;

  switch ( Type )
  {
    CASE_RET( None );
    CASE_RET( DrawLine );
    CASE_RET( DrawRectangle );
    default:
      return Stream << "UNKNOWN";
  }
}

// =========================================================

