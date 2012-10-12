
#pragma once

#include <cassert>

#include "graphics/mouse.h"

namespace graphics
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

    public:
      mouseCallBackContainer( plot &Plot ) : CallBack( new mouseCallBack(Plot) ) {}
      ~mouseCallBackContainer() { delete CallBack; }

      void set( mouseCallBack *C ) { assert( C != NULL ); CallBack = C; }
      mouseCallBack& get() { return *CallBack; }
  };

// ============================================================

}

