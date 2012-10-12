

// ============================================================

#include "graphics/plotelementcollection.h"
#include "graphics/axisset.h"
#include "graphics/scale.h"

// ============================================================

const graphics::scale* graphics::plotElementsCollection::axisSetsPair::scaleX() const
{ 
  return isZero() ? NULL : AxisX->getScale(); 
} 

// ------------------------------------------------------------

const graphics::scale* graphics::plotElementsCollection::axisSetsPair::scaleY() const 
{ 
  return isZero() ? NULL : AxisY->getScale(); 
} 

// ------------------------------------------------------------
          
graphics::pairScales graphics::plotElementsCollection::axisSetsPair::createPairScales() const
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

void graphics::plotElementsCollection::append( plotElement *PlotElement )
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

void graphics::plotElementsCollection::erase( plotElement *PlotElement )
{
  plotElementList::iterator PlotIterator = std::find( PlotElementsList.begin(), PlotElementsList.end(), PlotElement );
  if ( PlotIterator == PlotElementsList.end() )
    return;
  AxisBindMap.erase( PlotElement );
  PlotElementsList.erase( PlotIterator );
}

// ------------------------------------------------------------
      
void graphics::plotElementsCollection::clear() 
{ 
  PlotElementsList.clear(); 
  AxisBindMap.clear(); 
}

// ------------------------------------------------------------
      
bool graphics::plotElementsCollection::exist( const plotElement *PlotElement ) const
{
  return std::find( PlotElementsList.begin(), PlotElementsList.end(), PlotElement ) != PlotElementsList.end();
}

// ------------------------------------------------------------

void graphics::plotElementsCollection::setDefaultAxisSets( const axisSet *X, const axisSet *Y ) 
{ 
  if ( X == NULL || Y == NULL )
    throw std::invalid_argument("Argument must be not NULL");
  DefaultAxisSets = axisSetsPair(X,Y); 
}

// ------------------------------------------------------------

void graphics::plotElementsCollection::draw( painter &Painter ) const
{
  for ( plotElementList::const_iterator p = PlotElementsList.begin(); p != PlotElementsList.end(); ++p )
  {
    pairScales Scales = getPairScales( *p );
    (*p)->draw(Painter,Scales);
  }
}

// ------------------------------------------------------------

graphics::pairScales graphics::plotElementsCollection::getPairScales( const plotElement *PlotElement ) const
{
  const axisSetsPair *Axis = getAxisSetsPair( PlotElement );
  assert( Axis != NULL );
  return Axis->createPairScales();
}

// ------------------------------------------------------------

const graphics::plotElementsCollection::axisSetsPair* graphics::plotElementsCollection::getAxisSetsPair( const plotElement *PlotElement ) const
{
  axisBindMap::const_iterator Axis = AxisBindMap.find( PlotElement );
  return Axis == AxisBindMap.end() ? NULL : &( Axis->second );
}

// ------------------------------------------------------------

const graphics::scale* graphics::plotElementsCollection::graphScaleX( const plotElement *PlotElement ) const
{
  const axisSetsPair *Axis = getAxisSetsPair( PlotElement );
  if ( Axis == NULL )
    return NULL;
  return Axis->scaleX();
}

// ------------------------------------------------------------

const graphics::scale* graphics::plotElementsCollection::graphScaleY( const plotElement *PlotElement ) const
{
  const axisSetsPair *Axis = getAxisSetsPair( PlotElement );
  if ( Axis == NULL )
    return NULL;
  return Axis->scaleY();
}

// ------------------------------------------------------------

const graphics::axisSet* graphics::plotElementsCollection::graphAxisSetX( const plotElement *PlotElement ) const
{
  const axisSetsPair *Axis = getAxisSetsPair( PlotElement );
  if ( Axis == NULL )
    return NULL;
  return Axis->axisSetX();
}

// ------------------------------------------------------------

const graphics::axisSet* graphics::plotElementsCollection::graphAxisSetY( const plotElement *PlotElement ) const
{
  const axisSetsPair *Axis = getAxisSetsPair( PlotElement );
  if ( Axis == NULL )
    return NULL;
  return Axis->axisSetY();
}

// ------------------------------------------------------------

std::set< const graphics::axisSet* > graphics::plotElementsCollection::setOfGraphAxisSet() const
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
      
void graphics::plotElementsCollection::bindGraphToAxisSet( const plotElement *PlotElement, const axisSet *X, const axisSet *Y )
{
  if ( X == NULL || Y == NULL || PlotElement == NULL )
    throw std::invalid_argument("Argument must be not NULL");
  
  if ( ! exist(PlotElement) )
    throw std::invalid_argument( "Plot element not found in collection" );

  AxisBindMap[ PlotElement ] = axisSetsPair(X,Y); 
  assert( PlotElementsList.size() == AxisBindMap.size() );
}

// ============================================================

