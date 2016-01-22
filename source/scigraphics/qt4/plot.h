
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

#include "scigraphics/plot.h"

#include <QWidget>

// ============================================================

namespace scigraphics
{

  // ============================================================
  
  class qt4settings;
  class qt4drawer;
  
  // ============================================================
  
  class qt4plot : public QWidget, public plot
  {
    Q_OBJECT

    friend class qt4mouseCallBack;

    protected:
      qt4drawer* getDrawerQt();
      void resizeEvent( QResizeEvent* );

    private:
      int sceneShift() const;
      void printTestCornerRectangles();

      static unsigned plotMouseModifiers( Qt::KeyboardModifiers Modifiers );
      static unsigned plotMouseButtons( const QMouseEvent *Event );
      static unsigned plotMouseButtons( QWheelEvent *Event );
      static wpoint plotMousePoisition( const QMouseEvent *Event );
      static wpoint plotMousePoisition( const QWheelEvent *Event );

      static QPixmap createTranspPixmap( const int Width, const int Height );
      
      void emitPositionObtained( const QMouseEvent *Event );
      void emitPlotChangedByMouse();
      
    public:
      explicit qt4plot( QWidget* Parent = NULL, Qt::WindowFlags Flags = 0 );
      virtual ~qt4plot();

      void resize( int Width, int Height );
      void enableDrop( bool Enable );

      void emitSelectionChanged();
      void emitSelectionChangingFinished();
     
    protected slots:
      void resizePlot();

      void mousePressed( QMouseEvent *Event );
      void mouseMoved( QMouseEvent *Event );
      void mouseReleased( QMouseEvent *Event );
      void mouseDoubleClicked( QMouseEvent *Event );
      void mouseWheel( QWheelEvent *Event );

    public slots:
      void replot();
      void setCrossCursor( bool Set );
      void updatePlotSettings( scigraphics::qt4settings *Settings );
      void updatePlotSettings();

    signals:
      void selectionChangingFinished();
      void selectionChanged();
      void dropped( QString );
      void positionObtained( double X, double Y );
      void plotChangedByMouse();
      void settingsUpdateRequired();
  };
  
  // ============================================================
}


