
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


#pragma once

// ============================================================

#include "scigraphics/drawer.h"

class QWidget;
class QGraphicsScene;
class QPainter;
class QGraphicsPixmapItem;
class QPixmap;
class QColor;
class QPoint;
class QRect;
class QPolygon;
class QPen;
class QFont;
class QBrush;
class QSize;
class QString;

// ================================================================

namespace scigraphics
{

  // ================================================================
  
  class qt4plotView;
  
  // ================================================================
  
  class qt4drawer : public scigraphics::drawer
  {
    private:
      QPainter *Painter;
      QPixmap *PlotPixmap;

    private:
      qt4drawer( const qt4drawer& );
      qt4drawer& operator=( const qt4drawer& );

    public:
      static QColor colorQt( color Color );
      static QPoint pointQt( const wpoint& Point );
      static QRect  rectangleQt( const wrectangle& Rect );
      static QPen   penQt( const lineStyle& Style );
      static QFont  fontQt( const textStyle &Style );
      static QBrush brushQt( const brushStyle &Style );
      static QPolygon polygonQt( const std::vector<wpoint> &Points );
      static QString stringQt( const std::string &String );
     
    public:
      void setLineStyle( const lineStyle &Style );
      void setBrushStyle( const brushStyle &Style );
      void setTextStyle( const textStyle &Style );

      void drawLine( const wpoint &A, const wpoint &B );
      void drawPolyline( const std::vector<wpoint> &Points );
      void drawRectangle( const wrectangle& Rectangle );
      void drawPolygon( const std::vector<wpoint> &Points );
      void drawText( const std::string &Text, const wrectangle& Rectangle, double RotAngle );
      void eraseRectangle( const wrectangle& Rectangle );
      void eraseAll();
      void flush();

      std::pair<wcoord,wcoord> textWidthHeight( const std::string &Text, const textStyle &Style );
      
      wcoord width()  const;
      wcoord height() const;

    public:
      explicit qt4drawer( QSize Sizes );
      ~qt4drawer();
      
      const QPixmap& pixmap() const;
  };
  
  // ================================================================
  
  class qt4drawerOnWidget : public qt4drawer
  {
    private:
      QWidget *const Parent;
      qt4plotView *View;
      QGraphicsScene *Scene;
      QGraphicsPixmapItem *PixmapItem;

    private:
      qt4drawerOnWidget( const qt4drawerOnWidget& );
      qt4drawerOnWidget& operator=( const qt4drawerOnWidget& );

    public:
      explicit qt4drawerOnWidget( QWidget *Parent );
      ~qt4drawerOnWidget();
      
      wcoord width()  const;
      wcoord height() const;
      
      void flush();
      
      QGraphicsScene* scene();
      qt4plotView* view();
  };
  
  // ================================================================

  class qt4drawerOnImage : public qt4drawer
  {
    public:
      explicit qt4drawerOnImage( QSize Size );
      ~qt4drawerOnImage();

      bool write( const QString &FileName, int Quality = 100 );
  };
  
  // ================================================================

}


