
// =========================================================

#include <algorithm>
#include <cstring>
#include <cmath>

#include <gtest/gtest.h>

#define private public
#define protected public


#include "scigraphics/datasequence.h"
#include "scigraphics/datamap.h"

namespace scigraphics {};
using namespace scigraphics;

// =========================================================

class test_dataSequenceVector : public testing::Test
{
};

// =========================================================

class test_dataMap : public testing::Test
{
};

// =========================================================

TEST_F( test_dataSequenceVector, point )
{
  typedef dataSequence::point point;

  point Point;
  ASSERT_NEAR( 0, Point.x(), 1e-5 );
  ASSERT_NEAR( 0, Point.y(), 1e-5 );
  ASSERT_NEAR( 0, Point.errX(), 1e-5 );
  ASSERT_NEAR( 0, Point.errY(), 1e-5 );

  ASSERT_TRUE( Point.isValid() );
  ASSERT_TRUE( Point.isValidError() );

  Point = point( invalidNumber(), 0 );
  ASSERT_TRUE( ! Point.isValid() );
}

// ---------------------------------------------------------

TEST_F( test_dataSequenceVector, append )
{
  dataSequenceVector Data;
  
  ASSERT_TRUE( Data.empty() );
  ASSERT_EQ( size_t(0), Data.size() );

  Data.append( 1.2, 2.3 );
  ASSERT_EQ( size_t(1), Data.size() );
  ASSERT_NEAR( 1.2, Data[0].x(), 1e-5 );
  ASSERT_NEAR( 2.3, Data[0].y(), 1e-5 );
  ASSERT_NEAR( 0, Data[0].errX(), 1e-5 );
  ASSERT_NEAR( 0, Data[0].errY(), 1e-5 );
  
  Data.append( 1, 2, 3 );
  ASSERT_EQ( size_t(2), Data.size() );
  ASSERT_NEAR( 0, Data[1].errX(), 1e-5 );
  ASSERT_NEAR( 3, Data[1].errY(), 1e-5 );
  
  Data.append( 1, 2, 3, 4 );
  ASSERT_EQ( size_t(3), Data.size() );
  ASSERT_NEAR( 1, Data[2].x(), 1e-5 );
  ASSERT_NEAR( 2, Data[2].y(), 1e-5 );
  ASSERT_NEAR( 3, Data[2].errX(), 1e-5 );
  ASSERT_NEAR( 4, Data[2].errY(), 1e-5 );
  
  Data.append( 2, invalidNumber(), 3 );
  ASSERT_EQ( size_t(4), Data.size() );
  ASSERT_EQ( false, Data[3].isValid() );
  ASSERT_NEAR( 2, Data[3].x(), 1e-5 );
  ASSERT_EQ( invalidNumber(), Data[3].y() );
}

// ---------------------------------------------------------

TEST_F( test_dataSequenceVector, iterator )
{
  dataSequenceVector Data;
  ASSERT_TRUE( Data.begin() == Data.end() );
  
  Data.append( 1, 1 );
  Data.append( 2, 4 );
  Data.append( 3, 9 );
  
  auto Iterator = Data.begin();
  ASSERT_TRUE( Iterator != Data.end() );
  ASSERT_NEAR( 1, Iterator->x(), 1e-5 );

  ++Iterator;
  ASSERT_TRUE( Iterator != Data.end() );
  ASSERT_NEAR( 2, Iterator->x(), 1e-5 );

  Iterator++;
  ASSERT_TRUE( Iterator != Data.end() );
  ASSERT_NEAR( 3, Iterator->x(), 1e-5 );

  ++Iterator;
  ASSERT_TRUE( Iterator == Data.end() ); 
}

// ---------------------------------------------------------

TEST_F( test_dataSequenceVector, limitsX )
{
  dataSequenceVector Data;
  ASSERT_TRUE( ! Data.limitsX().isValid() );

  Data.append(   -1, 2 );
  ASSERT_NEAR( -1, Data.limitsX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -1, Data.limitsX().totalLimits().max(), 1e-5 );

  Data.append(  -0.5, 2 );
  Data.append(     0, 2 );
  Data.append(   0.5, 2 );
  Data.append(   1.0, 2 );
  
  ASSERT_NEAR( -1,   Data.limitsX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( +1,   Data.limitsX().totalLimits().max(), 1e-5 );
  ASSERT_NEAR( -1,   Data.limitsX().negativeLimits().min(), 1e-5 );
  ASSERT_NEAR( -0.5, Data.limitsX().negativeLimits().max(), 1e-5 );
  ASSERT_NEAR( 0.5,  Data.limitsX().positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( 1.0,  Data.limitsX().positiveLimits().max(), 1e-5 );

  Data.clear();
  Data.append( 1, 2 );
  ASSERT_NEAR( 1,   Data.limitsX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( 1,   Data.limitsX().totalLimits().max(), 1e-5 );
  
  Data.append( 1, 3 );
  ASSERT_NEAR( 1,   Data.limitsX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( 1,   Data.limitsX().totalLimits().max(), 1e-5 );
  
  Data.append( 1, 5 );
  ASSERT_NEAR( 1,   Data.limitsX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( 1,   Data.limitsX().totalLimits().max(), 1e-5 );
  
  Data.append( 7, invalidNumber() );
  ASSERT_NEAR( 1,   Data.limitsX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( 1,   Data.limitsX().totalLimits().max(), 1e-5 );
  
  Data.append( 1, 5, 1, 0 );
  ASSERT_NEAR(  0,   Data.limitsX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( +2,   Data.limitsX().totalLimits().max(), 1e-5 );
  
  Data.append( 1, 5, 2, 0 );
  ASSERT_NEAR( -1,   Data.limitsX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( +3,   Data.limitsX().totalLimits().max(), 1e-5 ); 
  ASSERT_NEAR( 1.0,  Data.limitsX().positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( 3.0,  Data.limitsX().positiveLimits().max(), 1e-5 );

  Data.append( 0.5, 5 );
  Data.append( 1e-30, 5 );
  ASSERT_NEAR( 0.5,  Data.limitsX().positiveLimits().min(), 1e-5 );
  ASSERT_NEAR( 3.0,  Data.limitsX().positiveLimits().max(), 1e-5 );
  ASSERT_NEAR( -1.0, Data.limitsX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( 3.0,  Data.limitsX().totalLimits().max(), 1e-5 ); 

  Data.clear();
  Data.append( 0.5,   5 );
  Data.append( 0.25,  5 );
  Data.append( 1e-30, 5 );
  ASSERT_NEAR( 0.0, Data.limitsX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( 0.5, Data.limitsX().totalLimits().max(), 1e-5 ); 

  Data.append( 0.3, 2 );
  Data.append( -1, 3 );
  ASSERT_NEAR( -1,  Data.limitsX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( 0.5, Data.limitsX().totalLimits().max(), 1e-5 ); 
  
  Data.append( -3, 5 );
  Data.append( -2, 10 );
  ASSERT_NEAR( -3,  Data.limitsX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( 0.5, Data.limitsX().totalLimits().max(), 1e-5 ); 
}

// ---------------------------------------------------------

TEST_F( test_dataSequenceVector, limitsY )
{
  dataSequenceVector Data;
  ASSERT_TRUE( ! Data.limitsY( interval<number>(0,1) ).isValid() );
  
  Data.append(   -1,   -1 );
  ASSERT_TRUE( ! Data.limitsY( interval<number>(0,1) ).isValid() );
  ASSERT_NEAR( -1, Data.limitsY( interval<number>(-2,2) ).totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -1, Data.limitsY( interval<number>(-2,2) ).totalLimits().max(), 1e-5 );

  Data.append( -0.5, -0.5 );
  ASSERT_NEAR( -1,   Data.limitsY( interval<number>(-2,2) ).totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -0.5, Data.limitsY( interval<number>(-2,2) ).totalLimits().max(), 1e-5 );
  ASSERT_NEAR( -0.5, Data.limitsY( interval<number>(-0.3,-0.7) ).totalLimits().max(), 1e-5 );
  ASSERT_NEAR( -0.5, Data.limitsY( interval<number>(-0.3,-0.7) ).totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -1,   Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -0.5, Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().max(), 1e-5 );
  ASSERT_NEAR( -1,   Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().min(), 1e-5 );
  ASSERT_NEAR( -0.5, Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().max(), 1e-5 );

  Data.clear();
  Data.append( -1, 2 );
  Data.append( -1e-50, 1 );
  Data.append( 1, 2 );
  Data.append( 2, 4 );
  Data.append( 5, 5 );
  Data.append( 2, 3 );
  
  ASSERT_NEAR(  1, Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().min(), 1e-5 );
  ASSERT_NEAR(  5, Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().max(), 1e-5 );
  ASSERT_NEAR(  1, Data.limitsY( Data.limitsX().totalLimits() ).positiveLimits().min(), 1e-5 );
  ASSERT_NEAR(  5, Data.limitsY( Data.limitsX().totalLimits() ).positiveLimits().max(), 1e-5 );
  ASSERT_EQ( invalidNumber(), Data.limitsY( Data.limitsX().totalLimits() ).negativeLimits().min() );
  ASSERT_EQ( invalidNumber(), Data.limitsY( Data.limitsX().totalLimits() ).negativeLimits().max() );
  ASSERT_NEAR(  2, Data.limitsY( interval<number>(0,2) ).totalLimits().min(), 1e-5 );
  ASSERT_NEAR(  4, Data.limitsY( interval<number>(0,2) ).totalLimits().max(), 1e-5 );
  ASSERT_TRUE( ! Data.limitsY( interval<number>(invalidNumber(),invalidNumber()) ).isValid() );
  
  Data.append( invalidNumber(), 50 );
  ASSERT_NEAR(  1, Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().min(), 1e-5 );
  ASSERT_NEAR(  5, Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().max(), 1e-5 );
  
  Data.append( 5, 5, 2 );
  ASSERT_NEAR(  1, Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().min(), 1e-5 );
  ASSERT_NEAR(  7, Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().max(), 1e-5 );
  
  Data.append( 5, 1, 1 );
  ASSERT_NEAR(  0, Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().min(), 1e-5 );
  ASSERT_NEAR(  7, Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().max(), 1e-5 );
  ASSERT_NEAR(  1, Data.limitsY( Data.limitsX().totalLimits() ).positiveLimits().min(), 1e-5 );
  ASSERT_NEAR(  7, Data.limitsY( Data.limitsX().totalLimits() ).positiveLimits().max(), 1e-5 );
  
  Data.append( 5, 1e-30 );
  ASSERT_NEAR(  1, Data.limitsY( Data.limitsX().totalLimits() ).positiveLimits().min(), 1e-5 );
  ASSERT_NEAR(  7, Data.limitsY( Data.limitsX().totalLimits() ).positiveLimits().max(), 1e-5 );

}

// ---------------------------------------------------------

TEST_F( test_dataSequenceVector, clear )
{
  dataSequenceVector Data;
  ASSERT_TRUE( Data.empty() );
  
  Data.append( 1, 2 );
  Data.append( 2, 2 );

  ASSERT_TRUE( ! Data.empty() );

  Data.clear();
  ASSERT_TRUE( Data.empty() );
  ASSERT_EQ( invalidNumber(), Data.limitsX().totalLimits().min() );
  ASSERT_EQ( invalidNumber(), Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().min() );
  ASSERT_EQ( invalidNumber(), Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().min() );
  ASSERT_EQ( invalidNumber(), Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().max() );
}

// ---------------------------------------------------------

TEST_F( test_dataSequenceVector, isOrderedByX )
{
  dataSequenceVector Data;

  ASSERT_EQ( true, Data.isOrderedByX() );
  
  Data.append( 10, 2 );
  ASSERT_EQ( true, Data.isOrderedByX() );
  
  Data.append( 11, 3 );
  ASSERT_EQ( true, Data.isOrderedByX() );

  Data.append( 12, 4 );
  ASSERT_EQ( true, Data.isOrderedByX() );
   
  Data.append( 12, 1 );
  ASSERT_EQ( true, Data.isOrderedByX() );
  
  Data.append( 13, invalidNumber() );
  ASSERT_EQ( true, Data.isOrderedByX() );

  Data.append( 14, 2 );
  ASSERT_EQ( true, Data.isOrderedByX() );
  
  Data.append( 11, 1 );
  ASSERT_EQ( false, Data.isOrderedByX() );
  
  Data.append( 15, 1 );
  ASSERT_EQ( false, Data.isOrderedByX() );

  Data.clear();
  ASSERT_EQ( true, Data.isOrderedByX() );
  
  Data.append( 4, 2 );
  ASSERT_EQ( true, Data.isOrderedByX() );
  
  Data.append( invalidNumber(), 2 );
  ASSERT_EQ( false, Data.isOrderedByX() );
}

// ---------------------------------------------------------

TEST_F( test_dataMap, limits )
{
  dataMapVector Data( 10, interval<number>( 1, 5 ), 5, interval<number>(-10,-4) );

  ASSERT_EQ( (size_t)50, Data.size() );
  ASSERT_EQ( (size_t)10, Data.sizeX() );
  ASSERT_EQ( (size_t)5,  Data.sizeY() );
  
  ASSERT_EQ( 0,  Data.index(0,0) );
  ASSERT_EQ( 9, Data.indexX(49) );
  ASSERT_EQ( 4, Data.indexY(49) );
  ASSERT_EQ( 49, Data.index(9,4) );
  ASSERT_EQ( 7, Data.indexX(17) );
  ASSERT_EQ( 1, Data.indexY(17) );
  ASSERT_EQ( 17, Data.index(7,1) );
  ASSERT_NEAR( 0.4, Data.deltaX(), 1e-5 );
  ASSERT_NEAR( 1.2, Data.deltaY(), 1e-5 );
  ASSERT_NEAR( 3.8, Data.coordinateX(Data.indexX(17)), 1e-5 );
  ASSERT_NEAR( -8.8, Data.coordinateY(Data.indexY(17)), 1e-5 );

  ASSERT_NEAR( 1, Data.limitsX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( 5, Data.limitsX().totalLimits().max(), 1e-5 );
  ASSERT_NEAR( -10, Data.limitsY(Data.limitsX().totalLimits()).totalLimits().min(), 1e-5 );
  ASSERT_NEAR(  -4, Data.limitsY(Data.limitsX().totalLimits()).totalLimits().max(), 1e-5 );
  Data.setIntervalX( -1, 1 );
  ASSERT_NEAR( -1, Data.limitsX().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( +1, Data.limitsX().totalLimits().max(), 1e-5 );

  Data.set( 2, 3, 0.5 );
  Data.set( 2, 4, 0.7 );
  ASSERT_NEAR( 0.5, Data.limitsZ().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( 0.7, Data.limitsZ().totalLimits().max(), 1e-5 );
  Data.set( 2, 4, 0.2 );
  ASSERT_NEAR( 0.2, Data.limitsZ().totalLimits().min(), 1e-5 );
  ASSERT_NEAR( 0.5, Data.limitsZ().totalLimits().max(), 1e-5 );

}

// ---------------------------------------------------------

TEST_F( test_dataMap, set )
{
  dataMapVector Data( 10, interval<number>( 1, 5 ), 5, interval<number>(-10,-4) );
 
  Data.set( 3, 2, 0.5, 0.1 );
  auto Point = Data.at( 3, 2 );
  ASSERT_NEAR( 0.5, Point.z(), 1e-5 );
  ASSERT_NEAR( 0.1, Point.errZ(), 1e-5 );
}

// ---------------------------------------------------------

TEST_F( test_dataMap, resize )
{
  dataMapVector Data;

  Data.setIntervalX(  0,  10 );
  Data.setIntervalY( -5, -25 );
  Data.resize( 20, 10 );

  ASSERT_EQ( (size_t)20, Data.sizeX() );
  ASSERT_EQ( (size_t)10, Data.sizeY() );
  ASSERT_EQ( (size_t)200, Data.size() );
  
  Data.set( 14, 8, 0.5, 0.1 );
  ASSERT_NEAR( 0.5, Data.at(14,8).z(), 1e-5 );

  Data.resize( 25, 15 );
  ASSERT_NEAR( 0.5, Data.at(14,8).z(), 1e-5 );
  
  Data.resize( 5, 2 );
  ASSERT_EQ( (size_t)5, Data.sizeX() );
  ASSERT_EQ( (size_t)2, Data.sizeY() );
  ASSERT_EQ( (size_t)10, Data.size() );
}

// =========================================================

