
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


// ======================================================

#include "scigraphics/qt4/labelinput.h"

#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QFileInfo>
#include <QDebug>

// ======================================================

void labelInput::createWidgets( const QString& LabelText )
{
  Label = new QLabel(LabelText,this);
  InputWgt = createInput();

  InputPart = defaultInputPart();
  Skip = defaultSkip();
  resizeEvent(NULL);
  
  setMinimumHeight(20);
  setMaximumHeight(20);
  InputWgt->setMaximumHeight( 20 );
}

// ------------------------------------------------------

void labelInput::resizeEvent( QResizeEvent* )
{

  int InputWidth = static_cast<int>( InputPart * static_cast<double>(width()) );
  int LabelWidth = width() - InputWidth - Skip;
  Label->setGeometry( 0, 0, LabelWidth, height() );
  InputWgt->setGeometry( width()-InputWidth, 0, InputWidth, height() );

  Label->repaint();
  Label->update();
}

// ------------------------------------------------------

void labelInput::enterEvent( QEvent *) 
{ 
  emit focusEntered(); 
}

// ------------------------------------------------------

double labelInput::defaultInputPart() const 
{ 
  return 0.33; 
}

// ------------------------------------------------------

int labelInput::defaultSkip() const 
{ 
  return 5; 
}

// ------------------------------------------------------

labelInput::labelInput( const QString& , QWidget *Parent, Qt::WindowFlags Flags ) : 
  QWidget(Parent,Flags) 
{
}

// ------------------------------------------------------

labelInput::~labelInput() 
{
}
    
// ------------------------------------------------------

void labelInput::setReadOnly( const bool ) 
{
};

// ------------------------------------------------------

void labelInput::setInputPart( const double P ) 
{ 
  InputPart = P; 
  resizeEvent(NULL); 
}

// ------------------------------------------------------
    
void labelInput::setEnabled( int E ) 
{ 
  QWidget::setEnabled( E != 0 ); 
}

// ======================================================
    
QWidget* labelEdit::createInput()
{
  Edit = new QLineEdit(this);
  
  connect( Edit, SIGNAL(returnPressed()), SIGNAL(returnPressed()));
  connect( Edit, SIGNAL(editingFinished()), SIGNAL(editingFinished()));
  
  connect( Edit, SIGNAL(cursorPositionChanged(int,int)), SIGNAL(cursorPositionChanged(int,int)));
  connect( Edit, SIGNAL(selectionChanged()), SIGNAL(selectionChanged()));
  connect( Edit, SIGNAL(textChanged(const QString&)), SIGNAL(textChanged(const QString&)));
  connect( Edit, SIGNAL(textEdited(const QString&)), SIGNAL(textEdited(const QString&)));

  connect( Edit, SIGNAL(returnPressed()), SIGNAL(changed()) );  
  connect( Edit, SIGNAL(editingFinished()), SIGNAL(changed()) );  
  
  return Edit;
}

// ======================================================

QWidget* labelEditPath::createInput()
{
  QWidget *Widget = new QWidget(this);
  
  QWidget *EditWidget = labelEdit::createInput();
  QPushButton *Button = new QPushButton("...",Widget);
  Button->setMaximumWidth(20);
  Button->setMinimumWidth(20);
  connect( Button, SIGNAL(clicked()), SLOT(setEditFromFileDialog()) );

  QHBoxLayout *HLayout = new QHBoxLayout();
  HLayout->setContentsMargins(0,0,0,0);
  HLayout->addWidget( EditWidget );
  HLayout->addWidget( Button );

  Widget->setLayout( HLayout );

  return Widget;
};
    
// ------------------------------------------------------

void labelEditPath::setEditFromFileDialog()
{
  QString CurrDir = QFileInfo( text().length() > 0 ? text() : "." ).absoluteDir().path();

  QFileDialog Dialog( this );
  Dialog.setDirectory( CurrDir );
  Dialog.setFileMode( FileMode );
  Dialog.setAcceptMode( AcceptMode );
  
  if ( Dialog.exec() == QDialog::Accepted )
  {
    setText( Dialog.selectedFiles().value(0,"") );
    emit changed();
  }

}

// ======================================================

QWidget* labelSpin::createInput()
{
  QSpinBox *Spin = new QSpinBox(this);
  connect( Spin, SIGNAL(valueChanged(int)), SIGNAL(valueChanged(int)));
  connect( Spin, SIGNAL(valueChanged(int)), SIGNAL(changed()) );
  return Spin;
}

// ======================================================

QWidget* labelDoubleSpin::createInput()
{
  QDoubleSpinBox *Spin = new QDoubleSpinBox(this);
  connect( Spin,  SIGNAL(valueChanged(double)), SIGNAL(valueChanged(double))); 
  connect( Spin,  SIGNAL(valueChanged(double)), SIGNAL(changed()) ); 
  return Spin;
}

// ======================================================

QWidget* labelCombo::createInput()
{
  QComboBox *Combo = new QComboBox(this);
  Combo->setSizeAdjustPolicy( QComboBox::AdjustToContents );
  connect( Combo, SIGNAL(currentIndexChanged(int)), SIGNAL(currentIndexChanged(int)));
  connect( Combo, SIGNAL(currentIndexChanged(const QString&)), SIGNAL(currentIndexChanged(const QString&)));
  connect( Combo, SIGNAL(currentIndexChanged(int)), SIGNAL(changed()));
  return Combo;
}

// ======================================================


