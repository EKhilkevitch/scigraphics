
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

#include "scigraphics/plot.h"
#include "scigraphics/numberstyle.h"
#include "scigraphics/mouse.h"
#include "scigraphics/graphsequence.h"
#include "scigraphics/graphmap.h"
#include "scigraphics/qt4/plot.h"
#include "scigraphics/qt4/settings.h"
#include "scigraphics/qt4/settingsbox.h"
#include "scigraphics/qt4/settingscomposer.h"

#include <cmath>
#include <cstdlib>

#include <QApplication>
#include <QTranslator>
#include <QLocale>

// ======================================================

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

// ======================================================

int main( int argc, char *argv[] )
{
  QApplication app(argc,argv);
  Q_INIT_RESOURCE( translations );  
  QTranslator Translator;
  bool IsLoad = Translator.load( ":/scigraphics/settingsbox_" + QLocale::system().name() );
  if ( ! IsLoad )
    std::abort();
  app.installTranslator( &Translator );

  scigraphics::qt4plot Plot;

  Plot.setMouseOperations( 
    scigraphics::mouse::allowing::MoveAndZoom   |
    scigraphics::mouse::allowing::Wheel         |
    scigraphics::mouse::allowing::Reset         |
    scigraphics::mouse::allowing::MoveFloat     |
    scigraphics::mouse::allowing::PointGraph 
  );

  scigraphics::graphSV *Sin = Plot.createGraphSequenceVector( "Sinus" );
  for ( double x = -2*M_PI; x <= 2*M_PI; x+= 0.01 )
    Sin->append( x, std::sin(x) );
  Sin->setVisiblePoints(true);
  Sin->setVisibleLines(true);

  Plot.move( 200, 200 );
  Plot.resize( 900, 600 );
  Plot.show();
  Plot.replot();

  int RetValue = app.exec();
  return RetValue;
}

// ======================================================


