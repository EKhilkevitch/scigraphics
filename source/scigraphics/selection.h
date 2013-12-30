
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

#include "scigraphics/selectionstyle.h"
#include "scigraphics/plotelement.h"
#include "scigraphics/geometry.h"

#include <limits>

namespace scigraphics 
{

// ============================================================

  class pairScales;

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
      const interval<number> stripInterval() const { return Interval; }
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
      
      number minF( const pairScales &Scales ) const;
      number maxF( const pairScales &Scales ) const;
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
      
      number minF( const pairScales &Scales ) const;
      number maxF( const pairScales &Scales ) const;
  };

// ============================================================

}

