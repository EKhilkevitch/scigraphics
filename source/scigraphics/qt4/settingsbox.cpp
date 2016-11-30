
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


// ================================================================

#include "scigraphics/qt4/settingsbox.h"
#include "scigraphics/qt4/labeledline.h"
#include "scigraphics/qt4/plot.h"
#include "scigraphics/qt4/settings.h"
#include "scigraphics/selection.h"
#include "scigraphics/plotlimits.h"

#include <QGridLayout>
#include <QCheckBox>
#include <QRadioButton>
#include <QSettings>
#include <QButtonGroup>
#include <QDoubleValidator>

// ================================================================

scigraphics::qt4settingsGroupBox::qt4settingsGroupBox( const QString &Name, QWidget *Parent ) : 
  QGroupBox(Name,Parent) 
{
}

// ----------------------------------------------------------------
    
void scigraphics::qt4settingsGroupBox::updateWidgets() 
{ 
  emit settingsUpdated(); 
}

// ----------------------------------------------------------------
    
QString scigraphics::qt4settingsGroupBox::axisPositionString( axisPosition Axis )
{
  switch ( Axis )
  {
    case AxisLeft:      return tr("Y axis");
    case AxisRight:     return tr("Y axis (right)");
    case AxisBottom:    return tr("X axis");
    case AxisTop:       return tr("X axis (top)");
    default:            return tr("Unknown axis");
  }
}

// ----------------------------------------------------------------
      
void scigraphics::qt4settingsGroupBox::applySettings( qt4settings* ) 
{
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsGroupBox::collectSettings( qt4plot* ) 
{
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsGroupBox::saveSettings( QSettings* ) const 
{
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsGroupBox::loadSettings( QSettings* ) 
{
}

// ================================================================
      
scigraphics::qt4settingsGroupSuperBox::qt4settingsGroupSuperBox( unsigned Number, const QString &Name, QWidget *Parent ) : 
  qt4settingsGroupBox(Name,Parent),
  NumberOfRowsInLayout( Number )
{
  NumberOfRowsInLayout = qMax<unsigned>( NumberOfRowsInLayout, 1 );
}

// ----------------------------------------------------------------
    
void scigraphics::qt4settingsGroupSuperBox::init( const QList<axisPosition> &AxisPositions )
{
  if ( NumberOfRowsInLayout <= 0 )
    return;

  setLayout( new QGridLayout() );

  QGridLayout *Layout = new QGridLayout();

  foreach ( axisPosition Position, AxisPositions )
  {
    qt4settingsGroupBox *Box = createBoxForAxisPosition( Position );
    if ( Box != NULL )
      addToList( Position, Box );
  }

  updateLayout();
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsGroupSuperBox::updateLayout()
{
  QGridLayout *Layout = dynamic_cast< QGridLayout* >( layout() );
  Q_ASSERT( Layout != NULL );

  foreach ( qt4settingsGroupBox *Box, Boxes )
    Layout->removeWidget( Box );
  
  unsigned CurrRow = 0;
  unsigned CurrCol = 0;
  foreach ( qt4settingsGroupBox *Box, Boxes )
  {
    Layout->addWidget( Box, CurrRow, CurrCol );
    CurrRow += 1;
    if ( CurrRow >= numberOfRowsInLayout() )
    {
      CurrCol += 1;
      CurrRow  = 0;
    }
  }
}

// ----------------------------------------------------------------
    
QList<scigraphics::axisPosition> scigraphics::qt4settingsGroupSuperBox::defaultAxisPositions()
{
  QList< axisPosition > Positions;
  Positions << AxisBottom;
  Positions << AxisLeft;
  return Positions;
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsGroupSuperBox::addToList( axisPosition Position, qt4settingsGroupBox *Box ) 
{ 
  connect( Box, SIGNAL(settingsUpdated()), SIGNAL(settingsUpdated()) );

  delete Boxes.take( Position );
  Boxes.insert( Position, Box ); 
}

// ----------------------------------------------------------------

scigraphics::qt4settingsGroupBox* scigraphics::qt4settingsGroupSuperBox::createBoxForAxisPosition( scigraphics::axisPosition ) 
{ 
  return NULL; 
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsGroupSuperBox::applySettings( qt4settings* Settings ) 
{
  foreach ( qt4settingsGroupBox *Box, Boxes )
    Box->applySettings( Settings );
}

// ----------------------------------------------------------------
    
void scigraphics::qt4settingsGroupSuperBox::collectSettings( qt4plot* Plot )
{
  foreach ( qt4settingsGroupBox *Box, Boxes )
    Box->collectSettings( Plot );
}

// ----------------------------------------------------------------

scigraphics::qt4settingsGroupBox* scigraphics::qt4settingsGroupSuperBox::boxForAxisPosition( scigraphics::axisPosition Position )
{
  return Boxes.value( Position, NULL );
}

// ----------------------------------------------------------------
      
QList<scigraphics::axisPosition> scigraphics::qt4settingsGroupSuperBox::axisPositionsWithBoxes() const
{
  return Boxes.keys();
}

// ----------------------------------------------------------------
      
unsigned scigraphics::qt4settingsGroupSuperBox::numberOfRowsInLayout() const
{
  return NumberOfRowsInLayout;
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsGroupSuperBox::setNumberOfRowsInLayout( unsigned Number )
{
  NumberOfRowsInLayout = qMax<unsigned>( 1, Number );
  updateLayout();
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsGroupSuperBox::saveSettings( QSettings* Settings ) const
{
  Q_ASSERT( Settings != NULL );

  foreach ( qt4settingsGroupBox *Box, Boxes )
    Box->saveSettings(Settings);
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsGroupSuperBox::loadSettings( QSettings* Settings )
{
  Q_ASSERT( Settings != NULL );

  foreach ( qt4settingsGroupBox *Box, Boxes )
    Box->loadSettings(Settings);
}

// ----------------------------------------------------------------
    
void scigraphics::qt4settingsGroupSuperBox::updateWidgets() 
{ 
  foreach ( qt4settingsGroupBox *Box, Boxes )
    Box->updateWidgets();
  emit settingsUpdated(); 
}

// ================================================================

scigraphics::qt4settingsScaleIntervals::qt4settingsScaleIntervals( const axisPosition Axis, QWidget *Parent ) 
   : qt4settingsGroupBox( axisPositionString(Axis) + tr(" - scale"), Parent ), AxisType(Axis)
{
  ManualScaleBox = new QCheckBox(tr("Manual"),this);
  MinScaleEdit   = new qt4labeledLineEdit(tr("Min"),"",this);
  MaxScaleEdit   = new qt4labeledLineEdit(tr("Max"),"",this);

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

scigraphics::interval<scigraphics::number> scigraphics::qt4settingsScaleIntervals::getLimits() const
{
  if ( ! ManualScaleBox->isChecked() )
    return scigraphics::interval<scigraphics::number>( scigraphics::plotLimits::AutoScaleMin, scigraphics::plotLimits::AutoScaleMax );
  return scigraphics::interval<scigraphics::number>( MinScaleEdit->text().toDouble(), MaxScaleEdit->text().toDouble() );
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsScaleIntervals::applySettings( qt4settings* Settings ) 
{ 
  Q_ASSERT( Settings != NULL );
  Settings->plotSettings()->setLimits( getLimits(), AxisType ); 
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsScaleIntervals::updateWidgets() 
{
  MinScaleEdit->setEnabled( ManualScaleBox->isChecked() );
  MaxScaleEdit->setEnabled( ManualScaleBox->isChecked() );
  emit settingsUpdated();
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsScaleIntervals::saveSettings( QSettings* Settings ) const
{
  Settings->beginGroup( "scaleSettings::" + title() );
  Settings->setValue( "ManualScaleBox", ManualScaleBox->isChecked() );
  Settings->setValue( "MinScaleEdit", MinScaleEdit->text() );
  Settings->setValue( "MaxScaleEdit", MaxScaleEdit->text() );
  Settings->endGroup();
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsScaleIntervals::loadSettings( QSettings* Settings )
{
  Settings->beginGroup( "scaleSettings::" + title() );
  ManualScaleBox->setChecked( Settings->value( "ManualScaleBox", false ).toBool() );
  MinScaleEdit->setText( Settings->value( "MinScaleEdit", "-1" ).toString() );
  MaxScaleEdit->setText( Settings->value( "MaxScaleEdit", "1"  ).toString() );
  Settings->endGroup();
  updateWidgets();
}

// ================================================================

scigraphics::qt4settingsScaleIntervalsAllAxis::qt4settingsScaleIntervalsAllAxis( QWidget *Parent, const axisPositionsList &Positions ) : 
  qt4settingsGroupSuperBox( 2, name(), Parent ) 
{ 
  init(Positions); 
}

// ----------------------------------------------------------------

QString scigraphics::qt4settingsScaleIntervalsAllAxis::name() 
{ 
  return tr("Scale intervals");
}

// ----------------------------------------------------------------
      
scigraphics::qt4settingsGroupBox* scigraphics::qt4settingsScaleIntervalsAllAxis::createBoxForAxisPosition( scigraphics::axisPosition Pos ) 
{ 
  return new qt4settingsScaleIntervals( Pos, this ); 
}

// ================================================================

scigraphics::qt4settingsGraphType::qt4settingsGraphType( QWidget *Parent ) : 
  qt4settingsGroupBox( tr("Graph style"), Parent )
{
  ShowPointsBtn = new QRadioButton( tr("Show points"), this );
  ShowLinesBtn  = new QRadioButton( tr("Show lines"), this );
  ShowLinesAndPointsBtn = new QRadioButton( tr("Show points and lines"), this );
  ShowLinesAndPointsBtn->setChecked(true);
  ShowLineHystogramBtn = new QRadioButton( tr("Show hystogram"), this );
  ErrorBarsChk = new QCheckBox( tr("Show error bars"), this );

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

unsigned scigraphics::qt4settingsGraphType::getGraphType() const
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
    
void scigraphics::qt4settingsGraphType::showLineHystogramControl( bool Show ) 
{ 
  Q_ASSERT( ShowLineHystogramBtn != NULL );
  ShowLineHystogramBtn->setVisible( Show ); 
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsGraphType::showErrorBarsControl( bool Show ) 
{ 
  Q_ASSERT( ErrorBarsChk != NULL );
  ErrorBarsChk->setVisible( Show ); 
}

// ----------------------------------------------------------------
    
void scigraphics::qt4settingsGraphType::applySettings( qt4settings* Settings ) 
{ 
  Settings->plotSettings()->setGraphType( getGraphType() ); 
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsGraphType::saveSettings( QSettings* Settings ) const
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

void scigraphics::qt4settingsGraphType::loadSettings( QSettings* Settings )
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

scigraphics::qt4settingsDecoration::qt4settingsDecoration( QWidget *Parent ) :
  qt4settingsGroupBox( tr("Decorations"), Parent )
{
  ShowLegendChk = new QCheckBox(tr("Show legend"),this);
  ShowCursorPositionChk = new QCheckBox(tr("Show cursor position"),this);

  ShowLegendChk->setChecked(true);
  ShowCursorPositionChk->setChecked(true);

  QVBoxLayout *VLayout = new QVBoxLayout();
  VLayout->addWidget( ShowLegendChk );
  VLayout->addWidget( ShowCursorPositionChk );
  VLayout->addStretch();
  
  setLayout( VLayout );
  
  connect( ShowLegendChk, SIGNAL(clicked()), SLOT(updateWidgets()));
  connect( ShowCursorPositionChk, SIGNAL(clicked()), SLOT(updateWidgets()));
}

// ----------------------------------------------------------------

unsigned scigraphics::qt4settingsDecoration::getVisibleFloatingRectangles() const
{
  unsigned Result = 0;
  if ( ShowLegendChk->isChecked() )             Result |= scigraphics::settings::Legend;
  if ( ShowCursorPositionChk->isChecked() )     Result |= scigraphics::settings::CursorPosition;
  return Result;
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsDecoration::applySettings( qt4settings* Settings ) 
{ 
  Settings->plotSettings()->setVisibleFloatingRectangles( getVisibleFloatingRectangles() ); 
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsDecoration::saveSettings( QSettings* Settings ) const
{
  Q_ASSERT( Settings != NULL );
  Settings->beginGroup( "decorationSettings" );
  Settings->setValue( "ShowLegendChk", ShowLegendChk->isChecked() );
  Settings->setValue( "ShowCursorPositionChk", ShowCursorPositionChk->isChecked() );
  Settings->endGroup();
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsDecoration::loadSettings( QSettings* Settings )
{
  Q_ASSERT( Settings != NULL );
  Settings->beginGroup( "decorationSettings" );
  ShowLegendChk->setChecked( Settings->value( "ShowLegendChk", true ).toBool() );
  ShowCursorPositionChk->setChecked( Settings->value( "ShowCursorPositionChk", true ).toBool() );
  Settings->endGroup();
}

// ================================================================

scigraphics::qt4settingsScaleType::qt4settingsScaleType( const scigraphics::axisPosition Axis, QWidget *Parent ) 
   : qt4settingsGroupBox( axisPositionString(Axis) + tr(" - type"), Parent ), AxisType(Axis) 
{

  LinearBtn  = new QRadioButton( tr("Linear"), this );
  LinearBtn->setChecked(true);
  LogarithmPositiveBtn = new QRadioButton( tr("Logarithm (positive)"), this );
  LogarithmNegativeBtn = new QRadioButton( tr("Logarithm (negative)"), this );
  SquareBtn = new QRadioButton( tr("Square root"), this );

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

scigraphics::settings::scaleType scigraphics::qt4settingsScaleType::getScaleType() const
{
  if ( LinearBtn->isChecked() )                 return scigraphics::settings::Linear;
  if ( LogarithmPositiveBtn->isChecked() )      return scigraphics::settings::LogarithmPositive;
  if ( LogarithmNegativeBtn->isChecked() )      return scigraphics::settings::LogarithmNegative;
  if ( SquareBtn->isChecked() )                 return scigraphics::settings::Square;
  return scigraphics::settings::Linear;
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsScaleType::showLogarithmNegativeControl( bool Show )
{
  Q_ASSERT( LogarithmNegativeBtn != NULL );
  LogarithmNegativeBtn->setVisible( Show );
  LogarithmPositiveBtn->setText( Show ? tr("Logarithm (positive)") : tr("Logarithm") );
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsScaleType::showSquareControl( bool Show )
{
  Q_ASSERT( SquareBtn != NULL );
  SquareBtn->setVisible( Show );
}

// ----------------------------------------------------------------
    
void scigraphics::qt4settingsScaleType::applySettings( qt4settings* Settings ) 
{ 
  Settings->plotSettings()->setScaleType( getScaleType(), AxisType ); 
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsScaleType::saveSettings( QSettings* Settings ) const
{
  Settings->beginGroup( "scaleTypeSettings::" + title() );
  Settings->setValue( "LinearBtn", LinearBtn->isChecked() );
  Settings->setValue( "LogarithmPositiveBtn", LogarithmPositiveBtn->isChecked() );
  Settings->setValue( "LogarithmNegativeBtn", LogarithmNegativeBtn->isChecked() );
  Settings->setValue( "SquareBtn", SquareBtn->isChecked() );
  Settings->endGroup();
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsScaleType::loadSettings( QSettings* Settings )
{
  Settings->beginGroup( "scaleTypeSettings::" + title() );
  LinearBtn->setChecked( Settings->value( "LinearBtn", true ).toBool() );
  LogarithmPositiveBtn->setChecked( Settings->value( "LogarithmPositiveBtn", false ).toBool() );
  LogarithmNegativeBtn->setChecked( Settings->value( "LogarithmNegativeBtn", false ).toBool() );
  SquareBtn->setChecked( Settings->value( "SquareBtn", false ).toBool() );
  Settings->endGroup();
}

// ================================================================

scigraphics::qt4settingsGroupBox* scigraphics::qt4settingsScaleTypeAllAxis::createBoxForAxisPosition( scigraphics::axisPosition Pos ) 
{ 
  qt4settingsScaleType *ScaleBox = new qt4settingsScaleType( Pos, this ); 

  ScaleBox->showLogarithmNegativeControl(false);
  ScaleBox->showSquareControl(false);

  return ScaleBox;
}

// ----------------------------------------------------------------

QString scigraphics::qt4settingsScaleTypeAllAxis::name() 
{ 
  return tr("Scale types");
}

// ----------------------------------------------------------------
      
scigraphics::qt4settingsScaleTypeAllAxis::qt4settingsScaleTypeAllAxis( QWidget *Parent, const axisPositionsList &Positions ) : 
  qt4settingsGroupSuperBox( 2, name(), Parent ) 
{ 
  init(Positions); 
}

// ================================================================

scigraphics::qt4settingsSelections::qt4settingsSelections( QWidget *Parent ) : 
  qt4settingsGroupBox( "Selection", Parent )
{
  setMinimumWidth(185);

  EnableSelectionBox   = new QCheckBox( tr("Select interval"),this);
  MinValueEdit = new qt4labeledLineEdit( tr("Minimum"), "0", this );
  MaxValueEdit = new qt4labeledLineEdit( tr("Maximum"), "1", this );
  
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

void scigraphics::qt4settingsSelections::updateWidgetsEnables()
{
  MinValueEdit->setEnabled( EnableSelectionBox->isChecked() );
  MaxValueEdit->setEnabled( EnableSelectionBox->isChecked() );
}

// ----------------------------------------------

void scigraphics::qt4settingsSelections::updateWidgets() 
{ 
  updateWidgetsEnables();
  emit settingsUpdated(); 
}

// ----------------------------------------------

void scigraphics::qt4settingsSelections::collectSettings( qt4plot *Plot )
{
  selectionStrip *Selection = settings::firstSelectionStrip(Plot);
  
  if ( Selection == NULL )
  {
    EnableSelectionBox->setChecked(false);
  } else {
    EnableSelectionBox->setChecked(true);
    MinValueEdit->setText( QString::number( Selection->min() ) );
    MaxValueEdit->setText( QString::number( Selection->max() ) );
  }

  updateWidgetsEnables(); 
  emit limitsUpdatedFromPlot();
}

// ----------------------------------------------

void scigraphics::qt4settingsSelections::applySettings( qt4settings* Settings )
{
  double Min = MinValueEdit->text().toDouble();
  double Max = MaxValueEdit->text().toDouble(); 
  settings::selectionStripType StripType = EnableSelectionBox->isChecked() ? settings::VerticalStrip : settings::NoneStrip;
 
  bool NeedToEmitSelectionChanged = false;
  NeedToEmitSelectionChanged = NeedToEmitSelectionChanged || Settings->plotSettings()->getSelectionStripType() != StripType;
  NeedToEmitSelectionChanged = NeedToEmitSelectionChanged || Settings->plotSettings()->getSelectionStripInterval().min() != Min;
  NeedToEmitSelectionChanged = NeedToEmitSelectionChanged || Settings->plotSettings()->getSelectionStripInterval().max() != Max;
  Settings->needToEmitSelectionChangedAfterApplying( NeedToEmitSelectionChanged );
  
  Settings->plotSettings()->setSelectionInterval( StripType, Min, Max );
}

// ----------------------------------------------
      
void scigraphics::qt4settingsSelections::saveSettings( QSettings* ) const 
{
}

// ----------------------------------------------

void scigraphics::qt4settingsSelections::loadSettings( QSettings* ) 
{
}

// ================================================================


