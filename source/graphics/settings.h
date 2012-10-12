
#pragma once

#include "graphics/plot.h"

namespace graphics
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

    private:
      scaleType ScaleTypes[ axisSetCollection::PositionsCount ];
      interval<number> ScaleLimits[ axisSetCollection::PositionsCount ];
      unsigned GraphType;

      static scale* createScale( scaleType Type );
      static bool equalScaleTypes( const scale *S1, const scale *S2 );
      static interval<number> correctLimits( interval<number> Limits );

    private:
      void applyGraphTypeToGraph( graphSequence *Graph ) const;

    protected:
      void applyLimits( plot &Plot ) const;
      void applyGraphType( plot &Plot ) const;
      void applyScaleType( plot &Plot ) const;

    public:
      settings();
      virtual ~settings() {}

      void apply( plot &Plot ) const;

      void setLimits( const interval<number> &Lims, axisSetCollection::axisPosition AxisPos );
      void setGraphType( unsigned Type ) { GraphType = Type; }
      void setScaleType( scaleType Type, axisSetCollection::axisPosition AxisPos );
  };

// ============================================================

};

