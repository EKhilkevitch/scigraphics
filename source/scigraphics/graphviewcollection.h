
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

#include <stdexcept>

namespace scigraphics 
{
  namespace sequence
  {

    // ============================================================

    class graphViewCollection : public graphView
    {
      public:
        typedef container_ptr< graphView > viewsList;

      private:
        color DefaultColor;
        viewsList Views;

      private:
        viewsList::iterator viewBegin() { return Views.begin(); }
        viewsList::iterator viewEnd()   { return Views.end(); }
        
      public:
        graphViewCollection();
        virtual ~graphViewCollection();

        void setDefaultColor( const color &Color );
        void setColor( const color &Color );
        color getColor() const;

        void clearViews() { Views.clear(); };
        void addView( graphView *View, bool Show = true );
        template < class view > void addView( typename view::style Style, bool Show = true );

        template < class view > void eraseView();
        void erase( graphView *V );

        template < class view > view* getView();
        template < class view > const view* getView() const;

        template < class view > void setViewVisible( bool Show );
        template < class view > bool isViewVisible() const;
        void setAllViewsVisible( bool Show );

        template < class view > bool isViewExist();
        template < class view > typename view::style getViewStyle() const;
        template < class view > void setViewStyle( typename view::style Style );
        
        void draw( painter &Painter, const pairScales& Scales, const sequence::data &Data ) const;
        void drawLegendExample( painter &Painter, const wrectangle &Rectangle ) const;
        
        wcoord legendExampleWidth() const;
        wcoord legendExampleHeight() const;
    };
    
    // ------------------------------------------------------------

    class ordinarGraphViewCollection : public graphViewCollection
    {
      public:
        ordinarGraphViewCollection();
        
        void setLineStyle( lineStyle::style LineStyle );
        void setLineWidth( unsigned Width );
        void setPointSize( unsigned Size );
    };
    
    // ------------------------------------------------------------

    class coveredAreaGraphViewCollection : public graphViewCollection
    {
      public:
        coveredAreaGraphViewCollection();
    };

    // ============================================================
    
    template < class view > void graphViewCollection::addView( typename view::style Style, bool Show ) 
    { 
      view *View = new view(Style);
      addView( View, Show ); 
    }
    
    // ------------------------------------------------------------

    template < class view > void graphViewCollection::eraseView() 
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
    
    template < class view > view* graphViewCollection::getView() 
    { 
      viewsList::iterator Iterator = find_type<view>( Views.begin(), Views.end() );
      return ( Iterator == Views.end() ) ? NULL : dynamic_cast<view*>(&*Iterator);
    }
    
    // ------------------------------------------------------------

    template < class view > const view* graphViewCollection::getView() const 
    {
      return ( const_cast< graphViewCollection* >(this) )->getView<view>();
    }
    
    // ------------------------------------------------------------
    
    template < class view > void graphViewCollection::setViewVisible( bool Show )
    {
      graphView *View = getView<view>();
      if ( View != NULL )
        View->setVisible( Show );
    }
    
    // ------------------------------------------------------------
        
    template < class view > bool graphViewCollection::isViewVisible() const
    {
      const graphView *View = getView<view>();
      if ( View != NULL )
        return View->isVisible();
      return false;
    }
    
    // ------------------------------------------------------------

    template < class view > typename view::style graphViewCollection::getViewStyle() const
    { 
      const view *View = getView<view>();
      if ( View == NULL )
        throw std::runtime_error("No such view in collection");
      return View->getStyle(); 
    }
    
    // ------------------------------------------------------------

    template <class view> void graphViewCollection::setViewStyle( typename view::style Style )
    {
      view *View = getView<view>();
      if ( View == NULL )
        throw std::runtime_error("No such view in collection");
      View->setStyle(Style); 
    }
    
    // ------------------------------------------------------------
    
    template < class view > bool graphViewCollection::isViewExist() 
    { 
      return find_type<view>( Views.begin(), Views.end() ) != Views.end(); 
    }

    // ============================================================

  }
}

