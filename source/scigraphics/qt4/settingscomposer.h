
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


#pragma once

// ================================================================

#include <QWidget>
#include <QList>

class QBoxLayout;
class QTabWidget;

// ================================================================

namespace scigraphics
{

  // ================================================================
  
  class qt4settingsGroupBox;
  
  // ================================================================

  class qt4settingsComposer : public QWidget
  {
    private:
      QList<qt4settingsGroupBox*> WidgetCollection;

    protected:
      void addToCollection( qt4settingsGroupBox *Widget );
      void delFromCollection( qt4settingsGroupBox *Widget );
      qt4settingsGroupBox* getFromCollection( unsigned Index );
      unsigned sizeOfCollection() const { return WidgetCollection.size(); }

    protected:
      virtual void addToComposer( qt4settingsGroupBox *Widget ) = 0;
      virtual void delFromComposer( qt4settingsGroupBox *Widget ) = 0;

    public:
      qt4settingsComposer( QWidget *Parent ) : QWidget(Parent) {}
      void addWidget( qt4settingsGroupBox *Widget );
      void delWidget( qt4settingsGroupBox *Widget );
      void delWidget( unsigned Index );
      qt4settingsGroupBox* getSettingWidget( unsigned Index ) { return getFromCollection(Index); }
      unsigned numberOfSettingsWidget() const { return sizeOfCollection(); }
      void clearWidgets();

      virtual qt4settingsComposer* createNewComposer( QWidget *Parent ) const = 0;

      virtual ~qt4settingsComposer() {}
  };

  // ----------------------------------------------------------------

  class qt4settingsComposerLayout : public qt4settingsComposer
  {
    private:
      QBoxLayout *Layout;

    protected:
      void addToComposer( qt4settingsGroupBox *Widget );
      void delFromComposer( qt4settingsGroupBox *Widget );

    public:
      qt4settingsComposerLayout( QWidget *Parent = NULL, QBoxLayout *L = NULL );
      qt4settingsComposer* createNewComposer( QWidget *Parent ) const;
  };

  // ----------------------------------------------------------------

  class qt4settingsComposerTabs : public qt4settingsComposer
  {
    private:
      QTabWidget *TabWidget;

    protected:
      void addToComposer( qt4settingsGroupBox *Widget );
      void delFromComposer( qt4settingsGroupBox *Widget );
    
    public:
      qt4settingsComposerTabs( QWidget *Parent = NULL );
      qt4settingsComposer* createNewComposer( QWidget *Parent ) const;
  };
  
  // ================================================================

}

