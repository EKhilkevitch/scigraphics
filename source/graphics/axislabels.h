
#pragma once

#include "graphics/numbers.h"
#include "graphics/painter.h"
#include "graphics/textstyle.h"
#include "graphics/numberstyle.h"
#include "graphics/scale.h"

namespace graphics
{

// ============================================================

  class axisLabels
  {
    public:
      static const textStyle DefaultAxisTextStyle;

    private:
      textStyle TextStyle;
      numberStyle *NumberStyle;
      
    protected:
      virtual void drawOneLabel( painter &Painter, const scale &Scale, number Value ) = 0;

    public:
      axisLabels() : TextStyle(DefaultAxisTextStyle), NumberStyle(new generalNumberStyle()) {} 
      virtual ~axisLabels() { delete NumberStyle; };

      const textStyle& getTextStyle() const { return TextStyle; }
      void setTextStyle( const textStyle &S ) { TextStyle = S; }

      const numberStyle& getNumberStyle() const { return *NumberStyle; }

      virtual void draw( painter &Painter, const scale &Scale, const std::vector<number> &Marks ); 
  };

  class axisLabelsX : public axisLabels
  {
    private:
      fcoord BaseY;
    protected:
      void drawOneLabel( painter &Painter, const scale &Scale, number Value );
    public:
      axisLabelsX( fcoord Y ) : BaseY(Y) {}
  };
  
  class axisLabelsY : public axisLabels
  {
    private:
      fcoord BaseX;
    protected:
      void drawOneLabel( painter &Painter, const scale &Scale, number Value );
    public:
      axisLabelsY( fcoord X ) : BaseX(X) {}
  };

// ============================================================

}

