
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
#include "scigraphics/geometry.h"

// ============================================================

namespace scigraphics
{

// ============================================================

  class painter;

// ============================================================

  class axisTitle 
  {
    public:
      static const textStyle DefaultAxisTextStyle;

    private:
      textStyle TextStyle;
      std::string Title;

    public:
      axisTitle();
      virtual ~axisTitle();

      const std::string& title() const;
      void setTitle( const std::string &Title );
      
      const textStyle& getTextStyle() const;
      void setTextStyle( const textStyle &Style );

      virtual void draw( painter &Painter ) const = 0;

  };

  class axisTitleX : public axisTitle
  {
    private:
      fcoord BaseY;
    public:
      explicit axisTitleX( fcoord Y );
      void draw( painter &Painter ) const;
  };
  
  class axisTitleY : public axisTitle
  {
    private:
      fcoord BaseX;
    public:
      explicit axisTitleY( fcoord X );
      void draw( painter &Painter ) const;
  };


// ============================================================

}

