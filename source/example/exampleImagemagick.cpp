
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

#include "scigraphics/imagemagick/plot.h"

// ======================================================

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

// ======================================================

int main()
{
  scigraphics::implot Plot( 900, 700 );

  scigraphics::graphSV *Sqr = Plot.createGraphSV( "x^2" );
  
  for ( double x = -0.4; x <= 2; x+= 0.05 )
    Sqr->append( x, x*x, 0.5, std::sqrt(std::fabs(x)) );
  Sqr->setVisibleErrorBars(true);

  scigraphics::graphSV *Sin = Plot.createGraphSV( "Sinus" );
  for ( double x = -M_PI; x <= M_PI; x+= 0.1 )
    Sin->append( std::fabs(x) < 0.3 || x < -2.4 ? scigraphics::invalidNumber() : x, std::sin(x) );
  Sin->setVisiblePoints(true);
  Sin->setVisibleLineHystogram(true);
  Sin->setVisibleLines(false);

  scigraphics::graphSV *Abs = Plot.createGraphSV( "Abs" );
  for ( double x = -1; x <= 1; x+= 0.1 )
    Abs->append( x, std::fabs(x) );
  Abs->setLineWidth( 10 );
  scigraphics::color Color = scigraphics::color::Green;
  Color.setTransparency( 0.3 );
  Abs->setColor( Color );
  Plot.bindGraphToAxis( Abs, scigraphics::axisSetCollection::Bottom, scigraphics::axisSetCollection::Right );
  
  scigraphics::graphSV *Cos = Plot.createGraphSV( "Cos+1" );
  for ( double x = 0; x <= 3; x+= 0.1 )
    Cos->append( x, std::cos(x)+1 );
  Plot.bindGraphToAxis( Cos, scigraphics::axisSetCollection::Top, scigraphics::axisSetCollection::Right );

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
 
#if 1
  scigraphics::graphMV *Map = Plot.createGraphMV( "Map" );
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
#endif

  Plot.setAxisTitle( scigraphics::axisSetCollection::Bottom, "This is axis X bottom" " -> \xce\xbc" );
  Plot.setAxisTitle( scigraphics::axisSetCollection::Top, "This is axis X top" );
  Plot.setAxisTitle( scigraphics::axisSetCollection::Left, "This is axis Y left" );
  Plot.setAxisTitle( scigraphics::axisSetCollection::Right, "This is axis Y right" );

  //Plot.display();
  Plot.write("TestIM.png");
  return 0;
}

// ======================================================


