
#pragma once

// ============================================================

#include "graphics/plotelement.h"
#include "graphics/numlimits.h"
#include "graphics/painter.h"

#include <string>

// ============================================================

namespace graphics 
{

// ============================================================

  class graph : public plotElement
  {
    private:
      bool ShowLegend;
      std::string Legend;

    public:
      graph() : ShowLegend(true) {}
      virtual ~graph()  {}

      virtual numberLimits limitsX() const = 0;
      virtual numberLimits limitsY( const interval<number> &LimitsX ) const = 0;

      virtual std::string legend() const { return Legend; }
      virtual void setLegend( const std::string &L ) { Legend = L; }
      virtual bool showLegend() const { return ShowLegend; }
      virtual void setShowLegend( bool Enable ) { ShowLegend = Enable; }
      virtual wcoord legendExampleWidth() const  { return 0; }
      virtual wcoord legendExampleHeight() const { return 0; }

      virtual void draw( painter &Painter, const pairScales& Scales ) const = 0;
      virtual void drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const = 0;

  };

// ============================================================

}

