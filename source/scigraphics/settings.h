
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

// ============================================================

#include "scigraphics/interval.h"
#include "scigraphics/numbers.h"
#include "scigraphics/axisposition.h"

// ============================================================

namespace scigraphics
{
  
  // ============================================================

  class plot;
  class scale;
  class selectionStrip;
  
  namespace sequence 
  { 
    class graph; 
  }

  // ============================================================

  class settings
  {
    public:
      enum scaleType
      {
        Linear,
        LogarithmPositive,
        LogarithmNegative,
        Square
      };

      enum graphType 
      {
        Individual      = 0x00,
        Lines           = 0x01,
        Points          = 0x02,
        LinesAndPoints  = 0x03,
        LinesHystogram  = 0x04,
        ErrorBars       = 0x10
      };

      enum floatingRectangles
      {
        NoRectangles    = 0x00,
        Legend          = 0x01,
        CursorPosition  = 0x02
      };

      enum selectionStripType
      {
        UncontrollableStrip,
        NoneStrip,
        VerticalStrip,
        HorizontalStrip,
      };

    private:
      scaleType ScaleTypes[ AxisPositionsCount ];
      interval<number> ScaleLimits[ AxisPositionsCount ];
      selectionStripType SelectionStripType;
      interval<number> SelectionStripInterval;
      unsigned GraphType;
      unsigned VisibleFloatingRectangles;

    private:
      static scale* createScale( scaleType Type );
      static bool equalScaleTypes( const scale *Scale1, const scale *Scale2 );
      static interval<number> correctLimits( interval<number> Limits );
      void applyGraphTypeToGraph( sequence::graph *Graph ) const;

    protected:
      void applyLimits( plot *Plot ) const;
      void applyGraphType( plot *Plot ) const;
      void applyScaleType( plot *Plot ) const;
      void applyFloatingRectangles( plot *Plot ) const;
      void applySelectionIntervals( plot *Plot ) const;

    public:
      settings();
      virtual ~settings();

      virtual void apply( plot *Plot ) const;

      void setLimits( const interval<number> &Lims, axisPosition AxisPos );
      void setGraphType( unsigned Type );
      void setScaleType( scaleType Type, axisPosition AxisPos );
      void setVisibleFloatingRectangles( unsigned FloatRectangles );
      
      void setSelectionInterval( selectionStripType Type, interval<number> Interval );
      void setSelectionInterval( selectionStripType Type, number Min, number Max );
      selectionStripType getSelectionStripType() const { return SelectionStripType; }
      interval<number> getSelectionStripInterval() const { return SelectionStripInterval; }
      
      static selectionStrip* firstSelectionStrip( plot *Plot );

  };

// ============================================================

};

