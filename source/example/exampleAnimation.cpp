
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
  QObject::connect( &Timer, SIGNAL(timeout()), &Animator, SLOT(update()) );
  Timer.start(20);
  
  QTime WorkTime;
  WorkTime.start();

  auto RetCode = app.exec();

  qDebug() << "FPS = " << (double)Animator.count() / ( 1e-3 * WorkTime.elapsed() ) << " Max is " << 1.e3/Timer.interval();
  

  return RetCode;
}

// ======================================================


