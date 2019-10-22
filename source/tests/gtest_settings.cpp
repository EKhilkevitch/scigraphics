
// =========================================================

#include <gtest/gtest.h>

#include "settings.h"
#include "plotlimits.h"

#include <string>
#include <sstream>

using namespace scigraphics;

// =========================================================

TEST( settings, output )
{
  std::ostringstream Stream;
 
  settings Settings;
  Settings.setAxisScaleLimits( 0.5, 1.5, AxisLeft );
  Settings.setAxisScaleLimits(  10,  30, AxisTop );
  Settings.setEnabledAxisScaleLimits( false, AxisTop );
  Settings.setEnabledAxisScaleLimits( true, AxisLeft );
  Settings.setAxisScaleType( settings::LogarithmPositive, AxisBottom );
  Settings.setGraphType( settings::Points | settings::ErrorBars );
  Settings.setSelectionInterval( settings::VerticalStrip, 5, 6 );

  Stream << Settings;
  EXPECT_EQ( "{ "
    "[ 0.5 .. 1.5 ] Linear 1 "
    "[ -1.79769e+308 .. 1.79769e+308 ] Linear 0 "
    "[ 10 .. 30 ] Linear 0 "
    "[ -1.79769e+308 .. 1.79769e+308 ] LogarithmPositive 0 "
    "18 3 "
    "VerticalStrip [ 5 .. 6 ] }", Stream.str() );
}

// ---------------------------------------------------------

TEST( settings, input )
{
  std::istringstream Stream( "{ "
    "[ 0.5 .. 1.5 ] Linear 1 "
    "[ -1.79769e+308 .. 1.79769e+308 ] Linear 1 "
    "[ 10 .. 30 ] Linear 0 "
    "[ -1.79769e+308 .. 1.79769e+308 ] LogarithmPositive 1 "
    "18 3 "
    "VerticalStrip [ 5 .. 6 ] }" );
 
  settings Settings;
  Stream >> Settings;
  EXPECT_TRUE( ! Stream.fail() );

  EXPECT_NEAR( 0.5, Settings.axisScaleLimits(AxisLeft).min(), 1e-4 );
  EXPECT_NEAR( 1.5, Settings.axisScaleLimits(AxisLeft).max(), 1e-4 );
  EXPECT_TRUE( Settings.enabledAxisScaleLimits(AxisLeft) );

  EXPECT_EQ( plotLimits::autoScaleInterval(), Settings.axisScaleLimits(AxisRight) );
  EXPECT_TRUE( Settings.enabledAxisScaleLimits(AxisRight) );

  EXPECT_NEAR( 10, Settings.axisScaleLimits(AxisTop).min(), 1e-4 );
  EXPECT_NEAR( 30, Settings.axisScaleLimits(AxisTop).max(), 1e-4 );
  EXPECT_FALSE( Settings.enabledAxisScaleLimits(AxisTop) );

  EXPECT_EQ( plotLimits::autoScaleInterval(), Settings.axisScaleLimits(AxisBottom) );
  
  EXPECT_EQ( settings::Linear, Settings.axisScaleType(AxisLeft) );
  EXPECT_EQ( settings::LogarithmPositive, Settings.axisScaleType(AxisBottom) );
  
  EXPECT_EQ( static_cast<unsigned>(settings::Points|settings::ErrorBars), Settings.graphType() );
  
  EXPECT_EQ( settings::VerticalStrip, Settings.selectionType() );
  EXPECT_NEAR( 5, Settings.selectionInterval().min(), 1e-5 );
  EXPECT_NEAR( 6, Settings.selectionInterval().max(), 1e-5 );
}

// =========================================================

