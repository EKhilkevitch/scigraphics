
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
#include <istream>
#include <ostream>
#include <sstream>

#if _MSC_VER
#  pragma warning( disable : 4800 )
#endif

// ============================================================

namespace
{
  scigraphics::interval<scigraphics::number> autoScaleIntervalAfterStream()
  {
    std::stringstream Stream;
    Stream << scigraphics::plotLimits::autoScaleInterval();

    scigraphics::interval<scigraphics::number> Result;
    Stream >> Result;

    return Result;
  }
}

// ============================================================

scigraphics::settings::settings() : 
  SelectionStripType( UncontrollableStrip ),
  GraphTypeFlags( Individual ),
  VisibleFloatingRectanglesFlags( Legend|CursorPosition )
{
  for ( size_t i = 0; i < AxisPositionsCount; i++ )
  {
    ScaleTypes[i] = Linear;
    ScaleLimits[i].setMinMax( -1, 1 );
    EnableScaleLimits[i] = false;
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

const scigraphics::selectionStrip* scigraphics::settings::firstSelectionStrip( const plot &Plot )
{
  for ( scigraphics::selectionCollection::const_iterator Sel = Plot.beginSelection(); Sel != Plot.endSelection(); ++Sel )
  {
    const scigraphics::selectionStrip *Selection = dynamic_cast< const scigraphics::selectionStrip* >( &*Sel );
    if ( Selection != NULL )
      return Selection;
  }

  return NULL;
}

// ------------------------------------------------------------
      
std::string scigraphics::settings::serialize() const
{
  std::ostringstream Stream;
  Stream << *this;
  return Stream.str();
}

// ------------------------------------------------------------

bool scigraphics::settings::deserialize( const std::string &String )
{
  std::istringstream Stream(String);
  Stream >> *this;
  return static_cast<bool>(Stream);
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

  if ( GraphTypeFlags == Individual )
    return;

  Graph->getViews().setViewVisible<sequence::graphViewLine>( GraphTypeFlags & Lines ); 
  Graph->getViews().setViewVisible<sequence::graphViewPoints>( GraphTypeFlags & Points ); 
  Graph->getViews().setViewVisible<sequence::graphViewLineHystogram>( GraphTypeFlags & LinesHystogram ); 
  Graph->getViews().setViewVisible<sequence::graphViewErrorBars>( GraphTypeFlags & ErrorBars ); 
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
    interval<number> Limits = ScaleLimits[i];
    if ( ! EnableScaleLimits[i] )
      Limits = plotLimits::autoScaleInterval();
    Plot->setScaleInterval( Position, Limits );
  }
}
      
// ------------------------------------------------------------

void scigraphics::settings::applyFloatingRectangles( plot *Plot ) const
{
  assert( Plot != NULL );

  Plot->setVisibleLegend( VisibleFloatingRectanglesFlags & Legend );
  Plot->setVisibleCursorPositionViewer( VisibleFloatingRectanglesFlags & CursorPosition );
}

// ------------------------------------------------------------

void scigraphics::settings::applySelectionIntervals( plot *Plot ) const
{
  assert( Plot != NULL );

  selectionStrip *Selection = const_cast<selectionStrip*>( firstSelectionStrip( *Plot ) );

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
      assert( false );
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
      
void scigraphics::settings::throwIfAxisPosOutOfRange( axisPosition AxisPos )
{
  if ( static_cast<size_t>(AxisPos) >= static_cast<size_t>(AxisPositionsCount) )
    throw std::invalid_argument( "Axis position is invalid" );
}

// ------------------------------------------------------------

void scigraphics::settings::setAxisScaleType( scaleType Type, axisPosition AxisPos ) 
{
  throwIfAxisPosOutOfRange(AxisPos);
  ScaleTypes[ AxisPos ] = Type;
}

// ------------------------------------------------------------

scigraphics::settings::scaleType scigraphics::settings::axisScaleType( axisPosition AxisPos ) const
{
  throwIfAxisPosOutOfRange(AxisPos);
  return ScaleTypes[ AxisPos ];
}

// ------------------------------------------------------------
      
void scigraphics::settings::setAxisScaleLimits( const interval<number> &Limits, axisPosition AxisPos ) 
{ 
  throwIfAxisPosOutOfRange(AxisPos);
  ScaleLimits[ AxisPos ] = Limits;
}

// ------------------------------------------------------------
      
void scigraphics::settings::setAxisScaleLimits( number Min, number Max, axisPosition AxisPos )
{
  setAxisScaleLimits( interval<number>(Min,Max), AxisPos );
}

// ------------------------------------------------------------

scigraphics::interval<scigraphics::number> scigraphics::settings::axisScaleLimits( axisPosition AxisPos ) const 
{
  throwIfAxisPosOutOfRange(AxisPos);
  return ScaleLimits[ AxisPos ];
}

// ------------------------------------------------------------

void scigraphics::settings::setEnabledAxisScaleLimits( bool Enable, axisPosition AxisPos ) 
{
  throwIfAxisPosOutOfRange(AxisPos);
  EnableScaleLimits[AxisPos] = Enable;
}

// ------------------------------------------------------------

bool scigraphics::settings::enabledAxisScaleLimits( axisPosition AxisPos ) const
{
  throwIfAxisPosOutOfRange(AxisPos);
  return EnableScaleLimits[AxisPos];
}

// ------------------------------------------------------------

void scigraphics::settings::setGraphType( unsigned Type ) 
{ 
  GraphTypeFlags = Type; 
}

// ------------------------------------------------------------

unsigned scigraphics::settings::graphType() const
{
  return GraphTypeFlags;
}

// ------------------------------------------------------------

void scigraphics::settings::setVisibleFloatingRectangles( unsigned FloatRectangles )
{
  VisibleFloatingRectanglesFlags = FloatRectangles;
}

// ------------------------------------------------------------

unsigned scigraphics::settings::visibleFloatingRectangles() const
{
  return VisibleFloatingRectanglesFlags;
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

// ------------------------------------------------------------
      
scigraphics::settings::selectionStripType scigraphics::settings::selectionType() const 
{ 
  return SelectionStripType; 
}

// ------------------------------------------------------------

scigraphics::interval<scigraphics::number> scigraphics::settings::selectionInterval() const 
{ 
  return SelectionStripInterval; 
}

// ============================================================

std::ostream& scigraphics::operator<<( std::ostream &Stream, settings::scaleType  Type )
{
#define OUT_TYPE( Value ) \
  case settings::Value: return Stream << #Value;
  switch ( Type )
  {
    OUT_TYPE( Linear );
    OUT_TYPE( LogarithmPositive );
    OUT_TYPE( LogarithmNegative );
    OUT_TYPE( Square );
    default: return Stream << static_cast<int>(Type);
  }
#undef OUT_TYPE
}

// ------------------------------------------------------------

std::istream& scigraphics::operator>>( std::istream &Stream, settings::scaleType &Type )
{
#define IN_TYPE( Value ) \
  if ( String == #Value ) { Type = settings::Value; return Stream; }

  std::string String;  
  Stream >> String;

  IN_TYPE( Linear );
  IN_TYPE( LogarithmPositive );
  IN_TYPE( LogarithmNegative );
  IN_TYPE( Square );

  Stream.setstate( std::istream::failbit );
  return Stream;
#undef IN_TYPE
}

// ------------------------------------------------------------

std::ostream& scigraphics::operator<<( std::ostream &Stream, settings::selectionStripType  Type )
{
#define OUT_TYPE( Value ) \
  case settings::Value: return Stream << #Value;
  switch ( Type )
  {
    OUT_TYPE( UncontrollableStrip );
    OUT_TYPE( NoneStrip );
    OUT_TYPE( VerticalStrip );
    OUT_TYPE( HorizontalStrip );
    default: return Stream << static_cast<int>(Type);
  }
#undef OUT_TYPE

}

// ------------------------------------------------------------

std::istream& scigraphics::operator>>( std::istream &Stream, settings::selectionStripType &Type )
{
#define IN_TYPE( Value ) \
  if ( String == #Value ) { Type = settings::Value; return Stream; }

  std::string String;  
  Stream >> String;

  IN_TYPE( UncontrollableStrip );
  IN_TYPE( NoneStrip );
  IN_TYPE( VerticalStrip );
  IN_TYPE( HorizontalStrip );

  Stream.setstate( std::istream::failbit );
  return Stream;
#undef IN_TYPE
}

// ------------------------------------------------------------

std::ostream& scigraphics::operator<<( std::ostream &Stream, const settings &Settings )
{
  Stream << "{ ";
  for ( size_t i = 0; i < static_cast<size_t>(AxisPositionsCount); i++ )
  {
    const axisPosition AxisPos = static_cast<axisPosition>(i);
    Stream << Settings.axisScaleLimits( AxisPos ) << ' ';
    Stream << Settings.axisScaleType( AxisPos ) << ' ';
    Stream << Settings.enabledAxisScaleLimits( AxisPos ) << ' ';
  }
  Stream << Settings.graphType() << ' ';
  Stream << Settings.visibleFloatingRectangles() << ' ';
  Stream << Settings.selectionType() << ' ' << Settings.selectionInterval();
  Stream << " }";
  return Stream;
}

// ------------------------------------------------------------

std::istream& scigraphics::operator>>( std::istream &Stream, settings &Settings )
{
#define RET_ERROR() \
  do { Stream.setstate( std::istream::failbit ); return Stream; } while (false)

  std::istream::sentry Sentry(Stream);
  if ( ! Sentry )
    return Stream;

  const interval<number> AutoScaleInterval = autoScaleIntervalAfterStream();
 
  char Char = '\0';
  settings Result;

  Stream >> std::ws;
  if ( ! Stream.get(Char) || Char != '{' )
    RET_ERROR();

  for ( size_t i = 0; i < static_cast<size_t>(AxisPositionsCount); i++ )
  {
    const axisPosition AxisPos = static_cast<axisPosition>(i);

    interval<number> Interval;
    settings::scaleType ScaleType;
    bool EnableScaleLimits;
    Stream >> Interval >> ScaleType >> EnableScaleLimits;

    if ( !Stream )
      RET_ERROR();
    if ( Interval == AutoScaleInterval )
      Interval = plotLimits::autoScaleInterval();

    Result.setAxisScaleLimits( Interval, AxisPos );
    Result.setAxisScaleType( ScaleType, AxisPos );
    Result.setEnabledAxisScaleLimits( EnableScaleLimits, AxisPos );
  }

  unsigned GraphType = 0;
  Stream >> GraphType;
  Result.setGraphType(GraphType);

  unsigned VisibleFloatingRectangles = 0;
  Stream >> VisibleFloatingRectangles;
  Result.setVisibleFloatingRectangles(VisibleFloatingRectangles);

  settings::selectionStripType SelectionType;
  interval<number> SelectionInterval;
  Stream >> SelectionType;
  Stream >> SelectionInterval;
  Result.setSelectionInterval( SelectionType, SelectionInterval );

  Stream >> std::ws;
  if ( ! Stream.get(Char) || Char != '}' )
    RET_ERROR();

  Settings = Result;
  return Stream;
#undef RET_ERROR
}

// ============================================================

