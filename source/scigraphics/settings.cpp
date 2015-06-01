
/*
 * Copyright 2011,2012,2013 Evgeniy Khilkevitch 
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

#include "scigraphics/settings.h"
#include "scigraphics/plot.h"
#include "scigraphics/scale.h"
#include "scigraphics/selection.h"
#include "scigraphics/graphsequence.h"
#include "scigraphics/plotlimits.h"

#include <cassert>
#include <typeinfo>
#include <stdexcept>

#if _MSC_VER
#  pragma warning( disable : 4800 )
#endif

// ============================================================

scigraphics::settings::settings() : 
  SelectionStripType( UncontrollableStrip ),
  GraphType( Individual ),
  VisibleFloatingRectangles( Legend|CursorPosition )
{
  for ( size_t i = 0; i < AxisPositionsCount; i++ )
  {
    ScaleTypes[i] = Linear;
    ScaleLimits[i] = plotLimits::autoScaleInterval();
  }
}

// ------------------------------------------------------------
      
scigraphics::settings::~settings() 
{
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

scigraphics::selectionStrip* scigraphics::settings::firstSelectionStrip( plot *Plot )
{
  if ( Plot == NULL )
    return NULL;

  for ( scigraphics::selectionCollection::iterator Sel = Plot->beginSelection(); Sel != Plot->endSelection(); ++Sel )
  {
    scigraphics::selectionStrip *Selection = dynamic_cast< scigraphics::selectionStrip* >( &*Sel );
    if ( Selection != NULL )
      return Selection;
  }

  return NULL;
}

// ------------------------------------------------------------

void scigraphics::settings::apply( plot *Plot ) const
{
  if ( Plot == NULL )
    throw std::invalid_argument("Plot pointer must be not NULL");

  applyScaleType(Plot);
  applyGraphType(Plot);
  applyLimits(Plot);
  applyFloatingRectangles(Plot);
  applySelectionIntervals(Plot);

  Plot->replot();
}

// ------------------------------------------------------------

void scigraphics::settings::applyGraphType( plot *Plot ) const
{
  assert( Plot != NULL );

  for ( graphCollection::iterator Graph = Plot->beginGraph(); Graph != Plot->endGraph(); ++Graph )
    applyGraphTypeToGraph( dynamic_cast< sequence::graph* >( &*Graph ) );
}
      
// ------------------------------------------------------------

void scigraphics::settings::applyGraphTypeToGraph( sequence::graph *Graph ) const
{
  if ( Graph == NULL )
    return;

  if ( GraphType == Individual )
    return;

  Graph->getViews().setViewVisible<sequence::graphViewLine>( GraphType & Lines ); 
  Graph->getViews().setViewVisible<sequence::graphViewPoints>( GraphType & Points ); 
  Graph->getViews().setViewVisible<sequence::graphViewLineHystogram>( GraphType & LinesHystogram ); 
  Graph->getViews().setViewVisible<sequence::graphViewErrorBars>( GraphType & ErrorBars ); 
}

// ------------------------------------------------------------

void scigraphics::settings::applyScaleType( plot *Plot ) const
{
  assert( Plot != NULL );

  for ( size_t i = 0; i < AxisPositionsCount; i++ )
  {
    axisPosition Position = static_cast<axisPosition>(i);
    scale *Scale = createScale( ScaleTypes[i] );
    const scale *CurrScale = Plot->scaleWithPosition( Position );

    assert( Scale != NULL );
    assert( CurrScale != NULL );

    if ( equalScaleTypes(Scale,CurrScale) )
      delete Scale;
    else
      Plot->replaceScaleWithPosition(Position,Scale);
  }
}

// ------------------------------------------------------------
      
void scigraphics::settings::applyLimits( plot *Plot ) const
{
  assert( Plot != NULL );

  for ( size_t i = 0; i < AxisPositionsCount; i++ )
  {
    axisPosition Position = static_cast<axisPosition>(i);
    Plot->setScaleInterval( Position, ScaleLimits[i] );
  }
}
      
// ------------------------------------------------------------

void scigraphics::settings::applyFloatingRectangles( plot *Plot ) const
{
  assert( Plot != NULL );

  Plot->setVisibleLegend( VisibleFloatingRectangles & Legend );
  Plot->setVisibleCursorPositionViewer( VisibleFloatingRectangles & CursorPosition );
}

// ------------------------------------------------------------

void scigraphics::settings::applySelectionIntervals( plot *Plot ) const
{
  assert( Plot != NULL );

  selectionStrip *Selection = firstSelectionStrip( Plot );

  switch ( SelectionStripType )
  {
    case UncontrollableStrip:
      break;

    case NoneStrip:
      if ( Selection != NULL )
        Plot->clearSelections();
      break;

    case VerticalStrip:
      if ( dynamic_cast<selectionVertical*>( Selection ) == NULL )
      {
        Plot->clearSelections();
        Selection = Plot->createSelection<selectionVertical>();
      }
      Selection->setInterval( SelectionStripInterval );
      break;

    case HorizontalStrip:
      if ( dynamic_cast<selectionHorizontal*>( Selection ) == NULL )
      {
        Plot->clearSelections();
        Selection = Plot->createSelection<selectionHorizontal>();
      }
      Selection->setInterval( SelectionStripInterval );
      break;

    default:
      std::abort();
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

void scigraphics::settings::setScaleType( scaleType Type, axisPosition AxisPos ) 
{
  if ( AxisPos >= AxisPositionsCount )
    throw std::invalid_argument( "Axis position is invalid" );

  ScaleTypes[ AxisPos ] = Type;
}

// ------------------------------------------------------------
      
void scigraphics::settings::setLimits( const interval<number> &Limits, axisPosition AxisPos ) 
{ 
  if ( AxisPos >= AxisPositionsCount )
    throw std::invalid_argument( "Axis position is invalid" );

  ScaleLimits[ AxisPos ] = Limits;
}

// ------------------------------------------------------------

void scigraphics::settings::setGraphType( unsigned Type ) 
{ 
  GraphType = Type; 
}

// ------------------------------------------------------------

void scigraphics::settings::setVisibleFloatingRectangles( unsigned FloatRectangles )
{
  VisibleFloatingRectangles = FloatRectangles;
}

// ------------------------------------------------------------
      
void scigraphics::settings::setSelectionInterval( selectionStripType Type, interval<number> Interval )
{
  SelectionStripType = Type;
  SelectionStripInterval = Interval;
}

// ------------------------------------------------------------
      
void scigraphics::settings::setSelectionInterval( selectionStripType Type, number Min, number Max )
{
  setSelectionInterval( Type, interval<number>( Min, Max ) );
}

// ============================================================

