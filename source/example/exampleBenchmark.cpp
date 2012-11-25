
/*
 * Copyright 2011,2012 Evgeniy Khilkevitch 
 * 
 * This file is part of scigraphics.
 * 
 * Scigraphics is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Scigraphics is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with scigraphics.  If not, see <http://www.gnu.org/licenses/>.
 */


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




