
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

#include "drawer.h"
#include "painter.h"

#include <vector>

// =========================================================

namespace scigraphics
{

// =========================================================

  class mockDrawer : public drawer
  {
    public:
      struct action
      {
        enum type
        {
          DrawLine,
          DrawRectangle
        } Type;
        color Color;
        std::vector<wpoint> Points;
      };

    private:
      int Width, Height;
      std::vector<action> Actions;

    public:
      mockDrawer() : 
        Width(640), 
        Height(480) {}

      void setWidth( wcoord W )  { Width = W; }
      void setHeight( wcoord H ) { Height = H; }

      void clearActions() { Actions.clear(); }
      size_t numOfActions() const { return Actions.size(); }
      const action& getAction( unsigned i ) { return Actions.at(i); }

      void drawLine( const wpoint &A, const wpoint &B, const lineStyle& Style = lineStyle() );
      void drawRectangle( const wrectangle& Rectangle, const brushStyle& BrushStyle = brushStyle(), const lineStyle &LineStyle = lineStyle() );
      void drawText( const std::string &Text, const wrectangle& Rectangle, const textStyle &Style = textStyle(), double Angle = 0 );
      void drawPolygon( const std::vector<wpoint> &Points, const brushStyle& BrushStyle ) {}
    
      wcoord width() const  { return Width; }
      wcoord height() const { return Height; }
  };

  // =========================================================

}



