
// ======================================================

#include <QApplication>
#include <QtGui>
#include <cmath>

#include "graphics/plot.h"
#include "graphics/qt4/qt4plot.h"
#include "graphics/graphmap.h"
#include "graphics/qt4/qt4plotsettings.h"

// ======================================================

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

// ======================================================

int main( int argc, char *argv[] )
{
  QApplication app(argc,argv);

  qt4plot Plot;

  graphics::graphSequenceVector *Sqr = Plot.createGraphSequenceVector( "x^2" );
  
  for ( double x = -0.4; x <= 2; x+= 0.0005 )
    Sqr->append( x, x*x, 0.5, std::sqrt(std::fabs(x)) );
  Sqr->setVisibleErrorBars(true);

  graphics::graphSequenceVector *Sin = Plot.createGraphSequenceVector( "Sinus" );
  for ( double x = -M_PI; x <= M_PI; x+= 0.1 )
    Sin->append( std::fabs(x) < 0.3 || x < -2.4 ? graphics::invalidNumber() : x, std::sin(x) );
  Sin->setVisiblePoints(true);
  Sin->setVisibleLineHystogram(true);
  Sin->setVisibleLines(false);

  graphics::graphSequenceVector *Abs = Plot.createGraphSequenceVector( "Abs" );
  for ( double x = -1; x <= 1; x+= 0.1 )
    Abs->append( x, std::fabs(x) );
  graphics::color Color = graphics::color::DarkGreen;
  Color.setTransparency( 0.8 );
  Abs->setColor( Color );
  Plot.bindGraphToAxis( Abs, graphics::axisSetCollection::Bottom, graphics::axisSetCollection::Right );
  
  graphics::graphSequenceVector *Cos = Plot.createGraphSequenceVector( "Cos+1" );
  for ( double x = 0; x <= 3; x+= 0.1 )
    Cos->append( x, std::cos(x)+1 );
  Plot.bindGraphToAxis( Cos, graphics::axisSetCollection::Top, graphics::axisSetCollection::Right );

  graphics::graphAreaVector *Polygons = Plot.createGraph<graphics::graphAreaVector>("Polygons");
  Polygons->append( 0, 0 );
  Polygons->append( 0, 2 );
  Polygons->append( 2, 2 );
  Polygons->appendInvalid();
  Polygons->append( 3.1, 3.7 );
  Polygons->append( 3.1, 4.5 );
  Polygons->append( 3.7, 4.5 );
  Polygons->append( 3.7, 3.7 );
  Polygons->append( 3.1, 3.7 );

  graphics::graphMapVector *Map = Plot.createGraph<graphics::graphMapVector>( "Map" );
  Map->resize( 30, 30 );
  Map->setIntervalX( 2, 3 );
  Map->setIntervalY( 2, 3 );
  for ( unsigned ix = 0; ix < Map->sizeX(); ix++ )
  {
    for ( unsigned iy = 0; iy < Map->sizeY(); iy++ ) 
    {
      double x = Map->coordinateX(ix);
      double y = Map->coordinateY(iy);
      double Value = sin(0.1*x*x*y);
      Map->set( ix, iy, Value );
    }
  }

  Plot.setDisallowedMouseOperations( graphics::mouse::allowing::SelectH );
  
  Plot.setAxisTitle( graphics::axisSetCollection::Bottom, "This is axis X bottom" " -> \xce\xbc" );
  Plot.setAxisTitle( graphics::axisSetCollection::Top, "This is axis X top" );
  Plot.setAxisTitle( graphics::axisSetCollection::Left, "This is axis Y left" );
  Plot.setAxisTitle( graphics::axisSetCollection::Right, "This is axis Y right" );

  qt4plotSettingsGroupSuperBox::axisPositionsList AxisPositions = qt4plotSettingsGroupSuperBox::defaultAxisPositions();
  AxisPositions << graphics::axisSetCollection::Top;
  AxisPositions << graphics::axisSetCollection::Right;

  qt4plotSettings Settings( NULL, "", AxisPositions, new qt4plotSettingsComposerTabs() );
  Settings.addSettingWidget( new qt4plotSettingsSelections(&Settings) );
  //Settings.replaceLayout( new QVBoxLayout() );
  Settings.addPlot(&Plot);
  Settings.show();
 

  Plot.move( 200, 200 );
  Plot.resize( 700, 800 );
  Plot.show();
  Plot.resetScalesTo1x1(); 
  Plot.replot();

  return app.exec();
}

// ======================================================


