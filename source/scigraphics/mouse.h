
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

#include "scigraphics/numbers.h"
#include "scigraphics/geometry.h"

namespace scigraphics
{

  // ============================================================

  class plot;
  class painter;
  class floatRectangle;
  class selectionStrip;

  // ============================================================

  class mouse
  {
    public:
      typedef int wheeldelta;

      enum buttons
      {
        None    = 0x00,
        Left    = 0x01,
        Right   = 0x02,
        Middle  = 0x04,
        Ctrl    = 0x10,
        Alt     = 0x20,
        Shift   = 0x40
      };

      class allowing
      {
        public:
          enum operation
          {
            None            = 0x00000000,
            ZoomX           = 0x00000001,
            ZoomY           = 0x00000002,
            Zoom            = ZoomX | ZoomY,
            MoveX           = 0x00000004,
            MoveY           = 0x00000008,
            Move            = MoveX | MoveY,
            ShiftAndZoom    = Shift | Zoom,
            WheelH          = 0x00000100,
            WheelV          = 0x00000200,
            Reset           = 0x00000400,
            SelectV         = 0x00010000,
            SelectH         = 0x00020000,
            Select          = SelectV | SelectH,
            MoveSelect      = 0x00040000,
            ResetSelect     = 0x00080000,
            MoveFloat       = 0x01000000,
            Everything      = (~0)
          };

        private:
          unsigned AllowedOperations;

        public:
          allowing( unsigned A = Everything ) : AllowedOperations(A) {}
          unsigned value() const { return AllowedOperations; }

          void set( unsigned Op )               { AllowedOperations = Op;     }
          void allowOperation( unsigned Op )    { AllowedOperations |= Op;    }
          void disallowOperation( unsigned Op ) { AllowedOperations &= (~Op); }  

          bool isAllowed( unsigned Op ) const { return ( Op & AllowedOperations ) != 0; } 
      };

      class mouseHandler 
      {
        private:
          unsigned EnabledOperations;

        protected:
          plot &Plot;

        protected:
          double plotWidth()  const; 
          double plotHeight() const; 
          
          void mulZoomX( double Z ); 
          void mulZoomY( double Z ); 

          void addShiftX( double S ); 
          void addShiftY( double S ); 

          void resetScales();

          fpoint wpoint2fpoint( wpoint Point ) const;

        public:
          mouseHandler( plot &P ) : Plot(P) {}
          virtual ~mouseHandler() {}

          virtual unsigned requestedAllowing() const { return allowing::Everything; };
          bool isAllowed( const allowing &Allowing ) const { return Allowing.isAllowed( requestedAllowing() ); }
      };

      class mouseActionHandler : public mouseHandler
      {
        private:
          const wpoint InitPoint;
          wpoint LastPoint;

        protected:
          void setLastPositions( wpoint Pt ) { LastPoint = Pt; }

          wpoint initPoint() const { return InitPoint; }
          wpoint lastPoint() const { return LastPoint; }

        public:
          mouseActionHandler( plot &P, wpoint Point ) : mouseHandler(P), InitPoint(Point), LastPoint(Point) {}
          virtual ~mouseActionHandler() {}

          virtual void moved( wpoint ) {}
          virtual void released( wpoint ) {}
      };

      class noneAction : public mouseActionHandler
      {
        public:
          noneAction( plot &Plot, wpoint Point = wpoint::zero() ) : mouseActionHandler(Plot,Point) {}
          unsigned requestedAllowing() const { return allowing::Everything; }
      };

      class moveAction : public mouseActionHandler
      {
        public:
          moveAction( plot &P, wpoint Point ) : mouseActionHandler(P,Point) {}
          void moved( wpoint Point );
          unsigned requestedAllowing() const { return allowing::Move; }
      };

      class zoomAction : public mouseActionHandler
      {
        private:
          void applyShifts( wpoint Point );
          void applyZooms( wpoint Point );
          bool needToApplyChanges( wpoint Point );
        public:
          zoomAction( plot &P, wpoint Point );
          void moved( wpoint Point );
          void released( wpoint Point );
          unsigned requestedAllowing() const { return allowing::Zoom; }
      };

      class resetAction : public mouseActionHandler
      {
        private:
          void reset();
        public:
          resetAction( plot &P, wpoint Point ) : mouseActionHandler(P,Point) {}
          void released( wpoint Point );
          unsigned requestedAllowing() const { return allowing::Reset; }
      };
      
      class moveFloatAction : public mouseActionHandler
      {
        private:
          floatRectangle *Float;
        public:
          moveFloatAction( plot &P, wpoint Point );
          void moved( wpoint Point );
          unsigned requestedAllowing() const { return allowing::MoveFloat; }
      };

      class selectAction : public mouseActionHandler
      {
        protected:
          selectionStrip *Selection;
          void setSelectionInterval( wpoint Point ); 
          virtual selectionStrip* createSelection() = 0;
          void initSelection();

        public:
          selectAction( plot &P, wpoint Point );
          void moved( wpoint Point );
          void released( wpoint Point );
      };

      class selectHorizontalAction : public selectAction
      {
        protected:
          selectionStrip* createSelection();

        public:
          selectHorizontalAction( plot &P, wpoint Point ) : selectAction(P,Point) { initSelection(); } 
          unsigned requestedAllowing() const { return allowing::SelectH; }
      };

      class selectVerticalAction : public selectAction
      {
        protected:
          selectionStrip* createSelection();

        public:
          selectVerticalAction( plot &P, wpoint Point ) : selectAction(P,Point) { initSelection(); } 
          unsigned requestedAllowing() const { return allowing::SelectV; }
      };

      class moveSelectionAction : public mouseActionHandler
      {
        protected:
          selectionStrip *Selection;
          void changeSelectionInterval( wpoint Point );
          selectionStrip* getSelection( wpoint Point );

        public:
          moveSelectionAction( plot &P, wpoint Point );
          void moved( wpoint Point );
          unsigned requestedAllowing() const { return allowing::MoveSelect; }
      };
      
      class resetSelectionAction : public mouseActionHandler
      {
        public:
          resetSelectionAction( plot &P, wpoint Point ) : mouseActionHandler(P,Point) {}
          void released( wpoint );
          unsigned requestedAllowing() const { return allowing::ResetSelect; }
      };

      class mouseWheelHandler : public mouseHandler
      {
        protected:
          virtual double deltaDumpFactor() const { return 0.25; }

        public:
          mouseWheelHandler( plot &P ) : mouseHandler(P) {}
          virtual void wheel( wpoint, wheeldelta ) {};
      };

      class mouseNoneWheel : public mouseWheelHandler 
      {
        public:
          mouseNoneWheel( plot &P ) : mouseWheelHandler(P) {}
          unsigned requestedAllowing() const { return allowing::Everything; }
      };

      class mouseHorizontalWheel : public mouseWheelHandler
      {
        public:
          mouseHorizontalWheel( plot &P ) : mouseWheelHandler(P) {}
          void wheel( wpoint, wheeldelta Delta );
          unsigned requestedAllowing() const { return allowing::WheelH; }
      };
      
      class mouseVerticalWheel : public mouseWheelHandler
      {
        public:
          mouseVerticalWheel( plot &P ) : mouseWheelHandler(P) {}
          void wheel( wpoint, wheeldelta Delta );
          unsigned requestedAllowing() const { return allowing::WheelV; }
      };

      class mouseZoomWheel : public mouseWheelHandler
      {
        protected:
          virtual double deltaDumpFactor() const { return 0.001; }
        public:
          mouseZoomWheel( plot &P ) : mouseWheelHandler(P) {}
          void wheel( wpoint, wheeldelta Delta );
          unsigned requestedAllowing() const { return allowing::Zoom; }
      };


    private:
      plot &Plot;
      bool ReplotOnMouseActions;
      
      mouseActionHandler *ActionHandler;
      mouseWheelHandler *WheelHandler;

      allowing AllowedOperations;

      wpoint LastPosition;

    protected:
      mouseActionHandler* createMouseActionHandler( wpoint Point, unsigned Buttons );
      void replaceActionHandler( mouseActionHandler *NewHandler );
      void replaceActionHandler( wpoint Point, unsigned Buttons );
      void setNoneActionHandler();

      mouseWheelHandler* createMouseWheelHandler( unsigned Buttons );
      void replaceWheelHandler( mouseWheelHandler *NewHandler );
      void replaceWheelHandler( unsigned Buttons );
      void setNoneWheelHandler();

      bool isNoneHandler() const;

      void updateLastPosition( const wpoint Point );
      void replot();

    public:
      mouse( plot &P ); 
      virtual ~mouse();

      void mousePressed( wpoint Point, unsigned Buttons ); 
      void mouseMoved( wpoint Point ); 
      void mouseReleased( wpoint Point ); 
      void mouseDoubleClicked( wpoint Point ); 
      void mouseWheel( wpoint Point, wheeldelta Delta, unsigned Buttons );

      mouseActionHandler* actionHandler() { return ActionHandler; }

      void setAllowedOperations( unsigned Op )    { AllowedOperations.allowOperation(Op); }
      void setDisallowedOperations( unsigned Op ) { AllowedOperations.disallowOperation(Op); }
      void setOperations( unsigned Op )           { AllowedOperations.set(Op); }
      unsigned allowedOperations() const          { return AllowedOperations.value(); }

      wpoint lastPosition() const { return LastPosition; }

      void setReplotOnMouseActions( bool R ) { ReplotOnMouseActions = R; }
      bool replotOnMouseActions() const { return ReplotOnMouseActions; }
  };

// ============================================================

}

