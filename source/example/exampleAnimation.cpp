
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

#include "exampleAnimation.h"
#include "scigraphics/qt4/plot.h"

#include <QApplication>
#include <QThread>
#include <QTime>
#include <QDebug>

// ======================================================

const unsigned TimerInterval = 20;

// ======================================================

class timerThread : public QThread
{
  private:
    plotAnimator *Animator;
    bool NeedToStop;

    void run()
    {
      do
      {
	QTime Timer;
	Timer.start();
	while ( Timer.elapsed() < TimerInterval && !NeedToStop )
	  msleep(1);
	QMetaObject::invokeMethod( Animator, "updateGraphics", Qt::QueuedConnection );
      } while ( ! NeedToStop );
    }

  public:
    timerThread( plotAnimator *A ) : Animator(A), NeedToStop(false) {}
    void stop() 
    { 
      NeedToStop = true; 
      while ( isRunning() )
        msleep(1); 
    }
};

// ======================================================

int main( int argc, char **argv )
{
  QApplication app(argc,argv);
  
  scigraphics::qt4plot Plot;
  plotAnimator Animator( &Plot );

  Plot.move( 200, 200 );
  Plot.resize( 700, 800 );
  Plot.show();
  Plot.replot();

#if 0 
  // Easy way to do animation, but on Windows this is very slow.
  QTimer Timer;
  QObject::connect( &Timer, SIGNAL(timeout()), &Animator, SLOT(updateGraphics()) );
  Timer.start(TimerInterval);
#else
  // Difficult way to do animation, but on Windows this code is fast.
  timerThread Thread(&Animator);
  Thread.start();
#endif
  
  QTime WorkTime;
  WorkTime.start();

  int RetCode = app.exec();

  qDebug() << "FPS = " << (double)Animator.count() / ( 1e-3 * WorkTime.elapsed() ) << " Max is " << 1.e3/TimerInterval;
  
  Thread.stop();
  
  return RetCode;
}

// ======================================================


