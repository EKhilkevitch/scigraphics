
// ======================================================

#include <QApplication>
#include <QtGui>
#include <cmath>

#include "scigraphics/plot.h"
#include "scigraphics/qt4/qt4plot.h"

// ======================================================

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

// ======================================================

int main( int argc, char *argv[] )
{
  QApplication app(argc,argv);

  qt4plot Plot;

  scigraphics::graphSequenceVector *Sin = Plot.createGraphSequenceVector( "Sinus" );
  for ( double x = -M_PI; x <= M_PI; x+= 0.1 )
    Sin->append( x, std::sin(x) );

  Plot.move( 200, 200 );
  Plot.resize( 700, 800 );
  Plot.show();
  Plot.replot();

  return app.exec();
}

// ======================================================


