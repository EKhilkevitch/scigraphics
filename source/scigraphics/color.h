
#pragma once

#include <string>
#include <cstdio>

#ifdef _WIN32
#  define snprintf _snprintf
#endif

#include "scigraphics/interval.h"

namespace scigraphics
{

// ============================================================

    class color
    {
      public:
        typedef unsigned rgb;
        static const rgb Black          = 0x000000;
        static const rgb White          = 0xFFFFFF;
        static const rgb Blue           = 0x0000FF;
        static const rgb Green          = 0x00FF00;
        static const rgb Red            = 0xFF0000;
        static const rgb Cyan           = 0x00FFFF;
        static const rgb Yellow         = 0xFFFF00;
        static const rgb Magenta        = 0xFF00FF;
        static const rgb DarkRed        = 0x800000;
        static const rgb DarkBlue       = 0x000080;
        static const rgb DarkGreen      = 0x008000;
        static const rgb DarkYellow     = 0x808000;
        static const rgb DarkCyan       = 0x008080;
        static const rgb DarkMagenta    = 0x800080;
        static const rgb Transparency   = 0xFF000000;

      private:
        unsigned RGB;

      public:
        static color fromRGB( unsigned R, unsigned G, unsigned B, unsigned T = 0 );
        static color fromHSV( unsigned H, unsigned S, unsigned V, unsigned T = 0 );

        color( rgb Value = Black ) : RGB(Value) {}
        color( int R, int G, int B, int T = 0x00 ) 
        { 
          interval<int> Lims(0,0xFF); 
          RGB = ( Lims.toInterval(T) << 24 ) |
                ( Lims.toInterval(R) << 16 ) | 
                ( Lims.toInterval(G) << 8  ) | 
                ( Lims.toInterval(B) << 0  );
        }
        virtual ~color() {};

        color& operator=( rgb Value ) { RGB = Value; return *this; } 

        virtual unsigned valueRgb()      const { return RGB; }
        unsigned int red()   const { return ( valueRgb() >> 16 ) & 0xFF; }
        unsigned int green() const { return ( valueRgb() >>  8 ) & 0xFF; }
        unsigned int blue()  const { return ( valueRgb() >>  0 ) & 0xFF; }
        unsigned int transparency() const { return ( valueRgb() >> 24 ) & 0xFF; }

        unsigned maxRGB() const;
        unsigned minRGB() const;

        unsigned hue() const;
        unsigned saturation() const;
        unsigned value() const;

        void setTransparency( double Part );
        virtual std::string name( bool WithPrefix = true ) const;

        void darker( int Value )
        {
          *this = color( red()-Value, green()-Value, blue()-Value, transparency() ); 
        }

        static color nextColor();
        static color predefinedColor( unsigned Index );
        static color thisColor();
        static void resetNextColorSequence();
    };

    inline bool operator==( color C1, color C2 ) { return C1.valueRgb() == C2.valueRgb(); }


// ============================================================

}

