
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

#include "scigraphics/textstyle.h"
#include "scigraphics/floatrect.h"

#include <string>

// ============================================================

namespace scigraphics
{

// ============================================================

  class cursorPositionViewer : public floatRectangle
  {
    private:
      struct textSizesCache;

    private:
      static const wrectangle InitCursorRectangle;
      textStyle TextStyle;

      textSizesCache *TextSizesCache;

    private:
      unsigned textVerticalIndent() const;
      unsigned textHorizontalIndent() const;

      void updateTextSizesCacheIfNeed( painter &Painter, const std::string &Text );
  
      void drawText( painter &Painter, const std::string &Text );
      void updateRectangle( painter &Painter, const std::string &Text );
      std::string pointText( painter &Painter, const pairScales &Scales, const wpoint &Point ) const;

      cursorPositionViewer( const cursorPositionViewer& );
      cursorPositionViewer& operator=( const cursorPositionViewer& );

    public:
      cursorPositionViewer();
      ~cursorPositionViewer();
      
      const textStyle& getTextStyle() const { return TextStyle; }
      void setTextStyle( const textStyle &S ) { TextStyle = S; }

      void draw( painter &Painter, const pairScales &Scales, const wpoint &Point );
  };

// ============================================================

}


