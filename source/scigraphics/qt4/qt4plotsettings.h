
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

#include "scigraphics/settings.h"

#include <QList>
#include <QWidget>
#include <QGroupBox>

class QCheckBox;
class QRadioButton;
class QSettings;
class QTabWidget;
class QLineEdit;
class QLabel;
class QValidator;
class QBoxLayout;
class QHBoxLayout;

class qt4plot;

namespace scigraphics
{
  class selectionStrip;
}

// ================================================================

class qt4labeledLineEdit : public QWidget
{
  Q_OBJECT

  private:
    QLineEdit *Edit;
    QLabel *Label;
    QHBoxLayout *Layout;

  public:
    qt4labeledLineEdit( const QString &Label, const QString &Text, QWidget *Parent = NULL );
    void setValidator( QValidator *Validator );
    void setStretchFactors( int LabelStretch, int EditStretch );
    void setText( const QString &Text );
    QString text() const;

  signals:
    void editingFinished();
    void returnPressed();
};

// ----------------------------------------------------------------

class qt4plotSettingsGroupBox : public QGroupBox
{
  Q_OBJECT

  protected:
    static QString axisPositionString( scigraphics::axisSetCollection::axisPosition Axis );

  public:
    qt4plotSettingsGroupBox( const QString &Name = QString(), QWidget *Parent = NULL );
    virtual void applySettings( scigraphics::settings* ) {}
    virtual void collectSettings( qt4plot* ) {}
    virtual void saveSettings( QSettings* ) const {}
    virtual void loadSettings( QSettings* ) {}

  signals:
    void settingsUpdated();

  public slots:
    virtual void updateWidgets();
};

// ----------------------------------------------------------------

class qt4plotSettingsGroupSuperBox : public qt4plotSettingsGroupBox
{
  Q_OBJECT

  public:
    typedef QList<scigraphics::axisSetCollection::axisPosition> axisPositionsList;
    
    static axisPositionsList defaultAxisPositions();

  private:
    QList< qt4plotSettingsGroupBox* > Boxes; 

  protected:
    void addToList( qt4plotSettingsGroupBox *B );

    virtual int numOfRowsInLayout() { return 0; }
    virtual qt4plotSettingsGroupBox* createBoxForAxisPosition( scigraphics::axisSetCollection::axisPosition ) { return NULL; }

    void init( const axisPositionsList &AxisPositions = defaultAxisPositions() );

  public:
    qt4plotSettingsGroupSuperBox( const QString &Name = QString(), QWidget *Parent = NULL ) : qt4plotSettingsGroupBox(Name,Parent) {}

    void applySettings( scigraphics::settings* Settings );
    void collectSettings( qt4plot* Plot );

    void saveSettings( QSettings* Settings ) const;
    void loadSettings( QSettings* Settings );
  
  public slots:
    void updateWidgets();
};

// ----------------------------------------------------------------

class qt4plotSettingsScaleIntervals : public qt4plotSettingsGroupBox
{
  Q_OBJECT

  private:
    scigraphics::axisSetCollection::axisPosition AxisType;

    QCheckBox *ManualScaleBox;
    qt4labeledLineEdit *MinScaleEdit, *MaxScaleEdit;

  private:
    scigraphics::interval<scigraphics::number> getLimits() const;

  public:
    qt4plotSettingsScaleIntervals( const scigraphics::axisSetCollection::axisPosition Axis, QWidget *Parent = NULL );
    
    void applySettings( scigraphics::settings* Settings );
    void saveSettings( QSettings* ) const;
    void loadSettings( QSettings* );
  
  public slots:
    void updateWidgets();
};

// ----------------------------------------------------------------

class qt4plotSettingsScaleIntervalsAllAxis : public qt4plotSettingsGroupSuperBox
{
  Q_OBJECT

  private:
    int numOfRowsInLayout() { return 2; }
    qt4plotSettingsGroupBox* createBoxForAxisPosition( scigraphics::axisSetCollection::axisPosition Pos ) { return new qt4plotSettingsScaleIntervals(Pos,this); }
    static QString name() { return "Scale intervals"; }

  public:
    qt4plotSettingsScaleIntervalsAllAxis( QWidget *Parent = NULL, const axisPositionsList &Positions = defaultAxisPositions() ) : 
      qt4plotSettingsGroupSuperBox( name(), Parent ) { init(Positions); }
};

// ----------------------------------------------------------------

class qt4plotSettingsGraphType : public qt4plotSettingsGroupBox
{
  Q_OBJECT
      
  private:
    QRadioButton *ShowLinesBtn, 
                 *ShowPointsBtn, 
                 *ShowLinesAndPointsBtn, 
                 *ShowLineHystogramBtn;
    QCheckBox *ErrorBarsChk;

  private:
    unsigned getGraphType() const;

  public:
    qt4plotSettingsGraphType( QWidget *Parent = NULL );

    void applySettings( scigraphics::settings* Settings );

    void showLineHystogramControl( bool S );
    void showErrorBarsControl( bool S );
    
    void saveSettings( QSettings* Settings ) const;
    void loadSettings( QSettings* Settings );
};

// ----------------------------------------------------------------

class qt4plotSettingsDecoration : public qt4plotSettingsGroupBox
{
  Q_OBJECT
      
  private:
    QCheckBox *ShowLegendChk,
              *ShowCursorPositionChk; 

  private:
    unsigned getVisibleFloatingRectangles() const;

  public:
    qt4plotSettingsDecoration( QWidget *Parent = NULL );

    void applySettings( scigraphics::settings* Settings );

    void saveSettings( QSettings* Settings ) const;
    void loadSettings( QSettings* Settings );
};

// ----------------------------------------------------------------

class qt4plotSettingsScaleType : public qt4plotSettingsGroupBox
{
  Q_OBJECT

  private:
    scigraphics::axisSetCollection::axisPosition AxisType;

    QRadioButton *LinearBtn, *LogarithmPositiveBtn, *LogarithmNegativeBtn, *SquareBtn;

  protected:
    scigraphics::settings::scaleType getScaleType() const;

  public:
    qt4plotSettingsScaleType( const scigraphics::axisSetCollection::axisPosition Axis, QWidget *Parent = NULL );
    
    void applySettings( scigraphics::settings* Settings );
    
    void saveSettings( QSettings* Settings ) const;
    void loadSettings( QSettings* Settings );
};

// ----------------------------------------------------------------

class qt4plotSettingsScaleTypeAllAxis : public qt4plotSettingsGroupSuperBox
{
  Q_OBJECT

  protected:
    int numOfRowsInLayout() { return 1; }
    qt4plotSettingsGroupBox* createBoxForAxisPosition( scigraphics::axisSetCollection::axisPosition Pos ) { return new qt4plotSettingsScaleType(Pos,this); }
    static QString name() { return "Scale types"; }

  public:
    qt4plotSettingsScaleTypeAllAxis( QWidget *Parent = NULL, const axisPositionsList &Positions = defaultAxisPositions() ) : 
      qt4plotSettingsGroupSuperBox( name(), Parent ) { init(Positions); }
};

// ----------------------------------------------------------------

class qt4plotSettingsSelections : public qt4plotSettingsGroupBox
{
  Q_OBJECT

  private:
    QCheckBox *EnableSelectionBox;
    qt4labeledLineEdit *MinValueEdit, *MaxValueEdit;

  public:
    qt4plotSettingsSelections( QWidget *Parent = NULL );

    void applySettings( scigraphics::settings* Settings );
    void collectSettings( qt4plot* Plot );
    
    void saveSettings( QSettings* ) const {}
    void loadSettings( QSettings* ) {}
    
  public slots:
    void updateWidgetsEnables();
    void updateWidgets();

  signals:
    void limitsUpdatedFromPlot();
};

// ================================================================

class qt4plotSettingsComposer : public QWidget
{
  private:
    QList<qt4plotSettingsGroupBox*> WidgetCollection;

  protected:
    void addToCollection( qt4plotSettingsGroupBox *Widget );
    void delFromCollection( qt4plotSettingsGroupBox *Widget );
    qt4plotSettingsGroupBox* getFromCollection( unsigned Index );
    unsigned sizeOfCollection() const { return WidgetCollection.size(); }

  protected:
    virtual void addToComposer( qt4plotSettingsGroupBox *Widget ) = 0;
    virtual void delFromComposer( qt4plotSettingsGroupBox *Widget ) = 0;

  public:
    qt4plotSettingsComposer( QWidget *Parent ) : QWidget(Parent) {}
    void addWidget( qt4plotSettingsGroupBox *Widget );
    void delWidget( qt4plotSettingsGroupBox *Widget );
    void delWidget( unsigned Index );
    qt4plotSettingsGroupBox* getSettingWidget( unsigned Index ) { return getFromCollection(Index); }
    unsigned numberOfSettingsWidget() const { return sizeOfCollection(); }
    void clearWidgets();

    virtual qt4plotSettingsComposer* createNewComposer( QWidget *Parent ) const = 0;

    virtual ~qt4plotSettingsComposer() {}
};

// ----------------------------------------------------------------

class qt4plotSettingsComposerLayout : public qt4plotSettingsComposer
{
  private:
    QBoxLayout *Layout;

  protected:
    void addToComposer( qt4plotSettingsGroupBox *Widget );
    void delFromComposer( qt4plotSettingsGroupBox *Widget );

  public:
    qt4plotSettingsComposerLayout( QWidget *Parent = NULL, QBoxLayout *L = NULL );
    qt4plotSettingsComposer* createNewComposer( QWidget *Parent ) const;
};

// ----------------------------------------------------------------

class qt4plotSettingsComposerTabs : public qt4plotSettingsComposer
{
  private:
    QTabWidget *TabWidget;

  protected:
    void addToComposer( qt4plotSettingsGroupBox *Widget );
    void delFromComposer( qt4plotSettingsGroupBox *Widget );
  
  public:
    qt4plotSettingsComposerTabs( QWidget *Parent = NULL );
    qt4plotSettingsComposer* createNewComposer( QWidget *Parent ) const;
};

// ================================================================

class qt4plotSettings : public QWidget, public scigraphics::settings
{
  Q_OBJECT

  private:
    QString SettingsName;
    qt4plotSettingsComposer *SettingsComposer;

  protected:
    void updateSettingsFromSubWidgets();
    virtual void applySettings( qt4plot *Plot );
    void emitSettingsChanged();

    void setComposer( qt4plotSettingsComposer *Composer );

    void initSettings( const QString &Name, const qt4plotSettingsGroupSuperBox::axisPositionsList &Positions,
      qt4plotSettingsComposer *Composer );

  public:

    qt4plotSettings( QWidget *Parent = NULL, const QString &Name = QString() ) : QWidget(Parent), SettingsComposer(NULL) 
      { initSettings( Name, qt4plotSettingsGroupSuperBox::defaultAxisPositions(), NULL ); }

    qt4plotSettings( QWidget *Parent, const qt4plotSettingsGroupSuperBox::axisPositionsList &Positions ) : 
      QWidget(Parent), SettingsComposer(NULL)
      { initSettings( QString(), Positions, NULL ); } 
    
    qt4plotSettings( QWidget *Parent, const QString &Name, 
      const qt4plotSettingsGroupSuperBox::axisPositionsList &Positions, qt4plotSettingsComposer *Composer = NULL ) : 
      QWidget(Parent), SettingsComposer(NULL)
      { initSettings( Name, Positions, Composer ); } 
    
    qt4plotSettings( QWidget *Parent, qt4plotSettingsComposer *Composer ) : QWidget(Parent), SettingsComposer(NULL)
      { initSettings( QString(), qt4plotSettingsGroupSuperBox::defaultAxisPositions(), Composer ); }

    const QString& name() const { return SettingsName; }
    void setName( const QString &Name ) { SettingsName = Name; }

    void saveSettings( const QString &FileName ) const;
    void loadSettings( const QString &FileName );

    void saveSettings( QSettings *Settings, const QString &Prefix = QString() ) const;
    void loadSettings( QSettings *Settings, const QString &Prefix = QString() );

    void addSettingWidget( qt4plotSettingsGroupBox *Box );
    void delSettingWidget( unsigned Index );
    qt4plotSettingsGroupBox* getSettingWidget( unsigned Index );

    void connectToPlot( qt4plot *Plot );
    void disconnectFromPlot( qt4plot *Plot );

    void apply( qt4plot *Plot );

  public slots:
    void updatePlotState();
    void updatePlotSettings();

  signals:
    void settingsChanged();
    void settingsChanged( const scigraphics::settings& );
};

// ================================================================

