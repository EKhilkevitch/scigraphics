
#pragma once

// ================================================================

#include <QWidget>
#include <QList>

#include "qt4plotsettings.h"

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

    qt4plot* createNextPlot();
    QSplitter* createVerticalPlotSplitter( const unsigned NumOfPlotsinSplitter );
    void createSettings( const settingsMode SettingsMode, const qt4plotSettingsGroupSuperBox::axisPositionsList &Positions, qt4plotSettingsComposer *Composer);
    QWidget* createTabSettingsWidget( const qt4plotSettingsGroupSuperBox::axisPositionsList &Positions, qt4plotSettingsComposer *Composer );
    QWidget* createSharedSettingsWidget(const qt4plotSettingsGroupSuperBox::axisPositionsList &Positions, qt4plotSettingsComposer *Composer );
    qt4plotSettingsComposer* createComposer( qt4plotSettingsComposer *Composer, int Index );

    QTabWidget* tabWidget() { return SettingsTab; }
    QList<qt4plot*>& plotList() { return Plots; }

  public:
    qt4plotManager( const unsigned NumOfPlots, const unsigned NumOfRows = 1, 
      const settingsMode SettingsMode = TabSettings, QObject *Parent = NULL,
      const qt4plotSettingsGroupSuperBox::axisPositionsList &Positions = qt4plotSettingsGroupSuperBox::defaultAxisPositions(),
      qt4plotSettingsComposer *Composer = NULL );
    qt4plotManager( const QList<unsigned> &PlotsInRows, const settingsMode SettingsMode = TabSettings, QObject *Parent = NULL,
      const qt4plotSettingsGroupSuperBox::axisPositionsList &Positions = qt4plotSettingsGroupSuperBox::defaultAxisPositions(),
      qt4plotSettingsComposer *Composer = NULL );
    ~qt4plotManager();
    
    QWidget* plotWidget() { return PlotWidget; }
    QWidget* settingsWidget() { return SettingsWidget; }
    
    size_t numOfPlots() const { return Plots.size(); }
    qt4plot* getPlot( unsigned i ) { return Plots[i]; }

    size_t numOfSettings() const { return Settings.size(); }
    qt4plotSettings* getSettings( unsigned i ) { return Settings[i]; }

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

