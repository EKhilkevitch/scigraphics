
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

#include "scigraphics/axisposition.h"
#include "scigraphics/interval.h"
#include "scigraphics/numbers.h"
#include "scigraphics/settings.h"

#include <QGroupBox>
#include <QList>
#include <QMap>

class QCheckBox;
class QRadioButton;
class QGridLayout;

// ================================================================

namespace scigraphics
{

  // ================================================================
 
  class plot;
  class qt4plot;
  class qt4labeledLineEdit;

  // ================================================================

  class qt4settingsGroupBox : public QGroupBox
  {
    Q_OBJECT

    protected:
      static QString axisPositionString( axisPosition Axis );

    public:
      qt4settingsGroupBox( const QString &Name = QString(), QWidget *Parent = NULL );
      virtual void applySettings( settings* ) const;
      virtual void showSettings( const settings &Settings );
      virtual void collectSettings( const plot &Plot );

    signals:
      void settingsUpdated();

    public slots:
      virtual void updateWidgets();
  };

  // ----------------------------------------------------------------

  class qt4settingsGroupSuperBox : public qt4settingsGroupBox
  {
    Q_OBJECT

    public:
      typedef QList<axisPosition> axisPositionsList;
      
      static axisPositionsList defaultAxisPositions();

    private:
      QMap< axisPosition, qt4settingsGroupBox* > Boxes; 
      unsigned NumberOfRowsInLayout;

    private:
      void addToList( axisPosition Position, qt4settingsGroupBox *GroupBox );
      void updateLayout();

      virtual qt4settingsGroupBox* createBoxForAxisPosition( axisPosition Position ) = 0;

    protected:
      void init( const axisPositionsList &AxisPositions = defaultAxisPositions() );

    public:
      explicit qt4settingsGroupSuperBox( unsigned NumberOfRowsInLayout = 1, const QString &Name = QString(), QWidget *Parent = NULL );

      void applySettings( settings *Settings ) const;
      void showSettings( const settings &Settings );
      void collectSettings( const plot &Plot );

      qt4settingsGroupBox* boxForAxisPosition( axisPosition Position );
      QList<axisPosition> axisPositionsWithBoxes() const;

      unsigned numberOfRowsInLayout() const;
      void setNumberOfRowsInLayout( unsigned Number );

    public slots:
      void updateWidgets();
  };

  // ----------------------------------------------------------------

  class qt4settingsScaleIntervals : public qt4settingsGroupBox
  {
    Q_OBJECT

    private:
      axisPosition AxisType;

      QCheckBox *ManualScaleBox;
      qt4labeledLineEdit *MinScaleEdit, *MaxScaleEdit;
      
    private:
      void updateWidgetsEnables();

    public:
      explicit qt4settingsScaleIntervals( const axisPosition Axis, QWidget *Parent = NULL );
      
      void applySettings( settings *Settings ) const;
      void showSettings( const settings &Settings );
    
    public slots:
      void updateWidgets();
  };

  // ----------------------------------------------------------------

  class qt4settingsScaleIntervalsAllAxis : public qt4settingsGroupSuperBox
  {
    private:
      qt4settingsGroupBox* createBoxForAxisPosition( axisPosition Pos );
      static QString name();

    public:
      qt4settingsScaleIntervalsAllAxis( QWidget *Parent, const axisPositionsList &Positions );
  };

  // ----------------------------------------------------------------

  class qt4settingsGraphType : public qt4settingsGroupBox
  {
    private:
      QRadioButton *ShowLinesBtn, 
                   *ShowPointsBtn, 
                   *ShowLinesAndPointsBtn, 
                   *ShowLineHystogramBtn;
      QCheckBox *ErrorBarsChk;

    private:
      unsigned getGraphType() const;
      void setGraphType( unsigned GraphType );

    public:
      explicit qt4settingsGraphType( QWidget *Parent = NULL );

      void applySettings( settings *Settings ) const;
      void showSettings( const settings &Settings );

      void showLineHystogramControl( bool Show );
      void showErrorBarsControl( bool Show );
  };

  // ----------------------------------------------------------------

  class qt4settingsDecoration : public qt4settingsGroupBox
  {
    private:
      QCheckBox *ShowLegendChk,
                *ShowCursorPositionChk; 

    private:
      unsigned getVisibleFloatingRectangles() const;
      void setVisibleFloatingRectangles( unsigned VisibleFloatingRectangles );

    public:
      qt4settingsDecoration( QWidget *Parent = NULL );

      void applySettings( settings *Settings ) const;
      void showSettings( const settings &Settings );
  };

  // ----------------------------------------------------------------

  class qt4settingsScaleType : public qt4settingsGroupBox
  {
    private:
      axisPosition AxisType;

      QRadioButton *LinearBtn, 
                   *LogarithmPositiveBtn, 
                   *LogarithmNegativeBtn, 
                   *SquareBtn;

    protected:
      settings::scaleType getScaleType() const;
      void setScaleType( settings::scaleType ScaleType );

    public:
      explicit qt4settingsScaleType( const axisPosition Axis, QWidget *Parent = NULL );
      
      void applySettings( settings *Settings ) const;
      void showSettings( const settings &Settings );
      
      void showLogarithmNegativeControl( bool Show );
      void showSquareControl( bool Show );
  };

  // ----------------------------------------------------------------

  class qt4settingsScaleTypeAllAxis : public qt4settingsGroupSuperBox
  {
    protected:
      static QString name();
      qt4settingsGroupBox* createBoxForAxisPosition( axisPosition Pos );

    public:
      explicit qt4settingsScaleTypeAllAxis( QWidget *Parent = NULL, const axisPositionsList &Positions = defaultAxisPositions() );
  };

  // ----------------------------------------------------------------

  class qt4settingsSelections : public qt4settingsGroupBox
  {
    Q_OBJECT

    private:
      QCheckBox *EnableSelectionBox;
      qt4labeledLineEdit *MinValueEdit, *MaxValueEdit;

    private:
      void updateWidgetsEnables();

    public:
      explicit qt4settingsSelections( QWidget *Parent = NULL );

      void applySettings( settings *Settings ) const;
      void collectSettings( const plot &Plot );
      void showSettings( const settings &Settings );
      
    public slots:
      void updateWidgets();

    signals:
      void limitsUpdatedFromPlot();
      void needToSetEmitSelectionChangedAfterApplyingFlag( bool );
  };
    
  // ================================================================

}
