
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

// ============================================================

#include "scigraphics/container_ptr.h"
#include "scigraphics/plotlimits.h"
#include "scigraphics/axisposition.h"

#include <string>

// ============================================================

namespace scigraphics
{

// ============================================================

  class painter;
  class grid;
  class scale;
  class pairScales;
  class numberStyle;
  class axis;
  class axisTicks;
  class axisLabels;
  class axisTitle;

// ============================================================

  class axisSet
  {
    public:

    private:
      bool Visible;

    protected:
      axis *Axis;
      axisTicks *AxisTicks;
      axisLabels *AxisLabels;
      axisTitle *AxisTitle;
      grid *Grid;
      scale *Scale;

    private:
      axisSet( const axisSet& );
      axisSet& operator=( const axisSet& );

    public:
      axisSet( axis *Axis, axisTicks *Ticks, axisLabels *Labels, axisTitle *Title, grid *Grid, scale *Scale );
      virtual ~axisSet();

      void setVisible( bool Visible );
      bool isVisible() const;

      scale* getScale();
      const scale* getScale() const;
      void replaceScale( scale *Scale );

      void setNumberLimits( const numberLimits &Limits );

      void setAxisTitle( const std::string &Title );
      std::string getAxisTitle() const;

      void setNumberStyle( const numberStyle &Style );
      const numberStyle& getNumberStyle();

      virtual wcoord requiredIndent( bool Used ) const = 0;

      void drawAxis( painter &Painter ) const;
      void drawAxisLabels( painter &Painter ) const;
      void drawAxisTicks( painter &Painter ) const;
      void drawAxisTitle( painter &Painter ) const;
      void drawGrid( painter &Painter ) const;

      virtual axisDirection getDirection() const = 0;

      wcoord wcoordDimension( const painter &Painter ) const;
      double wpointsPerNumber( const painter &Painter ) const;
  };

  // ============================================================
  
  class axisSetX : public axisSet
  {
    public:
      explicit axisSetX( fcoord BaseY );
      axisDirection getDirection() const;
      wcoord requiredIndent( bool Used ) const;
  };
  
  class axisSetY : public axisSet
  {
    public:
      explicit axisSetY( fcoord BaseX );
      axisDirection getDirection() const;
      wcoord requiredIndent( bool Used ) const;
  };
  
  // ============================================================
  
  class axisSetCollection 
  {
    private:
      typedef container_ptr< axisSet > container;
      typedef container::iterator axis_iterator;
      typedef container::const_iterator axis_const_iterator;

    private:
      container AxisSets;
      bool KeepScales1x1;

    private:
      double minWPointsPerNPoints( const painter &Painter ) const;
      
      static void addScaleShift( scale *Scale, double Shift );
      static void setScaleShift( scale *Scale, double Shift );
      static double scaleShift( const scale *Scale );
      static void mulScaleZoom( scale *Scale, double Zoom );
      static void setScaleZoom( scale *Scale, double Zoom );
      static double scaleZoom( const scale *Scale );
      static void resetScale( scale *Scale, double Junk );
      
      void applyScaleChanging( double Value, axisDirection Direction, void (*Operation)( scale *Scale, double Value ) );
      double getScaleValue( axisDirection Direction, double (*Operation)( const scale *Scale ) ) const;

    public:
      axisSetCollection();
      ~axisSetCollection();

      axisSet& at( axisPosition Position );
      const axisSet& at( axisPosition Position ) const;

      axisSet& operator[]( axisPosition Position );
      const axisSet& operator[]( axisPosition Position ) const;

      pairScales getBottomLeftPairScales();
      void setAxisNumberLimits( plotLimits::limitsXY *Limits );

      void drawGrid( painter &Painter );
      void drawAxis( painter &Painter );
      void drawAxisTicks( painter &Painter );
      void drawAxisLabels( painter &Painter );
      void drawAxisTitles( painter &Painter );

      void addScaleShift( double Shift, axisDirection Direction );
      void setScaleShift( double Shift, axisDirection Direction );
      double scaleShift( axisDirection Direction ) const;
      void mulScaleZoom( double Zoom, axisDirection Direction );
      void setScaleZoom( double Zoom, axisDirection Direction );
      double scaleZoom( axisDirection Direction ) const;
      void resetScales( axisDirection Direction );
      void resetAllScales();

      void setScalesTo1x1( const painter &Painter );
      bool keepScales1x1() const { return KeepScales1x1; }
      void setFixedScalesTo1x1( bool K ) { KeepScales1x1 = K; }
      void setScalesTo1x1ifNeeded( const painter &Painter );
  };
  
  // ============================================================
}


