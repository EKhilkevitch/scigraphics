
#pragma once

#include "graphics/selectionstyle.h"
#include "graphics/scale.h"
#include "graphics/painter.h"
#include "graphics/plotelement.h"

#include <limits>

namespace scigraphics 
{

// ============================================================

  class selection : public plotElement
  {
    private:
      selectionStyle SelectionStyle;
      
    protected:
      virtual npoint firstCorner() const = 0;
      virtual npoint secondCorner() const = 0;

    private:
      fpoint firstCornerF( const pairScales& Scales ) const;
      fpoint secondCornerF( const pairScales& Scales ) const;
      frectangle getRectangleF( const pairScales& Scales ) const; 

    public:
      selection( const selectionStyle &S = selectionStyle() ) : SelectionStyle(S) {}
      virtual ~selection() {}

      void setSelectionStyle( const selectionStyle &S ) { SelectionStyle = S; }
      const selectionStyle& getSelectionStyle() const { return SelectionStyle; }
      
      virtual void draw( painter &Painter, const pairScales& Scales ) const;

      virtual bool pointInSelection( const fpoint &Point, const pairScales& Scales ) const; 
  };

  // ============================================================

  class selectionStrip : public selection
  {
    private:
      interval<number> Interval;

    protected:
      bool inInterval( number N ) const { return Interval.inInterval(N); }

    public:
      selectionStrip( const selectionStyle &S = selectionStyle() ) : 
        selection(S),
        Interval( invalidNumber(), invalidNumber() ) {}

      virtual void setIntervalPoints( const npoint &A, const npoint &B ) = 0;
      void setIntervalPointsF( const fpoint &A, const fpoint &B, const pairScales &Scales );
      virtual void shift( number DeltaX, number DeltaY ) = 0;
      void shiftPointsF( fpoint From, fpoint To, const pairScales &Scales );
      void shiftPoints( npoint From, npoint To );

      void setInterval( number A, number B ) { Interval.setMinMax(A,B); }
      void shiftInterval( number Delta ) { setInterval( min() + Delta, max() + Delta ); }

      virtual fcoord minF( const pairScales &Scales ) const = 0; 
      virtual fcoord maxF( const pairScales &Scales ) const = 0; 

      number min() const { return Interval.min(); }
      number max() const { return Interval.max(); }
  };

  // ============================================================

  class selectionHorizontal : public selectionStrip
  {
    protected:
      npoint firstCorner() const  { return npoint( - std::numeric_limits<number>::max(), min() ); }
      npoint secondCorner() const { return npoint( + std::numeric_limits<number>::max(), max() ); }

    public:
      selectionHorizontal( const selectionStyle &S = selectionStyle() ) : selectionStrip(S) {}

      void setIntervalPoints( const npoint &A, const npoint &B ) { setInterval(A.y(),B.y()); }
      void shift( number , number DeltaY ) { shiftInterval( DeltaY ); }
      
      number minF( const pairScales &Scales ) const { return Scales.numberToFractionY(min()); }
      number maxF( const pairScales &Scales ) const { return Scales.numberToFractionY(max()); }
  };

  // ============================================================

  class selectionVertical : public selectionStrip
  {
    protected:
      fpoint firstCorner() const  { return fpoint( min(), - std::numeric_limits<number>::max() ); }
      fpoint secondCorner() const { return fpoint( max(), + std::numeric_limits<number>::max() ); }

    public:
      selectionVertical( const selectionStyle &S = selectionStyle() ) : selectionStrip(S) {}
      void setIntervalPoints( const npoint &A, const npoint &B ) { setInterval(A.x(),B.x()); }
      void shift( number DeltaX, number ) { shiftInterval( DeltaX ); }
      
      number minF( const pairScales &Scales ) const { return Scales.numberToFractionX(min()); }
      number maxF( const pairScales &Scales ) const { return Scales.numberToFractionX(max()); }
  };

// ============================================================

}

