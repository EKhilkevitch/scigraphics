
// ======================================================

#include "graphics/plot.h"
#include "graphics/imagemagick/implot.h"
#include "graphics/graphmap.h"

// ======================================================

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

// ======================================================

int main()
{
  implot Plot( 900, 700 );

  graphics::graphSequenceVector *Sqr = Plot.createGraphSequenceVector( "x^2" );
  
  for ( double x = -0.4; x <= 2; x+= 0.05 )
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
  Abs->setLineWidth( 10 );
  graphics::color Color = graphics::color::Green;
  Color.setTransparency( 0.3 );
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
 
#if 1
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
#endif

  Plot.setDisallowedMouseOperations( graphics::mouse::allowing::SelectH );
  
  Plot.setAxisTitle( graphics::axisSetCollection::Bottom, "This is axis X bottom" " -> \xce\xbc" );
  Plot.setAxisTitle( graphics::axisSetCollection::Top, "This is axis X top" );
  Plot.setAxisTitle( graphics::axisSetCollection::Left, "This is axis Y left" );
  Plot.setAxisTitle( graphics::axisSetCollection::Right, "This is axis Y right" );

  //Plot.display();
  Plot.write("TestIM.png");
  return 0;
}

// ======================================================


