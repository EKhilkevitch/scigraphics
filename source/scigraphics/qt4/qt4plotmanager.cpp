
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
  createSettings( Plots.size() <= 1 ? SharedSettings : SettingsMode, Positions, Composer );
}

// ----------------------------------------------------------------
    
qt4plotManager::qt4plotManager( const QList<unsigned> &PlotsInRows, const settingsMode SettingsMode, QObject *Parent,
      const qt4plotSettingsGroupSuperBox::axisPositionsList &Positions,
      qt4plotSettingsComposer *Composer ) : QObject(Parent)
{
  Name = "qt4plotManager";
  createPlots( PlotsInRows );
  createSettings( Plots.size() <= 1 ? SharedSettings : SettingsMode, Positions, Composer );
}

// ----------------------------------------------------------------

qt4plotManager::~qt4plotManager() 
{ 
  delete PlotWidget; 
  delete SettingsWidget; 
}

// ----------------------------------------------------------------

void qt4plotManager::createPlots( const QList<unsigned> &PlotsInRows )
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

void qt4plotManager::createPlots( const unsigned NumOfPlots, const unsigned NumOfRows )
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

QSplitter* qt4plotManager::createVerticalPlotSplitter( const unsigned NumOfPlotsInSplitter )
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
  
  QList<int> MainSplitterSizes = MainSplitter->sizes();
  Q_ASSERT( MainSplitterSizes.size() == MainSplitter->count() );
  for ( int i = 0; i < MainSplitterSizes.size(); i++ ) 
    Settings->setValue( "MainSplitterSizes"+QString::number(i), MainSplitterSizes[i] );
  Settings->endGroup();
}

// ----------------------------------------------------------------

void qt4plotManager::loadSettings( QSettings* Settings )
{
  Settings->beginGroup( name() );
  for ( int i = 0; i < this->Settings.size(); i++ )
    this->Settings[i]->loadSettings( Settings, QString::number(i) + "_" + this->Settings[i]->name() );
  QList<int> MainSplitterSizes = MainSplitter->sizes();
  for ( int i = 0; i < MainSplitterSizes.size(); i++ )
    MainSplitterSizes[i] = Settings->value( "MainSplitterSizes" + QString::number(i), MainSplitter->height()/MainSplitter->count() ).toInt();
  MainSplitter->setSizes( MainSplitterSizes );
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

