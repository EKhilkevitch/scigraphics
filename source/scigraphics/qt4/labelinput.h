
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

#warning "Usage of this file is deprecated"

#pragma once

// ======================================================

#include <QWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QFileDialog>
#include <QSettings>

// ======================================================

#define LABELINPUT_SAVE_SETTINGS( Name ) do { (Name)->saveSettings( #Name, Settings ); } while (false)
#define LABELINPUT_LOAD_SETTINGS( Name, Default ) do { (Name)->loadSettings( #Name, Settings, Default ); } while (false)

// ======================================================

class labelInput : public QWidget
{
  Q_OBJECT

  private:
    QLabel *Label;
    QWidget *InputWgt;

    int Skip;
    double InputPart;

  protected:
    virtual void resizeEvent( QResizeEvent *Event );
    virtual void createWidgets( const QString& LabelText );
    virtual void enterEvent( QEvent *Event );

    virtual double defaultInputPart() const;
    virtual int defaultSkip() const;

    virtual QWidget* createInput() = 0;

    QWidget* inputWgt() const { return InputWgt; }

  public:
    labelInput( const QString& , QWidget *Parent = NULL, Qt::WindowFlags Flags = 0 );
    ~labelInput();

    QString label() const { return Label->text(); }
    
    virtual void setReadOnly( const bool );
    void setInputPart( const double P );

  signals:
    void focusEntered();
    void changed();

  public slots:
    void setEnabled( int E );
};

// ======================================================

class labelEdit : public labelInput
{
  Q_OBJECT

  private:
    QLineEdit *Edit;

  protected:
    QWidget* createInput(); 
    QLineEdit* edit() const { return Edit; }

    labelEdit( int, const QString& LabelText, QWidget *Parent = NULL, Qt::WindowFlags Flags = 0 )
       : labelInput(LabelText,Parent,Flags) {}

  public:
    labelEdit( const QString& LabelText, QWidget *Parent = NULL, Qt::WindowFlags Flags = 0 ) 
      : labelInput(LabelText,Parent,Flags) { createWidgets(LabelText); }

    labelEdit( const QString& LabelText, QValidator *Validator, QWidget *Parent = NULL, Qt::WindowFlags Flags = 0 )  
      : labelInput(LabelText,Parent,Flags) { createWidgets(LabelText); setValidator(Validator); }

    labelEdit( const QString& LabelText, const QString& Text, QValidator *Validator = NULL, 
      QWidget *Parent = NULL, Qt::WindowFlags Flags = 0 )
      : labelInput(LabelText,Parent,Flags) { createWidgets(LabelText); setValidator(Validator); setText(Text); }

    labelEdit( const QString& LabelText, const double Part, QWidget *Parent = NULL, Qt::WindowFlags Flags = 0 ) 
      : labelInput(LabelText,Parent,Flags) { createWidgets(LabelText); setInputPart(Part); }

    QString text() const { return edit()->text(); }
    void setText( const QString& S ) { edit()->setText(S); }
    void setReadOnly( const bool E ) { edit()->setReadOnly(E); };
    void setValidator( QValidator *V ) { if ( V != NULL ) edit()->setValidator(V); }
    
    void saveSettings( const QString &Name, QSettings *Settings ) { Settings->setValue(Name,text()); }
    void loadSettings( const QString &Name, QSettings *Settings, const QString &Default ) { setText( Settings->value(Name,Default).toString() ); }

  signals:
    void editingFinished();
    void returnPressed();
    void cursorPositionChanged(int,int);
    void selectionChanged();
    void textChanged(const QString&);
    void textEdited(const QString&);
};

// ======================================================

class labelEditPath : public labelEdit
{
  Q_OBJECT

  protected:
    QWidget* createInput();

  private:
    enum QFileDialog::FileMode FileMode;
    enum QFileDialog::AcceptMode AcceptMode;

    void setDefaultModes() { FileMode = QFileDialog::AnyFile; AcceptMode = QFileDialog::AcceptOpen; }

  public:
    labelEditPath( const QString& LabelText, QWidget *Parent = NULL, Qt::WindowFlags Flags = 0 )
      : labelEdit(1,LabelText,Parent,Flags) { setDefaultModes(); createWidgets(LabelText); }
    labelEditPath( const QString& LabelText, const QString& Text, QWidget *Parent = NULL, Qt::WindowFlags Flags = 0 )
      : labelEdit(1,LabelText,Parent,Flags) { setDefaultModes(); createWidgets(LabelText); setText(Text); }

    void setFileMode( QFileDialog::FileMode Mode ) { FileMode = Mode; }
    void setAcceptMode( QFileDialog::AcceptMode Mode ) { AcceptMode = Mode; }

  public slots:
    void setEditFromFileDialog();

};

// ======================================================

class labelSpin : public labelInput
{
  Q_OBJECT
  
  protected:
    QWidget* createInput(); 
    QSpinBox* spin() const { return dynamic_cast<QSpinBox*>( inputWgt() ); }

  public:
    labelSpin( const QString& LabelText, QWidget *Parent = NULL, Qt::WindowFlags Flags = 0 ) 
      : labelInput(LabelText,Parent,Flags) { createWidgets(LabelText); }

    labelSpin( const QString& LabelText, int Min, int Max, QWidget *Parent = NULL, Qt::WindowFlags Flags = 0 )  
      : labelInput(LabelText,Parent,Flags) { createWidgets(LabelText); setRange(Min,Max); }

    labelSpin( const QString& LabelText, const double Part, QWidget *Parent = NULL, Qt::WindowFlags Flags = 0 ) 
      : labelInput(LabelText,Parent,Flags) { createWidgets(LabelText); setInputPart(Part); }

    int value() const { return spin()->value(); }
    void setValue( const int V ) { spin()->setValue(V); }
    void setReadOnly( const bool E ) { spin()->setReadOnly(E); };
    void setRange( int Min, int Max ) { spin()->setRange(Min,Max); }
    
    void saveSettings( const QString &Name, QSettings *Settings ) { Settings->setValue(Name,value()); }
    void loadSettings( const QString &Name, QSettings *Settings, const int Default ) { setValue( Settings->value(Name,Default).toInt() ); }

  signals:
    void valueChanged( int i );
};

// ======================================================

class labelDoubleSpin : public labelInput
{
  Q_OBJECT
  
  protected:
    QWidget* createInput(); 
    QDoubleSpinBox* spin() const { return dynamic_cast<QDoubleSpinBox*>( inputWgt() ); }

  public:
    labelDoubleSpin( const QString& LabelText, QWidget *Parent = NULL, Qt::WindowFlags Flags = 0 ) 
      : labelInput(LabelText,Parent,Flags) { createWidgets(LabelText); }

    labelDoubleSpin( const QString& LabelText, double Min, double Max, QWidget *Parent = NULL, Qt::WindowFlags Flags = 0 )  
      : labelInput(LabelText,Parent,Flags) { createWidgets(LabelText); setRange(Min,Max); }

    labelDoubleSpin( const QString& LabelText, const double Part, QWidget *Parent = NULL, Qt::WindowFlags Flags = 0 ) 
      : labelInput(LabelText,Parent,Flags) { createWidgets(LabelText); setInputPart(Part); }

    double value() const { return spin()->value(); }
    void setValue( const double V ) { spin()->setValue(V); }
    void setReadOnly( const bool E ) { spin()->setReadOnly(E); };
    void setRange( double Min, double Max ) { spin()->setRange(Min,Max); }
    void setDecimals( int D ) { spin()->setDecimals(D); }
    void setSingleStep( double S ) { spin()->setSingleStep(S); }
    
    void saveSettings( const QString &Name, QSettings *Settings ) { Settings->setValue(Name,value()); }
    void loadSettings( const QString &Name, QSettings *Settings, const double Default ) { setValue( Settings->value(Name,Default).toDouble() ); }

  signals:
    void valueChanged( double d );
};

// ======================================================

class labelCombo : public labelInput 
{
  Q_OBJECT

  protected:
    QWidget* createInput(); 
    QComboBox* combo() const { return dynamic_cast<QComboBox*>( inputWgt() ); }
    
    virtual double defaultInputPart() const { return 0.6; }

  public:
    labelCombo( const QString& LabelText, QWidget *Parent = NULL, Qt::WindowFlags Flags = 0 ) 
      : labelInput(LabelText,Parent,Flags) { createWidgets(LabelText); }

    labelCombo( const QString& LabelText, const QStringList& Items, QWidget *Parent = NULL, Qt::WindowFlags Flags = 0 )  
      : labelInput(LabelText,Parent,Flags) { createWidgets(LabelText); addItems(Items); }

    labelCombo( const QString& LabelText, const double Part, QWidget *Parent = NULL, Qt::WindowFlags Flags = 0 ) 
      : labelInput(LabelText,Parent,Flags) { createWidgets(LabelText); setInputPart(Part); }

    int  currentIndex() const { return combo()->currentIndex(); }
    int  count() const { return combo()->count(); }
    QString currentText() const { return combo()->currentText(); }
    QVariant currentData() const { return combo()->itemData( currentIndex() ); }
    void addItem( const QString &Text, const QVariant &UserData = QVariant() ) { combo()->addItem(Text,UserData); }
    void addItems( const QStringList& List ) { foreach( QString S, List ) addItem(S); }
    void clear() { combo()->clear(); }
    void setCurrentIndex( const int Index ) { combo()->setCurrentIndex(Index); }
    void setCurrentData( const QVariant& V ) { for ( int i = 0; i < combo()->count(); i++ ) { if (combo()->itemData(i)==V) setCurrentIndex(i); } }
    
    void saveSettings( const QString &Name, QSettings *Settings ) { Settings->setValue(Name,currentIndex()); }
    void loadSettings( const QString &Name, QSettings *Settings, const int Default ) { setCurrentIndex( Settings->value(Name,Default).toInt() ); }

  signals:
    void currentIndexChanged( int i );
    void currentIndexChanged( const QString& );

};

// ======================================================


