
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

#if _WIN32
#  define _CRT_SECURE_NO_WARNINGS 1
#endif

#include "scigraphics/numberstyle.h"

#include <string>
#include <stdexcept>
#include <cstdio>
#include <cassert>

// ============================================================

scigraphics::numberStyle::numberTextFactory::~numberTextFactory()
{
}

// ============================================================

scigraphics::numberStyle::sprintfNumberTextFactory::sprintfNumberTextFactory( const std::string &F ) :
  Format( F )
{
}

// ------------------------------------------------------

scigraphics::numberStyle::sprintfNumberTextFactory* scigraphics::numberStyle::sprintfNumberTextFactory::clone() const
{
  return new sprintfNumberTextFactory( Format );
}

// ------------------------------------------------------

std::string scigraphics::numberStyle::sprintfNumberTextFactory::numberText( const number Number ) const
{
#if _WIN32 && _MSC_VER
  int (__cdecl *snprintf)( char *buffer, size_t count, const char *format, ... ) = _snprintf;
#endif

  char Buffer[ 128 ];
  Buffer[0] = Buffer[ sizeof(Buffer)-1 ] = '\0';
  snprintf( Buffer, sizeof(Buffer)-1, Format.c_str(), Number );
  return Buffer;
}

// ============================================================
      
scigraphics::numberStyle::numberStyle() :
  NumberTextFactory( NULL )
{
  setStyle( General );
}

// ------------------------------------------------------
  
scigraphics::numberStyle::numberStyle( style Style ) :
  NumberTextFactory( NULL )
{
  setStyle( Style );
}

// ------------------------------------------------------
      
scigraphics::numberStyle::numberStyle( numberTextFactory *Factory ) :
  NumberTextFactory( Factory )
{
  if ( Factory == NULL )
    throw std::invalid_argument( "Factory must be not NULL" );
}

// ------------------------------------------------------

scigraphics::numberStyle::numberStyle( const numberStyle &NumberStyle ) :
  NumberTextFactory( NumberStyle.NumberTextFactory->clone() )
{
  assert( NumberTextFactory != NULL );
}

// ------------------------------------------------------

scigraphics::numberStyle& scigraphics::numberStyle::operator=( const numberStyle &NumberStyle )
{
  if ( &NumberStyle != this )
  {
    numberTextFactory *NewFactory = NumberStyle.NumberTextFactory->clone();
    delete NumberTextFactory;
    NumberTextFactory = NewFactory;
  }
  return *this;
}

// ------------------------------------------------------

scigraphics::numberStyle::~numberStyle()
{
  delete NumberTextFactory;
}

// ------------------------------------------------------

std::string scigraphics::numberStyle::numberText( const number Number ) const
{
  assert( NumberTextFactory != NULL );
  return NumberTextFactory->numberText( Number );
}

// ------------------------------------------------------

void scigraphics::numberStyle::setTextFactory( numberTextFactory *Factory )
{
  if ( Factory == NULL )
    throw std::invalid_argument( "Factory must be not NULL" );

  if ( Factory != NumberTextFactory )
  {
    delete NumberTextFactory;
    NumberTextFactory = Factory;
  }
}

// ------------------------------------------------------

void scigraphics::numberStyle::setStyle( style Style )
{
  switch ( Style )
  {
    case Float:
      setStyle( "%f" );
      break;

    case Exponent:
      setStyle( "%e" );
      break;

    case Integer:
      setStyle( "%.0f" );
      break;

    case General:
      setStyle( "%g" );
      break;

    default:
      throw std::invalid_argument( "Invalid style" );
  }
}

// ------------------------------------------------------

void scigraphics::numberStyle::setStyle( const std::string &Format )
{
  if ( Format.empty() )
    throw std::invalid_argument( "Format must be not empty" );

  size_t PercentPosition = Format.find( '%' );
  if ( PercentPosition == std::string::npos )
    throw std::invalid_argument( "Format string must contain % symbol" );

  setTextFactory( new sprintfNumberTextFactory( Format ) );
}

// ============================================================

