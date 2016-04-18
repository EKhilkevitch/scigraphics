
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
  return getData().limitsX(); 
}

// ------------------------------------------------------------

scigraphics::numberLimits scigraphics::sequence::graph::limitsY( const interval<number> &LimitsX ) const 
{ 
  return getData().limitsY(LimitsX); 
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
  number X = R*std::cos(Phi);
  number Y = R*std::sin(Phi);
  append( X, Y ); 
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

