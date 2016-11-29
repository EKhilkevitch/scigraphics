
#pragma once

// ============================================================

#include "scigraphics/plot.h"

class QSize;
class QString;
class QApplication;

// ============================================================

namespace scigraphics
{

  // ============================================================
  
  class qt4drawerOnImage;
  
  // ============================================================
  
  class qt4plotOnImage : public plot
  {
    private:
      QApplication *LocalApplication;
      void *StoredMsgHandler;

    protected:
      qt4drawerOnImage* getDrawerQt();

    private:
      void makeQApplicationIfNeed();
    
    public:
      explicit qt4plotOnImage( QSize Size );
      qt4plotOnImage( size_t SizeX, size_t SizeY );
      virtual ~qt4plotOnImage();

      bool write( const QString &FileName );
      bool write( const std::string &FileName );
      bool write( const char *FileName );

      void display();
  };
  
  // ============================================================

}

