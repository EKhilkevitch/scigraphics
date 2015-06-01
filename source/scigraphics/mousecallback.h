
/*
 * Copyright 2011,2012,2013 Evgeniy Khilkevitch 
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

#include "scigraphics/mouse.h"

namespace scigraphics
{

// ============================================================

  class plot;

// ============================================================

  class mouseCallBack
  {
    private:
      plot &Plot;

    protected:
      plot& getPlot() { return Plot; }

    public:
      explicit mouseCallBack( plot &Plot );
      virtual ~mouseCallBack();

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
      mouseCallBackContainer& operator=( const mouseCallBackContainer& ); 

    public:
      explicit mouseCallBackContainer( plot &Plot );
      ~mouseCallBackContainer();

      void set( mouseCallBack *CallBack );
      mouseCallBack& get();
  };

// ============================================================

}

