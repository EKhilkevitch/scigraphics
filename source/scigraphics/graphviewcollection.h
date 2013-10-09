
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

#include "scigraphics/container_ptr.h"
#include "scigraphics/container_type_ptr.h"
#include "scigraphics/graphviewsequence.h"

#include <list>
#include <stdexcept>

namespace scigraphics 
{

// ============================================================

  class graphViewSequencesCollection : public graphViewSequence
  {
    public:
      typedef container_ptr< std::list, graphViewSequence > viewsList;

    private:
      color DefaultColor;
      viewsList Views;

    private:
      viewsList::iterator viewBegin() { return Views.begin(); }
      viewsList::iterator viewEnd()   { return Views.end(); }
      
    public:
      graphViewSequencesCollection() {}
      virtual ~graphViewSequencesCollection() {}

      void setDefaultColor( const color &Color );
      void setColor( const color &Color );
      color getColor() const;

      void clearViews() { Views.clear(); };
      void addView( graphViewSequence *View, bool Show = true );
      template < class view > void addView( typename view::style Style, bool Show = true );

      template < class view > void eraseView();
      void erase( graphViewSequence *V );

      template < class view > view* getView();
      template < class view > const view* getView() const;

      template < class view > void setViewVisible( bool Show );

      template < class view > bool isViewExist();
      template < class view > typename view::style getViewStyle() const;
      template < class view > void setViewStyle( typename view::style Style );
      
      void draw( painter &Painter, const pairScales& Scales, const dataSequence &Data ) const;
      void drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const;
      
      wcoord legendExampleWidth() const;
      wcoord legendExampleHeight() const;
  };

  class ordinarGraphSequenceViewCollection : public graphViewSequencesCollection
  {
    public:
      ordinarGraphSequenceViewCollection();
      
      void setLineWidth( unsigned Width );
      void setPointSize( unsigned Size );
  };

  class coveredAreaGraphSequenceViewCollection : public graphViewSequencesCollection
  {
    public:
      coveredAreaGraphSequenceViewCollection();
  };

  // ============================================================
  
  template < class view > void graphViewSequencesCollection::addView( typename view::style Style, bool Show ) 
  { 
    view *View = new view(Style);
    addView( View, Show ); 
  }
  
  // ------------------------------------------------------------

  template < class view > void graphViewSequencesCollection::eraseView() 
  {
    while ( true )
    {
      viewsList::iterator Iterator = find_type<view>( Views.begin(), Views.end() );
      if ( Iterator == Views.end() )
        break;
      Views.erase(Iterator);
    }
  }

  // ------------------------------------------------------------
  
  template < class view > view* graphViewSequencesCollection::getView() 
  { 
    viewsList::iterator Iterator = find_type<view>( Views.begin(), Views.end() );
    return ( Iterator == Views.end() ) ? NULL : dynamic_cast<view*>(&*Iterator);
  }
  
  // ------------------------------------------------------------

  template < class view > const view* graphViewSequencesCollection::getView() const 
  {
    return ( const_cast< graphViewSequencesCollection* >(this) )->getView<view>();
  }
  
  // ------------------------------------------------------------
  
  template < class view > void graphViewSequencesCollection::setViewVisible( bool Show )
  {
    graphViewSequence *View = getView<view>();
    if ( View != NULL )
      View->setVisible( Show );
  }
  
  // ------------------------------------------------------------

  template < class view > typename view::style graphViewSequencesCollection::getViewStyle() const
  { 
    const view *View = getView<view>();
    if ( View == NULL )
      throw std::runtime_error("No such view in collection");
    return View->getStyle(); 
  }
  
  // ------------------------------------------------------------

  template <class view> void graphViewSequencesCollection::setViewStyle( typename view::style Style )
  {
    view *View = getView<view>();
    if ( View == NULL )
      throw std::runtime_error("No such view in collection");
    View->setStyle(Style); 
  }
  
  // ------------------------------------------------------------

      
  template < class view > bool graphViewSequencesCollection::isViewExist() 
  { 
    return find_type<view>( Views.begin(), Views.end() ) != Views.end(); 
  }

  // ============================================================

}

