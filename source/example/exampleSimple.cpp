
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

#include "scigraphics/plot.h"
#include "scigraphics/color.h"
#include "scigraphics/qt4/plot.h"

// ======================================================

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

// ======================================================

int main( int argc, char *argv[] )
{
  QApplication app(argc,argv);

  scigraphics::qt4plot Plot;

  scigraphics::graphSV *Sin = Plot.createGraphSV( "Sinus" );
  const size_t GraphSize = 1000;
  for ( unsigned i = 0; i < GraphSize; i++ )
  {
    double x = 2 * M_PI * i / GraphSize - M_PI;
    double y = std::sin( x );
    Sin->append( x, y );
  }

  Plot.move( 200, 200 );
  Plot.resize( 700, 800 );
  Plot.show();
  Plot.replot();

  return app.exec();
}

// ======================================================


