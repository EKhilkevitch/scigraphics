
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

#include "exampleAnimation.h"

#include "scigraphics/qt4/qt4plot.h"
#include "scigraphics/qt4/qt4plotsettings.h"

// ======================================================

int main( int argc, char **argv )
{
  QApplication app(argc,argv);
  
  qt4plot Plot;
  plotAnimator Animator( &Plot );

  Plot.move( 200, 200 );
  Plot.resize( 700, 800 );
  Plot.show();
  Plot.replot();

  QTimer Timer;
  QObject::connect( &Timer, SIGNAL(timeout()), &Animator, SLOT(updateGraphics()) );
  Timer.start(20);
  
  QTime WorkTime;
  WorkTime.start();

  int RetCode = app.exec();

  qDebug() << "FPS = " << (double)Animator.count() / ( 1e-3 * WorkTime.elapsed() ) << " Max is " << 1.e3/Timer.interval();
  

  return RetCode;
}

// ======================================================


