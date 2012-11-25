
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

#pragma once

// ============================================================

namespace scigraphics
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

