
// ============================================================

#include "graphics/axisset.h"

#include <stdexcept>
#include <typeinfo>
#include <algorithm>

// ============================================================

graphics::axisSet::axisSet( axis *A, axisTicks *T, axisLabels *L, axisTitle *H, grid *G, scale *S ) :
  Visible(true),
  Axis(A),
  AxisTicks(T),
  AxisLabels(L),
  AxisTitle(H),
  Grid(G),
  Scale(S) 
{
}

// ------------------------------------------------------------

graphics::axisSet::~axisSet()
{
  delete Axis;
  delete AxisTicks;
  delete AxisLabels;
  delete AxisTitle;
  delete Grid;
  delete Scale;
}

// ------------------------------------------------------------

void graphics::axisSet::replaceScale( scale *S )
{
  if ( S == NULL )
    throw std::invalid_argument("Scale must be not NULL");

  delete Scale;
  Scale = S;
}

// ------------------------------------------------------------
      
void graphics::axisSet::drawAxis( painter &Painter ) 
{ 
  if ( ! isVisible() )
    return;

  assert( Axis != NULL );
  Axis->draw(Painter); 
}

// ------------------------------------------------------------

void graphics::axisSet::drawAxisTicks( painter &Painter )
{
  if ( ! isVisible() )
    return;

  assert( Scale != NULL );
  assert( AxisTicks != NULL );

  std::vector<number> Marks = Scale->marks();
  AxisTicks->draw( Painter, *Scale, Marks );
}
      
// ------------------------------------------------------------

void graphics::axisSet::drawAxisLabels( painter &Painter )
{
  if ( ! isVisible() )
    return;

  assert( Scale != NULL );
  assert( AxisLabels != NULL );

  std::vector<number> Marks = Scale->marks();
  AxisLabels->draw( Painter, *Scale, Marks );
}

// ------------------------------------------------------------
      
void graphics::axisSet::drawAxisTitle( painter &Painter )
{
  if ( ! isVisible() )
    return;

  assert( AxisTitle != NULL );
  AxisTitle->draw( Painter );
}

// ------------------------------------------------------------

void graphics::axisSet::drawGrid( painter &Painter )
{
  if ( ! isVisible() )
    return;

  assert( Scale != NULL );
  assert( Grid != NULL );
  
  std::vector<number> Marks = Scale->marks();
  Grid->draw( Painter, *Scale, Marks );
}
      
// ------------------------------------------------------------
      
graphics::wcoord graphics::axisSet::wcoordDimension( const painter &Painter ) const
{
  return ( getDirection() == axisSet::DirectionX ) ? Painter.width() : Painter.height();;
}

// ------------------------------------------------------------

double graphics::axisSet::wpointsPerNumber( const painter &Painter ) const
{
  assert( Scale != NULL );
  interval<number> NumberInterval = Scale->getNumberInterval();
    
  if ( ! numberLimits::isValidInterval( NumberInterval ) )
    return 0;
  if ( NumberInterval.distance() <= 0 )
    return 0;

  double Dimension = wcoordDimension( Painter ); 
  return Dimension / NumberInterval.distance();
}

// ============================================================

graphics::axisSetX::axisSetX( fcoord BaseY ) : 
  axisSet( 
    new axisX(BaseY), 
    new axisTicksX(BaseY), 
    new axisLabelsX(BaseY), 
    new axisTitleX(BaseY),
    new gridX(), 
    new scaleLinear() )
{
}

// ------------------------------------------------------------
      
graphics::wcoord graphics::axisSetX::requiredIndent( bool Used ) const
{
  wcoord Indent = 7;

  if ( ! isVisible() || !Used )
    return Indent;

  Indent += 8;

  Indent += AxisLabels->getTextStyle().getFontSize() + 5;
  if ( ! AxisTitle->title().empty() )
    Indent += AxisTitle->getTextStyle().getFontSize() + 5;
  return Indent;
}

// ============================================================

graphics::axisSetY::axisSetY( fcoord BaseX ) : 
  axisSet( 
    new axisY(BaseX), 
    new axisTicksY(BaseX), 
    new axisLabelsY(BaseX), 
    new axisTitleY(BaseX),
    new gridY(), 
    new scaleLinear() )
{
}

// ------------------------------------------------------------
      
graphics::wcoord graphics::axisSetY::requiredIndent( bool Used ) const
{
  wcoord Indent = 7;
  
  if ( ! isVisible() || !Used )
    return Indent;

  Indent += 8;

  Indent += AxisLabels->getTextStyle().getFontSize() * 4 + 5;
  if ( ! AxisTitle->title().empty() )
    Indent += AxisTitle->getTextStyle().getFontSize() + 5;
  return Indent;
}

// ============================================================

graphics::axisSetCollection::axisSetCollection()
{
  assert( PositionsCount == 4 );
  AxisSets.resize( PositionsCount );
  AxisSets[ Left ]   = new axisSetY(0);
  AxisSets[ Right ]  = new axisSetY(1);
  AxisSets[ Top ]    = new axisSetX(1);
  AxisSets[ Bottom ] = new axisSetX(0);
}

// ------------------------------------------------------------

graphics::axisSetCollection::~axisSetCollection()
{
}
      
// ------------------------------------------------------------

graphics::axisSet& graphics::axisSetCollection::at( axisPosition Position )
{
  unsigned Index = (unsigned)Position;
  if ( Index >= AxisSets.size() )
    throw std::out_of_range( "Invalid axis position" );
  return *(AxisSets[Index]);
}

// ------------------------------------------------------------
      
graphics::pairScales graphics::axisSetCollection::getBottomLeftPairScales()
{
  return pairScales( *(AxisSets[Bottom]->getScale()), *(AxisSets[Left]->getScale()) ); 
}

// ------------------------------------------------------------

const graphics::axisSet& graphics::axisSetCollection::at( axisPosition Position ) const
{
  return ( const_cast<axisSetCollection*>(this) )->at(Position);
}

// ------------------------------------------------------------
      
void graphics::axisSetCollection::setAxisNumberLimits( plotLimits::limitsXY *Limits )
{
  if ( Limits == NULL )
    throw std::invalid_argument( "Limits pointer must be not NULL" );
  
  for ( axis_iterator Axis = AxisSets.begin(); Axis != AxisSets.end(); ++Axis )
    Limits->setAxisNumberLimits( *Axis );
}

// ------------------------------------------------------------
      
void graphics::axisSetCollection::drawGrid( painter &Painter )
{
  at( Left ).drawGrid(Painter);
  at( Bottom ).drawGrid(Painter);
}

// ------------------------------------------------------------

void graphics::axisSetCollection::drawAxis( painter &Painter )
{
  for ( axis_iterator Axis = AxisSets.begin(); Axis != AxisSets.end(); ++Axis )
    (*Axis)->drawAxis(Painter);
}

// ------------------------------------------------------------

void graphics::axisSetCollection::drawAxisTicks( painter &Painter )
{
  for ( axis_iterator Axis = AxisSets.begin(); Axis != AxisSets.end(); ++Axis )
    (*Axis)->drawAxisTicks(Painter);
}

// ------------------------------------------------------------

void graphics::axisSetCollection::drawAxisLabels( painter &Painter )
{
  for ( axis_iterator Axis = AxisSets.begin(); Axis != AxisSets.end(); ++Axis )
    (*Axis)->drawAxisLabels(Painter);
}

// ------------------------------------------------------------

void graphics::axisSetCollection::drawAxisTitles( painter &Painter )
{
  for ( axis_iterator Axis = AxisSets.begin(); Axis != AxisSets.end(); ++Axis )
    (*Axis)->drawAxisTitle(Painter);
}

// ------------------------------------------------------------

void graphics::axisSetCollection::applyScalesChanging( double Value, axisSet::direction Direction, void (*Operation)( scale *Scale, double Value ) )
{
  if ( Operation == NULL )
    throw std::invalid_argument( "Operation pointer must be not NULL" );

  for ( axis_iterator a = AxisSets.begin(); a != AxisSets.end(); ++a )
  {
    axisSet *AxisSet = *a;
    assert( AxisSet != NULL );
    if ( AxisSet->getDirection() == Direction )
    {
      scale *Scale = AxisSet->getScale();
      assert( Scale != NULL );
      Operation( Scale, Value );
    }
  }
}

// ------------------------------------------------------------

void graphics::axisSetCollection::addScalesShift( double Shift, axisSet::direction Direction )
{
  applyScalesChanging( Shift, Direction, scale::addScaleShift );
}

// ------------------------------------------------------------

void graphics::axisSetCollection::mulScalesZoom( double Zoom, axisSet::direction Direction )
{
  applyScalesChanging( Zoom, Direction, scale::mulScaleZoom );
}

// ------------------------------------------------------------

void graphics::axisSetCollection::resetScales( axisSet::direction Direction )
{
  applyScalesChanging( 0, Direction, scale::resetScale );
}

// ------------------------------------------------------------

void graphics::axisSetCollection::resetAllScales()
{
  applyScalesChanging( 0, axisSet::DirectionX, scale::resetScale );
  applyScalesChanging( 0, axisSet::DirectionY, scale::resetScale );
}

// ------------------------------------------------------------

void graphics::axisSetCollection::resetScalesTo1x1( painter &Painter )
{
  resetAllScales();

  double MinWPointsPerNPoints = minWPointsPerNPoints(Painter);
  if ( MinWPointsPerNPoints <= 0 || MinWPointsPerNPoints >= 1e10 )
    return;

  for ( axis_iterator a = AxisSets.begin(); a != AxisSets.end(); ++a )
  {
    double WPointsPerNPoints = (*a)->wpointsPerNumber(Painter);
    if ( WPointsPerNPoints <= 0 )
      continue;
    double MulZoom = WPointsPerNPoints / MinWPointsPerNPoints;
    scale *Scale = (*a)->getScale();
    assert( Scale != NULL );
    scale::addScaleShift( Scale, ( 1 - MulZoom  )/2 );
    scale::mulScaleZoom( Scale, MulZoom );
  }

}

// ------------------------------------------------------------
      
double graphics::axisSetCollection::minWPointsPerNPoints( painter &Painter ) const
{
  double MinWPointsPerNPoints = 1e100;
  for ( axis_const_iterator a = AxisSets.begin(); a != AxisSets.end(); ++a )
  {
    double WPointsPerNPoints = (*a)->wpointsPerNumber(Painter);
    if ( WPointsPerNPoints <= 0 )
      continue;
    MinWPointsPerNPoints = std::min<double>( MinWPointsPerNPoints, WPointsPerNPoints );
  }
  return MinWPointsPerNPoints;
}

// ============================================================


