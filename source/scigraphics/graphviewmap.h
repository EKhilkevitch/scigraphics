
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

#include "scigraphics/datamap.h"
#include "scigraphics/numbers.h"
#include "scigraphics/painter.h"

namespace scigraphics 
{

  class scale;

  namespace map
  {

    // ============================================================

    class graphView 
    {
      private:
        bool Visible;

      public:
        graphView() : Visible(true) {}
        virtual ~graphView() {}

        void setVisible( bool V ) { Visible = V; }
        bool isVisible() const { return Visible; }

        virtual void draw( painter &Painter, const pairScales& Scales, const data &Data, const scale &ScaleZ ) const = 0;
        virtual void drawLegendExample( painter &Painter, const wrectangle &Rectangle, const scale &ScaleZ ) const = 0;
        
        virtual wcoord legendExampleWidth() const  { return 80; }
        virtual wcoord legendExampleHeight() const { return 200; }
    };

    // ============================================================

    class graphViewRectangle : public graphView
    { 
      public:
        class pointColorStrategy
        {
          protected:
            static double relativePointValue( const data::point_t &Point, const scale &Scale );
            static double relativeValueToUnitInterval( double Value );

          public:
            virtual color relativeValueColor( double RelativeValue ) const = 0;
            virtual color invalidValueColor() const;
            virtual color pointColor( const data::point_t &Point, const scale &Scale ) const;
            virtual ~pointColorStrategy() {}
        };

        class grayscalePointColorStrategy : public pointColorStrategy
        {
          public:
            color relativeValueColor( double RelativeValue ) const;
        };

        class redYellowBluePointColorStrategy : public pointColorStrategy
        {
          public:
            color relativeValueColor( double RelativeValue ) const;
        };

        class yellowRedBluePointColorStrategy : public pointColorStrategy
        {
          public:
            color relativeValueColor( double RelativeValue ) const;
        };


      private:
        pointColorStrategy *PointColorStrategy;

      private:
        graphViewRectangle( graphViewRectangle& );
        graphViewRectangle& operator=( graphViewRectangle& );

      protected:
        virtual void drawPoint( painter &Painter, const pairScales& Scales, const data::point_t &Point, const scale &ScaleZ ) const;

        virtual void drawRainbowRectangleBorder( painter &Painter, const wrectangle &Rectangle ) const;
        virtual void drawRainbowRectangle( painter &Painter, const wrectangle &Rectangle ) const;
        virtual void drawRainbowMarkers( painter &Painter, const wrectangle &Rectangle, const scale &ScaleZ ) const;
        wcoord rainbowRectangleWidth() const { return legendExampleWidth()/3; }

      public:
        graphViewRectangle();
        ~graphViewRectangle();

        void setColorStrategy( pointColorStrategy *Strategy );

        void draw( painter &Painter, const pairScales& Scales, const data &Data, const scale &ScaleZ ) const;
        void drawLegendExample( painter &Painter, const wrectangle &Rectangle, const scale &ScaleZ ) const;
    };

  // ============================================================

  }
}

