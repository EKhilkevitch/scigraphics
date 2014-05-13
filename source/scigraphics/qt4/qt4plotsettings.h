
/*
 * Copyright 2011,2012,2013 Evgeniy Khilkevitch 
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

#include "scigraphics/qt4/settings.h"
#include "scigraphics/qt4/settingsbox.h"
#include "scigraphics/qt4/settingscomposer.h"

typedef scigraphics::qt4settingsGroupSuperBox qt4plotSettingsGroupSuperBox;
typedef scigraphics::qt4settingsComposerTabs qt4plotSettingsComposerTabs;

class qt4plotSettings : public scigraphics::qt4settings
{
  public:
    qt4plotSettings( QWidget *Parent = NULL, const QString &Name = QString() ) : 
      scigraphics::qt4settings( Parent, Name ) {}
    qt4plotSettings( QWidget *Parent, const QList<scigraphics::axisSetCollection::axisPosition> &Positions ) :
      scigraphics::qt4settings( Parent, Positions ) {}
    qt4plotSettings( QWidget *Parent, const QString &Name, const QList<scigraphics::axisSetCollection::axisPosition> &Positions, scigraphics::qt4settingsComposer *Composer = NULL ) :
      scigraphics::qt4settings( Parent, Name, Positions, Composer ) {}
    qt4plotSettings( QWidget *Parent, scigraphics::qt4settingsComposer *Composer ) :
      scigraphics::qt4settings( Parent, Composer ) {}
};



