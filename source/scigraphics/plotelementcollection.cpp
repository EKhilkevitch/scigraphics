

// ============================================================

#include "graphics/plotelementcollection.h"
#include "graphics/axisset.h"
#include "graphics/scale.h"

// ============================================================

const scigraphics::scale* scigraphics::plotElementsCollection::axisSetsPair::scaleX() const
{ 
  return isZero() ? NULL : AxisX->getScale(); 
} 

// ------------------------------------------------------------

const scigraphics::scale* scigraphics::plotElementsCollection::axisSetsPair::scaleY() const 
{ 
  return isZero() ? NULL : AxisY->getScale(); 
} 

// ------------------------------------------------------------
          
scigraphics::pairScales scigraphics::plotElementsCollection::axisSetsPair::createPairScales() const
{
  if ( isZero() )
    throw std::runtime_error("No scales for graphic");

  const scale *ScaleX = scaleX(); 
  const scale *ScaleY = scaleY(); 

  assert( ScaleX != NULL );
  assert( ScaleY != NULL );

  return pairScales( *ScaleX, *ScaleY );
}

// ============================================================

void scigraphics::plotElementsCollection::append( plotElement *PlotElement )
{
  if ( PlotElement == NULL )
    throw std::invalid_argument("Object must be not NULL");

  if ( exist(PlotElement) )
    throw std::runtime_error("Object alredy in list");

  if ( DefaultAxisSets.isZero() )
    throw std::runtime_error("Default axis has not been set");

  PlotElementsList.push_back( PlotElement );
  AxisBindMap[ PlotElement ] = DefaultAxisSets;

  assert( PlotElementsList.size() == AxisBindMap.size() );
}

// ------------------------------------------------------------

void scigraphics::plotElementsCollection::erase( plotElement *PlotElement )
{
  plotElementList::iterator PlotIterator = std::find( PlotElementsList.begin(), PlotElementsList.end(), PlotElement );
  if ( PlotIterator == PlotElementsList.end() )
    return;
  AxisBindMap.erase( PlotElement );
  PlotElementsList.erase( PlotIterator );
}

// ------------------------------------------------------------
      
void scigraphics::plotElementsCollection::clear() 
{ 
  PlotElementsList.clear(); 
  AxisBindMap.clear(); 
}

// ------------------------------------------------------------
      
bool scigraphics::plotElementsCollection::exist( const plotElement *PlotElement ) const
{
  return std::find( PlotElementsList.begin(), PlotElementsList.end(), PlotElement ) != PlotElementsList.end();
}

// ------------------------------------------------------------

void scigraphics::plotElementsCollection::setDefaultAxisSets( const axisSet *X, const axisSet *Y ) 
{ 
  if ( X == NULL || Y == NULL )
    throw std::invalid_argument("Argument must be not NULL");
  DefaultAxisSets = axisSetsPair(X,Y); 
}

// ------------------------------------------------------------

void scigraphics::plotElementsCollection::draw( painter &Painter ) const
{
  for ( plotElementList::const_iterator p = PlotElementsList.begin(); p != PlotElementsList.end(); ++p )
  {
    pairScales Scales = getPairScales( *p );
    (*p)->draw(Painter,Scales);
  }
}

// ------------------------------------------------------------

scigraphics::pairScales scigraphics::plotElementsCollection::getPairScales( const plotElement *PlotElement ) const
{
  const axisSetsPair *Axis = getAxisSetsPair( PlotElement );
  assert( Axis != NULL );
  return Axis->createPairScales();
}

// ------------------------------------------------------------

const scigraphics::plotElementsCollection::axisSetsPair* scigraphics::plotElementsCollection::getAxisSetsPair( const plotElement *PlotElement ) const
{
  axisBindMap::const_iterator Axis = AxisBindMap.find( PlotElement );
  return Axis == AxisBindMap.end() ? NULL : &( Axis->second );
}

// ------------------------------------------------------------

const scigraphics::scale* scigraphics::plotElementsCollection::graphScaleX( const plotElement *PlotElement ) const
{
  const axisSetsPair *Axis = getAxisSetsPair( PlotElement );
  if ( Axis == NULL )
    return NULL;
  return Axis->scaleX();
}

// ------------------------------------------------------------

const scigraphics::scale* scigraphics::plotElementsCollection::graphScaleY( const plotElement *PlotElement ) const
{
  const axisSetsPair *Axis = getAxisSetsPair( PlotElement );
  if ( Axis == NULL )
    return NULL;
  return Axis->scaleY();
}

// ------------------------------------------------------------

const scigraphics::axisSet* scigraphics::plotElementsCollection::graphAxisSetX( const plotElement *PlotElement ) const
{
  const axisSetsPair *Axis = getAxisSetsPair( PlotElement );
  if ( Axis == NULL )
    return NULL;
  return Axis->axisSetX();
}

// ------------------------------------------------------------

const scigraphics::axisSet* scigraphics::plotElementsCollection::graphAxisSetY( const plotElement *PlotElement ) const
{
  const axisSetsPair *Axis = getAxisSetsPair( PlotElement );
  if ( Axis == NULL )
    return NULL;
  return Axis->axisSetY();
}

// ------------------------------------------------------------

std::set< const scigraphics::axisSet* > scigraphics::plotElementsCollection::setOfGraphAxisSet() const
{
  std::set< const axisSet* > Result;

  for ( axisBindMap::const_iterator Axis = AxisBindMap.begin(); Axis != AxisBindMap.end(); ++Axis )
  {
    Result.insert( Axis->second.axisSetX() );
    Result.insert( Axis->second.axisSetY() );
  }

  Result.insert( DefaultAxisSets.axisSetX() );
  Result.insert( DefaultAxisSets.axisSetY() );

  return Result;
}

// ------------------------------------------------------------
      
void scigraphics::plotElementsCollection::bindGraphToAxisSet( const plotElement *PlotElement, const axisSet *X, const axisSet *Y )
{
  if ( X == NULL || Y == NULL || PlotElement == NULL )
    throw std::invalid_argument("Argument must be not NULL");
  
  if ( ! exist(PlotElement) )
    throw std::invalid_argument( "Plot element not found in collection" );

  AxisBindMap[ PlotElement ] = axisSetsPair(X,Y); 
  assert( PlotElementsList.size() == AxisBindMap.size() );
}

// ============================================================

