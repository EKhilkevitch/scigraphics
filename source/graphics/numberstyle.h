
#pragma once

#include "graphics/numbers.h"

#include <string>

namespace graphics
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
    protected:
      virtual const char* format() const = 0;
      void fillBuffer( char *Buf, size_t Size, number Value )  const;
    public:
      std::string numberText( const number Number ) const;
  };
  
  // ------------------------------------------------------

  class generalNumberStyle : public sprintfNumberStyle
  {
    protected:
      const char* format() const { return "%g"; }
  };
  
  // ------------------------------------------------------
  
  class exponentionalNumberStyle : public sprintfNumberStyle
  {
    protected:
      const char* format() const { return "%e"; }
  };


// ======================================================

}

