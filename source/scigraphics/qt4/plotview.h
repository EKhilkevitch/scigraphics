
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

#include <QGraphicsView>

// ================================================================

namespace scigraphics 
{

  // ================================================================

  class qt4plotView : public QGraphicsView
  {
    Q_OBJECT

    private:
      bool DragEnabled;

    protected:
      virtual void resizeEvent( QResizeEvent* )                   { emit resized(); }
      virtual void mousePressEvent( QMouseEvent *Event )          { emit mousePressed( Event ); }
      virtual void mouseMoveEvent( QMouseEvent *Event )           { emit mouseMoved( Event ); }
      virtual void mouseReleaseEvent( QMouseEvent *Event )        { emit mouseReleased( Event ); } 
      virtual void mouseDoubleClickEvent( QMouseEvent *Event )    { emit mouseDoubleClick( Event ); }
      virtual void keyReleaseEvent( QKeyEvent *Event )            { emit keyReleased( Event ); }
      virtual void wheelEvent( QWheelEvent* Event )               { emit mouseWheel( Event ); }
     
      virtual void dragMoveEvent(QDragMoveEvent *event);
      virtual void dragEnterEvent( QDragEnterEvent *Event );
      virtual void dropEvent( QDropEvent *Event );

    public:
      qt4plotView( QWidget* Parent = NULL );
      qt4plotView( QGraphicsScene *Scene, QWidget *Parent = NULL );

      void enableDrop( bool Enable );

    signals:
      void resized();

      void mousePressed( QMouseEvent *Event );
      void mouseMoved( QMouseEvent *Event );
      void mouseReleased( QMouseEvent *Event );
      void mouseDoubleClick( QMouseEvent *Event );
      void mouseWheel( QWheelEvent* Event );
      void keyReleased( QKeyEvent *Event );

      void dropped( QString );
  };
  
  // ================================================================

}

