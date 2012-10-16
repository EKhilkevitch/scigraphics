
// ======================================================

#include <QApplication>
#include <QtGui>
#include <cmath>
#include <sstream>
#include <cstdlib>

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

  const size_t NumberOfGraphics = 330*4;
  const size_t NumberOfPointsPerGraph = 2000;

  for ( unsigned i = 0; i < 100; i++ )
  {
    std::stringstream Stream;
    Stream << "Sinus no " << i;

    double Amplitude = 7.0*(double)std::rand()/RAND_MAX + 0.1;
    double PhaseShift = 0.1 * i;

    scigraphics::graphSequenceVector *Sin = Plot.createGraphSequenceVector( Stream.str() );
    for ( double x = -M_PI; x <= M_PI; x+= (2*M_PI)/NumberOfPointsPerGraph )
      Sin->append( x, Amplitude * std::sin( x + PhaseShift ) );
  }

  Plot.move( 200, 200 );
  Plot.resize( 700, 800 );
  Plot.show();
  Plot.replot();

  for ( unsigned i = 0; i < 10; i++ )
  {
#if 1
    auto ScaleIntervalX = Plot.scaleIntervalX();
    auto ScaleIntervalY = Plot.scaleIntervalY();
    if ( i == 0 )
    {
      ScaleIntervalX.setMinMax( -M_PI, +M_PI );
      ScaleIntervalY.setMinMax( -8, +8 );
    }
    Plot.setScaleIntervalX( ScaleIntervalX.min()-1, ScaleIntervalX.max()+1 );
    Plot.setScaleIntervalY( ScaleIntervalY.min()+0.3, ScaleIntervalY.max()+0.3 );
#endif
    Plot.replot();
  }

//  return app.exec();
}

// ======================================================




