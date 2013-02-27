
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

#include "scigraphics/qt4/qt4plotsettings.h"
#include "scigraphics/qt4/qt4plot.h"

#include <QtGui>

// ================================================================

qt4labeledLineEdit::qt4labeledLineEdit( const QString &LabelText, const QString &EditText, QWidget *Parent ) : QWidget(Parent)
{
  Label = new QLabel( LabelText, this );
  Edit = new QLineEdit( EditText, this );

  Layout = new QHBoxLayout();
  Layout->setContentsMargins( 0, 0, 0, 0 );
  Layout->addWidget( Label );
  Layout->addSpacing(5);
  Layout->addWidget( Edit );
  setLayout(Layout);

  setStretchFactors( 7, 2 );  
  
  connect( Edit, SIGNAL(editingFinished()), SIGNAL(editingFinished()));
  connect( Edit, SIGNAL(returnPressed()), SIGNAL(returnPressed()));
}

// ----------------------------------------------------------------
    
void qt4labeledLineEdit::setStretchFactors( int LabelStretch, int EditStretch )
{
  Layout->setStretchFactor( Label, LabelStretch );
  Layout->setStretchFactor( Edit, EditStretch );
}

// ----------------------------------------------------------------

void qt4labeledLineEdit::setValidator( QValidator *Validator )
{
  Edit->setValidator(Validator);
}

// ----------------------------------------------------------------
    
void qt4labeledLineEdit::setText( const QString &Text ) 
{ 
  Edit->setText(Text); 
}

// ----------------------------------------------------------------
    
QString qt4labeledLineEdit::text() const
{
  return Edit->text();
}

// ================================================================
    
QString qt4plotSettingsGroupBox::axisPositionString( scigraphics::axisSetCollection::axisPosition Axis )
{
  switch ( Axis )
  {
    case scigraphics::axisSetCollection::Left:      return "Y axis";
    case scigraphics::axisSetCollection::Right:     return "Y axis (right)";
    case scigraphics::axisSetCollection::Bottom:    return "X axis";
    case scigraphics::axisSetCollection::Top:       return "X axis (top)";
    default:                                     return "Unknown axis";
  }
}

// ================================================================
    
void qt4plotSettingsGroupSuperBox::init( const QList<scigraphics::axisSetCollection::axisPosition> &AxisPositions )
{
  if ( numOfRowsInLayout() <= 0 )
    return;

  QGridLayout *Layout = new QGridLayout();

  int CurrRow = 0, CurrCol = 0;
  foreach ( scigraphics::axisSetCollection::axisPosition Position, AxisPositions )
  {
    qt4plotSettingsGroupBox *Box = createBoxForAxisPosition( Position );
    if ( Box == NULL )
      continue;

    addToList( Box );
    Layout->addWidget( Box, CurrRow, CurrCol );
    CurrRow += 1;
    if ( CurrRow >= numOfRowsInLayout() )
    {
      CurrCol += 1;
      CurrRow  = 0;
    }
  }

  setLayout( Layout );
}

// ----------------------------------------------------------------
    
QList<scigraphics::axisSetCollection::axisPosition> qt4plotSettingsGroupSuperBox::defaultAxisPositions()
{
  QList< scigraphics::axisSetCollection::axisPosition > Positions;
  Positions << scigraphics::axisSetCollection::Bottom;
  Positions << scigraphics::axisSetCollection::Left;
  return Positions;
}

// ----------------------------------------------------------------

void qt4plotSettingsGroupSuperBox::addToList( qt4plotSettingsGroupBox *B ) 
{ 
  connect( B, SIGNAL(settingsUpdated()), SIGNAL(settingsUpdated()) );
  Boxes.append(B); 
}

// ----------------------------------------------------------------

void qt4plotSettingsGroupSuperBox::apply( scigraphics::settings &Settings ) 
{
  foreach ( qt4plotSettingsGroupBox *Box, Boxes )
    Box->apply( Settings );
}

// ----------------------------------------------------------------

void qt4plotSettingsGroupSuperBox::saveSettings( QSettings* Settings ) 
{
  Q_ASSERT( Settings != NULL );

  foreach ( qt4plotSettingsGroupBox *Box, Boxes )
    Box->saveSettings(Settings);
}

// ----------------------------------------------------------------

void qt4plotSettingsGroupSuperBox::loadSettings( QSettings* Settings )
{
  Q_ASSERT( Settings != NULL );

  foreach ( qt4plotSettingsGroupBox *Box, Boxes )
    Box->loadSettings(Settings);
}

// ----------------------------------------------------------------
    
void qt4plotSettingsGroupSuperBox::updateWidgets() 
{ 
  foreach ( qt4plotSettingsGroupBox *Box, Boxes )
    Box->updateWidgets();
  emit settingsUpdated(); 
}

// ================================================================

qt4plotSettingsScaleIntervals::qt4plotSettingsScaleIntervals( const scigraphics::axisSetCollection::axisPosition Axis, QWidget *Parent ) 
   : qt4plotSettingsGroupBox( axisPositionString(Axis) + " scale", Parent ), AxisType(Axis)
{
  ManualScaleBox = new QCheckBox("Manual");
  MinScaleEdit   = new qt4labeledLineEdit("Min","",this);
  MaxScaleEdit   = new qt4labeledLineEdit("Max","",this);

  MinScaleEdit->setStretchFactors( 3, 7 );
  MaxScaleEdit->setStretchFactors( 3, 7 );

  QVBoxLayout *VLayout = NULL;
  QHBoxLayout *HLayout = NULL;

  VLayout = new QVBoxLayout();
  VLayout->addWidget( ManualScaleBox );
  HLayout = new QHBoxLayout();
  HLayout->addWidget( MinScaleEdit );
  HLayout->addWidget( MaxScaleEdit );
  VLayout->addLayout( HLayout );
  VLayout->addStretch();
  
  setLayout( VLayout );

  ManualScaleBox->setChecked(false);
  MinScaleEdit->setText("-1");
  MaxScaleEdit->setText("1");

  setMinimumWidth( 210 );

  updateWidgets();

  connect( ManualScaleBox, SIGNAL(clicked()),         SLOT(updateWidgets()));
  connect( MinScaleEdit,   SIGNAL(editingFinished()), SLOT(updateWidgets()));
  connect( MaxScaleEdit,   SIGNAL(editingFinished()), SLOT(updateWidgets()));
}

// ----------------------------------------------------------------

scigraphics::interval<scigraphics::number> qt4plotSettingsScaleIntervals::getLimits() const
{
  if ( ! ManualScaleBox->isChecked() )
    return scigraphics::interval<scigraphics::number>( scigraphics::plotLimits::AutoScaleMin, scigraphics::plotLimits::AutoScaleMax );
  return scigraphics::interval<scigraphics::number>( MinScaleEdit->text().toDouble(), MaxScaleEdit->text().toDouble() );
}

// ----------------------------------------------------------------

void qt4plotSettingsScaleIntervals::updateWidgets() 
{
  MinScaleEdit->setEnabled( ManualScaleBox->isChecked() );
  MaxScaleEdit->setEnabled( ManualScaleBox->isChecked() );
  emit settingsUpdated();
}

// ----------------------------------------------------------------

void qt4plotSettingsScaleIntervals::saveSettings( QSettings* Settings )
{
  Settings->beginGroup( "scaleSettings::" + title() );
  Settings->setValue( "ManualScaleBox", ManualScaleBox->isChecked() );
  Settings->setValue( "MinScaleEdit", MinScaleEdit->text() );
  Settings->setValue( "MaxScaleEdit", MaxScaleEdit->text() );
  Settings->endGroup();
}

// ----------------------------------------------------------------

void qt4plotSettingsScaleIntervals::loadSettings( QSettings* Settings )
{
  Settings->beginGroup( "scaleSettings::" + title() );
  ManualScaleBox->setChecked( Settings->value( "ManualScaleBox", false ).toBool() );
  MinScaleEdit->setText( Settings->value( "MinScaleEdit", "-1" ).toString() );
  MaxScaleEdit->setText( Settings->value( "MaxScaleEdit", "1"  ).toString() );
  Settings->endGroup();
  updateWidgets();
}

// ================================================================

qt4plotSettingsGraphType::qt4plotSettingsGraphType( QWidget *Parent ) : qt4plotSettingsGroupBox( "Graph style", Parent )
{

  ShowPointsBtn = new QRadioButton( "Show points" );
  ShowLinesBtn  = new QRadioButton( "Show lines" );
  ShowLinesAndPointsBtn = new QRadioButton( "Show points and lines" );
  ShowLinesAndPointsBtn->setChecked(true);
  ShowLineHystogramBtn = new QRadioButton("Show hystogram");
  ErrorBarsChk = new QCheckBox("Show error bars");

  QVBoxLayout *VLayout = new QVBoxLayout();
  VLayout->addWidget( ShowPointsBtn );
  VLayout->addWidget( ShowLinesBtn );
  VLayout->addWidget( ShowLinesAndPointsBtn );
  VLayout->addWidget( ShowLineHystogramBtn );
  VLayout->addSpacing(5);
  VLayout->addWidget( ErrorBarsChk );
  VLayout->addStretch();
  
  setLayout( VLayout );
  
  QButtonGroup *Group = new QButtonGroup(this);
  Group->addButton( ShowPointsBtn );
  Group->addButton( ShowLinesBtn );
  Group->addButton( ShowLinesAndPointsBtn );
  Group->addButton( ShowLineHystogramBtn );
  
  connect( Group, SIGNAL(buttonClicked(int)), SLOT(updateWidgets()));
  connect( ErrorBarsChk, SIGNAL(clicked()), SLOT(updateWidgets()));
}

// ----------------------------------------------------------------

unsigned qt4plotSettingsGraphType::getGraphType() const
{
  unsigned Result = 0;
  if ( ShowPointsBtn->isChecked() )             Result |= scigraphics::settings::Points;
  if ( ShowLinesBtn->isChecked()  )             Result |= scigraphics::settings::Lines;
  if ( ShowLinesAndPointsBtn->isChecked() )     Result |= scigraphics::settings::LinesAndPoints;
  if ( ShowLineHystogramBtn->isChecked() )      Result |= scigraphics::settings::LinesHystogram;
  if ( ErrorBarsChk->isChecked() )              Result |= scigraphics::settings::ErrorBars;
  return Result;
}

// ----------------------------------------------------------------
    
void qt4plotSettingsGraphType::showLineHystogramControl( bool S ) 
{ 
  Q_ASSERT( ShowLineHystogramBtn != NULL );
  ShowLineHystogramBtn->setVisible(S); 
}

// ----------------------------------------------------------------

void qt4plotSettingsGraphType::showErrorBarsControl( bool S ) 
{ 
  Q_ASSERT( ErrorBarsChk != NULL );
  ErrorBarsChk->setVisible(S); 
}

// ----------------------------------------------------------------

void qt4plotSettingsGraphType::saveSettings( QSettings* Settings )
{
  Settings->beginGroup( "graphTypeSettings" );
  Settings->setValue( "ShowPointsBtn", ShowPointsBtn->isChecked() );
  Settings->setValue( "ShowLinesBtn", ShowLinesBtn->isChecked() );
  Settings->setValue( "ShowLinesAndPointsBtn", ShowLinesAndPointsBtn->isChecked() );
  Settings->setValue( "ShowLineHystogramBtn", ShowLineHystogramBtn->isChecked() );
  Settings->setValue( "ErrorBarsChk", ErrorBarsChk->isChecked() );
  Settings->endGroup();
}

// ----------------------------------------------------------------

void qt4plotSettingsGraphType::loadSettings( QSettings* Settings )
{
  Settings->beginGroup( "graphTypeSettings" );
  ShowPointsBtn->setChecked( Settings->value( "ShowPointsBtn", false ).toBool() );
  ShowLinesBtn->setChecked( Settings->value( "ShowLinesBtn", false ).toBool() );
  ShowLinesAndPointsBtn->setChecked( Settings->value( "ShowLinesAndPointsBtn", true ).toBool() );
  ShowLineHystogramBtn->setChecked( Settings->value( "ShowLineHystogramBtn", false ).toBool() );
  ErrorBarsChk->setChecked( Settings->value( "ErrorBarsChk", false ).toBool() );
  Settings->endGroup();
}

// ================================================================

qt4plotSettingsScaleType::qt4plotSettingsScaleType( const scigraphics::axisSetCollection::axisPosition Axis, QWidget *Parent ) 
   : qt4plotSettingsGroupBox( axisPositionString(Axis) + " type", Parent ), AxisType(Axis) 
{

  LinearBtn  = new QRadioButton( "Linear" );
  LinearBtn->setChecked(true);
  LogarithmPositiveBtn = new QRadioButton( "Logarithm (positive)" );
  LogarithmNegativeBtn = new QRadioButton( "Logarithm (negative)" );
  SquareBtn = new QRadioButton( "Square root" );

  QVBoxLayout *VLayout = new QVBoxLayout();
  VLayout->addWidget( LinearBtn );
  VLayout->addWidget( LogarithmPositiveBtn );
  VLayout->addWidget( LogarithmNegativeBtn );
  VLayout->addWidget( SquareBtn );
  VLayout->addStretch();
  
  setLayout( VLayout );
  
  QButtonGroup *Group = new QButtonGroup(this);
  Group->addButton( LinearBtn );
  Group->addButton( LogarithmPositiveBtn );
  Group->addButton( LogarithmNegativeBtn );
  Group->addButton( SquareBtn );
  
  connect( Group, SIGNAL(buttonClicked(int)), SLOT(updateWidgets()) );
}

// ----------------------------------------------------------------

scigraphics::settings::scaleType qt4plotSettingsScaleType::getScaleType() const
{
  if ( LinearBtn->isChecked() )                 return scigraphics::settings::Linear;
  if ( LogarithmPositiveBtn->isChecked() )      return scigraphics::settings::LogarithmPositive;
  if ( LogarithmNegativeBtn->isChecked() )      return scigraphics::settings::LogarithmNegative;
  if ( SquareBtn->isChecked() )                 return scigraphics::settings::Square;
  return scigraphics::settings::Linear;
}

// ----------------------------------------------------------------

void qt4plotSettingsScaleType::saveSettings( QSettings* Settings )
{
  Settings->beginGroup( "scaleTypeSettings::" + title() );
  Settings->setValue( "LinearBtn", LinearBtn->isChecked() );
  Settings->setValue( "LogarithmPositiveBtn", LogarithmPositiveBtn->isChecked() );
  Settings->setValue( "LogarithmNegativeBtn", LogarithmNegativeBtn->isChecked() );
  Settings->setValue( "SquareBtn", SquareBtn->isChecked() );
  Settings->endGroup();
}

// ----------------------------------------------------------------

void qt4plotSettingsScaleType::loadSettings( QSettings* Settings )
{
  Settings->beginGroup( "scaleTypeSettings::" + title() );
  LinearBtn->setChecked( Settings->value( "LinearBtn", true ).toBool() );
  LogarithmPositiveBtn->setChecked( Settings->value( "LogarithmPositiveBtn", false ).toBool() );
  LogarithmNegativeBtn->setChecked( Settings->value( "LogarithmNegativeBtn", false ).toBool() );
  SquareBtn->setChecked( Settings->value( "SquareBtn", false ).toBool() );
  Settings->endGroup();
}

// ================================================================

qt4plotSettingsSelections::qt4plotSettingsSelections( QWidget *Parent ) : qt4plotSettingsGroupBox( "Selection", Parent )
{

  setMinimumWidth(185);

  EnableSelectionBox   = new QCheckBox("Select time interval");
  MinValueEdit = new qt4labeledLineEdit( "Minimum ", "0", this );
  MaxValueEdit = new qt4labeledLineEdit( "Maximum ", "1", this );
  
  QDoubleValidator *Validator = new QDoubleValidator( this );
  MinValueEdit->setValidator(Validator);
  MaxValueEdit->setValidator(Validator);

  MinValueEdit->setStretchFactors(5,7);
  MaxValueEdit->setStretchFactors(5,7);
  
  QVBoxLayout *VLayout = new QVBoxLayout();
  VLayout->addWidget( EnableSelectionBox );
  VLayout->addWidget( MinValueEdit );
  VLayout->addWidget( MaxValueEdit );
  VLayout->addStretch();

  setLayout( VLayout );

  connect( EnableSelectionBox, SIGNAL(clicked()), SLOT(updateWidgets()) );
  connect( MinValueEdit, SIGNAL(returnPressed()), SLOT(updateWidgets()) );
  connect( MaxValueEdit, SIGNAL(returnPressed()), SLOT(updateWidgets()) );
  
  updateWidgetsEnables(); 
}

// ----------------------------------------------------------------

void qt4plotSettingsSelections::updateWidgetsEnables()
{
  MinValueEdit->setEnabled( EnableSelectionBox->isChecked() );
  MaxValueEdit->setEnabled( EnableSelectionBox->isChecked() );
}

// ----------------------------------------------
    
scigraphics::selectionStrip* qt4plotSettingsSelections::getFirstStripSelection( scigraphics::plot *Plot )
{
  if ( Plot == NULL )
    return NULL;
    
  for ( scigraphics::selectionCollection::iterator Sel = Plot->beginSelection(); Sel != Plot->endSelection(); ++Sel )
  {
    scigraphics::selectionStrip *Selection = dynamic_cast< scigraphics::selectionStrip* >( *Sel );
    if ( Selection != NULL )
      return Selection;
  }

  return NULL;
}

// ----------------------------------------------

void qt4plotSettingsSelections::getLimitsFromPlot()
{
  foreach ( qt4plot *Plot, Plots )
  {
    scigraphics::selectionStrip *Selection = getFirstStripSelection(Plot);
    if ( Selection == NULL )
    {
      EnableSelectionBox->setChecked(false);
    } else {
      EnableSelectionBox->setChecked(true);
      MinValueEdit->setText( QString::number( Selection->min() ) );
      MaxValueEdit->setText( QString::number( Selection->max() ) );
    }

  }
  updateWidgetsEnables(); 
  emit limitsUpdatedFromPlot();
}

// ----------------------------------------------

void qt4plotSettingsSelections::apply( scigraphics::settings & )
{
  foreach ( qt4plot *Plot, Plots )
  {
    if ( EnableSelectionBox->isChecked() )
    {
      scigraphics::selectionStrip *Selection = getFirstStripSelection(Plot);
      if ( Selection == NULL )
        Selection = Plot->createSelection<scigraphics::selectionVertical>();

      double Min = MinValueEdit->text().toDouble();
      double Max = MaxValueEdit->text().toDouble(); 
      Selection->setInterval( Min, Max );

      Plot->emitSelectionChanged();
    } else {
      Plot->clearSelections();
    }
    Plot->emitSelectionChangingFinished();
  }
}
    
// ----------------------------------------------

void qt4plotSettingsSelections::addPlot( qt4plot* Plot ) 
{ 
  Plots.append(Plot); 
  connect( Plot, SIGNAL(selectionChanged()), this, SLOT(getLimitsFromPlot()) );
}

// ----------------------------------------------

void qt4plotSettingsSelections::clearPlots() 
{ 
  foreach ( qt4plot *Plot, Plots )
    disconnect( Plot, SIGNAL(selectionChanged()), this, SLOT(getLimitsFromPlot()) );
  Plots.clear(); 
}

// ================================================================

void qt4plotSettingsComposer::addToCollection( qt4plotSettingsGroupBox *Widget )
{
  Q_ASSERT( Widget != NULL );
  Q_ASSERT( ! WidgetCollection.contains(Widget) );

  WidgetCollection.append( Widget );
}

// ----------------------------------------------

void qt4plotSettingsComposer::delFromCollection( qt4plotSettingsGroupBox *Widget )
{
  WidgetCollection.removeAll( Widget );
  delete Widget;
}

// ----------------------------------------------

qt4plotSettingsGroupBox* qt4plotSettingsComposer::getFromCollection( unsigned Index )
{
  Q_ASSERT( (int)Index < WidgetCollection.size() );
  qt4plotSettingsGroupBox *Widget = WidgetCollection[Index];
  Q_ASSERT( Widget != NULL );
  return Widget;
}

// ----------------------------------------------

void qt4plotSettingsComposer::addWidget( qt4plotSettingsGroupBox *Widget )
{
  addToCollection( Widget );
  addToComposer( Widget );
}

// ----------------------------------------------

void qt4plotSettingsComposer::delWidget( qt4plotSettingsGroupBox *Widget )
{
  delFromComposer( Widget );
  delFromCollection( Widget );
}

// ----------------------------------------------

void qt4plotSettingsComposer::delWidget( unsigned Index )
{
  qt4plotSettingsGroupBox *Widget = getSettingWidget(Index);
  delWidget( Widget );
}

// ----------------------------------------------
    
void qt4plotSettingsComposer::clearWidgets()
{
  for ( int i = numberOfSettingsWidget() - 1; i >= 0; i-- )
  {
    qt4plotSettingsGroupBox *Widget = getSettingWidget(i);
    Q_ASSERT( Widget != NULL );
    delWidget( Widget );
  }
}

// ----------------------------------------------
    
qt4plotSettingsComposerLayout::qt4plotSettingsComposerLayout( QWidget *Parent, QBoxLayout *L ) :
  qt4plotSettingsComposer(Parent),
  Layout(L)
{
  if ( Layout == NULL )
    Layout = new QHBoxLayout();
  setLayout( Layout );
}

// ----------------------------------------------

void qt4plotSettingsComposerLayout::addToComposer( qt4plotSettingsGroupBox *Widget )
{
  Q_ASSERT( Widget != NULL );
  Q_ASSERT( Layout != NULL );
  Q_ASSERT( layout() == Layout );
  Layout->addWidget( Widget );
}

// ----------------------------------------------

void qt4plotSettingsComposerLayout::delFromComposer( qt4plotSettingsGroupBox *Widget )
{
  Q_ASSERT( Layout != NULL );
  Q_ASSERT( layout() == Layout );

  for ( int i = 0; i < Layout->count(); i++ )
  {
    QLayoutItem *Item = Layout->itemAt(i);
    if ( Item == NULL )
      continue;
    QWidget *ItemWidget = Item->widget();
    if ( ItemWidget != Widget )
      continue;
    Layout->takeAt(i);
    break;
  }
}

// ----------------------------------------------
    
qt4plotSettingsComposer* qt4plotSettingsComposerLayout::createNewComposer( QWidget *Parent ) const
{
  QBoxLayout *NewLayout = NULL;
  if ( dynamic_cast<QVBoxLayout*>(Layout) != NULL )
    NewLayout = new QVBoxLayout();
  else if ( dynamic_cast<QHBoxLayout*>(Layout) != NULL )
    NewLayout = new QHBoxLayout();
  return new qt4plotSettingsComposerLayout( Parent, NewLayout );
}

// ----------------------------------------------

qt4plotSettingsComposerTabs::qt4plotSettingsComposerTabs( QWidget *Parent ) :
  qt4plotSettingsComposer(Parent)
{
  TabWidget = new QTabWidget(this);

  QStackedLayout *Layout = new QStackedLayout();
  Layout->addWidget( TabWidget );
  setLayout( Layout );
}
    
// ----------------------------------------------

void qt4plotSettingsComposerTabs::addToComposer( qt4plotSettingsGroupBox *Widget )
{
  Q_ASSERT( TabWidget != NULL );
  Q_ASSERT( Widget != NULL );

  TabWidget->addTab( Widget, Widget->title() );
}

// ----------------------------------------------

void qt4plotSettingsComposerTabs::delFromComposer( qt4plotSettingsGroupBox *Widget )
{
  int Index = TabWidget->indexOf( Widget );
  if ( Index < 0 )
    return;
  TabWidget->removeTab(Index);
}

// ----------------------------------------------
    
qt4plotSettingsComposer* qt4plotSettingsComposerTabs::createNewComposer( QWidget *Parent ) const
{
  return new qt4plotSettingsComposerTabs(Parent);
}

// ================================================================

#if 0
qt4plotSettings::qt4plotSettings( QWidget *Parent, const QString &Name, const qt4plotSettingsGroupSuperBox::axisPositionsList &Positions ) : 
  QWidget(Parent), 
  SettingsName(Name),
  SettingsWidgetsContainer(NULL)
{
  setLayout( new QHBoxLayout() );

  addSettingWidget( new qt4plotSettingsScaleTypeAllAxis(this,Positions) );
  addSettingWidget( new qt4plotSettingsScaleIntervalsAllAxis(this,Positions) );
  addSettingWidget( new qt4plotSettingsGraphType(this) );
}
#endif

// ----------------------------------------------------------------

void qt4plotSettings::initSettings( const QString &Name, const qt4plotSettingsGroupSuperBox::axisPositionsList &Positions,
    qt4plotSettingsComposer *Composer )
{
  setName(Name);
  setComposer( Composer );

  addSettingWidget( new qt4plotSettingsScaleTypeAllAxis(this,Positions) );
  addSettingWidget( new qt4plotSettingsScaleIntervalsAllAxis(this,Positions) );
  addSettingWidget( new qt4plotSettingsGraphType(this) );
}

// ----------------------------------------------------------------

void qt4plotSettings::setComposer( qt4plotSettingsComposer *Composer )
{
  Q_ASSERT( SettingsComposer == NULL );
  if ( Composer == NULL )
    Composer = new qt4plotSettingsComposerLayout(this);
  SettingsComposer = Composer;

  QStackedLayout *Layout = new QStackedLayout();
  Layout->addWidget( SettingsComposer );
  setLayout( Layout );
}

// ----------------------------------------------------------------

void qt4plotSettings::addSettingWidget( qt4plotSettingsGroupBox *Widget )
{
  Q_ASSERT( Widget != NULL );

  SettingsComposer->addWidget( Widget );
  connect( Widget, SIGNAL(settingsUpdated()), SLOT(apply()) );

  Widget->clearPlots();
  foreach( qt4plot *Plot, Plots )
    Widget->addPlot( Plot );
}

// ----------------------------------------------------------------

void qt4plotSettings::delSettingWidget( unsigned Index )
{
  Q_ASSERT( SettingsComposer != NULL );
  SettingsComposer->delWidget( Index );
}

// ----------------------------------------------------------------
    
qt4plotSettingsGroupBox* qt4plotSettings::getSettingWidget( unsigned Index )
{
  Q_ASSERT( SettingsComposer != NULL );
  return SettingsComposer->getSettingWidget( Index );
}

// ----------------------------------------------------------------

void qt4plotSettings::addPlot( qt4plot *Plot ) 
{ 
  Plots.append(Plot); 
  for ( unsigned i = 0; i < SettingsComposer->numberOfSettingsWidget(); i++ )
    SettingsComposer->getSettingWidget(i)->addPlot(Plot);
  apply(); 
}

// ----------------------------------------------------------------

void qt4plotSettings::clearPlots() 
{ 
  Plots.clear(); 
  for ( unsigned i = 0; i < SettingsComposer->numberOfSettingsWidget(); i++ )
    SettingsComposer->getSettingWidget(i)->clearPlots();
  apply();
}

// ----------------------------------------------------------------

void qt4plotSettings::saveSettings( QSettings *Settings, const QString &Prefix )
{
  Settings->beginGroup( "qt4plotSettings::" + Prefix );
  for ( unsigned i = 0; i < SettingsComposer->numberOfSettingsWidget(); i++ )
    SettingsComposer->getSettingWidget(i)->saveSettings(Settings);
  Settings->endGroup();
}

// ----------------------------------------------------------------

void qt4plotSettings::loadSettings( QSettings *Settings, const QString &Prefix )
{
  Settings->beginGroup( "qt4plotSettings::" + Prefix );
  for ( unsigned i = 0; i < SettingsComposer->numberOfSettingsWidget(); i++ )
    SettingsComposer->getSettingWidget(i)->loadSettings(Settings);
  Settings->endGroup();
  apply();
}

// ----------------------------------------------------------------

void qt4plotSettings::saveSettings( const QString &FileName )
{
  QSettings Settings( FileName, QSettings::IniFormat );
  saveSettings( &Settings );
}

// ----------------------------------------------------------------

void qt4plotSettings::loadSettings( const QString &FileName )
{
  QSettings Settings( FileName, QSettings::IniFormat );
  loadSettings( &Settings );
}

// ----------------------------------------------------------------

void qt4plotSettings::apply()
{
  collectSettings();
  applySettings();
  emitSettingsChanged();
}

// ----------------------------------------------------------------
    
void qt4plotSettings::collectSettings()
{
  for ( unsigned i = 0; i < SettingsComposer->numberOfSettingsWidget(); i++ )
    SettingsComposer->getSettingWidget(i)->apply( *this );
}

// ----------------------------------------------------------------

void qt4plotSettings::applySettings()
{
  foreach ( scigraphics::plot *Plot, Plots )
    settings::apply(*Plot);
}

// ----------------------------------------------------------------

void qt4plotSettings::emitSettingsChanged()
{
  emit settingsChanged();
}

// ================================================================

