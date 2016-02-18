
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
  
// ============================================================

#include "scigraphics/numbers.h"
#include "scigraphics/geometry.h"
  
// ============================================================

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
            MoveAndZoom     = Move | Zoom,
            WheelH          = 0x00000100,
            WheelV          = 0x00000200,
            Wheel           = WheelH | WheelV,
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
          explicit allowing( unsigned AllowedOperations = Everything );

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
          explicit mouseHandler( plot &Plot );
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
          mouseActionHandler( plot &Plot, wpoint Point );
          virtual ~mouseActionHandler() {}

          virtual void moved( wpoint ) {}
          virtual void released( wpoint ) {}
      };

      class noneAction : public mouseActionHandler
      {
        public:
          explicit noneAction( plot &Plot, wpoint Point = wpoint::zero() );
          unsigned requestedAllowing() const { return allowing::Everything; }
      };

      class moveAction : public mouseActionHandler
      {
        public:
          moveAction( plot &Plot, wpoint Point );
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
          zoomAction( plot &Plot, wpoint Point );
          void moved( wpoint Point );
          void released( wpoint Point );
          unsigned requestedAllowing() const { return allowing::Zoom; }
      };

      class resetAction : public mouseActionHandler
      {
        private:
          void reset();
        public:
          resetAction( plot &Plot, wpoint Point );
          void released( wpoint Point );
          unsigned requestedAllowing() const { return allowing::Reset; }
      };
      
      class moveFloatAction : public mouseActionHandler
      {
        private:
          floatRectangle *Float;
        public:
          moveFloatAction( plot &Plot, wpoint Point );
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
          selectAction( plot &Plot, wpoint Point );
          void moved( wpoint Point );
          void released( wpoint Point );
      };

      class selectHorizontalAction : public selectAction
      {
        protected:
          selectionStrip* createSelection();

        public:
          selectHorizontalAction( plot &Plot, wpoint Point );
          unsigned requestedAllowing() const { return allowing::SelectH; }
      };

      class selectVerticalAction : public selectAction
      {
        protected:
          selectionStrip* createSelection();

        public:
          selectVerticalAction( plot &Plot, wpoint Point );
          unsigned requestedAllowing() const { return allowing::SelectV; }
      };

      class moveSelectionAction : public mouseActionHandler
      {
        protected:
          selectionStrip *Selection;
          void changeSelectionInterval( wpoint Point );
          selectionStrip* getSelection( wpoint Point );

        public:
          moveSelectionAction( plot &Plot, wpoint Point );
          void moved( wpoint Point );
          unsigned requestedAllowing() const { return allowing::MoveSelect; }
      };
      
      class resetSelectionAction : public mouseActionHandler
      {
        public:
          resetSelectionAction( plot &Plot, wpoint Point );
          void released( wpoint );
          unsigned requestedAllowing() const { return allowing::ResetSelect; }
      };

      class mouseWheelHandler : public mouseHandler
      {
        protected:
          virtual double deltaDumpFactor() const;

        public:
          explicit mouseWheelHandler( plot &Plot );
          virtual void wheel( wpoint, wheeldelta ) {};
      };

      class mouseNoneWheel : public mouseWheelHandler 
      {
        public:
          explicit mouseNoneWheel( plot &Plot );
          unsigned requestedAllowing() const { return allowing::Everything; }
      };

      class mouseHorizontalWheel : public mouseWheelHandler
      {
        public:
          explicit mouseHorizontalWheel( plot &Plot );
          void wheel( wpoint, wheeldelta Delta );
          unsigned requestedAllowing() const { return allowing::WheelH; }
      };
      
      class mouseVerticalWheel : public mouseWheelHandler
      {
        public:
          explicit mouseVerticalWheel( plot &Plot );
          void wheel( wpoint, wheeldelta Delta );
          unsigned requestedAllowing() const { return allowing::WheelV; }
      };

      class mouseZoomWheel : public mouseWheelHandler
      {
        protected:
          virtual double deltaDumpFactor() const;

        public:
          explicit  mouseZoomWheel( plot &Plot );
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
      explicit mouse( plot &Plot ); 
      ~mouse();

      void mousePressed( wpoint Point, unsigned Buttons ); 
      void mouseMoved( wpoint Point ); 
      void mouseReleased( wpoint Point ); 
      void mouseDoubleClicked( wpoint Point ); 
      void mouseWheel( wpoint Point, wheeldelta Delta, unsigned Buttons );

      mouseActionHandler* actionHandler();

      void setAllowedOperations( unsigned Operations );
      void setDisallowedOperations( unsigned Operations );
      void setOperations( unsigned Operations );
      unsigned allowedOperations() const;

      wpoint lastPosition() const;

      void setReplotOnMouseActions( bool IsReplot );
      bool replotOnMouseActions() const;
  };

// ============================================================

}

