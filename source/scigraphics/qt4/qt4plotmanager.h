
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

// ================================================================

#include <QWidget>
#include <QList>

#include "scigraphics/qt4/qt4plotsettings.h"

class QTabWidget;
class QSettings;
class QSplitter;

// ================================================================

class qt4plotManager : public QObject
{
  Q_OBJECT

  public:
    enum settingsMode { SharedSettings, TabSettings };

  private:
    QString Name;

    QSplitter *MainSplitter;
    QList<qt4plot*> Plots;
    QList<qt4plotSettings*> Settings;

    QTabWidget *SettingsTab;

    QWidget *PlotWidget;
    QWidget *SettingsWidget;

  protected:
    void createPlots( const QList<unsigned> &PlotsInRows );
    void createPlots( const unsigned NumOfPlots, const unsigned NumOfRows );
    void setDefaultName() { setName("qt4plotManager"); }

    qt4plot* createNextPlot();
    QSplitter* createVerticalPlotSplitter( const unsigned NumOfPlotsinSplitter );
    void createSettings( const settingsMode SettingsMode, const qt4plotSettingsGroupSuperBox::axisPositionsList &Positions, qt4plotSettingsComposer *Composer);
    QWidget* createTabSettingsWidget( const qt4plotSettingsGroupSuperBox::axisPositionsList &Positions, qt4plotSettingsComposer *Composer );
    QWidget* createSharedSettingsWidget(const qt4plotSettingsGroupSuperBox::axisPositionsList &Positions, qt4plotSettingsComposer *Composer );
    qt4plotSettingsComposer* createComposer( qt4plotSettingsComposer *Composer, int Index );

    QTabWidget* tabWidget() { return SettingsTab; }
    QList<qt4plot*>& plotList() { return Plots; }

  public:

    qt4plotManager( unsigned NumOfPlots, unsigned NumOfRows = 1, QObject *Parent = NULL );

    qt4plotManager( unsigned NumOfPlots, unsigned NumOfRows, QObject *Parent,
      const settingsMode SettingsMode, const qt4plotSettingsGroupSuperBox::axisPositionsList &Positions = qt4plotSettingsGroupSuperBox::defaultAxisPositions(),
      qt4plotSettingsComposer *Composer = NULL );

    qt4plotManager( const QList<unsigned> &PlotsInRows, QObject *Parent, const settingsMode SettingsMode,
      const qt4plotSettingsGroupSuperBox::axisPositionsList &Positions = qt4plotSettingsGroupSuperBox::defaultAxisPositions(),
      qt4plotSettingsComposer *Composer = NULL );

    ~qt4plotManager();
    
    QWidget* plotWidget() { return PlotWidget; }
    QWidget* settingsWidget() { return SettingsWidget; }
    
    size_t numberOfPlots() const { return Plots.size(); }
    qt4plot* plot( unsigned i ) { return Plots[i]; }

    size_t numberOfSettings() const { return Settings.size(); }
    qt4plotSettings* settings( unsigned i ) { return Settings[i]; }

    const QString& name() const { return Name; }
    void setName( const QString &N ) { Name = N; }
   
    void setSettingsName( unsigned i, const QString &N );
    void updateTabNames();
    
    void saveSettings( QSettings* Settings );
    void loadSettings( QSettings* Settings );
    
  public slots:
    void replot();
    void show();

  signals:
    void settingsChanged();
};

// ================================================================

