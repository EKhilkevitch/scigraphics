
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

#include "scigraphics/mousecallback.h"

#include <cassert>

// ============================================================

scigraphics::mouseCallBack::mouseCallBack( plot &P ) : 
  Plot(P) 
{
}

// ------------------------------------------------------------

scigraphics::mouseCallBack::~mouseCallBack() 
{
}

// ============================================================

scigraphics::mouseCallBackContainer::mouseCallBackContainer( plot &Plot ) : 
  CallBack( new mouseCallBack(Plot) ) 
{
}

// ------------------------------------------------------------
      
scigraphics::mouseCallBackContainer::~mouseCallBackContainer() 
{ 
  delete CallBack; 
}

// ------------------------------------------------------------
      
void scigraphics::mouseCallBackContainer::set( mouseCallBack *NewCallBack ) 
{ 
  if ( NewCallBack == NULL )
    return;

  if ( NewCallBack != CallBack )
  {
    delete CallBack; 
    CallBack = NewCallBack; 
  }
}

// ------------------------------------------------------------
      
scigraphics::mouseCallBack& scigraphics::mouseCallBackContainer::get() 
{ 
  assert( CallBack != NULL );
  return *CallBack; 
}

// ============================================================

