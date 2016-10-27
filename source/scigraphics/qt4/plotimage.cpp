
// ================================================================

#include "scigraphics/qt4/plotimage.h"
#include "scigraphics/qt4/drawer.h"

#include <QString>
#include <QSize>
#include <QApplication>

#include <cstring>

// ================================================================

scigraphics::qt4plotOnImage::qt4plotOnImage( QSize Size ) :
  LocalApplication(NULL)
{
  makeQApplicationIfNeed();
  setDrawer( new qt4drawerOnImage(Size) );
}

// ----------------------------------------------------------------

scigraphics::qt4plotOnImage::qt4plotOnImage( size_t SizeX, size_t SizeY ) : 
  LocalApplication(NULL)
{
  makeQApplicationIfNeed();
  setDrawer( new qt4drawerOnImage( QSize(SizeX,SizeY) ) );
}

// ----------------------------------------------------------------

scigraphics::qt4plotOnImage::~qt4plotOnImage()
{
  delete LocalApplication;
}

// ----------------------------------------------------------------

void scigraphics::qt4plotOnImage::makeQApplicationIfNeed()
{
  static char Dummy[32]; 
  static int argc;
  char *argv[] = { Dummy, NULL }; 
  Q_ASSERT( LocalApplication == NULL );
  if ( QApplication::instance() == NULL )
  {
    argc = 1;
    std::strncpy( Dummy, "dummy", sizeof(Dummy)-1 );
    argv[0] = Dummy;
    argv[1] = NULL;
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

// ================================================================

