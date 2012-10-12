
#pragma once

#include <cppunit/extensions/HelperMacros.h>

#include "drawer.h"
#include "painter.h"
#include <vector>

// =========================================================

namespace scigraphics
{
  namespace tests
  {

// =========================================================

    class mockDrawer : public drawer
    {
      public:
        struct action
        {
          enum type
          {
            DrawLine,
            DrawRectangle
          } Type;
          color Color;
          std::vector<wpoint> Points;
        };

      private:
        int Width, Height;
        std::vector<action> Actions;

      public:
        mockDrawer() : 
          Width(640), 
          Height(480) {}

        void setWidth( wcoord W )  { Width = W; }
        void setHeight( wcoord H ) { Height = H; }

        void clearActions() { Actions.clear(); }
        size_t numOfActions() const { return Actions.size(); }
        const action& getAction( unsigned i ) { return Actions.at(i); }

        void drawLine( const wpoint &A, const wpoint &B, const lineStyle& Style = lineStyle() );
        void drawRectangle( const wrectangle& Rectangle, const brushStyle& BrushStyle = brushStyle(), const lineStyle &LineStyle = lineStyle() );
        void drawText( const std::string &Text, const wrectangle& Rectangle, const textStyle &Style = textStyle(), double Angle = 0 );
        void drawPolygon( const std::vector<wpoint> &Points, const brushStyle& BrushStyle ) {}
      
        wcoord width() const  { return Width; }
        wcoord height() const { return Height; }
    };

// =========================================================

    class test_painter : public CppUnit::TestFixture
    {
      struct painterMorozov : public painter
      {
        friend class test_painter;
        painterMorozov( drawer *D = nullptr ) : painter(D) {}
      };

        CPPUNIT_TEST_SUITE( test_painter );
        CPPUNIT_TEST( setDrawer );
        CPPUNIT_TEST( setIndents );
        CPPUNIT_TEST( update );
        CPPUNIT_TEST( fpoint2wpoint );
        CPPUNIT_TEST( canSeparatePoints );
        CPPUNIT_TEST( textWidth );
        CPPUNIT_TEST( textHeight );
        CPPUNIT_TEST_SUITE_END();

      public:
        void setDrawer();
        void setIndents();
        void update();
        void fpoint2wpoint();
        void canSeparatePoints();
        void textWidth();
        void textHeight();
        void setUp() {}
        void tearDown() {}
    };

    // =========================================================
    
  }
}



