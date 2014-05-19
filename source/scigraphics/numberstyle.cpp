
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

#include <cstdio>
#include <string>
#include <cassert>

// ============================================================

void scigraphics::sprintfNumberStyle::fillBuffer( char *Buf, size_t Size, number Value )  const
{ 
#if _WIN32 && _MSC_VER
  int (__cdecl *snprintf)( char *buffer, size_t count, const char *format, ... ) = _snprintf;
#endif

  assert( Buf != NULL );
  assert( Size > 0 );
  Buf[Size-1] = '\0'; 
  snprintf( Buf, Size-1, format(), Value );
}
  
// ------------------------------------------------------
      
std::string scigraphics::sprintfNumberStyle::numberText( const number Number ) const
{
  char Buf[64];
  fillBuffer( Buf, sizeof(Buf), Number );
  return std::string(Buf); 
}

// ============================================================


