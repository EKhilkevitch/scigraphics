
#pragma once

#include "scigraphics/numbers.h"
#include "scigraphics/linestyle.h"
#include "scigraphics/painter.h"

namespace scigraphics
{

// ============================================================

  class axis
  {
    public:
      static const lineStyle DefaultAxisLineStyle;

    private:
      lineStyle LineStyle;
      
    public:
      axis() : LineStyle(DefaultAxisLineStyle) {} 
      virtual ~axis() {};

      const lineStyle& getLineStyle() const { return LineStyle; }
      void setLineStyle( const lineStyle &S ) { LineStyle = S; }

      virtual void draw( painter &Painter ) = 0;
  };

  class axisX : public axis
  {
    private:
      fcoord BaseY;
    public:
      axisX( fcoord Y ) : BaseY(Y) {}
      void draw( painter &Painter );
  };

  class axisY : public axis
  {
    private:
      fcoord BaseX;
    public:
      axisY( fcoord X ) : BaseX(X) {}
      void draw( painter &Painter );
  };

// ============================================================

}
