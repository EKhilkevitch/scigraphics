
// ======================================================

#include <QApplication>
#include <QtGui>
#include <cmath>

#include "scigraphics/plot.h"
#include "scigraphics/qt4/qt4plot.h"
#include "scigraphics/graphmap.h"
#include "scigraphics/qt4/qt4plotsettings.h"

// ======================================================

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

// ======================================================

int main( int argc, char *argv[] )
{
  QApplication app(argc,argv);

  qt4plot Plot;

  scigraphics::graphSequenceVector *Sqr = Plot.createGraphSequenceVector( "x^2" );
  
  for ( double x = -0.4; x <= 2; x+= 0.0005 )
    Sqr->append( x, x*x, 0.5, std::sqrt(std::fabs(x)) );
  Sqr->setVisibleErrorBars(true);

  scigraphics::graphSequenceVector *Sin = Plot.createGraphSequenceVector( "Sinus" );
  for ( double x = -M_PI; x <= M_PI; x+= 0.1 )
    Sin->append( std::fabs(x) < 0.3 || x < -2.4 ? scigraphics::invalidNumber() : x, std::sin(x) );
  Sin->setVisiblePoints(true);
  Sin->setVisibleLineHystogram(true);
  Sin->setVisibleLines(false);

  scigraphics::graphSequenceVector *Abs = Plot.createGraphSequenceVector( "Abs" );
  for ( double x = -1; x <= 1; x+= 0.1 )
    Abs->append( x, std::fabs(x) );
  scigraphics::color Color = scigraphics::color::DarkGreen;
  Color.setTransparency( 0.8 );
  Abs->setColor( Color );
  Plot.bindGraphToAxis( Abs, scigraphics::axisSetCollection::Bottom, scigraphics::axisSetCollection::Right );
  
  scigraphics::graphSequenceVector *Cos = Plot.createGraphSequenceVector( "Cos+1" );
  for ( double x = 0; x <= 3; x+= 0.1 )
    Cos->append( x, std::cos(x)+1 );
  Plot.bindGraphToAxis( Cos, scigraphics::axisSetCollection::Top, scigraphics::axisSetCollection::Right );

  scigraphics::graphAreaVector *Polygons = Plot.createGraph<scigraphics::graphAreaVector>("Polygons");
  Polygons->append( 0, 0 );
  Polygons->append( 0, 2 );
  Polygons->append( 2, 2 );
  Polygons->appendInvalid();
  Polygons->append( 3.1, 3.7 );
  Polygons->append( 3.1, 4.5 );
  Polygons->append( 3.7, 4.5 );
  Polygons->append( 3.7, 3.7 );
  Polygons->append( 3.1, 3.7 );

  scigraphics::graphMapVector *Map = Plot.createGraph<scigraphics::graphMapVector>( "Map" );
  Map->resize( 30, 30 );
  Map->setIntervalX( 2, 3 );
  Map->setIntervalY( 2, 3 );
  Map->setForcedIntervalZ( 0.5, 0.9 );
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

  Plot.setDisallowedMouseOperations( scigraphics::mouse::allowing::SelectH );
  
  Plot.setAxisTitle( scigraphics::axisSetCollection::Bottom, "This is axis X bottom" " -> \xce\xbc" );
  Plot.setAxisTitle( scigraphics::axisSetCollection::Top, "This is axis X top" );
  Plot.setAxisTitle( scigraphics::axisSetCollection::Left, "This is axis Y left" );
  Plot.setAxisTitle( scigraphics::axisSetCollection::Right, "This is axis Y right" );

  qt4plotSettingsGroupSuperBox::axisPositionsList AxisPositions = qt4plotSettingsGroupSuperBox::defaultAxisPositions();
  AxisPositions << scigraphics::axisSetCollection::Top;
  AxisPositions << scigraphics::axisSetCollection::Right;

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


