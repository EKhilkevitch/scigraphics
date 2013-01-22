
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

#include "scigraphics/qt4/qt4plot.h"
#include "scigraphics/qt4/qt4plotsettings.h"

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
    scigraphics::graphSequenceVector *Sin, *Sqr, *SpiralPl, *SpiralNg;

    unsigned Count;

    void fillSin()
    {
      Q_ASSERT( Sin != NULL );
      Sin->clear();
      
      for ( double x = -M_PI; x <= M_PI; x+= 0.001 )
        Sin->append( std::fabs(x) < 0.3 || x < -2.4 ? scigraphics::invalidNumber() : x, std::sin(x+Count/100.0) );
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

      Plot->setScaleIntervalX( scigraphics::interval<scigraphics::number>( -4, 4 ) );
      Plot->setScaleIntervalY( scigraphics::interval<scigraphics::number>( -1.5, 4.5 ) );
    };

    unsigned count() const { return Count; }

  public slots:
    void updateGraphics() { ++Count; fillGraphics(); Plot->replot(); } 

};

// ======================================================


