
// ======================================================

#include <QApplication>
#include <QtGui>
#include <cmath>

#include "graphics/plot.h"
#include "graphics/graphmap.h"
#include "graphics/qt4/qt4plot.h"
#include "graphics/qt4/qt4plotsettings.h"

// ======================================================

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

// ======================================================

int main( int argc, char **argv )
{
  QApplication app(argc,argv);

  qt4plot Plot;

  std::vector< graphics::graphMapVector* > Maps(12,NULL);

  Maps[0] = Plot.createGraph<graphics::graphMapVector>( "gray" );
  Maps[1] = Plot.createGraph<graphics::graphMapVector>( "color1" );
  Maps[2] = Plot.createGraph<graphics::graphMapVector>( "color2" );

  Maps[0]->getViewMap().setColorStrategy( new graphics::graphViewRectangleMap::grayscalePointColorStrategy() );
  Maps[1]->getViewMap().setColorStrategy( new graphics::graphViewRectangleMap::redYellowBluePointColorStrategy() );
  Maps[2]->getViewMap().setColorStrategy( new graphics::graphViewRectangleMap::yellowRedBluePointColorStrategy() );
 
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
      graphics::graphMapVector *Map = Maps[ ix > iy && SizeX-ix > iy ? 0 : 
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

  qt4plotSettings Settings;
  Settings.addSettingWidget( new qt4plotSettingsSelections(&Settings) );
  //Settings.replaceLayout( new QVBoxLayout() );
  Settings.addPlot(&Plot);
  Settings.show();
 

  Plot.move( 200, 200 );
  Plot.resize( 700, 800 );
  Plot.show();
  Plot.replot();

  return app.exec();
}

// ======================================================



