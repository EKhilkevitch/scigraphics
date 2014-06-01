
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

#warning "Usage of this file is deprecated"

#include "scigraphics/qt4/manager.h"
#include "scigraphics/qt4/qt4plot.h"
#include "scigraphics/qt4/qt4plotsettings.h"

#include <cassert>

class qt4plotManager : public scigraphics::qt4plotManager
{
  public:
    qt4plotManager( unsigned NumOfPlots, unsigned NumOfRows = 1, QObject *Parent = NULL ) : 
      scigraphics::qt4plotManager( NumOfPlots, NumOfRows, Parent ) {}
    qt4plotManager( unsigned NumOfPlots, unsigned NumOfRows, QObject *Parent, const settingsMode SettingsMode ) :
      scigraphics::qt4plotManager( NumOfPlots, NumOfRows, Parent, SettingsMode ) {}
    inline qt4plot* plot( unsigned i );
    inline qt4plotSettings* settings( unsigned i );
};

qt4plot* qt4plotManager::plot( unsigned i ) 
{ 
  assert( sizeof(qt4plot) == sizeof(scigraphics::qt4plot) );
  return reinterpret_cast<qt4plot*>(scigraphics::qt4plotManager::plot(i)); 
}
    
inline qt4plotSettings* qt4plotManager::settings( unsigned i )
{
  assert( sizeof(qt4plotSettings) == sizeof(scigraphics::qt4settings) );
  return reinterpret_cast<qt4plotSettings*>(scigraphics::qt4plotManager::settings(i));
}


