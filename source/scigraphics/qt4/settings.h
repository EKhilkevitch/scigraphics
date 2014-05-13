
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

// ================================================================

#include "scigraphics/settings.h"

#include <QWidget>
#include <QList>

class QSettings;

// ================================================================

namespace scigraphics
{

  // ================================================================
  
  class qt4plot;
  class qt4settingsComposer;
  class qt4settingsGroupBox;
  class qt4settingsGroupSuperBox;
  
  // ================================================================

  class qt4settings : public QWidget, public settings
  {
    Q_OBJECT

    private:
      QString SettingsName;
      qt4settingsComposer *SettingsComposer;

    protected:
      void updateSettingsFromSubWidgets();
      virtual void applySettings( qt4plot *Plot );
      void emitSettingsChanged();

      void setComposer( qt4settingsComposer *Composer );

      void initSettings( const QString &Name, const QList<axisSetCollection::axisPosition> &Positions,
        qt4settingsComposer *Composer );

    public:
      qt4settings( QWidget *Parent = NULL, const QString &Name = QString() );
      qt4settings( QWidget *Parent, const QList<axisSetCollection::axisPosition> &Positions );
      qt4settings( QWidget *Parent, const QString &Name, const QList<axisSetCollection::axisPosition> &Positions, qt4settingsComposer *Composer = NULL );
      qt4settings( QWidget *Parent, qt4settingsComposer *Composer );

      const QString& name() const { return SettingsName; }
      void setName( const QString &Name ) { SettingsName = Name; }

      void saveSettings( const QString &FileName ) const;
      void loadSettings( const QString &FileName );

      void saveSettings( QSettings *Settings, const QString &Prefix = QString() ) const;
      void loadSettings( QSettings *Settings, const QString &Prefix = QString() );

      void addSettingWidget( qt4settingsGroupBox *Box );
      void delSettingWidget( unsigned Index );
      qt4settingsGroupBox* getSettingWidget( unsigned Index );

      void connectToPlot( qt4plot *Plot );
      void disconnectFromPlot( qt4plot *Plot );

      void apply( qt4plot *Plot );

    public slots:
      void updatePlotState();
      void updatePlotSettings();

    signals:
      void settingsChanged();
      void settingsChanged( const settings& );
  };

  // ================================================================

}

