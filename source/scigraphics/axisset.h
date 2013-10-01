
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

#include "scigraphics/axis.h"
#include "scigraphics/axislabels.h"
#include "scigraphics/axisticks.h"
#include "scigraphics/axistitle.h"
#include "scigraphics/grid.h"
#include "scigraphics/scale.h"
#include "scigraphics/painter.h"
#include "scigraphics/container_ptr.h"
#include "scigraphics/plotlimits.h"

#include <vector>
#include <algorithm>
#include <functional>

// ============================================================

namespace scigraphics
{

// ============================================================

  class axisSet
  {
    public:
      enum direction
      {
        DirectionX,
        DirectionY
      };

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
      axisSet( axis *A, axisTicks *T, axisLabels *L, axisTitle *H, grid *G, scale *S );
      virtual ~axisSet();

      void setVisible( bool V ) { Visible = V; }
      bool isVisible() const { return Visible; }

      scale* getScale() { return Scale; }
      const scale* getScale() const { return Scale; }
      void replaceScale( scale *S );

      void setNumberLimits( numberLimits L ) { Scale->setNumberLimits(L); }

      void setAxisTitle( const std::string &T ) { AxisTitle->setTitle(T); }
      std::string getAxisTitle() const { return AxisTitle->title(); }

      virtual wcoord requiredIndent( bool Used ) const = 0;

      void drawAxis( painter &Painter );
      void drawAxisLabels( painter &Painter );
      void drawAxisTicks( painter &Painter );
      void drawAxisTitle( painter &Painter );
      void drawGrid( painter &Painter );

      virtual direction getDirection() const = 0;

      wcoord wcoordDimension( const painter &Painter ) const;
      double wpointsPerNumber( const painter &Painter ) const;
  };

  // ============================================================
  
  class axisSetX : public axisSet
  {
    public:
      axisSetX( fcoord BaseY );
      direction getDirection() const { return DirectionX; }
      wcoord requiredIndent( bool Used ) const;
  };
  
  class axisSetY : public axisSet
  {
    public:
      axisSetY( fcoord BaseX );
      direction getDirection() const { return DirectionY; }
      wcoord requiredIndent( bool Used ) const;
  };
  
  // ============================================================
  
  class axisSetCollection 
  {
    public:
      enum axisPosition
      {
        Left,
        Right,
        Top,
        Bottom,
        PositionsCount
      };
      
    private:

      typedef container_ptr< std::vector, axisSet > container;
      typedef container::iterator axis_iterator;
      typedef container::const_iterator axis_const_iterator;

      container_ptr< std::vector, axisSet > AxisSets;
      bool KeepScales1x1;

    private:
      double minWPointsPerNPoints( const painter &Painter ) const;

    public:
      axisSetCollection();
      virtual ~axisSetCollection();

      axisSet& at( axisPosition Position );
      const axisSet& at( axisPosition Position ) const;

      axisSet& operator[]( axisPosition Position ) { return at(Position); }
      const axisSet& operator[]( axisPosition Position ) const { return at(Position); }

      pairScales getBottomLeftPairScales();

      void setAxisNumberLimits( plotLimits::limitsXY *Limits );

      void drawGrid( painter &Painter );
      void drawAxis( painter &Painter );
      void drawAxisTicks( painter &Painter );
      void drawAxisLabels( painter &Painter );
      void drawAxisTitles( painter &Painter );

      void applyScalesChanging( double Value, axisSet::direction Direction, void (*Operation)( scale *Scale, double Value ) );
      void addScalesShift( double Shift, axisSet::direction Direction );
      void mulScalesZoom( double Zoom, axisSet::direction Direction );
      void resetScales( axisSet::direction Direction );
      void resetAllScales();

      void setScalesTo1x1( const painter &Painter );
      bool keepScales1x1() const { return KeepScales1x1; }
      void setFixedScalesTo1x1( bool K ) { KeepScales1x1 = K; }
      void setScalesTo1x1ifNeeded( const painter &Painter );
  };
  
  // ============================================================
}


