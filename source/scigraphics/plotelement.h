
#pragma once

// ============================================================

namespace scigraphics 
{

// ============================================================

  class painter;
  class pairScales;

// ============================================================

  class plotElement
  {
    public:
      virtual void draw( painter &Painter, const pairScales& Scales ) const = 0;
      virtual bool isDrawOverGrid() const { return true; }
      virtual ~plotElement() {};
  };

// ============================================================

}

