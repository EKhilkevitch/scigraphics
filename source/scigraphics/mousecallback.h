
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

#include <cassert>

#include "scigraphics/mouse.h"

namespace scigraphics
{

// ============================================================

  class plot;

  class mouseCallBack
  {
    private:
      plot &Plot;

    protected:
      plot& getPlot() { return Plot; }

    public:
      mouseCallBack( plot &P ) : Plot(P) {}
      virtual ~mouseCallBack() {}

      virtual void onPressed( mouse::mouseActionHandler* ) {}
      virtual void onMoved( mouse::mouseActionHandler* ) {}
      virtual void onRelease( mouse::mouseActionHandler* ) {}
      virtual void onWheel( mouse::mouseWheelHandler* ) {}
  };

  class mouseCallBackContainer
  {
    private:
      mouseCallBack *CallBack;

    private:
      mouseCallBackContainer( const mouseCallBackContainer& );
      mouseCallBackContainer& operator=( mouseCallBackContainer ); 

    public:
      mouseCallBackContainer( plot &Plot ) : CallBack( new mouseCallBack(Plot) ) {}
      ~mouseCallBackContainer() { delete CallBack; }

      void set( mouseCallBack *C ) { assert( C != NULL ); CallBack = C; }
      mouseCallBack& get() { return *CallBack; }
  };

// ============================================================

}

