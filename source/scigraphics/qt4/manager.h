
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

#include <QObject>
#include <QList>

class QWidget;
class QSplitter;
class QTabWidget;
class QSettings;

// ================================================================

namespace scigraphics
{

  // ================================================================
  
  class qt4plot;
  class qt4settings;
  class qt4settingsComposer;
  
  // ================================================================

  class qt4plotManager : public QObject
  {
    Q_OBJECT

    public:
      enum settingsMode 
      { 
        SharedSettings, 
        TabSettings 
      };

    private:
      QString Name;

      QSplitter *MainSplitter;
      QList<qt4plot*> Plots;
      QList<qt4settings*> Settings;

      QTabWidget *SettingsTab;

      QWidget *PlotWidget;
      QWidget *SettingsWidget;

    protected:
      void createPlots( const QList<size_t> &PlotsInRows );
      void createPlots( size_t NumOfPlots, size_t NumOfRows );
      void setDefaultName();

      qt4plot* createNextPlot();
      QSplitter* createVerticalPlotSplitter( size_t NumOfPlotsinSplitter );
      void createSettings( const settingsMode SettingsMode, const QList<axisPosition> &Positions, qt4settingsComposer *Composer);
      QWidget* createTabSettingsWidget( const QList<axisPosition> &Positions, qt4settingsComposer *Composer );
      QWidget* createSharedSettingsWidget(const QList<axisPosition> &Positions, qt4settingsComposer *Composer );
      qt4settingsComposer* createComposer( qt4settingsComposer *Composer, int Index );

      QTabWidget* tabWidget() { return SettingsTab; }
      QList<qt4plot*>& plotList() { return Plots; }

    public:
      explicit qt4plotManager( size_t NumOfPlots, size_t NumOfRows = 1, QObject *Parent = NULL );
      qt4plotManager( size_t NumOfPlots, size_t NumOfRows, QObject *Parent, const settingsMode SettingsMode );
      qt4plotManager( size_t NumOfPlots, size_t NumOfRows, QObject *Parent, const settingsMode SettingsMode, const QList<axisPosition> &Positions,
        qt4settingsComposer *Composer = NULL );
      ~qt4plotManager();
      
      QWidget* plotWidget() { return PlotWidget; }
      QWidget* settingsWidget() { return SettingsWidget; }
      
      size_t numberOfPlots() const;
      qt4plot* plot( size_t Index );

      size_t numberOfSettings() const;
      qt4settings* settings( size_t Index );

      const QString& name() const;
      void setName( const QString &Name );

      void setPlotVisible( size_t Index, bool Visible );
     
      void setSettingsName( size_t Index, const QString &Name );
      void updateTabNames();
      
      void saveSettings( QSettings* Settings ) const;
      void loadSettings( QSettings* Settings );
      
    public slots:
      void replot();
      void show();

    signals:
      void settingsChanged();
  };
  
  // ================================================================

}

