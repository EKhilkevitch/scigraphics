
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

#include "scigraphics/qt4/settingscomposer.h"
#include "scigraphics/qt4/settingsbox.h"

#include <QHBoxLayout>
#include <QStackedLayout>
#include <QTabWidget>

// ================================================================
      
scigraphics::qt4settingsComposer::qt4settingsComposer( QWidget *Parent ) : 
  QWidget(Parent) 
{
}

// ----------------------------------------------

void scigraphics::qt4settingsComposer::addToCollection( qt4settingsGroupBox *Widget )
{
  Q_ASSERT( Widget != NULL );
  Q_ASSERT( ! WidgetCollection.contains(Widget) );

  WidgetCollection.append( Widget );
}

// ----------------------------------------------

void scigraphics::qt4settingsComposer::delFromCollection( qt4settingsGroupBox *Widget )
{
  WidgetCollection.removeAll( Widget );
  delete Widget;
}

// ----------------------------------------------

scigraphics::qt4settingsGroupBox* scigraphics::qt4settingsComposer::getFromCollection( size_t Index )
{
  Q_ASSERT( static_cast<int>(Index) < WidgetCollection.size() );
  qt4settingsGroupBox *Widget = WidgetCollection[ static_cast<int>(Index) ];
  Q_ASSERT( Widget != NULL );
  return Widget;
}

// ----------------------------------------------

void scigraphics::qt4settingsComposer::addWidget( qt4settingsGroupBox *Widget )
{
  addToCollection( Widget );
  addToComposer( Widget );
}

// ----------------------------------------------

void scigraphics::qt4settingsComposer::delWidget( qt4settingsGroupBox *Widget )
{
  delFromComposer( Widget );
  delFromCollection( Widget );
}

// ----------------------------------------------

void scigraphics::qt4settingsComposer::delWidget( size_t Index )
{
  qt4settingsGroupBox *Widget = getSettingWidget(Index);
  delWidget( Widget );
}

// ----------------------------------------------
      
scigraphics::qt4settingsGroupBox* scigraphics::qt4settingsComposer::getSettingWidget( size_t Index ) 
{ 
  return getFromCollection(Index); 
}

// ----------------------------------------------

size_t scigraphics::qt4settingsComposer::numberOfSettingsWidget() const 
{ 
  return WidgetCollection.size(); 
}

// ----------------------------------------------
    
void scigraphics::qt4settingsComposer::clearWidgets()
{
  for ( int i = static_cast<int>(numberOfSettingsWidget()) - 1; i >= 0; i-- )
  {
    qt4settingsGroupBox *Widget = getSettingWidget(i);
    Q_ASSERT( Widget != NULL );
    delWidget( Widget );
  }
}

// ================================================================
    
scigraphics::qt4settingsComposerLayout::qt4settingsComposerLayout( QWidget *Parent, QBoxLayout *L ) :
  qt4settingsComposer(Parent),
  Layout(L)
{
  if ( Layout == NULL )
    Layout = new QHBoxLayout();
  setLayout( Layout );
}

// ----------------------------------------------

void scigraphics::qt4settingsComposerLayout::addToComposer( qt4settingsGroupBox *Widget )
{
  Q_ASSERT( Widget != NULL );
  Q_ASSERT( Layout != NULL );
  Q_ASSERT( layout() == Layout );
  Layout->addWidget( Widget );
}

// ----------------------------------------------

void scigraphics::qt4settingsComposerLayout::delFromComposer( qt4settingsGroupBox *Widget )
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
    
scigraphics::qt4settingsComposer* scigraphics::qt4settingsComposerLayout::createNewComposer( QWidget *Parent ) const
{
  QBoxLayout *NewLayout = NULL;
  if ( dynamic_cast<QVBoxLayout*>(Layout) != NULL )
    NewLayout = new QVBoxLayout();
  else if ( dynamic_cast<QHBoxLayout*>(Layout) != NULL )
    NewLayout = new QHBoxLayout();
  return new qt4settingsComposerLayout( Parent, NewLayout );
}

// ================================================================

scigraphics::qt4settingsComposerTabs::qt4settingsComposerTabs( QWidget *Parent ) :
  qt4settingsComposer(Parent)
{
  TabWidget = new QTabWidget(this);

  QStackedLayout *Layout = new QStackedLayout();
  Layout->addWidget( TabWidget );
  setLayout( Layout );
}
    
// ----------------------------------------------

void scigraphics::qt4settingsComposerTabs::addToComposer( qt4settingsGroupBox *Widget )
{
  Q_ASSERT( TabWidget != NULL );
  Q_ASSERT( Widget != NULL );

  TabWidget->addTab( Widget, Widget->title() );
}

// ----------------------------------------------

void scigraphics::qt4settingsComposerTabs::delFromComposer( qt4settingsGroupBox *Widget )
{
  int Index = TabWidget->indexOf( Widget );
  if ( Index < 0 )
    return;
  TabWidget->removeTab(Index);
}

// ----------------------------------------------
    
scigraphics::qt4settingsComposer* scigraphics::qt4settingsComposerTabs::createNewComposer( QWidget *Parent ) const
{
  return new qt4settingsComposerTabs(Parent);
}

// ================================================================

