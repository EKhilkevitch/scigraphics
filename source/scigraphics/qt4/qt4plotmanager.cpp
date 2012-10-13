
// ================================================================

#include "scigraphics/qt4/qt4plotmanager.h"
#include "scigraphics/qt4/qt4plotsettings.h"
#include "scigraphics/qt4/qt4plot.h"

#include <QtGui>

// ================================================================

qt4plotManager::qt4plotManager( const unsigned NumOfPlots, const unsigned NumOfRows, const settingsMode SettingsMode,
  QObject *Parent, const qt4plotSettingsGroupSuperBox::axisPositionsList &Positions, 
  qt4plotSettingsComposer *Composer ) : QObject(Parent)
{
  Name = "qt4plotManager";
  createPlots( NumOfPlots, NumOfRows );
  createSettings( NumOfPlots <= 1 ? SharedSettings : SettingsMode, Positions, Composer );
}

// ----------------------------------------------------------------

qt4plotManager::~qt4plotManager() 
{ 
  delete PlotWidget; 
  delete SettingsWidget; 
}

// ----------------------------------------------------------------

void qt4plotManager::createPlots( const unsigned NumOfPlots, const unsigned NumOfRows )
{
  QWidget *Parent = dynamic_cast<QWidget*>( parent() );
  PlotWidget = new QWidget(Parent);

  QSplitter *MainSplitter = new QSplitter( Qt::Horizontal, PlotWidget );

  while ( Plots.size() < (int)NumOfPlots )
  {
    int NumOfPlotsInSplitter = qMin<int>( NumOfRows, NumOfPlots - Plots.size() );
    QSplitter *Splitter = createVerticalPlotSplitter( NumOfPlotsInSplitter );
    MainSplitter->addWidget( Splitter );
  }

  Q_ASSERT( Plots.size() == (int)NumOfPlots );
  
  QLayout *Layout = new QStackedLayout();
  Layout->addWidget( MainSplitter );
  PlotWidget->setLayout( Layout );
}

// ----------------------------------------------------------------

QSplitter* qt4plotManager::createVerticalPlotSplitter( const unsigned NumOfPlotsInSplitter )
{
  QSplitter *Splitter = new QSplitter(PlotWidget);
  Splitter->setOrientation( Qt::Vertical );

  for ( unsigned i = 0; i < NumOfPlotsInSplitter; i++ )
  {
    qt4plot *Plot = createNextPlot();
    Splitter->addWidget( Plot );
  }

  return Splitter;
}

// ----------------------------------------------------------------

qt4plot* qt4plotManager::createNextPlot()
{
  qt4plot *Plot = new qt4plot(PlotWidget);
  Plot->setMinimumSize(200,200);
  Plots.append( Plot );
  return Plot;
}

// ----------------------------------------------------------------

void qt4plotManager::createSettings( const settingsMode SettingsMode, const qt4plotSettingsGroupSuperBox::axisPositionsList &Positions, qt4plotSettingsComposer *Composer )
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

  foreach ( qt4plotSettings *S, Settings )
    connect( S, SIGNAL(settingsChanged()), SIGNAL(settingsChanged()) );

        
  SettingsWidget->setLayout( Layout );
  SettingsWidget->setMaximumHeight( 260 );
}

// ----------------------------------------------------------------

QWidget* qt4plotManager::createTabSettingsWidget( const qt4plotSettingsGroupSuperBox::axisPositionsList &Positions,
  qt4plotSettingsComposer *Composer )
{
  SettingsTab = new QTabWidget(SettingsWidget); 
  for ( int i = 0; i < Plots.size(); i++ )  
  {
    qt4plotSettings *PlotSettings = new qt4plotSettings(SettingsWidget,"----",Positions,createComposer(Composer,i));
    Settings.append( PlotSettings );
    PlotSettings->addPlot( Plots[i] );
    SettingsTab->addTab( PlotSettings, "" );
  }
  updateTabNames();
  return SettingsTab;
}

// ----------------------------------------------------------------
    
QWidget* qt4plotManager::createSharedSettingsWidget( const qt4plotSettingsGroupSuperBox::axisPositionsList &Positions,
  qt4plotSettingsComposer *Composer )
{
  qt4plotSettings *SharedSettings = new qt4plotSettings(SettingsWidget,"",Positions,Composer);
  Settings.append( SharedSettings );
  foreach( qt4plot *Plot, Plots )
    SharedSettings->addPlot( Plot );
  return SharedSettings;
}

// ----------------------------------------------------------------

qt4plotSettingsComposer* qt4plotManager::createComposer( qt4plotSettingsComposer *Composer, int Index )
{
  if ( Composer == NULL )
    return NULL;
  if ( Index == 0 )
    return Composer;
  return Composer->createNewComposer(SettingsWidget);
}

// ----------------------------------------------------------------

void qt4plotManager::setSettingsName( unsigned i, const QString &N )
{
  Q_ASSERT( (int)i < Settings.size() );
  Settings[i]->setName(N);
  updateTabNames();
}

// ----------------------------------------------------------------

void qt4plotManager::updateTabNames()
{
  for ( int i = 0; SettingsTab != NULL && i < SettingsTab->count(); i++ )
  {
    qt4plotSettings *SettingsWgt = dynamic_cast<qt4plotSettings*>( SettingsTab->widget(i) );
    if ( SettingsWgt != NULL )
      SettingsTab->setTabText( i, SettingsWgt->name() );
  }
}

// ----------------------------------------------------------------

void qt4plotManager::saveSettings( QSettings* Settings )
{
  Settings->beginGroup( name() );
  for ( int i = 0; i < this->Settings.size(); i++ )
    this->Settings[i]->saveSettings( Settings, QString::number(i) + "_" + this->Settings[i]->name() );
  Settings->endGroup();
}

// ----------------------------------------------------------------

void qt4plotManager::loadSettings( QSettings* Settings )
{
  Settings->beginGroup( name() );
  for ( int i = 0; i < this->Settings.size(); i++ )
    this->Settings[i]->loadSettings( Settings, QString::number(i) + "_" + this->Settings[i]->name() );
  Settings->endGroup();
}

// ----------------------------------------------------------------
    
void qt4plotManager::replot()
{
  foreach ( qt4plotSettings *S, Settings )
    S->apply();
}

// ----------------------------------------------------------------

void qt4plotManager::show()
{
  PlotWidget->show();
  SettingsWidget->show();
}

// ================================================================

