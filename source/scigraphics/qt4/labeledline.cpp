
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

#include "scigraphics/qt4/labeledline.h"

#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>

// ================================================================

scigraphics::qt4labeledLineEdit::qt4labeledLineEdit( const QString &LabelText, const QString &EditText, QWidget *Parent ) : 
  QWidget(Parent)
{
  Label = new QLabel( LabelText, this );
  Edit = new QLineEdit( EditText, this );

  Layout = new QHBoxLayout();
  Layout->setContentsMargins( 0, 0, 0, 0 );
  Layout->addWidget( Label );
  Layout->addSpacing(5);
  Layout->addWidget( Edit );
  setLayout(Layout);

  setStretchFactors( 7, 2 );  
  
  connect( Edit, SIGNAL(editingFinished()), SIGNAL(editingFinished()));
  connect( Edit, SIGNAL(returnPressed()), SIGNAL(returnPressed()));
}

// ----------------------------------------------------------------
    
void scigraphics::qt4labeledLineEdit::setStretchFactors( int LabelStretch, int EditStretch )
{
  Layout->setStretchFactor( Label, LabelStretch );
  Layout->setStretchFactor( Edit, EditStretch );
}

// ----------------------------------------------------------------

void scigraphics::qt4labeledLineEdit::setValidator( QValidator *Validator )
{
  Edit->setValidator(Validator);
}

// ----------------------------------------------------------------
    
void scigraphics::qt4labeledLineEdit::setText( const QString &Text ) 
{ 
  Edit->setText(Text); 
}

// ----------------------------------------------------------------
    
QString scigraphics::qt4labeledLineEdit::text() const
{
  return Edit->text();
}

// ================================================================

