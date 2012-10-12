
// ============================================================

#if _WIN32
#  define _CRT_SECURE_NO_WARNINGS 1
#endif

#include "graphics/numberstyle.h"

#include <cstdio>
#include <string>
#include <cassert>

#if _WIN32
#  define snprintf _snprintf
#endif

// ============================================================

void graphics::sprintfNumberStyle::fillBuffer( char *Buf, size_t Size, number Value )  const
{ 
  assert( Buf != NULL );
  assert( Size > 0 );
  Buf[Size-1] = '\0'; 
  snprintf( Buf, Size-1, format(), Value );
}
  
// ------------------------------------------------------
      
std::string graphics::sprintfNumberStyle::numberText( const number Number ) const
{
  char Buf[64];
  fillBuffer( Buf, sizeof(Buf), Number );
  return std::string(Buf); 
}

// ============================================================


