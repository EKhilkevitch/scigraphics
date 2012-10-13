
// ======================================================

#include "graphics/qt4/labelinput.h"

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

  int InputWidth = (int)( InputPart * (double)width() );
  int LabelWidth = width() - InputWidth - Skip;
  Label->setGeometry( 0, 0, LabelWidth, height() );
  InputWgt->setGeometry( width()-InputWidth, 0, InputWidth, height() );

  Label->repaint();
  Label->update();
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
    setText( Dialog.selectedFiles().value(0,"") );

}

// ======================================================

QWidget* labelSpin::createInput()
{
  QSpinBox *Spin = new QSpinBox(this);
  connect( Spin, SIGNAL(valueChanged(int)), SIGNAL(valueChanged(int)));
  return Spin;
}

// ======================================================

QWidget* labelDoubleSpin::createInput()
{
  QDoubleSpinBox *Spin = new QDoubleSpinBox(this);
  connect( Spin,  SIGNAL(valueChanged(double)), SIGNAL(valueChanged(double))); 
  return Spin;
}

// ======================================================

QWidget* labelCombo::createInput()
{
  QComboBox *Combo = new QComboBox(this); 
  connect( Combo, SIGNAL(currentIndexChanged(int)), SIGNAL(currentIndexChanged(int)));
  connect( Combo, SIGNAL(currentIndexChanged(const QString&)), SIGNAL(currentIndexChanged(const QString&)));
  return Combo;
}

// ======================================================


