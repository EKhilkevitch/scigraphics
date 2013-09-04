
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


#pragma once

#include "scigraphics/plot.h"

namespace scigraphics
{

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

    private:
      scaleType ScaleTypes[ axisSetCollection::PositionsCount ];
      interval<number> ScaleLimits[ axisSetCollection::PositionsCount ];
      unsigned GraphType;
      unsigned VisibleFloatingRectangles;

      static scale* createScale( scaleType Type );
      static bool equalScaleTypes( const scale *S1, const scale *S2 );
      static interval<number> correctLimits( interval<number> Limits );

    private:
      void applyGraphTypeToGraph( graphSequence *Graph ) const;

    protected:
      void applyLimits( plot *Plot ) const;
      void applyGraphType( plot *Plot ) const;
      void applyScaleType( plot *Plot ) const;
      void applyFloatingRectangles( plot *Plot ) const;

    public:
      settings();
      virtual ~settings() {}

      void apply( plot *Plot ) const;

      void setLimits( const interval<number> &Lims, axisSetCollection::axisPosition AxisPos );
      void setGraphType( unsigned Type ) { GraphType = Type; }
      void setScaleType( scaleType Type, axisSetCollection::axisPosition AxisPos );
      void setVisibleFloatingRectangles( unsigned FloatRectangles );
  };

// ============================================================

};

