
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

  scigraphics::graphSV *Sqr = Plot.createGraphSequenceVector( "x^2" );
  for ( double x = -0.4; x <= 2; x+= 0.0005 )
    Sqr->append( x, x*x, 0.5, std::sqrt(std::fabs(x)) );
  Sqr->setVisibleErrorBars(true);

  scigraphics::graphSV *Sin = Plot.createGraphSequenceVector( "Sinus" );
  for ( double x = -M_PI; x <= M_PI; x+= 0.01 )
    Sin->append( std::fabs(x) < 0.3 || x < -2.4 ? scigraphics::invalidNumber() : x, std::sin(x) );
  Sin->setVisiblePoints(true);
  Sin->setVisibleLineHystogram(true);
  Sin->setVisibleLines(false);

  scigraphics::graphSV *Abs = Plot.createGraphSequenceVector( "Abs" );
  for ( double x = -1; x <= 1; x+= 0.1 )
    Abs->append( x, std::fabs(x) );
  scigraphics::color Color = scigraphics::color::DarkGreen;
  Color.setTransparency( 0.8 );
  Abs->setColor( Color );
  Plot.bindGraphToAxis( Abs, scigraphics::AxisBottom, scigraphics::AxisRight );
  
  scigraphics::graphSV *Cos = Plot.createGraphSequenceVector( "Cos+1" );
  for ( double x = 0; x <= 3; x+= 0.1 )
    Cos->append( x, std::cos(x)+1 );
  Cos->setLineStyle( scigraphics::lineStyle::Dash );
  Cos->setPointShape( scigraphics::pointStyle::Cross );
  Cos->setPointSize( 25 );
  Cos->setLineWidth( 5 );
  Plot.bindGraphToAxis( Cos, scigraphics::AxisTop, scigraphics::AxisRight );

  scigraphics::graphAV *Polygons = Plot.createGraphAV("Polygons");
  Polygons->append( 0, 0 );
  Polygons->append( 0, 2 );
  Polygons->append( 2, 2 );
  Polygons->appendInvalid();
  Polygons->append( 3.1, 3.7 );
  Polygons->append( 3.1, 4.5 );
  Polygons->append( 3.7, 4.5 );
  Polygons->append( 3.7, 3.7 );
  Polygons->append( 3.1, 3.7 );

  scigraphics::graphMV *Map = Plot.createGraph<scigraphics::graphMV>( "Map" );
  Map->resize( 30, 30 );
  Map->setIntervalX( 2, 3 );
  Map->setIntervalY( 2, 3 );
  Map->setForcedIntervalZ( 0.5, 0.9 );
  for ( scigraphics::map::data::int_t ix = 0; ix < Map->sizeX(); ix++ )
  {
    for ( scigraphics::map::data::int_t iy = 0; iy < Map->sizeY(); iy++ ) 
    {
      scigraphics::number x = Map->coordinateX(ix);
      scigraphics::number y = Map->coordinateY(iy);
      scigraphics::number Value = sin(0.1*x*x*y);
      Map->set( ix, iy, Value );
    }
  }

  Plot.setDisallowedMouseOperations( scigraphics::mouse::allowing::SelectH );
  
  Plot.setAxisTitle( scigraphics::AxisBottom, "This is axis X bottom" " -> \xce\xbc" );
  Plot.setAxisTitle( scigraphics::AxisTop, "This is axis X top" );
  Plot.setAxisTitle( scigraphics::AxisLeft, "This is axis Y left" );
  Plot.setAxisTitle( scigraphics::AxisRight, "This is axis Y right" );
  Plot.setAxisNumberStyle( scigraphics::AxisRight, scigraphics::numberStyle(scigraphics::numberStyle::Exponent) );
  
  scigraphics::qt4settingsGroupSuperBox::axisPositionsList AxisPositions = scigraphics::qt4settingsGroupSuperBox::defaultAxisPositions();
  AxisPositions << scigraphics::AxisTop;
  AxisPositions << scigraphics::AxisRight;

  scigraphics::qt4settings Settings( NULL, "", AxisPositions, new scigraphics::qt4settingsComposerTabs() );
  Settings.addSettingWidget( new scigraphics::qt4settingsSelections() );
  Settings.addSettingWidget( new scigraphics::qt4settingsDecoration() );
  
  scigraphics::qt4settingsScaleTypeAllAxis *ScaleWidget = Settings.getSettingWidget<scigraphics::qt4settingsScaleTypeAllAxis>();
  foreach( scigraphics::axisPosition Position, ScaleWidget->axisPositionsWithBoxes() )
  {
    scigraphics::qt4settingsScaleType *Box = dynamic_cast<scigraphics::qt4settingsScaleType*>( ScaleWidget->boxForAxisPosition( Position ) );
    Box->showLogarithmNegativeControl(true);
    Box->showSquareControl(true);
  }
  ScaleWidget->setNumberOfRowsInLayout(1);

  //Settings.getSettingWidget<scigraphics::qt4settingsGraphType>()->showErrorBarsControl( false );
  //Settings.replaceLayout( new QVBoxLayout() );
  Settings.connectToPlot(&Plot);


  Settings.show();

  Plot.move( 200, 200 );
  Plot.resize( 700, 800 );
  Plot.show();
  //Plot.resetScalesTo1x1(); 
  Plot.replot();

  Settings.loadSettings( "settings.conf" );
  int RetValue = app.exec();
  Settings.saveSettings( "settings.conf" );
  return RetValue;
}

// ======================================================


