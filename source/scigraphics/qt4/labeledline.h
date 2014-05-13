
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

class QLineEdit;
class QLabel;
class QHBoxLayout;
class QValidator;

// ================================================================

namespace scigraphics
{

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
  
  // ================================================================

}

