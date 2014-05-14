
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

#include "scigraphics/imagemagick/plot.h"
#include "scigraphics/imagemagick/drawer.h"

// ================================================================

scigraphics::implot::implot( size_t SizeX, size_t SizeY )
{
  imdrawer *Drawer = new imdrawer(SizeX,SizeY);
  setDrawer( Drawer );

  setVisibleCursorPositionViewer(false);
}

// ----------------------------------------------------------------

scigraphics::imdrawer* scigraphics::implot::getDrawerIm() 
{ 
  return dynamic_cast<imdrawer*>( getDrawer() ); 
}

// ----------------------------------------------------------------
    
void scigraphics::implot::write( const std::string &FileName )
{
  imdrawer *Drawer = getDrawerIm();
  if ( Drawer == NULL )
    return;

  replot();
  Drawer->write( FileName );
}

// ----------------------------------------------------------------
    
void scigraphics::implot::display()
{
  imdrawer *Drawer = getDrawerIm();
  if ( Drawer == NULL )
    return;

  replot();
  Drawer->display();
}

// ================================================================

