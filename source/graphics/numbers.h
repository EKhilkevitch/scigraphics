
// ============================================================

#pragma once

// ============================================================

namespace graphics
{

// ============================================================

  // Тип для представления данных
  typedef double number;

  // Координаты в окне (в точках)
  typedef int    wcoord;

  // Относительные (fraction) координаты в окне (от 0.0 до 1.0)
  typedef double fcoord;

  static inline number invalidNumber() { return 1.2345e21; }

  static inline bool isValidNumber( number Number )                   { return ( Number != invalidNumber() ); }
  static inline bool isValidNumbers( number Number1, number Number2 ) { return isValidNumber(Number1) && isValidNumber(Number2); }

// ============================================================

}

