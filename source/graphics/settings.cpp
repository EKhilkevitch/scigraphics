
#include "graphics/settings.h"

#if _WIN32
#  pragma warning( disable : 4800 )
#endif

#include <cassert>
#include <typeinfo>
#include <stdexcept>

// ============================================================

scigraphics::settings::settings() : GraphType(Individual)
{
  for ( unsigned i = 0; i < axisSetCollection::PositionsCount; i++ )
  {
    ScaleTypes[i] = Linear;
    ScaleLimits[i] = interval<number>( plotLimits::AutoScaleMin, plotLimits::AutoScaleMax );
  }
}

// ------------------------------------------------------------

scigraphics::scale* scigraphics::settings::createScale( scaleType Type )
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

void scigraphics::settings::apply( plot &Plot ) const
{
  applyScaleType(Plot);
  applyGraphType(Plot);
  applyLimits(Plot);

  Plot.replot();
}

// ------------------------------------------------------------

void scigraphics::settings::applyGraphType( plot &Plot ) const
{
  for ( graphCollection::iterator Graph = Plot.beginGraph(); Graph != Plot.endGraph(); ++Graph )
    applyGraphTypeToGraph( dynamic_cast< graphSequence* >( *Graph ) );
}
      
// ------------------------------------------------------------

void scigraphics::settings::applyGraphTypeToGraph( graphSequence *Graph ) const
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

void scigraphics::settings::applyScaleType( plot &Plot ) const
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
      
void scigraphics::settings::applyLimits( plot &Plot ) const
{
  for ( unsigned i = 0; i < axisSetCollection::PositionsCount; i++ )
  {
    axisSetCollection::axisPosition Position = static_cast<axisSetCollection::axisPosition>(i);
    Plot.setScaleInterval( Position, ScaleLimits[i] );
  }
}
      
// ------------------------------------------------------------

scigraphics::interval<scigraphics::number> scigraphics::settings::correctLimits( interval<number> Limits )
{
  if ( Limits.isSingular() )
    return interval<number>( Limits.min() * 0.9, Limits.max()* 1.1 );
  return Limits;
}

// ------------------------------------------------------------

bool scigraphics::settings::equalScaleTypes( const scale *S1, const scale *S2 )
{
  assert( S1 != NULL );
  assert( S2 != NULL );
  return typeid(*S1) == typeid(*S2);
}

// ------------------------------------------------------------

void scigraphics::settings::setScaleType( scaleType Type, axisSetCollection::axisPosition AxisPos ) 
{
  if ( AxisPos >= axisSetCollection::PositionsCount )
    throw std::invalid_argument( "Axis position is invalid" );

  ScaleTypes[ AxisPos ] = Type;
}

// ------------------------------------------------------------
      
void scigraphics::settings::setLimits( const interval<number> &Limits, axisSetCollection::axisPosition AxisPos ) 
{ 
  if ( AxisPos >= axisSetCollection::PositionsCount )
    throw std::invalid_argument( "Axis position is invalid" );

  ScaleLimits[ AxisPos ] = Limits;
}

// ============================================================

