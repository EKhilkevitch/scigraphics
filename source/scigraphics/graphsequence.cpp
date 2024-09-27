
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


// ============================================================

#include "scigraphics/graphsequence.h"

#include <limits>
#include <cassert>
#include <cmath>

// ============================================================

void scigraphics::sequence::graph::draw( painter &Painter, const pairScales& Scales ) const
{
  assert( Views != NULL );
  assert( Data != NULL );

  Views->draw( Painter, Scales, *Data );
}

// ------------------------------------------------------------

void scigraphics::sequence::graph::drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const
{
  assert( Views != NULL );
  Views->drawLegendExample( Painter, Rectangle );
}
      
// ------------------------------------------------------------

scigraphics::sequence::graphViewCollection& scigraphics::sequence::graph::getViews() 
{ 
  return *Views; 
}

// ------------------------------------------------------------

const scigraphics::sequence::graphViewCollection& scigraphics::sequence::graph::getViews() const 
{ 
  return *Views; 
}

// ------------------------------------------------------------

scigraphics::sequence::data& scigraphics::sequence::graph::getData() 
{ 
  return *Data; 
}

// ------------------------------------------------------------

const scigraphics::sequence::data& scigraphics::sequence::graph::getData() const 
{ 
  return *Data; 
}

// ------------------------------------------------------------

scigraphics::numberLimits scigraphics::sequence::graph::limitsX() const 
{ 
  return Data->limitsX(); 
}

// ------------------------------------------------------------

scigraphics::numberLimits scigraphics::sequence::graph::limitsY( const interval<number> &LimitsX ) const 
{ 
  return Data->limitsY(LimitsX); 
}

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::sequence::graph::legendExampleWidth()  const
{
  assert( Views != NULL );
  return Views->legendExampleWidth();
}

// ------------------------------------------------------------

scigraphics::wcoord scigraphics::sequence::graph::legendExampleHeight() const
{
  assert( Views != NULL );
  return Views->legendExampleHeight();
}

// ------------------------------------------------------------

scigraphics::sequence::graph::graph( const std::string &Lgnd ) : 
  Views(NULL), 
  Data(NULL)
{
  setLegend( Lgnd );
}

// ------------------------------------------------------------

void scigraphics::sequence::graph::setData( data *D )
{
  if ( D != Data )
  {
    delete Data;
    Data = D;
  }
}

// ------------------------------------------------------------

void scigraphics::sequence::graph::setViews( graphViewCollection *V )
{
  if ( V != Views )
  {
    delete Views;
    Views = V;
  }
}

// ------------------------------------------------------------

scigraphics::sequence::graph::~graph()
{
  delete Views;
  delete Data;
}

// ============================================================

scigraphics::sequence::graphVector::graphVector( const color &Color ) : 
  graphSpecified< dataVector, ordinarGraphViewCollection >( Color ) 
{
}

// ------------------------------------------------------------

scigraphics::sequence::graphVector::graphVector( const std::string &Legend, const color &Color ) : 
  graphSpecified< dataVector, ordinarGraphViewCollection >( Legend, Color ) 
{
}

// ------------------------------------------------------------

scigraphics::sequence::dataVector& scigraphics::sequence::graphVector::getDataVector() 
{ 
  return getCastedData(); 
}

// ------------------------------------------------------------

scigraphics::sequence::ordinarGraphViewCollection& scigraphics::sequence::graphVector::getViews() 
{ 
  return getCastedViews(); 
}

// ------------------------------------------------------------

void scigraphics::sequence::graphVector::append( number X, number Y )                           
{ 
  getCastedData().append(X,Y); 
}

// ------------------------------------------------------------

void scigraphics::sequence::graphVector::append( number X, number Y, number ErrY )              
{ 
  getCastedData().append(X,Y,ErrY); 
}

// ------------------------------------------------------------

void scigraphics::sequence::graphVector::append( number X, number Y, number ErrX, number ErrY ) 
{ 
  getCastedData().append(X,Y,ErrX,ErrY); 
}

// ------------------------------------------------------------
        
void scigraphics::sequence::graphVector::appendInvalid()                                        
{ 
  append( invalidNumber(), invalidNumber() ); 
}

// ------------------------------------------------------------

void scigraphics::sequence::graphVector::appendPolar( number Phi, number R )                    
{ 
  const number X = R*std::cos(Phi);
  const number Y = R*std::sin(Phi);
  append( X, Y ); 
} 

// ============================================================

const scigraphics::color scigraphics::sequence::graphCreatedByMouseVector::DefaultColor = scigraphics::color::Black;

// ------------------------------------------------------------

scigraphics::sequence::graphCreatedByMouseVector::graphCreatedByMouseVector( const color &Color ) : 
  graphSpecified< dataVector, ordinarGraphViewCollection >( Color ) 
{
}

// ------------------------------------------------------------

scigraphics::sequence::dataVector& scigraphics::sequence::graphCreatedByMouseVector::getDataVector() 
{ 
  return getCastedData(); 
}

// ------------------------------------------------------------

scigraphics::sequence::ordinarGraphViewCollection& scigraphics::sequence::graphCreatedByMouseVector::getViews() 
{ 
  return getCastedViews(); 
}

// ------------------------------------------------------------

void scigraphics::sequence::graphCreatedByMouseVector::append( number X, number Y )                           
{ 
  getCastedData().append(X,Y); 
}

// ------------------------------------------------------------
        
void scigraphics::sequence::graphCreatedByMouseVector::set( size_t Index, number X, number Y )
{
  getCastedData().set( Index, X, Y );
}

// ------------------------------------------------------------
        
void scigraphics::sequence::graphCreatedByMouseVector::erase( size_t Index )
{
  getCastedData().erase( Index );
}

// ------------------------------------------------------------
        
size_t scigraphics::sequence::graphCreatedByMouseVector::indexOfPoint( number X, number Y ) const
{
  const npoint Base( X, Y );
  const sequence::data &Data = getData();
  const size_t Size = Data.size();

  std::pair< size_t, number > IndexAndDistance( std::numeric_limits<size_t>::max(), std::numeric_limits<number>::max() );
  for ( size_t i = 0; i < Size; i++ )
  {
    const sequence::data::point_t Point = Data[i];
    const npoint NPoint( Point.x(), Point.y() );
    const number Distance = distance( NPoint, Base );
    if ( Distance < IndexAndDistance.second )
      IndexAndDistance = std::make_pair( i, Distance );
  }
 
  return IndexAndDistance.first;
}

// ------------------------------------------------------------
        
scigraphics::number scigraphics::sequence::graphCreatedByMouseVector::distance( const npoint &Pt1, const npoint &Pt2 )
{
  const number DeltaX = Pt1.x() - Pt2.x();
  const number DeltaY = Pt1.y() - Pt2.y();

  return std::sqrt( DeltaX*DeltaX + DeltaY*DeltaY );
}

// ============================================================

scigraphics::sequence::graphUniformVector::graphUniformVector( const color &Color ) : 
  graphSpecified< dataUniformVector, ordinarGraphViewCollection >( Color ) 
{
}

// ------------------------------------------------------------

scigraphics::sequence::graphUniformVector::graphUniformVector( const std::string &Legend, const color &Color ) : 
  graphSpecified< dataUniformVector, ordinarGraphViewCollection >( Legend, Color ) 
{
}

// ------------------------------------------------------------
        
scigraphics::sequence::dataUniformVector& scigraphics::sequence::graphUniformVector::getDataVector() 
{ 
  return getCastedData(); 
}

// ------------------------------------------------------------

const scigraphics::sequence::dataUniformVector& scigraphics::sequence::graphUniformVector::getDataVector() const 
{ 
  return getCastedData(); 
}

// ------------------------------------------------------------

scigraphics::sequence::ordinarGraphViewCollection& scigraphics::sequence::graphUniformVector::getViews() 
{ 
  return getCastedViews(); 
}

// ------------------------------------------------------------

void scigraphics::sequence::graphUniformVector::setStepX( number Step ) 
{ 
  getDataVector().setStepX( Step ); 
}

// ------------------------------------------------------------

scigraphics::number scigraphics::sequence::graphUniformVector::stepX() const 
{ 
  return getDataVector().stepX(); 
}

// ------------------------------------------------------------
        
void scigraphics::sequence::graphUniformVector::setShiftX( number Shift )
{
  getDataVector().setShiftX( Shift );
}

// ------------------------------------------------------------

scigraphics::number scigraphics::sequence::graphUniformVector::shiftX() const
{
  return getDataVector().shiftX();
}

// ------------------------------------------------------------
        
void scigraphics::sequence::graphUniformVector::append( number Y )              
{ 
  getCastedData().append(Y); 
}

// ------------------------------------------------------------

void scigraphics::sequence::graphUniformVector::append( number Y, number ErrY ) 
{ 
  getCastedData().append(Y,ErrY); 
}

// ============================================================

scigraphics::sequence::graphAreaVector::graphAreaVector( const color &Color ) : 
          graphSpecified< dataVector, coveredAreaGraphViewCollection >(Color) 
{
}

// ------------------------------------------------------------

scigraphics::sequence::graphAreaVector::graphAreaVector( const std::string &Legend, const color &Color ) : 
          graphSpecified< dataVector, coveredAreaGraphViewCollection >( Legend, Color ) 
{
}

// ------------------------------------------------------------

scigraphics::sequence::dataVector& scigraphics::sequence::graphAreaVector::getDataVector() 
{ 
  return getCastedData(); 
}

// ------------------------------------------------------------

scigraphics::sequence::coveredAreaGraphViewCollection& scigraphics::sequence::graphAreaVector::getViewsColveredArea() 
{ 
  return getCastedViews(); 
}

// ------------------------------------------------------------

void scigraphics::sequence::graphAreaVector::append( number X, number Y )                           
{ 
  getCastedData().append(X,Y); 
}

// ------------------------------------------------------------

void scigraphics::sequence::graphAreaVector::append( number X, number Y, number ErrY )              
{ 
  getCastedData().append(X,Y,ErrY); 
}

// ------------------------------------------------------------

void scigraphics::sequence::graphAreaVector::append( number X, number Y, number ErrX, number ErrY ) 
{ 
  getCastedData().append(X,Y,ErrX,ErrY); 
}

// ------------------------------------------------------------

void scigraphics::sequence::graphAreaVector::appendInvalid()                                        
{ 
  append( invalidNumber(), invalidNumber() ); 
}

// ============================================================

