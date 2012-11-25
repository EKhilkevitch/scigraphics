
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

#include "scigraphics/geometry.h"
#include "scigraphics/textstyle.h"
#include "scigraphics/painter.h"

// ============================================================

namespace scigraphics
{

// ============================================================

  class axisTitle 
  {
    public:
      static const textStyle DefaultAxisTextStyle;

    private:
      textStyle TextStyle;

      std::string Title;

    public:
      axisTitle() : TextStyle( DefaultAxisTextStyle ) {} 

      const std::string& title() const { return Title; }
      void setTitle( const std::string &S ) { Title = S; }
      
      const textStyle& getTextStyle() const { return TextStyle; }
      void setTextStyle( const textStyle &S ) { TextStyle = S; }

      virtual void draw( painter &Painter ) const = 0;

      virtual ~axisTitle() {}
  };

  class axisTitleX : public axisTitle
  {
    private:
      fcoord BaseY;
    public:
      axisTitleX( fcoord Y ) : BaseY(Y) {}
      void draw( painter &Painter ) const;
  };
  
  class axisTitleY : public axisTitle
  {
    private:
      fcoord BaseX;
    public:
      axisTitleY( fcoord X ) : BaseX(X) {}
      void draw( painter &Painter ) const;
  };


// ============================================================

}

