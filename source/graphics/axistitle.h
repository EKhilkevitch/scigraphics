
#pragma once

// ============================================================

#include "graphics/geometry.h"
#include "graphics/textstyle.h"
#include "graphics/painter.h"

// ============================================================

namespace graphics
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

