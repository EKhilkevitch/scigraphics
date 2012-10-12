
// =========================================================

#include "test_painter.h"
#include "test_legend.h"
#include "graphics/legend.h"
#include "graphics/axisset.h"
#include "graphics/color.h"
#include "graphics/graphsequence.h"

// =========================================================

CPPUNIT_TEST_SUITE_REGISTRATION( graphics::tests::test_legend );

// =========================================================
        
void graphics::tests::test_legend::setUp()
{
  auto Drawer = new mockDrawer();
  Drawer->setWidth(400);
  Drawer->setHeight(300);
  Painter.setDrawer( Drawer );
  Painter.update();
}

// ---------------------------------------------------------

void graphics::tests::test_legend::shouldDrawGraphLegend()
{
  graphSequenceVector Graph;

  CPPUNIT_ASSERT( ! legendMorozov::shouldDrawGraphLegend(Graph) );

  Graph.setLegend("x");
  CPPUNIT_ASSERT( legendMorozov::shouldDrawGraphLegend(Graph) );

  Graph.setShowLegend(false);
  CPPUNIT_ASSERT( ! legendMorozov::shouldDrawGraphLegend(Graph) );
}

// ---------------------------------------------------------

void graphics::tests::test_legend::legendsList()
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
  CPPUNIT_ASSERT_EQUAL( (size_t)2, Legends.size() );
  CPPUNIT_ASSERT_EQUAL( std::string("1"), Legends.front() );
  CPPUNIT_ASSERT_EQUAL( std::string("3"), Legends.back() );
}

// ---------------------------------------------------------

void graphics::tests::test_legend::sizesForLegendRectangle()
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

  CPPUNIT_ASSERT_EQUAL( ExpectedWidth,  Size.width()  );
  CPPUNIT_ASSERT_EQUAL( ExpectedHeight, Size.height() );

  Graphics.create<graphSequenceVector>("1");
  Graphics.create<graphSequenceVector>("222");
  Graphics.create<graphSequenceVector>("33");

  Size = legendMorozov::sizesForLegendRectangle( Painter, TextStyle, Graphics );

  ExpectedWidth  = 2*3*TextStyle.getFontSize();
  ExpectedHeight = 4*legendMorozov::interTextVerticalDistance(TextStyle) + 3*TextStyle.getFontSize();
}

// ---------------------------------------------------------

void graphics::tests::test_legend::updateLegendRectangleShortList()
{
  graphCollection Graphics;
  axisSetX SetX(0);
  axisSetY SetY(0);
  Graphics.setDefaultAxisSets( &SetX, &SetY );
 
  Graphics.create<graphSequenceVector>("11");

  legendMorozov Legend;
  CPPUNIT_ASSERT_EQUAL( 0, Legend.getRectangle().width() );
  CPPUNIT_ASSERT_EQUAL( 0, Legend.getRectangle().height() );
  CPPUNIT_ASSERT_EQUAL( 0, Legend.getRectangle().up() );
  CPPUNIT_ASSERT_EQUAL( 0, Legend.getRectangle().left() );
  
  auto FontSize = Legend.getLegendTextStyle().getFontSize();
  CPPUNIT_ASSERT_EQUAL( 12U, FontSize );

  auto TextStyle = Legend.updateLegendRectangle( Painter, Graphics );
  CPPUNIT_ASSERT_EQUAL( FontSize, TextStyle.getFontSize() );
  CPPUNIT_ASSERT_EQUAL( FontSize, Legend.getLegendTextStyle().getFontSize() );
  CPPUNIT_ASSERT_EQUAL( 0U, TextStyle.getColor().valueRgb() );

  int ExpectedWidth  = Graphics.front()->legendExampleWidth() + Legend.textHorizontalIndent()*3 + FontSize*Graphics.front()->legend().size();
  int ExpectedHeight = 2*legendMorozov::interTextVerticalDistance(TextStyle) + FontSize;

  CPPUNIT_ASSERT_EQUAL( 5,  legendMorozov::textHorizontalIndent() );
  CPPUNIT_ASSERT_EQUAL( 4,  legendMorozov::interTextVerticalDistance(TextStyle) );
  CPPUNIT_ASSERT_EQUAL( ExpectedWidth,  Legend.getRectangle().width() );
  CPPUNIT_ASSERT_EQUAL( ExpectedHeight, Legend.getRectangle().height() );

  CPPUNIT_ASSERT_EQUAL( 400-ExpectedWidth, Legend.getRectangle().left() );
  CPPUNIT_ASSERT_EQUAL( 90, Legend.getRectangle().up() );
}

// ---------------------------------------------------------

void graphics::tests::test_legend::updateLegendRectangleLongList()
{
  graphCollection Graphics;
  axisSetX SetX(0);
  axisSetY SetY(0);
  Graphics.setDefaultAxisSets( &SetX, &SetY );
  for ( unsigned i = 0; i < 25; i++ )
    Graphics.create<graphSequenceVector>( "xxx" );

  legendMorozov Legend;
  auto TextStyle = Legend.updateLegendRectangle( Painter, Graphics );

  CPPUNIT_ASSERT_EQUAL( 8U,  TextStyle.getFontSize() );
  CPPUNIT_ASSERT_EQUAL( 12U, Legend.getLegendTextStyle().getFontSize() );
  
  int ExpectedWidth  = Graphics.front()->legendExampleWidth() + Legend.textHorizontalIndent()*3 + TextStyle.getFontSize()*Graphics.front()->legend().size();
  int ExpectedHeight = (Graphics.size()+1)*legendMorozov::interTextVerticalDistance(TextStyle) + TextStyle.getFontSize()*Graphics.size();
  
  CPPUNIT_ASSERT_EQUAL( ExpectedWidth,  Legend.getRectangle().width() );
  CPPUNIT_ASSERT_EQUAL( ExpectedHeight, Legend.getRectangle().height() );
  
  CPPUNIT_ASSERT_EQUAL( ExpectedHeight, Legend.getRectangle().up() );
  CPPUNIT_ASSERT_EQUAL( 400-ExpectedWidth, Legend.getRectangle().left() );
}

// ---------------------------------------------------------

void graphics::tests::test_legend::updateLegendRectangleVeryLongList()
{
  graphCollection Graphics;
  axisSetX SetX(0);
  axisSetY SetY(0);
  Graphics.setDefaultAxisSets( &SetX, &SetY );
  for ( unsigned i = 0; i < 100; i++ )
    Graphics.create<graphSequenceVector>( "xxx" );

  legendMorozov Legend;
  auto TextStyle = Legend.updateLegendRectangle( Painter, Graphics );
  
  CPPUNIT_ASSERT_EQUAL( Legend.minFontSize(), TextStyle.getFontSize() );
  CPPUNIT_ASSERT( Legend.minFontSize() > 0 );
  CPPUNIT_ASSERT( Legend.minFontSize() < 20 );
}

// =========================================================


