
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
#include "scigraphics/plot.h"
#include "scigraphics/selection.h"
#include "scigraphics/plotlimits.h"

#include <QGridLayout>
#include <QCheckBox>
#include <QRadioButton>
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
      
void scigraphics::qt4settingsGroupBox::applySettings( settings* ) const
{
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsGroupBox::showSettings( const settings& )
{
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsGroupBox::collectSettings( const plot& ) 
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

scigraphics::qt4settingsGroupBox* scigraphics::qt4settingsGroupSuperBox::createBoxForAxisPosition( axisPosition ) 
{ 
  return NULL; 
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsGroupSuperBox::applySettings( settings *Settings ) const
{
  foreach ( qt4settingsGroupBox *Box, Boxes )
    Box->applySettings( Settings );
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsGroupSuperBox::showSettings( const settings &Settings )
{
  foreach ( qt4settingsGroupBox *Box, Boxes )
    Box->showSettings( Settings );
}

// ----------------------------------------------------------------
    
void scigraphics::qt4settingsGroupSuperBox::collectSettings( const plot &Plot )
{
  foreach ( qt4settingsGroupBox *Box, Boxes )
    Box->collectSettings( Plot );
}

// ----------------------------------------------------------------

scigraphics::qt4settingsGroupBox* scigraphics::qt4settingsGroupSuperBox::boxForAxisPosition( axisPosition Position )
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
    
void scigraphics::qt4settingsGroupSuperBox::updateWidgets() 
{ 
  foreach ( qt4settingsGroupBox *Box, Boxes )
    Box->updateWidgets();
  emit settingsUpdated(); 
}

// ================================================================

scigraphics::qt4settingsScaleIntervals::qt4settingsScaleIntervals( const axisPosition Axis, QWidget *Parent ) : 
  qt4settingsGroupBox( axisPositionString(Axis) + tr(" - scale"), Parent ), 
  AxisType(Axis)
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

  updateWidgetsEnables();
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsScaleIntervals::updateWidgetsEnables()
{
  MinScaleEdit->setEnabled( ManualScaleBox->isChecked() );
  MaxScaleEdit->setEnabled( ManualScaleBox->isChecked() );
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsScaleIntervals::applySettings( settings* Settings ) const
{
  Settings->setEnabledAxisScaleLimits( ManualScaleBox->isChecked(), AxisType );
  Settings->setAxisScaleLimits( MinScaleEdit->text().toDouble(), MaxScaleEdit->text().toDouble(), AxisType ); 
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsScaleIntervals::showSettings( const settings &Settings )
{
  bool Enabled = Settings.enabledAxisScaleLimits( AxisType );
  interval<number> Interval = Settings.axisScaleLimits( AxisType );

  ManualScaleBox->setChecked( Enabled );
  MinScaleEdit->setText( QString::number( Interval.min() ) );
  MaxScaleEdit->setText( QString::number( Interval.max() ) );
  updateWidgetsEnables();
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsScaleIntervals::updateWidgets() 
{
  updateWidgetsEnables();
  emit settingsUpdated();
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
      
scigraphics::qt4settingsGroupBox* scigraphics::qt4settingsScaleIntervalsAllAxis::createBoxForAxisPosition( axisPosition Pos ) 
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
  if ( ShowPointsBtn->isChecked() )             Result |= settings::Points;
  if ( ShowLinesBtn->isChecked()  )             Result |= settings::Lines;
  if ( ShowLinesAndPointsBtn->isChecked() )     Result |= settings::LinesAndPoints;
  if ( ShowLineHystogramBtn->isChecked() )      Result |= settings::LinesHystogram;
  if ( ErrorBarsChk->isChecked() )              Result |= settings::ErrorBars;
  return Result;
}

// ----------------------------------------------------------------
      
void scigraphics::qt4settingsGraphType::setGraphType( unsigned GraphType )
{
  ErrorBarsChk->setChecked( GraphType & settings::ErrorBars );

  if ( GraphType == settings::LinesHystogram )
    ShowLineHystogramBtn->setChecked( true );
  else if ( GraphType == settings::LinesAndPoints )
    ShowLinesAndPointsBtn->setChecked( true );
  else if ( GraphType == settings::Points )
    ShowPointsBtn->setChecked( true );
  else 
    ShowLinesBtn->setChecked( true );
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
    
void scigraphics::qt4settingsGraphType::applySettings( settings* Settings ) const
{ 
  Settings->setGraphType( getGraphType() ); 
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsGraphType::showSettings( const settings& Settings )
{
  setGraphType( Settings.graphType() );
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
  if ( ShowLegendChk->isChecked() )             Result |= settings::Legend;
  if ( ShowCursorPositionChk->isChecked() )     Result |= settings::CursorPosition;
  return Result;
}

// ----------------------------------------------------------------
      
void scigraphics::qt4settingsDecoration::setVisibleFloatingRectangles( unsigned VisibleFloatingRectangles )
{
  ShowLegendChk->setChecked( VisibleFloatingRectangles & settings::Legend );
  ShowCursorPositionChk->setChecked( VisibleFloatingRectangles & settings::CursorPosition );
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsDecoration::applySettings( settings* Settings ) const 
{ 
  Settings->setVisibleFloatingRectangles( getVisibleFloatingRectangles() ); 
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsDecoration::showSettings( const settings &Settings ) 
{ 
  setVisibleFloatingRectangles( Settings.visibleFloatingRectangles() );
}

// ================================================================

scigraphics::qt4settingsScaleType::qt4settingsScaleType( const axisPosition Axis, QWidget *Parent ) :
  qt4settingsGroupBox( axisPositionString(Axis) + tr(" - type"), Parent ), 
  AxisType(Axis) 
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
  if ( LinearBtn->isChecked() )                 return settings::Linear;
  if ( LogarithmPositiveBtn->isChecked() )      return settings::LogarithmPositive;
  if ( LogarithmNegativeBtn->isChecked() )      return settings::LogarithmNegative;
  if ( SquareBtn->isChecked() )                 return settings::Square;
  return scigraphics::settings::Linear;
}

// ----------------------------------------------------------------
      
void scigraphics::qt4settingsScaleType::setScaleType( settings::scaleType ScaleType )
{
  switch ( ScaleType )
  {
    case settings::Square:
      SquareBtn->setChecked( true );
      break;

    case settings::LogarithmNegative:
      LogarithmNegativeBtn->setChecked( true );
      break;

    case settings::LogarithmPositive:
      LogarithmPositiveBtn->setChecked( true );
      break;

    default:
      LinearBtn->setChecked( true );
      break;
  }
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
    
void scigraphics::qt4settingsScaleType::applySettings( settings *Settings ) const
{ 
  Settings->setAxisScaleType( getScaleType(), AxisType ); 
}

// ----------------------------------------------------------------

void scigraphics::qt4settingsScaleType::showSettings( const settings &Settings ) 
{ 
  setScaleType( Settings.axisScaleType(AxisType) );
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

void scigraphics::qt4settingsSelections::collectSettings( const plot &Plot )
{
  const selectionStrip *Selection = settings::firstSelectionStrip(Plot);
  
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

void scigraphics::qt4settingsSelections::applySettings( settings *Settings ) const
{
  double Min = MinValueEdit->text().toDouble();
  double Max = MaxValueEdit->text().toDouble(); 
  settings::selectionStripType StripType = EnableSelectionBox->isChecked() ? settings::VerticalStrip : settings::NoneStrip;
 
  bool NeedToEmitSelectionChanged = false;
  NeedToEmitSelectionChanged = NeedToEmitSelectionChanged || Settings->selectionType() != StripType;
  NeedToEmitSelectionChanged = NeedToEmitSelectionChanged || Settings->selectionInterval().min() != Min;
  NeedToEmitSelectionChanged = NeedToEmitSelectionChanged || Settings->selectionInterval().max() != Max;
  
  Settings->setSelectionInterval( StripType, Min, Max );

  // emit for non-const object
  const_cast<qt4settingsSelections*>(this)->needToSetEmitSelectionChangedAfterApplyingFlag( NeedToEmitSelectionChanged );
}

// ----------------------------------------------

void scigraphics::qt4settingsSelections::showSettings( const settings &Settings )
{
  settings::selectionStripType Type = Settings.selectionType();
  interval<number> Interval = Settings.selectionInterval();

  EnableSelectionBox->setChecked( Type != settings::NoneStrip );
  MinValueEdit->setText( QString::number( Interval.min() ) );
  MaxValueEdit->setText( QString::number( Interval.max() ) );
}

// ================================================================


