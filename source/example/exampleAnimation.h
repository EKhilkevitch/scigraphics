
// ======================================================

#include <QApplication>
#include <QtGui>
#include <cmath>

#include "graphics/qt4/qt4plot.h"
#include "graphics/qt4/qt4plotsettings.h"

// ======================================================

#ifndef M_PI
#define M_PI 3.141592653589793
#endif

// ======================================================

class plotAnimator : public QObject
{
  Q_OBJECT

  private:
    qt4plot *Plot;
    graphics::graphSequenceVector *Sin, *Sqr, *SpiralPl, *SpiralNg;

    unsigned Count;

    void fillSin()
    {
      Q_ASSERT( Sin != NULL );
      Sin->clear();
      
      for ( double x = -M_PI; x <= M_PI; x+= 0.001 )
        Sin->append( std::fabs(x) < 0.3 || x < -2.4 ? graphics::invalidNumber() : x, std::sin(x+Count/100.0) );
      //Sin->setVisiblePoints(true);
      //Sin->setVisibleLineHystogram(true);
      //Sin->setVisibleLines(false);
    }

    void fillSqr()
    {
      Q_ASSERT( Sqr != NULL );
      
      Sqr->clear();
      for ( double x = -0.4; x <= 2; x+= 0.05 )
        Sqr->append( x, x*x, 0.5, std::sqrt(std::fabs(x)) );
    }

    void fillSpirals()
    {
      Q_ASSERT( SpiralPl != NULL );
      Q_ASSERT( SpiralNg != NULL );
  
      SpiralPl->clear();
      SpiralNg->clear();
      for ( double Phi = 0; Phi <= 10*M_PI; Phi += 0.05 )
      {
        SpiralPl->appendPolar( Phi + Count*0.01, + 0.8 * sqrt( Phi ) );
        SpiralNg->appendPolar( Phi + Count*0.01, - 0.8 * sqrt( Phi ) );
      }
      SpiralPl->append( 0, 0 );
      SpiralNg->append( 0, 0 );
    }

    void fillGraphics()
    {
      fillSin();
      fillSqr();
      fillSpirals();
    }


  public:
    plotAnimator( qt4plot *P ) : Plot(P)
    {
      Count = 0;

      Sqr = Plot->createGraphSequenceVector( "x^2" );
      Sin = Plot->createGraphSequenceVector( "Sinus" );
      SpiralPl = Plot->createGraphSequenceVector( "Spiral+");
      SpiralNg = Plot->createGraphSequenceVector( "Spiral-");
      SpiralPl->setLineWidth( 2 );
      SpiralNg->setLineWidth( 2 );

      fillGraphics();

      Plot->setScaleIntervalX( graphics::interval<graphics::number>( -4, 4 ) );
      Plot->setScaleIntervalY( graphics::interval<graphics::number>( -1.5, 4.5 ) );
    };

    unsigned count() const { return Count; }

  public slots:
    void update() { ++Count; fillGraphics(); Plot->replot(); Plot->update(); } 

};

// ======================================================


