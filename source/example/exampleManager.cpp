
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

#include "scigraphics/qt4/plot.h"
#include "scigraphics/qt4/settings.h"
#include "scigraphics/qt4/settingsbox.h"
#include "scigraphics/qt4/manager.h"
#include "scigraphics/scale.h"
#include "scigraphics/graphsequence.h"
#include "scigraphics/graphmap.h"

// ======================================================

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

// ======================================================

int main( int argc, char **argv )
{
  QApplication app(argc,argv);

  scigraphics::qt4plotManager Manager( 8, 3, NULL, scigraphics::qt4plotManager::TabSettings );
  for ( unsigned i = 0; i < Manager.numberOfSettings(); i++ )
    Manager.settings(i)->addSettingWidget( new scigraphics::qt4settingsSelections(NULL) );

  scigraphics::graphSV *Sqr = Manager.plot(0)->createGraphSV( "x^2" );
  
  for ( double x = -0.4; x <= 2; x+= 0.05 )
    Sqr->append( x, x*x, 0.5, std::sqrt(std::fabs(x)) );
  Sqr->setVisibleErrorBars(true);

  scigraphics::graphSV *Sin = Manager.plot(1)->createGraphSV( "Sinus" );
  for ( double x = -M_PI; x <= M_PI; x+= 0.1 )
    Sin->append( std::fabs(x) < 0.3 || x < -2.4 ? scigraphics::invalidNumber() : x, std::sin(x) );
  Sin->setVisiblePoints(true);
  Sin->setVisibleLineHystogram(true);
  Sin->setVisibleLines(false);

  scigraphics::graphSV *Abs = Manager.plot(2)->createGraphSV( "Abs" );
  for ( double x = -1; x <= 1; x+= 0.1 )
    Abs->append( x, std::fabs(x) );
  Manager.plot(2)->scaleWithPosition( scigraphics::AxisLeft )->lock();
  
  scigraphics::graphSV *Exp = Manager.plot(3)->createGraphSV( "Exp" );
  for ( double x = -2; x <= 10; x+= 0.1 )
    Exp->append( 1e5*x, 1e6*std::exp(x) );

  scigraphics::graphSV *Rounds = Manager.plot(4)->createGraphSV( "Ellipses" );
  for ( double Phi = 0; Phi <= 2*M_PI; Phi+=0.01 )
    Rounds->appendPolar( Phi, (1+sin(Phi)) * (1+0.9*cos(8*Phi)) * (1+0.1*cos(24*Phi)) );
  Rounds->append( scigraphics::invalidNumber(), scigraphics::invalidNumber() );
  for ( double t = 0; t < 2*M_PI; t+=0.01 )
    Rounds->append( 0.4 * cos(t) + 4, 0.7 * sin(t) - 2 );

  scigraphics::graphSV *SpiralPl = Manager.plot(4)->createGraphSV("Spiral + ");
  for ( double Phi = 0; Phi <= 10*M_PI; Phi += 0.05 )
    SpiralPl->appendPolar( Phi, 2 * sqrt( Phi ) );
  
  scigraphics::graphSV *SpiralNg = Manager.plot(4)->createGraphSV("Spiral - ");
  for ( double Phi = 0; Phi <= 10*M_PI; Phi += 0.05 )
    SpiralNg->appendPolar( Phi, -2 * sqrt( Phi ) );

  for ( unsigned i = 0; i < 20; i++ )
  {
    scigraphics::graphSV *Graph = Manager.plot(5)->createGraphSV( ( "Circle " + QString::number(i) ).toStdString() );
    for ( double Phi = 0; Phi <= 2*M_PI; Phi += 0.05 )
      Graph->appendPolar( Phi, i+1 );
  }

  scigraphics::graphAV *Polygons = Manager.plot(6)->createGraphAV("Polygons");
  Polygons->append( 0, 0 );
  Polygons->append( 0, 2 );
  Polygons->append( 2, 2 );
  Polygons->appendInvalid();
  Polygons->append( 6, 7 );
  Polygons->append( 6, 9 );
  Polygons->append( 7, 9 );
  Polygons->append( 7, 7 );
  Polygons->append( 6, 7 );

  scigraphics::graphSV *SpiralSqr = Manager.plot(7)->createGraphSV("Sqr ");
  SpiralSqr->append( 0, 0 );
  SpiralSqr->append( 0, 4 );
  SpiralSqr->append( 4, 4 );
  SpiralSqr->append( 4, 0 );
  SpiralSqr->append( 0, 0 );
  Manager.plot(7)->setScalesTo1x1( true );

  Manager.plotWidget()->move( 100, 100 );
  Manager.plotWidget()->resize( 1000, 800 );
  Manager.plotWidget()->show();
  Manager.show();
  Manager.replot();
  
  return app.exec();
}

// ======================================================


