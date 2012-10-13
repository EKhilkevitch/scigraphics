
#pragma once

// ================================================================

#include "scigraphics/settings.h"
#include "scigraphics/qt4/labelinput.h"

#include <QList>
#include <QWidget>
#include <QGroupBox>

class QCheckBox;
class QRadioButton;
class QBoxLayout;
class QSettings;
class QTabWidget;

class qt4plot;

// ================================================================

class qt4plotSettingsGroupBox : public QGroupBox
{
  Q_OBJECT

  protected:
    static QString axisPositionString( scigraphics::axisSetCollection::axisPosition Axis );

  public:
    qt4plotSettingsGroupBox( const QString &Name = QString(), QWidget *Parent = NULL ) : QGroupBox(Name,Parent) {}
    virtual void apply( scigraphics::settings & ) {}
    virtual void saveSettings( QSettings* ) {}
    virtual void loadSettings( QSettings* ) {}

    virtual void addPlot( qt4plot* ) {}
    virtual void clearPlots() {}

  signals:
    void settingsUpdated();

  public slots:
    virtual void updateWidgets() { emit settingsUpdated(); }
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

    void apply( scigraphics::settings &Settings );
    void saveSettings( QSettings* Settings );
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
    labelEdit *MinScaleEdit, *MaxScaleEdit;

  protected:
    scigraphics::interval<scigraphics::number> getLimits() const;

  public:
    qt4plotSettingsScaleIntervals( const scigraphics::axisSetCollection::axisPosition Axis, QWidget *Parent = NULL );
    
    void apply( scigraphics::settings &Settings ) { Settings.setLimits(getLimits(),AxisType); }
    void saveSettings( QSettings* );
    void loadSettings( QSettings* );
  
  public slots:
    void updateWidgets();
};

// ----------------------------------------------------------------

class qt4plotSettingsScaleIntervalsAllAxis : public qt4plotSettingsGroupSuperBox
{
  Q_OBJECT

  protected:
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

  protected:
    unsigned getGraphType() const;

  public:
    qt4plotSettingsGraphType( QWidget *Parent = NULL );

    void apply( scigraphics::settings &Settings ) { Settings.setGraphType(getGraphType()); }

    void showLineHystogramControl( bool S );
    void showErrorBarsControl( bool S );
    
    void saveSettings( QSettings* Settings );
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
    
    void apply( scigraphics::settings &Settings ) { Settings.setScaleType(getScaleType(),AxisType); }
    
    void saveSettings( QSettings* Settings );
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
    QList<qt4plot*> Plots;
    
    QCheckBox *EnableSelectionBox;
    labelEdit *MinValueEdit, *MaxValueEdit;

  private:
    scigraphics::selectionStrip* getFirstStripSelection( scigraphics::plot *Plot );

  public:
    qt4plotSettingsSelections( QWidget *Parent = NULL );

    void apply( scigraphics::settings & );
    
    void saveSettings( QSettings* ) {}
    void loadSettings( QSettings* ) {}
    
    void addPlot( qt4plot* Plot );
    void clearPlots();

  public slots:
    void getLimitsFromPlot();
    void updateWidgetsEnables();
    void updateWidgets() { updateWidgetsEnables(); emit settingsUpdated(); }

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
    QList<qt4plot*> Plots;
    QString SettingsName;

    qt4plotSettingsComposer *SettingsComposer;

  protected:
    void collectSettings();
    virtual void applySettings();
    void emitSettingsChanged();

    void setComposer( qt4plotSettingsComposer *Composer );

    void initSettings( const QString &Name, const qt4plotSettingsGroupSuperBox::axisPositionsList &Positions,
      qt4plotSettingsComposer *Composer );

    const QList<qt4plot*>& listOfPlots() { return Plots; }

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

    void saveSettings( const QString &FileName );
    void loadSettings( const QString &FileName );

    void saveSettings( QSettings *Settings, const QString &Prefix = QString() );
    void loadSettings( QSettings *Settings, const QString &Prefix = QString() );

    void addSettingWidget( qt4plotSettingsGroupBox *Box );
    void delSettingWidget( unsigned Index );
    qt4plotSettingsGroupBox* getSettingWidget( unsigned Index );

    void addPlot( qt4plot *Plot );
    void clearPlots();

  public slots:
    virtual void apply();

  signals:
    void settingsChanged();
};

// ================================================================

