
#pragma once

// ============================================================

#include "graphics/axis.h"
#include "graphics/axislabels.h"
#include "graphics/axisticks.h"
#include "graphics/axistitle.h"
#include "graphics/grid.h"
#include "graphics/scale.h"
#include "graphics/painter.h"
#include "graphics/container_ptr.h"
#include "graphics/plotlimits.h"

#include <vector>
#include <algorithm>
#include <functional>

// ============================================================

namespace graphics
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

    private:
      double minWPointsPerNPoints( painter &Painter ) const;

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
      void resetScalesTo1x1( painter &Painter );
  };
  
  // ============================================================
}


