
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

#include "scigraphics/qt4/settings.h"
#include "scigraphics/qt4/settingsbox.h"
#include "scigraphics/qt4/settingscomposer.h"
#include "scigraphics/qt4/plot.h"

#include <QStackedLayout>
#include <QSettings>

// ================================================================

scigraphics::qt4settings::qt4settings( QWidget *Parent, const QString &Name ) : 
  QWidget(Parent), 
  SettingsComposer(NULL) 
{ 
  initSettings( Name, qt4settingsGroupSuperBox::defaultAxisPositions(), NULL ); 
}

// ----------------------------------------------------------------

scigraphics::qt4settings::qt4settings( QWidget *Parent, const QList<axisSetCollection::axisPosition> &Positions ) : 
  QWidget(Parent), 
  SettingsComposer(NULL)
{ 
  initSettings( QString(), Positions, NULL ); 
}
      
// ----------------------------------------------------------------

scigraphics::qt4settings::qt4settings( QWidget *Parent, const QString &Name, const QList<axisSetCollection::axisPosition> &Positions, qt4settingsComposer *Composer ) : 
  QWidget(Parent), 
  SettingsComposer(NULL)
{ 
  initSettings( Name, Positions, Composer ); 
}
      
// ----------------------------------------------------------------

scigraphics::qt4settings::qt4settings( QWidget *Parent, qt4settingsComposer *Composer ) : 
  QWidget(Parent), 
  SettingsComposer(NULL)
{ 
  initSettings( QString(), qt4settingsGroupSuperBox::defaultAxisPositions(), Composer ); 
}

// ----------------------------------------------------------------

void scigraphics::qt4settings::initSettings( const QString &Name, const qt4settingsGroupSuperBox::axisPositionsList &Positions,
    qt4settingsComposer *Composer )
{
  setName(Name);
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

void scigraphics::qt4settings::addSettingWidget( qt4settingsGroupBox *Widget )
{
  Q_ASSERT( Widget != NULL );

  SettingsComposer->addWidget( Widget );
  connect( Widget, SIGNAL(settingsUpdated()), SLOT(updatePlotSettings()) );
}

// ----------------------------------------------------------------

void scigraphics::qt4settings::delSettingWidget( unsigned Index )
{
  Q_ASSERT( SettingsComposer != NULL );
  SettingsComposer->delWidget( Index );
}

// ----------------------------------------------------------------
    
scigraphics::qt4settingsGroupBox* scigraphics::qt4settings::getSettingWidget( unsigned Index )
{
  Q_ASSERT( SettingsComposer != NULL );
  return SettingsComposer->getSettingWidget( Index );
}

// ----------------------------------------------------------------

void scigraphics::qt4settings::connectToPlot( qt4plot *Plot )
{
  if ( Plot == NULL )
    return;
  
  connect( Plot, SIGNAL(selectionChanged()), SLOT(updatePlotState()) );
  connect( this, SIGNAL(settingsChanged(const scigraphics::settings&)), Plot, SLOT(updatePlotSettings(const scigraphics::settings&)) );

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
  Q_ASSERT( Settings != NULL );
  
  Settings->beginGroup( "qt4settings::" + Prefix );
  for ( unsigned i = 0; i < SettingsComposer->numberOfSettingsWidget(); i++ )
    SettingsComposer->getSettingWidget(i)->saveSettings(Settings);
  Settings->endGroup();
}

// ----------------------------------------------------------------

void scigraphics::qt4settings::loadSettings( QSettings *Settings, const QString &Prefix )
{
  Q_ASSERT( Settings != NULL );

  Settings->beginGroup( "qt4settings::" + Prefix );
  for ( unsigned i = 0; i < SettingsComposer->numberOfSettingsWidget(); i++ )
    SettingsComposer->getSettingWidget(i)->loadSettings(Settings);
  Settings->endGroup();
  updatePlotSettings();
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

void scigraphics::qt4settings::updatePlotState()
{
  qt4plot *Plot = dynamic_cast<qt4plot*>( sender() );
  if ( Plot == NULL )
    return;

  for ( unsigned i = 0; i < SettingsComposer->numberOfSettingsWidget(); i++ )
    SettingsComposer->getSettingWidget(i)->collectSettings( Plot );
}

// ----------------------------------------------------------------

void scigraphics::qt4settings::updatePlotSettings()
{
  updateSettingsFromSubWidgets();
  emit settingsChanged( *this );
}

// ----------------------------------------------------------------

void scigraphics::qt4settings::apply( qt4plot *Plot )
{
  if ( Plot == NULL )
    return;

  updateSettingsFromSubWidgets();
  applySettings(Plot);
  emitSettingsChanged();
}

// ----------------------------------------------------------------
    
void scigraphics::qt4settings::updateSettingsFromSubWidgets()
{
  for ( unsigned i = 0; i < SettingsComposer->numberOfSettingsWidget(); i++ )
    SettingsComposer->getSettingWidget(i)->applySettings( this );
}

// ----------------------------------------------------------------

void scigraphics::qt4settings::applySettings( qt4plot *Plot )
{
  settings::apply(Plot);
}

// ----------------------------------------------------------------

void scigraphics::qt4settings::emitSettingsChanged()
{
  emit settingsChanged();
}

// ================================================================

