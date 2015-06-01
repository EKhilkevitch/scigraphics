
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

#include "scigraphics/numbers.h"
#include "scigraphics/textstyle.h"
#include "scigraphics/numberstyle.h"

#include <vector>

// ============================================================

namespace scigraphics
{

// ============================================================

  class painter;
  class scale;

// ============================================================

  class axisLabels
  {
    public:
      static const textStyle DefaultAxisTextStyle;

    private:
      textStyle TextStyle;
      numberStyle NumberStyle;

    private:
      axisLabels( const axisLabels& );
      axisLabels& operator=( const axisLabels& );
      
    protected:
      virtual void drawOneLabel( painter &Painter, const scale &Scale, number Value ) = 0;

    public:
      axisLabels(); 
      virtual ~axisLabels();

      const textStyle& getTextStyle() const { return TextStyle; }
      void setTextStyle( const textStyle &S ) { TextStyle = S; }

      const numberStyle& getNumberStyle() const { return NumberStyle; }
      void setNumberStyle( const numberStyle &S ) { NumberStyle = S; }

      virtual void draw( painter &Painter, const scale &Scale, const std::vector<number> &Marks ); 
  };

  class axisLabelsX : public axisLabels
  {
    private:
      fcoord BaseY;

    protected:
      void drawOneLabel( painter &Painter, const scale &Scale, number Value );

    public:
      explicit axisLabelsX( fcoord Y );
  };
  
  class axisLabelsY : public axisLabels
  {
    private:
      fcoord BaseX;

    protected:
      void drawOneLabel( painter &Painter, const scale &Scale, number Value );

    public:
      explicit axisLabelsY( fcoord X );
  };

// ============================================================

}

