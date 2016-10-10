
// ============================================================

#include "scigraphics/qt4/debugprint.h"
#include "scigraphics/color.h"
#include "scigraphics/interval.h"
#include "scigraphics/numlimits.h"
#include "scigraphics/datasequence.h"

#include <QDebug>
#include <QString>

#include <sstream>
#include <string>

// ============================================================

namespace
{

  // ------------------------------------------------------------
  
  template <class T> QString toQtStringByStdStream( const T &Value )
  {
    std::stringstream Stream;
    Stream << Value;
    std::string String = Stream.str();
    return QString::fromStdString( String );
  }

  // ------------------------------------------------------------

}

// ============================================================

QDebug operator<<( QDebug Out, scigraphics::color Color )
{
  Out.nospace() << toQtStringByStdStream( Color );
  return Out.space();
}

// ------------------------------------------------------------

QDebug operator<<( QDebug Out, const scigraphics::numberLimits &Limits )
{
  Out.nospace() << toQtStringByStdStream( Limits );
  return Out.space();
}

// ------------------------------------------------------------

QDebug operator<<( QDebug Out, const scigraphics::interval<scigraphics::number> &Interval )
{
  Out.nospace() << toQtStringByStdStream( Interval );
  return Out.space();
}

// ------------------------------------------------------------

QDebug operator<<( QDebug Out, const scigraphics::sequence::point &Point )
{
  Out.nospace() << toQtStringByStdStream( Point );
  return Out.space();
}

// ============================================================

