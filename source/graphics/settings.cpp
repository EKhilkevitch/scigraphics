
#include "graphics/settings.h"

#if _WIN32
#  pragma warning( disable : 4800 )
#endif

#include <cassert>
#include <typeinfo>
#include <stdexcept>

// ============================================================

graphics::settings::settings() : GraphType(Individual)
{
  for ( unsigned i = 0; i < axisSetCollection::PositionsCount; i++ )
  {
    ScaleTypes[i] = Linear;
    ScaleLimits[i] = interval<number>( plotLimits::AutoScaleMin, plotLimits::AutoScaleMax );
  }
}

// ------------------------------------------------------------

graphics::scale* graphics::settings::createScale( scaleType Type )
{
  switch ( Type )
  {
    case Linear:
      return new scaleLinear();

    case LogarithmPositive:
      return new scaleLogarithmPositive();

    case LogarithmNegative:
      return new scaleLogarithmNegative();

    case Square:
      return new scaleSquare();

    default:
      throw std::invalid_argument("Invalid ScaleType");
      return NULL;
  }
}

// ------------------------------------------------------------

void graphics::settings::apply( plot &Plot ) const
{
  applyScaleType(Plot);
  applyGraphType(Plot);
  applyLimits(Plot);

  Plot.replot();
}

// ------------------------------------------------------------

void graphics::settings::applyGraphType( plot &Plot ) const
{
  for ( graphCollection::iterator Graph = Plot.beginGraph(); Graph != Plot.endGraph(); ++Graph )
    applyGraphTypeToGraph( dynamic_cast< graphSequence* >( *Graph ) );
}
      
// ------------------------------------------------------------

void graphics::settings::applyGraphTypeToGraph( graphSequence *Graph ) const
{
  if ( Graph == NULL )
    return;
  if ( GraphType == Individual )
    return;

  Graph->getViews().setViewVisible<graphViewLine>( GraphType & Lines ); 
  Graph->getViews().setViewVisible<graphViewPoints>( GraphType & Points ); 
  Graph->getViews().setViewVisible<graphViewLineHystogram>( GraphType & LinesHystogram ); 
  Graph->getViews().setViewVisible<graphViewErrorBars>( GraphType & ErrorBars ); 
}

// ------------------------------------------------------------

void graphics::settings::applyScaleType( plot &Plot ) const
{
  for ( unsigned i = 0; i < axisSetCollection::PositionsCount; i++ )
  {
    axisSetCollection::axisPosition Position = static_cast<axisSetCollection::axisPosition>(i);
    scale *Scale = createScale( ScaleTypes[i] );
    const scale *CurrScale = Plot.scaleWithPosition( Position );
    assert( Scale != NULL );
    assert( CurrScale != NULL );

    if ( equalScaleTypes(Scale,CurrScale) )
      delete Scale;
    else
      Plot.replaceScaleWithPosition(Position,Scale);
  }
}

// ------------------------------------------------------------
      
void graphics::settings::applyLimits( plot &Plot ) const
{
  for ( unsigned i = 0; i < axisSetCollection::PositionsCount; i++ )
  {
    axisSetCollection::axisPosition Position = static_cast<axisSetCollection::axisPosition>(i);
    Plot.setScaleInterval( Position, ScaleLimits[i] );
  }
}
      
// ------------------------------------------------------------

graphics::interval<graphics::number> graphics::settings::correctLimits( interval<number> Limits )
{
  if ( Limits.isSingular() )
    return interval<number>( Limits.min() * 0.9, Limits.max()* 1.1 );
  return Limits;
}

// ------------------------------------------------------------

bool graphics::settings::equalScaleTypes( const scale *S1, const scale *S2 )
{
  assert( S1 != NULL );
  assert( S2 != NULL );
  return typeid(*S1) == typeid(*S2);
}

// ------------------------------------------------------------

void graphics::settings::setScaleType( scaleType Type, axisSetCollection::axisPosition AxisPos ) 
{
  if ( AxisPos >= axisSetCollection::PositionsCount )
    throw std::invalid_argument( "Axis position is invalid" );

  ScaleTypes[ AxisPos ] = Type;
}

// ------------------------------------------------------------
      
void graphics::settings::setLimits( const interval<number> &Limits, axisSetCollection::axisPosition AxisPos ) 
{ 
  if ( AxisPos >= axisSetCollection::PositionsCount )
    throw std::invalid_argument( "Axis position is invalid" );

  ScaleLimits[ AxisPos ] = Limits;
}

// ============================================================

