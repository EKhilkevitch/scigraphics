
// ================================================================

#include "scigraphics/qt4/plotimage.h"
#include "scigraphics/qt4/drawer.h"

#include <QString>
#include <QSize>
#include <QWidget>
#include <QApplication>
#include <QLabel>

#include <cstring>
#include <cstdio>
#include <cstdlib>

// ================================================================

namespace
{
  void silentMsgHandler( QtMsgType Type, const char *Message )
  {
    switch ( Type ) 
    {
      case QtDebugMsg:
        break;

      case QtWarningMsg:
        break;

      case QtCriticalMsg:
        break;

      case QtFatalMsg:
        std::fprintf( stderr, "Fatal: %s\n", Message );
        std::abort();
    }
  }
}

// ================================================================

scigraphics::qt4plotOnImage::qt4plotOnImage( QSize Size ) :
  LocalApplication(NULL),
  StoredMsgHandler(NULL)
{
  makeQApplicationIfNeed();
  setDrawer( new qt4drawerOnImage(Size) );
  
  setVisibleCursorPositionViewer(false);
}

// ----------------------------------------------------------------

scigraphics::qt4plotOnImage::qt4plotOnImage( size_t SizeX, size_t SizeY ) : 
  LocalApplication(NULL),
  StoredMsgHandler(NULL)
{
  makeQApplicationIfNeed();
  setDrawer( new qt4drawerOnImage( QSize( static_cast<int>(SizeX), static_cast<int>(SizeY) ) ) );
}

// ----------------------------------------------------------------

scigraphics::qt4plotOnImage::~qt4plotOnImage()
{
  if ( LocalApplication != NULL )
  {
    qInstallMsgHandler( reinterpret_cast<QtMsgHandler>(StoredMsgHandler) );
    delete LocalApplication;
  }
}

// ----------------------------------------------------------------

void scigraphics::qt4plotOnImage::makeQApplicationIfNeed()
{
  Q_ASSERT( LocalApplication == NULL );
  
  static char Dummy[32]; 
  static int argc;
  static char *argv[] = { NULL, NULL }; 

  if ( QApplication::instance() == NULL )
  {
    StoredMsgHandler = reinterpret_cast<void*>( qInstallMsgHandler( silentMsgHandler ) );

    std::strncpy( Dummy, "dummy", sizeof(Dummy)-1 );
    argv[0] = Dummy;
    argv[1] = NULL;
    argc = 1;
    LocalApplication = new QApplication(argc,argv);
  }
}

// ----------------------------------------------------------------
    
scigraphics::qt4drawerOnImage* scigraphics::qt4plotOnImage::getDrawerQt() 
{ 
  return dynamic_cast<qt4drawerOnImage*>( getDrawer() ); 
}

// ----------------------------------------------------------------

bool scigraphics::qt4plotOnImage::write( const QString &FileName )
{
  replot();
  return getDrawerQt()->write( FileName );
}

// ----------------------------------------------------------------

bool scigraphics::qt4plotOnImage::write( const std::string &FileName )
{
  return write( QString::fromStdString(FileName) );
}

// ----------------------------------------------------------------

bool scigraphics::qt4plotOnImage::write( const char *FileName )
{
  if ( FileName == NULL )
    return false;
  return write( QString(FileName) );
}

// ----------------------------------------------------------------

void scigraphics::qt4plotOnImage::display()
{
  replot();

  const QPixmap &Pixmap = getDrawerQt()->pixmap();

  QLabel *Label = new QLabel();
  Label->setPixmap( Pixmap );
  Label->show();
  QApplication::instance()->exec();
}

// ================================================================

