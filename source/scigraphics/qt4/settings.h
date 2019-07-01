
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

#include "scigraphics/axisposition.h"

#include <QWidget>
#include <QList>

class QSettings;

// ================================================================

namespace scigraphics
{

  // ================================================================
 
  class settings;
  class plot;

  class qt4plot;
  class qt4settingsComposer;
  class qt4settingsGroupBox;
  class qt4settingsGroupSuperBox;
  
  // ================================================================

  class qt4settings : public QWidget
  {
    Q_OBJECT

    private:
      settings *Settings;

      QString SettingsName;
      qt4settingsComposer *SettingsComposer;
      bool NeedToEmitSelectionChangedAfterApplying;

    private:
      void updateSettingsFromSubWidgets() const;
      void updateSubWidgetsFromSettings();
      void applySettings( plot *Plot );
      void emitSettingsChanged();

      void setComposer( qt4settingsComposer *Composer );

      void initSettings( const QString &Name, const QList<axisPosition> &Positions,
        qt4settingsComposer *Composer );

    private:
      qt4settings( const qt4settings& );
      qt4settings& operator=( const qt4settings& );

    private slots:
      void setEmitSelectionChangedAfterApplyingFlag( bool NeedToEmit );

    public:
      explicit qt4settings( QWidget *Parent = NULL, const QString &Name = QString() );
      qt4settings( QWidget *Parent, const QList<axisPosition> &Positions );
      qt4settings( QWidget *Parent, const QString &Name, const QList<axisPosition> &Positions, qt4settingsComposer *Composer = NULL );
      qt4settings( QWidget *Parent, qt4settingsComposer *Composer );
      ~qt4settings();

      const QString& name() const;
      void setName( const QString &Name );

      void saveSettings( const QString &FileName ) const;
      void saveSettings( QSettings *Settings, const QString &Prefix = QString() ) const;
      void loadSettings( const QString &FileName );
      void loadSettings( QSettings *Settings, const QString &Prefix = QString() );
      QString serialize() const;
      bool deserialize( const QString &String );

      void addSettingWidget( qt4settingsGroupBox *Box );
      void delSettingWidget( size_t Index );
      qt4settingsGroupBox* getSettingWidget( size_t Index );
      template <class widgetType> widgetType* getSettingWidget();
      size_t numberOfSettingsWidget() const;

      void connectToPlot( qt4plot *Plot );
      void disconnectFromPlot( qt4plot *Plot );

      const settings& plotSettings() const;
      void setPlotSettings( const settings &Settings );

      void apply( qt4plot *Plot );

    public slots:
      void updatePlotState();
      void updatePlotSettings();

    signals:
      void settingsChanged();
      void settingsChanged( scigraphics::qt4settings* );
  };

  // ================================================================
  
  template <class widgetType> widgetType* qt4settings::getSettingWidget()
  {
    size_t NumberOfSettingsWidget = numberOfSettingsWidget();
    for ( size_t i = 0; i < NumberOfSettingsWidget; i++ )
    {
      widgetType* Widget = dynamic_cast<widgetType*>( getSettingWidget(i) );
      if ( Widget != NULL )
        return Widget;
    }
    return NULL;
  }
  
  // ================================================================

}

