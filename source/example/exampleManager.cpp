
// ======================================================

#include <QApplication>
#include <QtGui>
#include <cmath>

#include "scigraphics/qt4/qt4plot.h"
#include "scigraphics/qt4/qt4plotsettings.h"
#include "scigraphics/qt4/qt4plotmanager.h"

// ======================================================

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

// ======================================================

int main( int argc, char **argv )
{
  QApplication app(argc,argv);

  qt4plotManager Manager( 8, 3, qt4plotManager::TabSettings );
  for ( unsigned i = 0; i < Manager.numOfSettings(); i++ )
    Manager.getSettings(i)->addSettingWidget( new qt4plotSettingsSelections(NULL) );

  auto Sqr = Manager.getPlot(0)->createGraphSequenceVector( "x^2" );
  
  for ( double x = -0.4; x <= 2; x+= 0.05 )
    Sqr->append( x, x*x, 0.5, std::sqrt(std::fabs(x)) );
  Sqr->setVisibleErrorBars(true);

  auto Sin = Manager.getPlot(1)->createGraphSequenceVector( "Sinus" );
  for ( double x = -M_PI; x <= M_PI; x+= 0.1 )
    Sin->append( std::fabs(x) < 0.3 || x < -2.4 ? scigraphics::invalidNumber() : x, std::sin(x) );
  Sin->setVisiblePoints(true);
  Sin->setVisibleLineHystogram(true);
  Sin->setVisibleLines(false);

  auto Abs = Manager.getPlot(2)->createGraphSequenceVector( "Abs" );
  for ( double x = -1; x <= 1; x+= 0.1 )
    Abs->append( x, std::fabs(x) );
  Manager.getPlot(2)->scaleWithPosition( scigraphics::axisSetCollection::Left )->lock();
  
  auto Exp = Manager.getPlot(3)->createGraphSequenceVector( "Exp" );
  for ( double x = -2; x <= 10; x+= 0.1 )
    Exp->append( 1e5*x, 1e6*std::exp(x) );

  auto Rounds = Manager.getPlot(4)->createGraphSequenceVector( "Ellipses" );
  for ( double Phi = 0; Phi <= 2*M_PI; Phi+=0.01 )
    Rounds->appendPolar( Phi, (1+sin(Phi)) * (1+0.9*cos(8*Phi)) * (1+0.1*cos(24*Phi)) );
  Rounds->append( scigraphics::invalidNumber(), scigraphics::invalidNumber() );
  for ( double t = 0; t < 2*M_PI; t+=0.01 )
    Rounds->append( 0.4 * cos(t) + 4, 0.7 * sin(t) - 2 );

  auto SpiralPl = Manager.getPlot(4)->createGraphSequenceVector("Spiral + ");
  for ( double Phi = 0; Phi <= 10*M_PI; Phi += 0.05 )
    SpiralPl->appendPolar( Phi, 2 * sqrt( Phi ) );
  
  auto SpiralNg = Manager.getPlot(4)->createGraphSequenceVector("Spiral - ");
  for ( double Phi = 0; Phi <= 10*M_PI; Phi += 0.05 )
    SpiralNg->appendPolar( Phi, -2 * sqrt( Phi ) );

  auto Polygons = Manager.getPlot(6)->createGraph<scigraphics::graphAreaVector>("Polygons");
  Polygons->append( 0, 0 );
  Polygons->append( 0, 2 );
  Polygons->append( 2, 2 );
  Polygons->appendInvalid();
  Polygons->append( 6, 7 );
  Polygons->append( 6, 9 );
  Polygons->append( 7, 9 );
  Polygons->append( 7, 7 );
  Polygons->append( 6, 7 );

  Manager.plotWidget()->move( 100, 100 );
  Manager.plotWidget()->resize( 1000, 800 );
  Manager.plotWidget()->show();
  Manager.show();
  Manager.replot();

  return app.exec();
}

// ======================================================


