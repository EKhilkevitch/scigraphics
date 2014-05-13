
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
#include "scigraphics/graphmap.h"
#include "scigraphics/qt4/plot.h"
#include "scigraphics/qt4/settings.h"
#include "scigraphics/qt4/settingsbox.h"

// ======================================================

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

// ======================================================

int main( int argc, char **argv )
{
  QApplication app(argc,argv);

  scigraphics::qt4plot Plot;

  std::vector< scigraphics::graphMV* > Maps( 12, (scigraphics::graphMV*)NULL );

  Maps[0] = Plot.createGraph<scigraphics::graphMV>( "gray" );
  Maps[1] = Plot.createGraph<scigraphics::graphMV>( "color1" );
  Maps[2] = Plot.createGraph<scigraphics::graphMV>( "color2" );

  Maps[0]->getView().setColorStrategy( new scigraphics::map::graphViewRectangle::grayscalePointColorStrategy() );
  Maps[1]->getView().setColorStrategy( new scigraphics::map::graphViewRectangle::redYellowBluePointColorStrategy() );
  Maps[2]->getView().setColorStrategy( new scigraphics::map::graphViewRectangle::yellowRedBluePointColorStrategy() );
 
  const size_t SizeX = 20*8, SizeY = 20*8;
  for ( unsigned i = 0; i < Maps.size(); ++i )
  {
    if ( Maps[i] == NULL )
      continue;
    Maps[i]->resize( SizeX, SizeY );
    Maps[i]->setIntervalX( -3, 2 );
    Maps[i]->setIntervalY( -5, 3 );
  }

  for ( unsigned ix = 0; ix < SizeX; ix++ )
  {
    for ( unsigned iy = 0; iy < SizeY; iy++ ) 
    {
      scigraphics::graphMV *Map = Maps[ ix > iy && SizeX-ix > iy ? 0 : 
                                            ix < iy && SizeX-ix < iy ? 1 : 2 ];
      if ( Map == NULL )
        continue;
      double x = Map->coordinateX(ix);
      double y = Map->coordinateY(iy);
      double Value = std::sin(x*y);
      if ( ix == SizeX/4 )
        Value = 1;
      Map->set( ix, iy, Value );
      //std::cout << "Set: " << ix << " " << iy << " -> ( " << x << " " << y << " ) -> " << Sqr->at( ix, iy ) << std::endl;
    }
  }

  scigraphics::qt4settings Settings;
  Settings.addSettingWidget( new scigraphics::qt4settingsSelections(&Settings) );
  //Settings.replaceLayout( new QVBoxLayout() );
  Settings.connectToPlot(&Plot);
  Settings.show();
 

  Plot.move( 200, 200 );
  Plot.resize( 700, 800 );
  Plot.show();
  Plot.replot();

  return app.exec();
}

// ======================================================



