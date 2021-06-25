
// ================================================================

#include "scigraphics/qt4/plotimage.h"
#include "scigraphics/qt4/drawer.h"

#include <QString>
#include <QSize>
#include <QWidget>
#include <QApplication>
#include <QLabel>
#include <QtGlobal>

#include <cstring>
#include <cstdio>
#include <cstdlib>

// ================================================================

namespace
{
  // ----------------------------------------------------------------
  
#if QT_VERSION >= QT_VERSION_CHECK( 5, 0, 0 )
  void silentMsgHandler( QtMsgType Type, const QMessageLogContext&, const QString &Message )
  {
    if ( Type == QtFatalMsg )
    {
      std::fprintf( stderr, "Fatal: %s\n", Message.toLocal8Bit().data() );
      std::abort();
    }
  }
#else
  void silentMsgHandler( QtMsgType Type, const char *Message )
  {
    if ( Type == QtFatalMsg )
    {
      std::fprintf( stderr, "Fatal: %s\n", Message );
      std::abort();
    }
  }
#endif
  
  // ----------------------------------------------------------------

  char FakeProgramName[] = "SciGraphics";
  char* FakeArgv[2] = { FakeProgramName, NULL };
  int FakeArgc = 1;

  // ----------------------------------------------------------------

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
#if QT_VERSION >= QT_VERSION_CHECK( 5, 0, 0 )
    QtMessageHandler Handler = NULL;
    *reinterpret_cast<void**>(&Handler) = StoredMsgHandler;
    qInstallMessageHandler( Handler );
#else
    QtMsgHandler Handler = NULL;
    *reinterpret_cast<void**>(&Handler) = StoredMsgHandler;
    qInstallMsgHandler( Handler );
#endif
    delete LocalApplication;
  }
}

// ----------------------------------------------------------------

void scigraphics::qt4plotOnImage::makeQApplicationIfNeed()
{
  Q_ASSERT( LocalApplication == NULL );
  
  if ( QApplication::instance() == NULL )
  {
#if QT_VERSION >= QT_VERSION_CHECK( 5, 0, 0 )
    QtMessageHandler Handler = qInstallMessageHandler( silentMsgHandler );
#else
    QtMsgHandler Handler = qInstallMsgHandler( silentMsgHandler );
#endif
    StoredMsgHandler = *reinterpret_cast<void**>( &Handler );
    LocalApplication = new QApplication( FakeArgc, FakeArgv );
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

