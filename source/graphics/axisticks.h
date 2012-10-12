
#pragma once

#include "graphics/numbers.h"
#include "graphics/painter.h"
#include "graphics/linestyle.h"
#include "graphics/scale.h"

namespace graphics
{

// ============================================================

  class axisTicks
  {
    public:
      static const lineStyle DefaultAxisTickStyle;

    private:
      lineStyle TickStyle;
      wcoord Length;
      
    protected:
      virtual void drawOneTick( painter &Painter, const scale &Scale, number Value ) = 0;

    public:
      axisTicks() : TickStyle(DefaultAxisTickStyle), Length(8) {} 
      virtual ~axisTicks() {};

      const lineStyle& getTickStyle() const { return TickStyle; }
      wcoord getTickLength() const { return Length; }

      virtual void draw( painter &Painter, const scale &Scale, const std::vector<number> &Marks ); 
  };

  class axisTicksX : public axisTicks
  {
    private:
      fcoord BaseY;
    protected:
      void drawOneTick( painter &Painter, const scale &Scale, number Value );
    public:
      axisTicksX( fcoord Y ) : BaseY(Y) {}
  };
  
  class axisTicksY : public axisTicks
  {
    private:
      fcoord BaseX;
    protected:
      void drawOneTick( painter &Painter, const scale &Scale, number Value );
    public:
      axisTicksY( fcoord X ) : BaseX(X) {}
  };

// ============================================================

}

