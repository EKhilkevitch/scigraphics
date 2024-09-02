
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

#include "scigraphics/numbers.h"

#include <ostream>

// ============================================================

namespace scigraphics
{

// ============================================================

  template <class T> class point
  {
    private:
      T X, Y;

    public:
      point( T x, T y );
      template <class pnt> point( const pnt &Point );

      T x() const { return X; }
      T y() const { return Y; }

      point<T>& moveX( T Delta );
      point<T>& moveY( T Delta );
      point<T>& moveXY( T DeltaX, T DeltaY );

      static point<T> zero();
  };

  template <class T> bool operator==( const point<T> &P1, const point<T> &P2 );
  template <class T> bool operator!=( const point<T> &P1, const point<T> &P2 );
  template <class T> std::ostream& operator<<( std::ostream &Stream, point<T> Point );

  // ============================================================

  template <class T> class rectangle
  {
    private:
      point<T> CornerLD, CornerRU;

    private:
      static T abs( T Number );
      static T min( T Number1, T Number2 );
      static T max( T Number1, T Number2 );

    public:
      rectangle();
      rectangle( const point<T> &Point1, const point<T> &Point2 );
      
      point<T>  leftUp() const;
      point<T>  rightDown() const;
      point<T>  leftDown() const;
      point<T>  rightUp() const;
      point<T>  center() const;

      T left() const   { return CornerLD.x(); }
      T right() const  { return CornerRU.x(); }
      T up() const     { return CornerRU.y(); }
      T down() const   { return CornerLD.y(); }
      T width() const;
      T height() const;
      
      template <class pnt> bool contain( const point<pnt> &Point ) const;
      
      void moveX( T Delta );
      void moveY( T Delta );
      void moveXY( T DeltaX, T DeltaY );
  };

  template <class T> bool operator==( const rectangle<T> &R1, const rectangle<T> &R2 );
  template <class T> std::ostream& operator<<( std::ostream &Stream, const rectangle<T> &Rectangle );

  // ============================================================
     
  template <class T> class indents
  {
    private:
      T Left, Right;
      T Up, Down; 

    public:
      indents();
      indents( T Left, T Right, T Up, T Down );

      T left() const { return Left; }
      void setLeft( T Value ) { Left = Value; }

      T right() const { return Right; }
      void setRight( T Value ) { Right = Value; }

      T up() const { return Up; }
      void setUp( T Value ) { Up = Value; }

      T down() const { return Down; }
      void setDown( T Value ) { Down = Value; }

      rectangle<T> apply( const rectangle<T> &Rectangle ) const;
  };

  // ============================================================

  typedef point<wcoord> wpoint;
  typedef rectangle<wcoord> wrectangle;

  typedef point<fcoord> fpoint;
  typedef rectangle<fcoord> frectangle;
  
  typedef point<number> npoint;
  typedef rectangle<number> nrectangle;

  // ============================================================

  template <class T> point<T>::point( T x, T y ) : 
        X(x), 
        Y(y) 
  {
  }
  
  // ------------------------------------------------------------
      
  template <class T> template <class pnt> point<T>::point( const pnt &Pnt ) : 
        X( Pnt.x() ), 
        Y( Pnt.y() ) 
  {
  }
  
  // ------------------------------------------------------------
      
  template <class T> point<T>& point<T>::moveX( T Delta ) 
  { 
    X += Delta; 
    return *this; 
  }
  
  // ------------------------------------------------------------
  
  template <class T> point<T>& point<T>::moveY( T Delta ) 
  { 
    Y += Delta; 
    return *this; 
  }
  
  // ------------------------------------------------------------
  
  template <class T> point<T>& point<T>::moveXY( T DX, T DY ) 
  { 
    moveX(DX); 
    moveY(DY); 
    return *this; 
  }
  
  // ------------------------------------------------------------

  template <class T> point<T> point<T>::zero() 
  { 
    return point<T>( 0, 0 ); 
  }

  // ------------------------------------------------------------
  
  template <class T> bool operator==( const point<T> &Point1, const point<T> &Point2 ) 
  { 
    return Point1.x() == Point2.x() && Point1.y() == Point2.y(); 
  }
  
  // ------------------------------------------------------------
  
  template <class T> bool operator!=( const point<T> &Point1, const point<T> &Point2 ) 
  { 
    return !( Point1 == Point2 ); 
  }
  
  // ------------------------------------------------------------
  
  template <class T> std::ostream& operator<<( std::ostream &Stream, point<T> Point ) 
  { 
    Stream << "( " << Point.x() << " : " << Point.y() << " )"; 
    return Stream; 
  }

  // ============================================================
  
  template <class T> T rectangle<T>::abs( T Number )
  { 
    return Number >= static_cast<T>(0) ? +Number : -Number; 
  }
      
  // ------------------------------------------------------------
  
  template <class T> T rectangle<T>::min( T Number1, T Number2 )
  {
    return Number1 < Number2 ? Number1 : Number2;
  }
  
  // ------------------------------------------------------------
  
  template <class T> T rectangle<T>::max( T Number1, T Number2 )
  {
    return Number1 > Number2 ? Number1 : Number2;
  }
  
  // ------------------------------------------------------------
  
  template <class T> rectangle<T>::rectangle() : 
        CornerLD( point<T>::zero() ), 
        CornerRU( point<T>::zero() ) 
  {
  }

  // ------------------------------------------------------------

  template <class T> rectangle<T>::rectangle( const point<T>& Point1, const point<T>& Point2 ) :
         CornerLD( min( Point1.x(), Point2.x() ), min( Point1.y(), Point2.y() ) ), 
         CornerRU( max( Point1.x(), Point2.x() ), max( Point1.y(), Point2.y() ) ) 
  {
  }
  
  // ------------------------------------------------------------
      
  template <class T> point<T>  rectangle<T>::leftUp()    const 
  { 
    return point<T>( left(), up() ); 
  }

  // ------------------------------------------------------------
  
  template <class T> point<T>  rectangle<T>::rightDown() const 
  { 
    return point<T>( right(), down() ); 
  }
  
  // ------------------------------------------------------------
  
  template <class T> point<T>  rectangle<T>::leftDown()  const 
  { 
    return point<T>( left(), down() ); 
  }

  // ------------------------------------------------------------
  
  template <class T> point<T>  rectangle<T>::rightUp()   const 
  { 
    return point<T>( right(), up() ); 
  }

  // ------------------------------------------------------------
  
  template <class T> point<T>  rectangle<T>::center()    const 
  { 
    return point<T>( (left()+right())/2, (up()+down())/2 ); 
  }

  // ------------------------------------------------------------
  
  template <class T> T rectangle<T>::width() const  
  { 
    return abs( right() - left() ); 
  }
  
  // ------------------------------------------------------------
  
  template <class T> T rectangle<T>::height() const 
  { 
    return abs( down() - up() ); 
  }
  
  // ------------------------------------------------------------
      
  template <class T> void rectangle<T>::moveX( T Delta ) 
  { 
    CornerLD.moveX(Delta); 
    CornerRU.moveX(Delta); 
  }
  
  // ------------------------------------------------------------
  
  template <class T> void rectangle<T>::moveY( T Delta ) 
  { 
    CornerLD.moveY(Delta); 
    CornerRU.moveY(Delta); 
  }
  
  // ------------------------------------------------------------
  
  template <class T> void rectangle<T>::moveXY( T DeltaX, T DeltaY ) 
  { 
    moveX(DeltaX); 
    moveY(DeltaY); 
  }

  // ------------------------------------------------------------
      
  template <class T> template <class pnt> bool rectangle<T>::contain( const point<pnt> &Point ) const 
  { 
    return left() <= Point.x()  && 
           Point.x() <= right() && 
           down() <= Point.y()  && 
           Point.y() <= up(); 
  }
  
  // ------------------------------------------------------------
 
  template <class T> bool operator==( const rectangle<T> &R1, const rectangle<T> &R2 ) 
  { 
    return R1.leftUp()==R2.leftUp() && 
           R1.rightDown() == R2.rightDown(); 
  }

  // ------------------------------------------------------------
  
  template <class T> std::ostream& operator<<( std::ostream &Stream, const rectangle<T> &Rectangle ) 
  {
    Stream << "[ " << Rectangle.leftDown() << " .. " << Rectangle.rightUp() << " ]"; 
    return Stream;
  }
  
  // ============================================================

  template <class T> indents<T>::indents() : 
    Left(0), 
    Right(0),
    Up(0),
    Down(0) 
  {
  }

  // ------------------------------------------------------------
      
  template <class T> indents<T>::indents( T L, T R, T U, T D ) :
    Left(L), 
    Right(R), 
    Up(U), 
    Down(D) 
  {
  }

  // ------------------------------------------------------------

  template <class T> rectangle<T> indents<T>::apply( const rectangle<T> &R ) const 
  {
    T NewLeft  = R.left() + Left;
    T NewRight = R.right() - Right;
    T NewUp    = R.up() - Up;
    T NewDown  = R.down() + Down;

    if ( NewLeft >= NewRight )
      NewLeft = NewRight = ( NewLeft+NewRight)/2;

    if ( NewDown >= NewUp )
      NewUp = NewDown = ( NewUp + NewDown )/2;

    return rectangle<T>( point<T>( NewLeft, NewUp ),
                         point<T>( NewRight, NewDown ) );
  }

  // ============================================================

}

