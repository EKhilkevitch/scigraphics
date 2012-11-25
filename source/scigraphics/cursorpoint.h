
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

#include "textstyle.h"
#include "floatrect.h"

// ============================================================

namespace scigraphics
{

// ============================================================

  class cursorPositionViewer : public floatRectangle
  {
    private:
      static const wrectangle InitCursorRectangle;
      textStyle TextStyle;

    protected:
      void updateRectangle( const std::string &Text, drawer &Drawer );
      
      unsigned textVerticalIndent() const { return std::max( getTextStyle().getFontSize()/3, 2U ); }
      unsigned textHorizontalIndent() const { return 5; }
  
      void drawText( painter &Painter, const std::string &Text );
      void updateRectangle( painter &Painter, const std::string &Text );
      std::string pointText( painter &Painter, const pairScales &Scales, const wpoint &Point ) const;

    public:
      cursorPositionViewer();
      
      const textStyle& getTextStyle() const { return TextStyle; }
      void setTextStyle( const textStyle &S ) { TextStyle = S; }

      void draw( painter &Painter, const pairScales &Scales, const wpoint &Point );
  };

// ============================================================

}


