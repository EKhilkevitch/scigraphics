
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


// ============================================================

#include "scigraphics/textstyle.h"

#include <cstring>
#include <stdexcept>

// ============================================================
      
scigraphics::textStyle::textStyle( unsigned FS ) : 
  FontSize(FS), 
  Color() 
{
  Font[0] = '\0';
}

// ------------------------------------------------------------
      
void scigraphics::textStyle::setFontName( const std::string &F ) 
{ 
  const size_t MaxFontLength = sizeof(Font);
  
  if ( F.length() >= MaxFontLength )
    throw std::invalid_argument( "Font name length too big" );

  std::strncpy( Font, F.c_str(), MaxFontLength );
  Font[ MaxFontLength-1 ] = '\0';
}

// ------------------------------------------------------------
      
std::string scigraphics::textStyle::getFontName() const 
{ 
  return std::string(Font);
}
      
// ============================================================

