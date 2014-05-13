
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

#include "scigraphics/qt4/mousecallback.h"
#include "scigraphics/qt4/plot.h"

// ================================================================

scigraphics::qt4mouseCallBack::qt4mouseCallBack( qt4plot &Plot ) : 
  mouseCallBack(Plot) 
{
}

// ----------------------------------------------------------------

scigraphics::qt4plot& scigraphics::qt4mouseCallBack::getQt4Plot() 
{
  return dynamic_cast<qt4plot&>( getPlot() );
}

// ----------------------------------------------------------------
    
void scigraphics::qt4mouseCallBack::onPressed( mouse::mouseActionHandler* )
{
  getQt4Plot().emitPlotChangedByMouse();
}

// ----------------------------------------------------------------

void scigraphics::qt4mouseCallBack::onMoved( mouse::mouseActionHandler *Handler )
{
  if ( isSelectionMouseAction(Handler) )
    getQt4Plot().emitSelectionChanged();
  getQt4Plot().emitPlotChangedByMouse();
}

// ----------------------------------------------------------------

void scigraphics::qt4mouseCallBack::onRelease( mouse::mouseActionHandler *Handler )
{
  if ( isSelectionMouseAction(Handler) )
  {
    getQt4Plot().emitSelectionChanged();
    getQt4Plot().emitSelectionChangingFinished();
  } 
  getQt4Plot().emitPlotChangedByMouse();
}

// ----------------------------------------------------------------
    
void scigraphics::qt4mouseCallBack::onWheel( mouse::mouseWheelHandler* )
{
  getQt4Plot().emitPlotChangedByMouse();
}

// ----------------------------------------------------------------

bool scigraphics::qt4mouseCallBack::isSelectionMouseAction( mouse::mouseActionHandler *Handler )
{
  return ( 
            dynamic_cast< mouse::selectAction* >(Handler)         != NULL   ||
            dynamic_cast< mouse::moveSelectionAction* >(Handler)  != NULL   ||
            dynamic_cast< mouse::resetSelectionAction* >(Handler) != NULL   
         );
}

// ================================================================

