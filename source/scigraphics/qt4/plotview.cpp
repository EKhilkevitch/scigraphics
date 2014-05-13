
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


// ================================================================

#include "scigraphics/qt4/plotview.h"

#include <QDragEnterEvent>
#include <QUrl>

// ================================================================

void scigraphics::qt4plotView::dragEnterEvent( QDragEnterEvent *Event )
{
  if ( DragEnabled )
    Event->acceptProposedAction();
}
    
// ----------------------------------------------------------------

void scigraphics::qt4plotView::dragMoveEvent( QDragMoveEvent *Event )
{
  if ( DragEnabled )
    Event->acceptProposedAction();
}

// ----------------------------------------------------------------

void scigraphics::qt4plotView::dropEvent( QDropEvent *Event )
{
  if ( DragEnabled )
  {
    QString String;
    String = "";
    QList<QUrl> List = Event->mimeData()->urls();
    foreach ( QUrl Url, List )
      String += Url.toString() + "\n";
    Event->acceptProposedAction();
    emit dropped( String );
  }
}

// ----------------------------------------------------------------
      
scigraphics::qt4plotView::qt4plotView( QWidget* Parent ) : 
  QGraphicsView(Parent), 
  DragEnabled(false) 
{ 
  setMouseTracking(true); 
}

// ----------------------------------------------------------------

scigraphics::qt4plotView::qt4plotView( QGraphicsScene *Scene, QWidget *Parent ) : 
  QGraphicsView(Scene,Parent), 
  DragEnabled(false) 
{
}

// ----------------------------------------------------------------

void scigraphics::qt4plotView::enableDrop( bool Enable ) 
{ 
  setAcceptDrops(Enable); 
  DragEnabled = Enable; 
}

// ================================================================
    
