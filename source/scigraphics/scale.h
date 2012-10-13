
#pragma once

// ============================================================

#include "graphics/numbers.h"
#include "graphics/numlimits.h"
#include "graphics/geometry.h"
#include "graphics/marker.h"

// ============================================================

namespace scigraphics
{

  class painter;

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
      
      void setMarker( marker *M );
      const marker* getMarker() const { return Marker; }
      marker* getMarker() { return Marker; }

    public:
      scale();
      virtual ~scale();

      void setNumberLimits( const numberLimits &N ) { NumberLimits = N; }
      const numberLimits& getNumberLimits() const { return NumberLimits; }
      
      virtual interval<number> getNumberInterval() const { return getNumberLimits().totalLimits(); }
      interval<number> getVisivleInterval() const;

      fcoord numberToFraction( number Number ) const;
      number fractionToNumber( fcoord Coordinate ) const;

      double shift() const { return Shift; }
      void setShift( double S );
     
      double zoom()  const { return Zoom; }
      void setZoom( double Z );

      void reset();

      void setLock( bool L );
      void lock()   { setLock(true);  }
      void unlocK() { setLock(false); }
      bool isLocked() const { return Locked; }

      void setNumberOfMarks( unsigned Min, unsigned Max );
      std::vector<number> marks() const;

      static void addScaleShift( scale *Scale, double Shift );
      static void mulScaleZoom( scale *Scale, double Zoom );
      static void resetScale( scale *Scale, double Junk = 0 );
  };

// ============================================================

  class scaleLinear : public scale
  {
    protected:
      double numberToPartOfDistance( number Number ) const;
      number partOfDistanceToNumber( double Part ) const;
    public:
      scaleLinear() { setMarker( new markerLinear() ); }
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
      scaleLogarithmPositive() { setMarker( new markerLogarithmPositive() ); }
      interval<number> getNumberInterval() const { return getNumberLimits().positiveLimits(); }
  };
  
  class scaleLogarithmNegative : public scaleLogarithm
  {
    protected:
      double numberToPartOfDistance( number Number ) const;
      number partOfDistanceToNumber( double Part ) const;
    public:
      scaleLogarithmNegative() { setMarker( new markerLogarithmNegative() ); }
      interval<number> getNumberInterval() const { return getNumberLimits().negativeLimits(); }
  };

  class scaleSquare : public scale
  {
    protected:
      static number sign( number Val )     { return Val < 0 ? -1 : Val > 0 ? +1 : 0; }
      static number signsqr( number Val )  { return Val*Val*sign(Val); }
      static number signsqrt( number Val );
      
      double numberToPartOfDistance( number Number ) const;
      number partOfDistanceToNumber( double Part ) const;

    public:
      scaleSquare() { setMarker( new markerLinear() ); }
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

