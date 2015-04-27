
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

#include "scigraphics/qt4/manager.h"
#include "scigraphics/qt4/settings.h"
#include "scigraphics/qt4/settingsbox.h"
#include "scigraphics/qt4/settingscomposer.h"
#include "scigraphics/qt4/plot.h"

#include <QSplitter>
#include <QSettings>
#include <QStackedLayout>
#include <QTabWidget>
#include <QDebug>

// ================================================================

scigraphics::qt4plotManager::qt4plotManager( unsigned NumOfPlots, unsigned NumOfRows, QObject *Parent ) :
  QObject( Parent )
{
  setDefaultName();
  createPlots( NumOfPlots, NumOfRows );
  createSettings( SharedSettings, qt4settingsGroupSuperBox::defaultAxisPositions(), NULL );
}

// ----------------------------------------------------------------

scigraphics::qt4plotManager::qt4plotManager( unsigned NumOfPlots, unsigned NumOfRows, QObject *Parent,
      const settingsMode SettingsMode ) : 
  QObject(Parent)
{
  setDefaultName();
  createPlots( NumOfPlots, NumOfRows );
  createSettings( Plots.size() <= 1 ? SharedSettings : SettingsMode, qt4settingsGroupSuperBox::defaultAxisPositions(), NULL );
}

// ----------------------------------------------------------------

scigraphics::qt4plotManager::qt4plotManager( unsigned NumOfPlots, unsigned NumOfRows, QObject *Parent,
      const settingsMode SettingsMode, const QList<axisSetCollection::axisPosition> &Positions,
      qt4settingsComposer *Composer ) : 
  QObject(Parent)
{
  setDefaultName();
  createPlots( NumOfPlots, NumOfRows );
  createSettings( Plots.size() <= 1 ? SharedSettings : SettingsMode, Positions, Composer );
}

// ----------------------------------------------------------------

scigraphics::qt4plotManager::~qt4plotManager() 
{ 
  delete PlotWidget; 
  delete SettingsWidget; 
}

// ----------------------------------------------------------------

size_t scigraphics::qt4plotManager::numberOfPlots() const 
{ 
  return Plots.size(); 
}

// ----------------------------------------------------------------
      
scigraphics::qt4plot* scigraphics::qt4plotManager::plot( size_t Index ) 
{ 
  return Plots[Index]; 
}

// ----------------------------------------------------------------

size_t scigraphics::qt4plotManager::numberOfSettings() const 
{ 
  return Settings.size(); 
}

// ----------------------------------------------------------------
      
scigraphics::qt4settings* scigraphics::qt4plotManager::settings( size_t Index ) 
{ 
  return Settings[Index]; 
}

// ----------------------------------------------------------------
      
const QString& scigraphics::qt4plotManager::name() const 
{ 
  return Name; 
}

// ----------------------------------------------------------------

void scigraphics::qt4plotManager::setName( const QString &N ) 
{ 
  Name = N; 
}

// ----------------------------------------------------------------

void scigraphics::qt4plotManager::createPlots( const QList<unsigned> &PlotsInRows )
{
  QWidget *Parent = dynamic_cast<QWidget*>( parent() );
  PlotWidget = new QWidget(Parent);

  MainSplitter = new QSplitter( Qt::Vertical, PlotWidget );

  for ( int Row = 0; Row < PlotsInRows.size(); ++Row )
  {
    QSplitter *Splitter = createVerticalPlotSplitter( PlotsInRows[Row] );
    MainSplitter->addWidget( Splitter );
  }

  QLayout *Layout = new QStackedLayout();
  Layout->addWidget( MainSplitter );
  PlotWidget->setLayout( Layout );
}

// ----------------------------------------------------------------

void scigraphics::qt4plotManager::createPlots( const unsigned NumOfPlots, const unsigned NumOfRows )
{
  QList<unsigned> PlotsInRows;
  unsigned PlotsInList = 0;
  while ( PlotsInList < NumOfPlots )
  {
    int NumOfPlotsInRow = NumOfPlots / NumOfRows + ( NumOfPlots % NumOfRows == 0 ? 0 : 1 );
    NumOfPlotsInRow = qMin<int>( NumOfPlotsInRow, NumOfPlots - PlotsInList );
    Q_ASSERT( NumOfPlotsInRow > 0 );
    PlotsInRows.append( NumOfPlotsInRow );
    PlotsInList += NumOfPlotsInRow;
  }
  Q_ASSERT( PlotsInList == NumOfPlots );

  createPlots( PlotsInRows );
}

// ----------------------------------------------------------------

void scigraphics::qt4plotManager::setDefaultName() 
{ 
  setName( "qt4plotManager" ); 
}

// ----------------------------------------------------------------

QSplitter* scigraphics::qt4plotManager::createVerticalPlotSplitter( const unsigned NumOfPlotsInSplitter )
{
  QSplitter *Splitter = new QSplitter(PlotWidget);
  Splitter->setOrientation( Qt::Horizontal );

  for ( unsigned i = 0; i < NumOfPlotsInSplitter; i++ )
  {
    qt4plot *Plot = createNextPlot();
    Splitter->addWidget( Plot );
  }

  return Splitter;
}

// ----------------------------------------------------------------

scigraphics::qt4plot* scigraphics::qt4plotManager::createNextPlot()
{
  qt4plot *Plot = new qt4plot(PlotWidget);
  Plot->setMinimumSize(200,200);
  Plots.append( Plot );
  return Plot;
}

// ----------------------------------------------------------------

void scigraphics::qt4plotManager::createSettings( const settingsMode SettingsMode, 
  const qt4settingsGroupSuperBox::axisPositionsList &Positions, qt4settingsComposer *Composer )
{
  QWidget *Parent = dynamic_cast<QWidget*>( parent() );
  SettingsWidget = new QWidget(Parent);
  SettingsTab = NULL;

  QStackedLayout *Layout = new QStackedLayout();

  switch ( SettingsMode )
  {
    case SharedSettings:
      Layout->addWidget( createSharedSettingsWidget(Positions,Composer) );
      break;
    case TabSettings:
      Layout->addWidget( createTabSettingsWidget(Positions,Composer) );
      break;
  }

  foreach ( qt4settings *S, Settings )
    connect( S, SIGNAL(settingsChanged()), SIGNAL(settingsChanged()) );

        
  SettingsWidget->setLayout( Layout );
  SettingsWidget->setMaximumHeight( 260 );
}

// ----------------------------------------------------------------

QWidget* scigraphics::qt4plotManager::createTabSettingsWidget( const qt4settingsGroupSuperBox::axisPositionsList &Positions,
  qt4settingsComposer *Composer )
{
  SettingsTab = new QTabWidget(SettingsWidget); 
  for ( int i = 0; i < Plots.size(); i++ )  
  {
    qt4settings *PlotSettings = new qt4settings(SettingsWidget,"----",Positions,createComposer(Composer,i));
    Settings.append( PlotSettings );
    PlotSettings->connectToPlot( Plots[i] );
    SettingsTab->addTab( PlotSettings, "" );
  }
  updateTabNames();
  return SettingsTab;
}

// ----------------------------------------------------------------
    
QWidget* scigraphics::qt4plotManager::createSharedSettingsWidget( const qt4settingsGroupSuperBox::axisPositionsList &Positions,
  qt4settingsComposer *Composer )
{
  qt4settings *SharedSettings = new qt4settings(SettingsWidget,"",Positions,Composer);
  Settings.append( SharedSettings );
  foreach( qt4plot *Plot, Plots )
    SharedSettings->connectToPlot( Plot );
  return SharedSettings;
}

// ----------------------------------------------------------------

scigraphics::qt4settingsComposer* scigraphics::qt4plotManager::createComposer( qt4settingsComposer *Composer, int Index )
{
  if ( Composer == NULL )
    return NULL;
  if ( Index == 0 )
    return Composer;
  return Composer->createNewComposer(SettingsWidget);
}

// ----------------------------------------------------------------

void scigraphics::qt4plotManager::setSettingsName( size_t Index, const QString &Name )
{
  if ( Index >= static_cast<size_t>(Settings.size()) )
  {
    qWarning() << "setSettingsName: index " << Index << "out of rage: 0 .." << Settings.size();
    return;
  } 

  Settings[Index]->setName(Name);
  updateTabNames();
}

// ----------------------------------------------------------------

void scigraphics::qt4plotManager::updateTabNames()
{
  for ( int i = 0; SettingsTab != NULL && i < SettingsTab->count(); i++ )
  {
    qt4settings *SettingsWgt = dynamic_cast<qt4settings*>( SettingsTab->widget(i) );
    if ( SettingsWgt != NULL )
      SettingsTab->setTabText( i, SettingsWgt->name() );
  }
}

// ----------------------------------------------------------------

void scigraphics::qt4plotManager::saveSettings( QSettings* Settings ) const
{
  Settings->beginGroup( name() );
  for ( int i = 0; i < this->Settings.size(); i++ )
    this->Settings[i]->saveSettings( Settings, QString::number(i) + "_" + this->Settings[i]->name() );
  
  QList<int> MainSplitterSizes = MainSplitter->sizes();
  Q_ASSERT( MainSplitterSizes.size() == MainSplitter->count() );
  for ( int i = 0; i < MainSplitterSizes.size(); i++ ) 
    Settings->setValue( "MainSplitterSizes"+QString::number(i), MainSplitterSizes[i] );
  Settings->endGroup();
}

// ----------------------------------------------------------------

void scigraphics::qt4plotManager::loadSettings( QSettings* Settings )
{
  Settings->beginGroup( name() );
  for ( int i = 0; i < this->Settings.size(); i++ )
    this->Settings[i]->loadSettings( Settings, QString::number(i) + "_" + this->Settings[i]->name() );
  QList<int> MainSplitterSizes = MainSplitter->sizes();
  for ( int i = 0; i < MainSplitterSizes.size(); i++ )
    MainSplitterSizes[i] = Settings->value( "MainSplitterSizes" + QString::number(i), MainSplitter->height()/MainSplitter->count() ).toInt();
  int SizesSum = 0;
  for ( int i = 0; i < MainSplitterSizes.size(); i++ )
    SizesSum += MainSplitterSizes[i];
  if ( SizesSum > 0 )
    MainSplitter->setSizes( MainSplitterSizes );
  Settings->endGroup();
}

// ----------------------------------------------------------------
    
void scigraphics::qt4plotManager::replot()
{
  foreach ( qt4settings *S, Settings )
    S->updatePlotSettings();
}

// ----------------------------------------------------------------

void scigraphics::qt4plotManager::show()
{
  PlotWidget->show();
  SettingsWidget->show();
}

// ================================================================

