
#pragma once

// ============================================================

#include "scigraphics/numbers.h"
#include "scigraphics/numlimits.h"
#include "scigraphics/graphcollection.h"

#include <map>

// ============================================================

namespace scigraphics
{

// ============================================================

  class plotLimits
  {
    public:
      static const number AutoScaleMin, AutoScaleMax;

      class limits
      {
        private:
          numberLimits Limits;

        protected:
          static void updateLimitsByInterval( numberLimits *Limits, interval<number> Interval );

          static interval<number> applyStretchToTotalInterval( interval<number> Interval, double Stretch );
          static interval<number> applyStretchToPositiveInterval( interval<number> Interval, double Stretch );
          static interval<number> applyStretchToNegativeInterval( interval<number> Interval, double Stretch );
        
        public:
          void updateLimits( const numberLimits &L );
          void set( const numberLimits &L ) { Limits = L; }
          void applyStretch( double Stretch );

          numberLimits get() const { return Limits; }
      };

      class limitsXY 
      {
        private:
          typedef std::map< const axisSet*, limits > limitsMap;
          limitsMap Limits;

        public:
          void updateLimits( const axisSet* AxisSet, const numberLimits &L );
          void applyStretch( double StretchX, double StretchY ); 
         
          void set( const axisSet* AxisSet, const numberLimits &L );
          numberLimits get( const axisSet* AxisSet ) const; 
          numberLimits getX() const; 
          numberLimits getY() const; 

          void setAxisNumberLimits( axisSet* AxisSet ) const;
          
          static bool isAxisSetX( const axisSet *Axis );
          static bool isAxisSetY( const axisSet *Axis );

          static numberLimits defaultAxisNumberLimits();
      };

    private:
      typedef std::map< const axisSet*, interval<number> > forcedIntervalsMap;
      forcedIntervalsMap ForcedIntervals;
      double StretchFactorX, StretchFactorY;

      static numberLimits applyForcedLimitsMinMax( const numberLimits &Limits, interval<number> ForcedInterval );
      static interval<number> applyForcedLimitsToSubintervalMinMax( interval<number> LInterval, interval<number> ForcedInterval, int Signum );
      static bool isNumberHaveSignum( number Number, int Signum );

      numberLimits applyForcedLimits( const axisSet *AxisSet, const numberLimits &Limits ) const;
      numberLimits correctInvalidLimits( const numberLimits &Limits ) const;

      void updateLimitsXYForGraphic( limitsXY *Limits, const graph &Graph, const graphCollection &GraphCollection ) const;
      void correctLimitsXYForAxisSet( limitsXY *Limits, const axisSet *AxisSet ) const;

    public:
      plotLimits();
      virtual ~plotLimits() {}

      void setInterval( const axisSet *AxisSet, interval<number> I ) { ForcedIntervals[AxisSet] = I; }
      interval<number> getInterval( const axisSet *AxisSet ) const;
      
      void setStretchFactor( double S )  { StretchFactorX = StretchFactorY = S; }
      void setStretchFactorX( double S ) { StretchFactorX = S; }
      void setStretchFactorY( double S ) { StretchFactorY = S; }
      double stretchFactorX() const { return StretchFactorX; }
      double stretchFactorY() const { return StretchFactorY; }

      limitsXY limitsForGraphics( const graphCollection& Graphics ) const;
  };

// ============================================================

}

