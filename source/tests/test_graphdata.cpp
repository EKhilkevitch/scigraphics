
// =========================================================

#include "test_graphdata.h"
#include "graphics/datasequence.h"
#include "graphics/datamap.h"

#include <algorithm>
#include <cstring>
#include <memory>
#include <cmath>

// =========================================================

CPPUNIT_TEST_SUITE_REGISTRATION( scigraphics::tests::test_dataSequenceVector );
CPPUNIT_TEST_SUITE_REGISTRATION( scigraphics::tests::test_dataMap );

// =========================================================

void scigraphics::tests::test_dataSequenceVector::point()
{
  typedef dataSequence::point point;

  point Point;
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, Point.x(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, Point.y(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, Point.errX(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, Point.errY(), 1e-5 );

  CPPUNIT_ASSERT( Point.isValid() );
  CPPUNIT_ASSERT( Point.isValidError() );

  Point = point( invalidNumber(), 0 );
  CPPUNIT_ASSERT( ! Point.isValid() );
}

// ---------------------------------------------------------

void scigraphics::tests::test_dataSequenceVector::append()
{
  dataSequenceVector Data;
  
  CPPUNIT_ASSERT( Data.empty() );
  CPPUNIT_ASSERT_EQUAL( size_t(0), Data.size() );

  Data.append( 1.2, 2.3 );
  CPPUNIT_ASSERT_EQUAL( size_t(1), Data.size() );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.2, Data[0].x(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2.3, Data[0].y(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, Data[0].errX(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, Data[0].errY(), 1e-5 );
  
  Data.append( 1, 2, 3 );
  CPPUNIT_ASSERT_EQUAL( size_t(2), Data.size() );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0, Data[1].errX(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 3, Data[1].errY(), 1e-5 );
  
  Data.append( 1, 2, 3, 4 );
  CPPUNIT_ASSERT_EQUAL( size_t(3), Data.size() );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1, Data[2].x(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2, Data[2].y(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 3, Data[2].errX(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 4, Data[2].errY(), 1e-5 );
  
  Data.append( 2, invalidNumber(), 3 );
  CPPUNIT_ASSERT_EQUAL( size_t(4), Data.size() );
  CPPUNIT_ASSERT_EQUAL( false, Data[3].isValid() );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2, Data[3].x(), 1e-5 );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Data[3].y() );
}
        
// ---------------------------------------------------------

void scigraphics::tests::test_dataSequenceVector::iterator()
{
  dataSequenceVector Data;
  CPPUNIT_ASSERT( Data.begin() == Data.end() );
  
  Data.append( 1, 1 );
  Data.append( 2, 4 );
  Data.append( 3, 9 );
  
  auto Iterator = Data.begin();
  CPPUNIT_ASSERT( Iterator != Data.end() );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1, Iterator->x(), 1e-5 );

  ++Iterator;
  CPPUNIT_ASSERT( Iterator != Data.end() );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 2, Iterator->x(), 1e-5 );

  Iterator++;
  CPPUNIT_ASSERT( Iterator != Data.end() );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 3, Iterator->x(), 1e-5 );

  ++Iterator;
  CPPUNIT_ASSERT( Iterator == Data.end() ); 
}

// ---------------------------------------------------------

void scigraphics::tests::test_dataSequenceVector::limitsX()
{
  dataSequenceVector Data;
  CPPUNIT_ASSERT( ! Data.limitsX().isValid() );

  Data.append(   -1, 2 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1, Data.limitsX().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1, Data.limitsX().totalLimits().max(), 1e-5 );

  Data.append(  -0.5, 2 );
  Data.append(     0, 2 );
  Data.append(   0.5, 2 );
  Data.append(   1.0, 2 );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1,   Data.limitsX().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +1,   Data.limitsX().totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1,   Data.limitsX().negativeLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.5, Data.limitsX().negativeLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5,  Data.limitsX().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0,  Data.limitsX().positiveLimits().max(), 1e-5 );

  Data.clear();
  Data.append( 1, 2 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1,   Data.limitsX().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1,   Data.limitsX().totalLimits().max(), 1e-5 );
  
  Data.append( 1, 3 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1,   Data.limitsX().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1,   Data.limitsX().totalLimits().max(), 1e-5 );
  
  Data.append( 1, 5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1,   Data.limitsX().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1,   Data.limitsX().totalLimits().max(), 1e-5 );
  
  Data.append( 7, invalidNumber() );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1,   Data.limitsX().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1,   Data.limitsX().totalLimits().max(), 1e-5 );
  
  Data.append( 1, 5, 1, 0 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  0,   Data.limitsX().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +2,   Data.limitsX().totalLimits().max(), 1e-5 );
  
  Data.append( 1, 5, 2, 0 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1,   Data.limitsX().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +3,   Data.limitsX().totalLimits().max(), 1e-5 ); 
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0,  Data.limitsX().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 3.0,  Data.limitsX().positiveLimits().max(), 1e-5 );

  Data.append( 0.5, 5 );
  Data.append( 1e-30, 5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5,  Data.limitsX().positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 3.0,  Data.limitsX().positiveLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1.0, Data.limitsX().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 3.0,  Data.limitsX().totalLimits().max(), 1e-5 ); 

  Data.clear();
  Data.append( 0.5,   5 );
  Data.append( 0.25,  5 );
  Data.append( 1e-30, 5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, Data.limitsX().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5, Data.limitsX().totalLimits().max(), 1e-5 ); 
}

// ---------------------------------------------------------

void scigraphics::tests::test_dataSequenceVector::limitsY()
{
  dataSequenceVector Data;
  CPPUNIT_ASSERT( ! Data.limitsY( interval<number>(0,1) ).isValid() );
  
  Data.append(   -1,   -1 );
  CPPUNIT_ASSERT( ! Data.limitsY( interval<number>(0,1) ).isValid() );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1, Data.limitsY( interval<number>(-2,2) ).totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1, Data.limitsY( interval<number>(-2,2) ).totalLimits().max(), 1e-5 );

  Data.append( -0.5, -0.5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1,   Data.limitsY( interval<number>(-2,2) ).totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.5, Data.limitsY( interval<number>(-2,2) ).totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.5, Data.limitsY( interval<number>(-0.3,-0.7) ).totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.5, Data.limitsY( interval<number>(-0.3,-0.7) ).totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1,   Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.5, Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1,   Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -0.5, Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().max(), 1e-5 );

  Data.clear();
  Data.append( -1, 2 );
  Data.append( -1e-50, 1 );
  Data.append( 1, 2 );
  Data.append( 2, 3 );
  Data.append( 2, 4 );
  Data.append( 5, 5 );
  
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  1, Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  5, Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  1, Data.limitsY( Data.limitsX().totalLimits() ).positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  5, Data.limitsY( Data.limitsX().totalLimits() ).positiveLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Data.limitsY( Data.limitsX().totalLimits() ).negativeLimits().min() );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Data.limitsY( Data.limitsX().totalLimits() ).negativeLimits().max() );
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  2, Data.limitsY( interval<number>(0,2) ).totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  4, Data.limitsY( interval<number>(0,2) ).totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT( ! Data.limitsY( interval<number>(invalidNumber(),invalidNumber()) ).isValid() );
  
  Data.append( invalidNumber(), 50 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  1, Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  5, Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().max(), 1e-5 );
  
  Data.append( 5, 5, 2 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  1, Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  7, Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().max(), 1e-5 );
  
  Data.append( 5, 1, 1 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  0, Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  7, Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  1, Data.limitsY( Data.limitsX().totalLimits() ).positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  7, Data.limitsY( Data.limitsX().totalLimits() ).positiveLimits().max(), 1e-5 );
  
  Data.append( 5, 1e-30 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  1, Data.limitsY( Data.limitsX().totalLimits() ).positiveLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  7, Data.limitsY( Data.limitsX().totalLimits() ).positiveLimits().max(), 1e-5 );
}

// ---------------------------------------------------------

void scigraphics::tests::test_dataSequenceVector::clear()
{
  dataSequenceVector Data;
  CPPUNIT_ASSERT( Data.empty() );
  
  Data.append( 1, 2 );
  Data.append( 2, 2 );

  CPPUNIT_ASSERT( ! Data.empty() );

  Data.clear();
  CPPUNIT_ASSERT( Data.empty() );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Data.limitsX().totalLimits().min() );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().min() );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().min() );
  CPPUNIT_ASSERT_EQUAL( invalidNumber(), Data.limitsY( Data.limitsX().totalLimits() ).totalLimits().max() );
}

// ---------------------------------------------------------
        
void scigraphics::tests::test_dataSequenceVector::isOrderedByX()
{
  dataSequenceVector Data;

  CPPUNIT_ASSERT_EQUAL( true, Data.isOrderedByX() );
  
  Data.append( 10, 2 );
  CPPUNIT_ASSERT_EQUAL( true, Data.isOrderedByX() );
  
  Data.append( 11, 3 );
  CPPUNIT_ASSERT_EQUAL( true, Data.isOrderedByX() );

  Data.append( 12, 4 );
  CPPUNIT_ASSERT_EQUAL( true, Data.isOrderedByX() );
   
  Data.append( 12, 1 );
  CPPUNIT_ASSERT_EQUAL( true, Data.isOrderedByX() );
  
  Data.append( 13, invalidNumber() );
  CPPUNIT_ASSERT_EQUAL( true, Data.isOrderedByX() );

  Data.append( 14, 2 );
  CPPUNIT_ASSERT_EQUAL( true, Data.isOrderedByX() );
  
  Data.append( 11, 1 );
  CPPUNIT_ASSERT_EQUAL( false, Data.isOrderedByX() );
  
  Data.append( 15, 1 );
  CPPUNIT_ASSERT_EQUAL( false, Data.isOrderedByX() );

  Data.clear();
  CPPUNIT_ASSERT_EQUAL( true, Data.isOrderedByX() );
  
  Data.append( 4, 2 );
  CPPUNIT_ASSERT_EQUAL( true, Data.isOrderedByX() );
  
  Data.append( invalidNumber(), 2 );
  CPPUNIT_ASSERT_EQUAL( false, Data.isOrderedByX() );
}

// =========================================================

void scigraphics::tests::test_dataMap::limits()
{
  dataMapVector Data( 10, interval<number>( 1, 5 ), 5, interval<number>(-10,-4) );

  CPPUNIT_ASSERT_EQUAL( (size_t)50, Data.size() );
  CPPUNIT_ASSERT_EQUAL( (size_t)10, Data.sizeX() );
  CPPUNIT_ASSERT_EQUAL( (size_t)5,  Data.sizeY() );
  
  CPPUNIT_ASSERT_EQUAL( 0,  Data.index(0,0) );
  CPPUNIT_ASSERT_EQUAL( 9, Data.indexX(49) );
  CPPUNIT_ASSERT_EQUAL( 4, Data.indexY(49) );
  CPPUNIT_ASSERT_EQUAL( 49, Data.index(9,4) );
  CPPUNIT_ASSERT_EQUAL( 7, Data.indexX(17) );
  CPPUNIT_ASSERT_EQUAL( 1, Data.indexY(17) );
  CPPUNIT_ASSERT_EQUAL( 17, Data.index(7,1) );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.4, Data.deltaX(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.2, Data.deltaY(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 3.8, Data.coordinateX(Data.indexX(17)), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -8.8, Data.coordinateY(Data.indexY(17)), 1e-5 );

  CPPUNIT_ASSERT_DOUBLES_EQUAL( 1, Data.limitsX().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 5, Data.limitsX().totalLimits().max(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -10, Data.limitsY(Data.limitsX().totalLimits()).totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL(  -4, Data.limitsY(Data.limitsX().totalLimits()).totalLimits().max(), 1e-5 );
  Data.setIntervalX( -1, 1 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( -1, Data.limitsX().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( +1, Data.limitsX().totalLimits().max(), 1e-5 );

  Data.set( 2, 3, 0.5 );
  Data.set( 2, 4, 0.7 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5, Data.limitsZ().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.7, Data.limitsZ().totalLimits().max(), 1e-5 );
  Data.set( 2, 4, 0.2 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.2, Data.limitsZ().totalLimits().min(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5, Data.limitsZ().totalLimits().max(), 1e-5 );

}

// ---------------------------------------------------------

void scigraphics::tests::test_dataMap::set()
{
  dataMapVector Data( 10, interval<number>( 1, 5 ), 5, interval<number>(-10,-4) );
 
  Data.set( 3, 2, 0.5, 0.1 );
  auto Point = Data.at( 3, 2 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5, Point.z(), 1e-5 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.1, Point.errZ(), 1e-5 );
}

// ---------------------------------------------------------

void scigraphics::tests::test_dataMap::resize()
{
  dataMapVector Data;

  Data.setIntervalX(  0,  10 );
  Data.setIntervalY( -5, -25 );
  Data.resize( 20, 10 );

  CPPUNIT_ASSERT_EQUAL( (size_t)20, Data.sizeX() );
  CPPUNIT_ASSERT_EQUAL( (size_t)10, Data.sizeY() );
  CPPUNIT_ASSERT_EQUAL( (size_t)200, Data.size() );
  
  Data.set( 14, 8, 0.5, 0.1 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5, Data.at(14,8).z(), 1e-5 );

  Data.resize( 25, 15 );
  CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.5, Data.at(14,8).z(), 1e-5 );
  
  Data.resize( 5, 2 );
  CPPUNIT_ASSERT_EQUAL( (size_t)5, Data.sizeX() );
  CPPUNIT_ASSERT_EQUAL( (size_t)2, Data.sizeY() );
  CPPUNIT_ASSERT_EQUAL( (size_t)10, Data.size() );
}

// =========================================================

