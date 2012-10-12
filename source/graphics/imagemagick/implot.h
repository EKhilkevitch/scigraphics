
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

class drawerIm : public scigraphics::drawer
{
  private:
    Magick::Image *Image;

  private:
    void prepareForLineStyle( const scigraphics::lineStyle &Style );
    void prepareForBrushStyle( const scigraphics::brushStyle& Style );
    void prepareForTextStyle( const scigraphics::textStyle &Style );

  public:
    static Magick::Color colorIm( const scigraphics::color &Color );
    static Magick::Coordinate coordinateIm( const scigraphics::wpoint &Point );
    static std::list< Magick::Coordinate > polygonIm( const std::vector<scigraphics::wpoint> &Points );

  public:
    drawerIm( size_t SizeX, size_t SizeY );

    void drawLine( const scigraphics::wpoint &A, const scigraphics::wpoint &B, const scigraphics::lineStyle &Style );
    void drawRectangle( const scigraphics::wrectangle& Rectangle, const scigraphics::brushStyle& BrushStyle, const scigraphics::lineStyle &LineStyle );
    void drawPolygon( const std::vector<scigraphics::wpoint> &Points, const scigraphics::brushStyle& BrushStyle );
    void drawText( const std::string &Text, const scigraphics::wrectangle& Rectangle, const scigraphics::textStyle &Style, double RotAngle );
    
    scigraphics::wcoord textWidth( const std::string &Text, const scigraphics::textStyle &Style );
    scigraphics::wcoord textHeight( const std::string &Text, const scigraphics::textStyle &Style );

    scigraphics::wcoord width()  const;
    scigraphics::wcoord height() const;

    void write( const std::string &FileName );
    void display();
};

// ----------------------------------------------------------------

class implot : public scigraphics::plot
{
  protected:
    drawerIm* getDrawerIm() { return dynamic_cast<drawerIm*>( getDrawer() ); }

  public:
    implot( size_t SizeX, size_t SizeY );

    void write( const std::string &FileName );
    void display();
};

// ================================================================

