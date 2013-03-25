
// =========================================================

#include <gtest/gtest.h>

#include "scigraphics/axisset.h"
#include "scigraphics/color.h"
#include "scigraphics/graphsequence.h"

#include "mockpainter.h"

#define private public
#define protected public
#include "scigraphics/legend.h"

namespace scigraphics {};
using namespace scigraphics;

// =========================================================

struct test_legend : public testing::Test
{
  struct legendMorozov : public legend { friend class test_legend; };

  painter Painter;
  void SetUp();
};

// =========================================================


// =========================================================

void test_legend::SetUp()
{
  auto Drawer = new mockDrawer();
  Drawer->setWidth(400);
  Drawer->setHeight(300);
  Painter.setDrawer( Drawer );
  Painter.update();
}

// =========================================================

TEST_F( test_legend, shouldDrawGraphLegend )
{
  graphSequenceVector Graph;

  ASSERT_TRUE( ! legendMorozov::shouldDrawGraphLegend(Graph) );

  Graph.setLegend("x");
  ASSERT_TRUE( legendMorozov::shouldDrawGraphLegend(Graph) );

  Graph.setShowLegend(false);
  ASSERT_TRUE( ! legendMorozov::shouldDrawGraphLegend(Graph) );
}

// ---------------------------------------------------------

TEST_F( test_legend, legendsList )
{
  graphCollection Graphics;
  axisSetX SetX(0);
  axisSetY SetY(0);
  Graphics.setDefaultAxisSets( &SetX, &SetY );

  auto Graph = Graphics.create<graphSequenceVector>();
  
  Graph = Graphics.create<graphSequenceVector>();
  Graph->setLegend("1");
  
  Graph = Graphics.create<graphSequenceVector>();
  Graph->setLegend("2");
  Graph->setShowLegend(false);

  Graph = Graphics.create<graphSequenceVector>();
  Graph->setLegend("3");

  auto Legends = legendMorozov::legendsList( Graphics );
  ASSERT_EQ( (size_t)2, Legends.size() );
  ASSERT_EQ( std::string("1"), Legends.front() );
  ASSERT_EQ( std::string("3"), Legends.back() );
}

// ---------------------------------------------------------

TEST_F( test_legend, sizesForLegendRectangle )
{
  textStyle TextStyle;
  TextStyle.setFontSize(10);
  
  graphCollection Graphics;
  axisSetX SetX(0);
  axisSetY SetY(0);
  Graphics.setDefaultAxisSets( &SetX, &SetY );

  auto Size = legendMorozov::sizesForLegendRectangle( Painter, TextStyle, Graphics );

  int ExpectedWidth  = 0;
  int ExpectedHeight = 0; //legendMorozov::interTextVerticalDistance(TextStyle);

  ASSERT_EQ( ExpectedWidth,  Size.width()  );
  ASSERT_EQ( ExpectedHeight, Size.height() );

  Graphics.create<graphSequenceVector>("1",color());
  Graphics.create<graphSequenceVector>("222",color());
  Graphics.create<graphSequenceVector>("33",color());

  Size = legendMorozov::sizesForLegendRectangle( Painter, TextStyle, Graphics );

  ExpectedWidth  = 2*3*TextStyle.getFontSize();
  ExpectedHeight = 4*legendMorozov::interTextVerticalDistance(TextStyle) + 3*TextStyle.getFontSize();
}

// ---------------------------------------------------------

TEST_F( test_legend, updateLegendRectangleShortList )
{
  graphCollection Graphics;
  axisSetX SetX(0);
  axisSetY SetY(0);
  Graphics.setDefaultAxisSets( &SetX, &SetY );
 
  Graphics.create<graphSequenceVector>("11",color());

  legendMorozov Legend;
  ASSERT_EQ( 0, Legend.getRectangle().width() );
  ASSERT_EQ( 0, Legend.getRectangle().height() );
  ASSERT_EQ( 0, Legend.getRectangle().up() );
  ASSERT_EQ( 0, Legend.getRectangle().left() );
  
  auto FontSize = Legend.getLegendTextStyle().getFontSize();
  ASSERT_EQ( 12U, FontSize );

  auto TextStyle = Legend.updateLegendRectangle( Painter, Graphics );
  ASSERT_EQ( FontSize, TextStyle.getFontSize() );
  ASSERT_EQ( FontSize, Legend.getLegendTextStyle().getFontSize() );
  ASSERT_EQ( 0U, TextStyle.getColor().valueRgb() );

  int ExpectedWidth  = Graphics.front()->legendExampleWidth() + Legend.textHorizontalIndent()*3 + FontSize*Graphics.front()->legend().size();
  int ExpectedHeight = 2*legendMorozov::interTextVerticalDistance(TextStyle) + FontSize;

  ASSERT_EQ( 5,  legendMorozov::textHorizontalIndent() );
  ASSERT_EQ( 4,  legendMorozov::interTextVerticalDistance(TextStyle) );
  ASSERT_EQ( ExpectedWidth,  Legend.getRectangle().width() );
  ASSERT_EQ( ExpectedHeight, Legend.getRectangle().height() );

  ASSERT_EQ( 400-ExpectedWidth, Legend.getRectangle().left() );
  ASSERT_EQ( 90, Legend.getRectangle().up() );
}

// ---------------------------------------------------------

TEST_F( test_legend, updateLegendRectangleLongList )
{
  graphCollection Graphics;
  axisSetX SetX(0);
  axisSetY SetY(0);
  Graphics.setDefaultAxisSets( &SetX, &SetY );
  for ( unsigned i = 0; i < 25; i++ )
    Graphics.create<graphSequenceVector>( "xxx", color() );

  legendMorozov Legend;
  auto TextStyle = Legend.updateLegendRectangle( Painter, Graphics );

  ASSERT_EQ( 8U,  TextStyle.getFontSize() );
  ASSERT_EQ( 12U, Legend.getLegendTextStyle().getFontSize() );
  
  int ExpectedWidth  = Graphics.front()->legendExampleWidth() + Legend.textHorizontalIndent()*3 + TextStyle.getFontSize()*Graphics.front()->legend().size();
  int ExpectedHeight = (Graphics.size()+1)*legendMorozov::interTextVerticalDistance(TextStyle) + TextStyle.getFontSize()*Graphics.size();
  
  ASSERT_EQ( ExpectedWidth,  Legend.getRectangle().width() );
  ASSERT_EQ( ExpectedHeight, Legend.getRectangle().height() );
  
  ASSERT_EQ( ExpectedHeight, Legend.getRectangle().up() );
  ASSERT_EQ( 400-ExpectedWidth, Legend.getRectangle().left() );
}

// ---------------------------------------------------------

TEST_F( test_legend, updateLegendRectangleVeryLongList )
{
  graphCollection Graphics;
  axisSetX SetX(0);
  axisSetY SetY(0);
  Graphics.setDefaultAxisSets( &SetX, &SetY );
  for ( unsigned i = 0; i < 100; i++ )
    Graphics.create<graphSequenceVector>( "xxx", color() );

  legendMorozov Legend;
  auto TextStyle = Legend.updateLegendRectangle( Painter, Graphics );
  
  ASSERT_EQ( Legend.minFontSize(), TextStyle.getFontSize() );
  ASSERT_TRUE( Legend.minFontSize() > 0 );
  ASSERT_TRUE( Legend.minFontSize() < 20 );
}

// =========================================================

