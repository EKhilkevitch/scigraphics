
/*
 * Copyright 2011,2012 Evgeniy Khilkevitch 
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


#pragma once

// ============================================================

#include "scigraphics/plot.h"
#include "scigraphics/color.h"
#include "scigraphics/drawer.h"

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

