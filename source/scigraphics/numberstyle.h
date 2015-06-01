
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
      class numberTextFactory 
      {
        public:
          virtual ~numberTextFactory();
          virtual numberTextFactory* clone() const = 0;
          virtual std::string numberText( const number Number ) const = 0;
      };
  
      class sprintfNumberTextFactory : public numberTextFactory
      {
        private:
          std::string Format;

        public:
          explicit sprintfNumberTextFactory( const std::string &Format );
          sprintfNumberTextFactory* clone() const;
          std::string numberText( const number Number ) const;
      };

    public:
      enum style
      {
        Float,
        Exponent,
        Integer,
        General
      };

    private:
      numberTextFactory *NumberTextFactory;

    public:
      numberStyle();
      explicit numberStyle( style Style );
      explicit numberStyle( numberTextFactory *Factory );
      numberStyle( const numberStyle &NumberStyle );
      numberStyle& operator=( const numberStyle &NumberStyle );
      ~numberStyle();

      std::string numberText( const number Number ) const;

      void setTextFactory( numberTextFactory *Factory );
      void setStyle( style Style );
      void setStyle( const std::string &Format );
  };
  
  // ============================================================

}

