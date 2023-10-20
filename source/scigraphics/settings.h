
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

#include <iosfwd>

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

      enum graphTypeFlags
      {
        Individual      = 0x00,
        Lines           = 0x01,
        Points          = 0x02,
        LinesAndPoints  = 0x03,
        LinesHystogram  = 0x04,
        ErrorBars       = 0x10
      };

      enum floatingRectanglesFlags
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
        HorizontalStrip
      };

    private:
      scaleType ScaleTypes[ AxisPositionsCount ];
      interval<number> ScaleLimits[ AxisPositionsCount ];
      bool EnableScaleLimits[ AxisPositionsCount ];

      selectionStripType SelectionStripType;
      interval<number> SelectionStripInterval;
      unsigned GraphTypeFlags;
      unsigned VisibleFloatingRectanglesFlags;

    private:
      static scale* createScale( scaleType Type );
      static bool equalScaleTypes( const scale *Scale1, const scale *Scale2 );
      static interval<number> correctLimits( interval<number> Limits );
      void applyGraphTypeToGraph( sequence::graph *Graph ) const;
      static void throwIfAxisPosOutOfRange( axisPosition AxisPos );

    protected:
      void applyLimits( plot *Plot ) const;
      void applyGraphType( plot *Plot ) const;
      void applyScaleType( plot *Plot ) const;
      void applyFloatingRectangles( plot *Plot ) const;
      void applySelectionIntervals( plot *Plot ) const;

    public:
      settings();
      ~settings();

      void apply( plot *Plot ) const;

      void setAxisScaleLimits( const interval<number> &Lims, axisPosition AxisPos );
      void setAxisScaleLimits( number Min, number Max, axisPosition AxisPos );
      void setEnabledAxisScaleLimits( bool Enable, axisPosition AxisPos );
      interval<number> axisScaleLimits( axisPosition AxisPos ) const;
      bool enabledAxisScaleLimits( axisPosition AxisPos ) const;

      void setAxisScaleType( scaleType Type, axisPosition AxisPos );
      scaleType axisScaleType( axisPosition AxisPos ) const;

      void setGraphType( unsigned Type );
      unsigned graphType() const;

      void setVisibleFloatingRectangles( unsigned FloatRectangles );
      unsigned visibleFloatingRectangles() const;
      
      void setSelectionInterval( selectionStripType Type, interval<number> Interval );
      void setSelectionInterval( selectionStripType Type, number Min, number Max );
      selectionStripType selectionType() const;
      interval<number> selectionInterval() const;

      std::string serialize() const;
      bool deserialize( const std::string &String );
      
      static const selectionStrip* firstSelectionStrip( const plot &Plot );
  };

  // ============================================================
  
  std::ostream& operator<<( std::ostream &Stream, settings::scaleType  Type );
  std::istream& operator>>( std::istream &Stream, settings::scaleType &Type );
  
  std::ostream& operator<<( std::ostream &Stream, settings::selectionStripType  Type );
  std::istream& operator>>( std::istream &Stream, settings::selectionStripType &Type );
  
  std::ostream& operator<<( std::ostream &Stream, const settings &Settings );
  std::istream& operator>>( std::istream &Stream, settings &Settings );
  
  // ============================================================

}

