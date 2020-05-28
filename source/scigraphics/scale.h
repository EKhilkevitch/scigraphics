
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

#include "scigraphics/numbers.h"
#include "scigraphics/numlimits.h"
#include "scigraphics/geometry.h"

#include <vector>

// ============================================================

namespace scigraphics
{

// ============================================================

  class painter;
  class marker;

// ============================================================

  class scale
  {
    private:
      numberLimits NumberLimits;

      double Shift;
      double Zoom;
      bool Locked;

      marker *Marker;

    protected:
      virtual double numberToPartOfDistance( number Number ) const = 0;
      virtual number partOfDistanceToNumber( double Part ) const = 0;
      
      void setMarker( marker *Marker );
      const marker* getMarker() const;
      marker* getMarker();

    public:
      scale();
      virtual ~scale() = 0;

      void setNumberLimits( const numberLimits &NumberLimits );
      const numberLimits& getNumberLimits() const;
      
      virtual interval<number> getNumberInterval() const;
      interval<number> getVisivleInterval() const;

      fcoord numberToFraction( number Number ) const;
      number fractionToNumber( fcoord Coordinate ) const;

      double shift() const;
      void setShift( double Shift );
     
      double zoom()  const;
      void setZoom( double Zoom );

      void reset();

      void setLock( bool Locked );
      void lock();
      void unlocK();
      bool isLocked() const;

      void setNumberOfMarks( unsigned Min, unsigned Max );
      std::vector<number> marks() const;
  };

// ============================================================

  class scaleLinear : public scale
  {
    protected:
      double numberToPartOfDistance( number Number ) const;
      number partOfDistanceToNumber( double Part ) const;

    public:
      scaleLinear();
  };

  class scaleLogarithm : public scale 
  {
    protected:
      static double logDistance( interval<number> Interval );
  };

  class scaleLogarithmPositive : public scaleLogarithm 
  {
    protected:
      double numberToPartOfDistance( number Number ) const;
      number partOfDistanceToNumber( double Part ) const;

    public:
      scaleLogarithmPositive();
      interval<number> getNumberInterval() const;
  };
  
  class scaleLogarithmNegative : public scaleLogarithm
  {
    protected:
      double numberToPartOfDistance( number Number ) const;
      number partOfDistanceToNumber( double Part ) const;

    public:
      scaleLogarithmNegative();
      interval<number> getNumberInterval() const;
  };

  class scaleSquare : public scale
  {
    protected:
      static inline number sign( number Val );
      static inline number signsqr( number Val );
      static inline number signsqrt( number Val );
      
      double numberToPartOfDistance( number Number ) const;
      number partOfDistanceToNumber( double Part ) const;

    public:
      scaleSquare();
  };

// ============================================================

  class pairScales 
  {
    private:
      const scale &ScaleX, &ScaleY;

    public:
      pairScales( const scale &SX, const scale &SY );
      point<fcoord> npoint2fpoint( point<number> Point ) const;
      point<number> fpoint2npoint( point<fcoord> Point ) const;
      point<number> wpoint2npoint( point<wcoord> Point, const painter &Painter ) const;

      fcoord numberToFractionX( number N ) const { return ScaleX.numberToFraction(N); }
      fcoord numberToFractionY( number N ) const { return ScaleY.numberToFraction(N); }

      number fractionToNumberX( fcoord N ) const { return ScaleX.fractionToNumber(N); }
      number fractionToNumberY( fcoord N ) const { return ScaleY.fractionToNumber(N); }
  };

// ============================================================

}

