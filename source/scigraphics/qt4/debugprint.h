
#pragma once

// ============================================================

#include "scigraphics/numbers.h"

class QDebug;

namespace scigraphics
{
  class color;
  class numberLimits;
  template <class T> class interval;

  namespace sequence
  {
    class point;
  }
}

// ============================================================

QDebug operator<<( QDebug Out, scigraphics::color Color );
QDebug operator<<( QDebug Out, const scigraphics::numberLimits &Limits );
QDebug operator<<( QDebug Out, const scigraphics::interval<scigraphics::number> &Interval );
QDebug operator<<( QDebug Out, const scigraphics::sequence::point &Point );

// ============================================================

