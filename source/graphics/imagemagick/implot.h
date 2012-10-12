
#pragma once

// ============================================================

#include "graphics/plot.h"
#include "graphics/color.h"
#include "graphics/drawer.h"

#include <list>
#include <vector>

// ================================================================

namespace Magick 
{
  class Image;
  class Color;
  class Coordinate;
};

class drawerIm : public graphics::drawer
{
  private:
    Magick::Image *Image;

  private:
    void prepareForLineStyle( const graphics::lineStyle &Style );
    void prepareForBrushStyle( const graphics::brushStyle& Style );
    void prepareForTextStyle( const graphics::textStyle &Style );

  public:
    static Magick::Color colorIm( const graphics::color &Color );
    static Magick::Coordinate coordinateIm( const graphics::wpoint &Point );
    static std::list< Magick::Coordinate > polygonIm( const std::vector<graphics::wpoint> &Points );

  public:
    drawerIm( size_t SizeX, size_t SizeY );

    void drawLine( const graphics::wpoint &A, const graphics::wpoint &B, const graphics::lineStyle &Style );
    void drawRectangle( const graphics::wrectangle& Rectangle, const graphics::brushStyle& BrushStyle, const graphics::lineStyle &LineStyle );
    void drawPolygon( const std::vector<graphics::wpoint> &Points, const graphics::brushStyle& BrushStyle );
    void drawText( const std::string &Text, const graphics::wrectangle& Rectangle, const graphics::textStyle &Style, double RotAngle );
    
    graphics::wcoord textWidth( const std::string &Text, const graphics::textStyle &Style );
    graphics::wcoord textHeight( const std::string &Text, const graphics::textStyle &Style );

    graphics::wcoord width()  const;
    graphics::wcoord height() const;

    void write( const std::string &FileName );
    void display();
};

// ----------------------------------------------------------------

class implot : public graphics::plot
{
  protected:
    drawerIm* getDrawerIm() { return dynamic_cast<drawerIm*>( getDrawer() ); }

  public:
    implot( size_t SizeX, size_t SizeY );

    void write( const std::string &FileName );
    void display();
};

// ================================================================

