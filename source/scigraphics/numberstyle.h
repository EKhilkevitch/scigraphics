
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

#include "scigraphics/numbers.h"

#include <string>

// ============================================================

namespace scigraphics
{

  // ============================================================

  class numberStyle 
  {
    public:
      virtual std::string numberText( const number Number ) const = 0;
      virtual ~numberStyle() {}
  };
  
  // ------------------------------------------------------

  class sprintfNumberStyle : public numberStyle
  {
    private:
      std::string Format;
    protected:
      const char* format() const { return Format.c_str(); };
      void fillBuffer( char *Buf, size_t Size, number Value )  const;
    public:
      sprintfNumberStyle( const std::string &F ) : Format(F) {}
      std::string numberText( const number Number ) const;
  };
  
  // ------------------------------------------------------

  class generalNumberStyle : public sprintfNumberStyle
  {
    public:
      generalNumberStyle() : sprintfNumberStyle( "%g" ) {}
  };

  // ======================================================

}

