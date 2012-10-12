
// ============================================================

#pragma once

// ============================================================

#include "graphics/numbers.h"
#include <algorithm>
#include <ostream>

// ============================================================

namespace graphics
{

// ============================================================

  template <class N> class point
  {
    private:
      N X, Y;
    public:
      point( N x, N y ) : X(x), Y(y) {}
      template <class pnt> point( const pnt &P ) : X(P.x()), Y(P.y()) {}
      N x() const { return X; }
      N y() const { return Y; }

      point<N>& moveX( N Delta ) { X += Delta; return *this; }
      point<N>& moveY( N Delta ) { Y += Delta; return *this; }
      point<N>& moveXY( N DX, N DY ) { moveX(DX); moveY(DY); return *this; }

      static point<N> zero() { return point<N>(0,0); }
  };

  template <class T> bool operator==( const point<T> &P1, const point<T> &P2 ) { return P1.x() == P2.x() && P1.y() == P2.y(); }
  template <class T> bool operator!=( const point<T> &P1, const point<T> &P2 ) { return !( P1 == P2 ); }
  template <class T> std::ostream& operator<<( std::ostream &Stream, point<T> Point ) 
  { 
    Stream << "( " << Point.x() << " : " << Point.y() << " )"; 
    return Stream; 
  }

  // ============================================================

  template <class N> class rectangle
  {
    private:
      point<N> CornerLD, CornerRU;
    protected:
      static N abs( N Val ) { return Val >= 0 ? Val : -Val; }

    public:
      rectangle() : CornerLD( point<N>::zero() ), CornerRU( point<N>::zero() ) {}
      rectangle( const point<N>& Pt1, const point<N>& Pt2 ) :
         CornerLD( std::min( Pt1.x(), Pt2.x() ), std::min( Pt1.y(), Pt2.y() ) ), 
         CornerRU( std::max( Pt1.x(), Pt2.x() ), std::max( Pt1.y(), Pt2.y() ) ) {}
      
      point<N>  leftUp()    const { return point<N>( left(), up() ); }
      point<N>  rightDown() const { return point<N>( right(), down() ); }
      point<N>  leftDown()  const { return point<N>( left(), down() ); }
      point<N>  rightUp()   const { return point<N>( right(), up() ); }
      point<N>  center()    const { return point<N>( (left()+right())/2, (up()+down())/2 ); }

      N left() const   { return CornerLD.x(); }
      N right() const  { return CornerRU.x(); }
      N up() const     { return CornerRU.y(); }
      N down() const   { return CornerLD.y(); }
      N width() const  { return abs( right() - left() ); }
      N height() const { return abs( down() - up() ); }
      
      template <class T> bool contain( const point<T> &Point ) const 
        { return left() <= Point.x() && Point.x() <= right() && down() <= Point.y() && Point.y() <= up(); }
      
      void moveX( N Delta ) { CornerLD.moveX(Delta); CornerRU.moveX(Delta); }
      void moveY( N Delta ) { CornerLD.moveY(Delta); CornerRU.moveY(Delta); }
      void moveXY( N DeltaX, N DeltaY ) { moveX(DeltaX); moveY(DeltaY); }
  };

  template <class T> bool operator==( const rectangle<T> &R1, const rectangle<T> &R2 ) { return R1.leftUp()==R2.leftUp() && R1.rightDown() == R2.rightDown(); }
  template <class T> std::ostream& operator<<( std::ostream &Stream, const rectangle<T> &Rectangle ) 
  {
    Stream << "[ " << Rectangle.leftDown() << " .. " << Rectangle.rightUp() << " ]"; 
    return Stream;
  }

  // ============================================================
     
  template <class N> class indents
  {
    private:
      N Left, Right;
      N Up, Down; 
    public:
      indents() : Left(0), 
                  Right(0),
                  Up(0),
                  Down(0) {}
      indents( N L, N R, N U, N D ) :
                 Left(L), Right(R), Up(U), Down(D) {}

      N left() const { return Left; }
      void setLeft( N Value ) { Left = Value; }

      N right() const { return Right; }
      void setRight( N Value ) { Right = Value; }

      N up() const { return Up; }
      void setUp( N Value ) { Up = Value; }

      N down() const { return Down; }
      void setDown( N Value ) { Down = Value; }

      rectangle<N> apply( const rectangle<N> &R ) const 
      {
        N NewLeft  = R.left() + Left;
        N NewRight = R.right() - Right;
        N NewUp    = R.up() - Up;
        N NewDown  = R.down() + Down;

        if ( NewLeft >= NewRight )
          NewLeft = NewRight = ( NewLeft+NewRight)/2;
        if ( NewDown >= NewUp )
          NewUp = NewDown = ( NewUp + NewDown )/2;

        return rectangle<N>( point<N>( NewLeft, NewUp ),
                             point<N>( NewRight, NewDown ) );
      }
  };

// ============================================================

}

