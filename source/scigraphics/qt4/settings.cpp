
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

#include "scigraphics/settings.h"
#include "scigraphics/qt4/settings.h"
#include "scigraphics/qt4/settingsbox.h"
#include "scigraphics/qt4/settingscomposer.h"
#include "scigraphics/qt4/plot.h"

#include <QStackedLayout>
#include <QSettings>
#include <QDebug>

// ================================================================

scigraphics::qt4settings::qt4settings( QWidget *Parent, const QString &Name ) : 
  QWidget(Parent),
  Settings( new settings() ),
  SettingsComposer(NULL) 
{ 
  initSettings( Name, qt4settingsGroupSuperBox::defaultAxisPositions(), NULL ); 
}

// ----------------------------------------------------------------

scigraphics::qt4settings::qt4settings( QWidget *Parent, const QList<axisPosition> &Positions ) : 
  QWidget(Parent), 
  Settings( new settings() ),
  SettingsComposer(NULL)
{ 
  initSettings( QString(), Positions, NULL ); 
}
      
// ----------------------------------------------------------------

scigraphics::qt4settings::qt4settings( QWidget *Parent, const QString &Name, const QList<axisPosition> &Positions, qt4settingsComposer *Composer ) : 
  QWidget(Parent), 
  Settings( new settings() ),
  SettingsComposer(NULL)
{ 
  initSettings( Name, Positions, Composer ); 
}
      
// ----------------------------------------------------------------

scigraphics::qt4settings::qt4settings( QWidget *Parent, qt4settingsComposer *Composer ) : 
  QWidget(Parent), 
  Settings( new settings() ),
  SettingsComposer(NULL)
{ 
  initSettings( QString(), qt4settingsGroupSuperBox::defaultAxisPositions(), Composer ); 
}

// ----------------------------------------------------------------

scigraphics::qt4settings::~qt4settings()
{
  delete Settings;
  Settings = NULL;
}

// ----------------------------------------------------------------
      
const QString& scigraphics::qt4settings::name() const 
{ 
  return SettingsName; 
}

// ----------------------------------------------------------------

void scigraphics::qt4settings::setName( const QString &Name ) 
{ 
  SettingsName = Name; 
}

// ----------------------------------------------------------------

const scigraphics::settings& scigraphics::qt4settings::plotSettings() const
{ 
  return *Settings; 
}

// ----------------------------------------------------------------
      
void scigraphics::qt4settings::setPlotSettings( const settings &Settings )
{
  *(this->Settings) = Settings;
  updateSubWidgetsFromSettings();
  emit settingsChanged( this );
}

// ----------------------------------------------------------------

void scigraphics::qt4settings::initSettings( const QString &Name, const qt4settingsGroupSuperBox::axisPositionsList &Positions,
    qt4settingsComposer *Composer )
{
  NeedToEmitSelectionChangedAfterApplying = false;

  setName( Name );
  setComposer( Composer );

  addSettingWidget( new qt4settingsScaleTypeAllAxis(this,Positions) );
  addSettingWidget( new qt4settingsScaleIntervalsAllAxis(this,Positions) );
  addSettingWidget( new qt4settingsGraphType(this) );
}

// ----------------------------------------------------------------

void scigraphics::qt4settings::setComposer( qt4settingsComposer *Composer )
{
  Q_ASSERT( SettingsComposer == NULL );
  if ( Composer == NULL )
    Composer = new qt4settingsComposerLayout(this);
  SettingsComposer = Composer;

  QStackedLayout *Layout = new QStackedLayout();
  Layout->addWidget( SettingsComposer );
  setLayout( Layout );
}

// ----------------------------------------------------------------

void scigraphics::qt4settings::setEmitSelectionChangedAfterApplyingFlag( bool NeedToEmit )
{
  if ( NeedToEmit )
    NeedToEmitSelectionChangedAfterApplying = true; 
}

// ----------------------------------------------------------------

void scigraphics::qt4settings::addSettingWidget( qt4settingsGroupBox *Widget )
{
  Q_ASSERT( Widget != NULL );

  SettingsComposer->addWidget( Widget );
  connect( Widget, SIGNAL(settingsUpdated()), SLOT(updatePlotSettings()) );

  if ( qt4settingsSelections *SelectionWidget = dynamic_cast<qt4settingsSelections*>(Widget) )
    connect( SelectionWidget, SIGNAL(needToSetEmitSelectionChangedAfterApplyingFlag(bool)), SLOT(setEmitSelectionChangedAfterApplyingFlag(bool)) );
}

// ----------------------------------------------------------------

void scigraphics::qt4settings::delSettingWidget( size_t Index )
{
  Q_ASSERT( SettingsComposer != NULL );
  SettingsComposer->delWidget( Index );
}

// ----------------------------------------------------------------

scigraphics::qt4settingsGroupBox* scigraphics::qt4settings::getSettingWidget( size_t Index )
{
  Q_ASSERT( SettingsComposer != NULL );
  return SettingsComposer->getSettingWidget( Index );
}

// ----------------------------------------------------------------

size_t scigraphics::qt4settings::numberOfSettingsWidget() const
{
  Q_ASSERT( SettingsComposer != NULL );
  return SettingsComposer->numberOfSettingsWidget();
}

// ----------------------------------------------------------------

void scigraphics::qt4settings::connectToPlot( qt4plot *Plot )
{
  if ( Plot == NULL )
    return;
  
  connect( Plot, SIGNAL(selectionChanged()), SLOT(updatePlotState()) );
  connect( Plot, SIGNAL(settingsUpdateRequired()), SLOT(updatePlotSettings()) );
  connect( this, SIGNAL(settingsChanged(scigraphics::qt4settings*)), Plot, SLOT(updatePlotSettings(scigraphics::qt4settings*)) );

  updatePlotSettings();
}

// ----------------------------------------------------------------

void scigraphics::qt4settings::disconnectFromPlot( qt4plot *Plot )
{
  if ( Plot == NULL )
    return;

  disconnect( this, NULL, Plot, NULL );
  disconnect( Plot, NULL, this, NULL );
}

// ----------------------------------------------------------------

void scigraphics::qt4settings::saveSettings( QSettings *Settings, const QString &Prefix ) const
{
  Settings->setValue( "qt4settings::" + Prefix, serialize() );
}

// ----------------------------------------------------------------

void scigraphics::qt4settings::loadSettings( QSettings *Settings, const QString &Prefix )
{
  QString Serialized = Settings->value( "qt4settings::" + Prefix ).toString();
  deserialize( Serialized );
}

// ----------------------------------------------------------------

void scigraphics::qt4settings::saveSettings( const QString &FileName ) const
{
  QSettings Settings( FileName, QSettings::IniFormat );
  saveSettings( &Settings );
}

// ----------------------------------------------------------------

void scigraphics::qt4settings::loadSettings( const QString &FileName )
{
  QSettings Settings( FileName, QSettings::IniFormat );
  loadSettings( &Settings );
}

// ----------------------------------------------------------------
      
QString scigraphics::qt4settings::serialize() const
{
  updateSettingsFromSubWidgets();
  std::string Serialized = Settings->serialize();
  return QString::fromStdString( Serialized );
}

// ----------------------------------------------------------------

bool scigraphics::qt4settings::deserialize( const QString &String )
{
  bool Ok = Settings->deserialize( String.toStdString() );
  if ( Ok )
  {
    updateSubWidgetsFromSettings();
    emit settingsChanged( this );
  }

  return Ok;
}

// ----------------------------------------------------------------

void scigraphics::qt4settings::updatePlotState()
{
  qt4plot *Plot = dynamic_cast<qt4plot*>( sender() );
  if ( Plot == NULL )
    return;

  for ( size_t i = 0; i < SettingsComposer->numberOfSettingsWidget(); i++ )
    SettingsComposer->getSettingWidget(i)->collectSettings( *Plot );
}

// ----------------------------------------------------------------

void scigraphics::qt4settings::updatePlotSettings()
{
  updateSettingsFromSubWidgets();
  emit settingsChanged( this );
}

// ----------------------------------------------------------------

void scigraphics::qt4settings::apply( qt4plot *Plot ) 
{
  if ( Plot == NULL )
    return;

  updateSettingsFromSubWidgets();
  applySettings(Plot);
  emitSettingsChanged();

  if ( NeedToEmitSelectionChangedAfterApplying )
  {
    Plot->emitSelectionChanged();
    NeedToEmitSelectionChangedAfterApplying = false;
  }
}

// ----------------------------------------------------------------
    
void scigraphics::qt4settings::updateSettingsFromSubWidgets() const
{
  for ( size_t i = 0; i < SettingsComposer->numberOfSettingsWidget(); i++ )
    SettingsComposer->getSettingWidget(i)->applySettings( Settings );
}

// ----------------------------------------------------------------
      
void scigraphics::qt4settings::updateSubWidgetsFromSettings()
{
  for ( size_t i = 0; i < SettingsComposer->numberOfSettingsWidget(); i++ )
    SettingsComposer->getSettingWidget(i)->showSettings( *Settings );
}

// ----------------------------------------------------------------

void scigraphics::qt4settings::applySettings( plot *Plot )
{
  plotSettings().apply(Plot);
}

// ----------------------------------------------------------------

void scigraphics::qt4settings::emitSettingsChanged()
{
  emit settingsChanged();
}

// ================================================================

