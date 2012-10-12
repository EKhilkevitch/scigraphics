
#pragma once

#include "graphics/container_ptr.h"
#include "graphics/graphviewsequence.h"

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

      template < class view > bool isViewExist() { return Views.find_type<view>() != Views.end(); }
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

  template < class view > void graphViewSequencesCollection::eraseView() 
  {
    while ( true )
    {
      viewsList::iterator Iterator = Views.find_type<view>();
      if ( Iterator == Views.end() )
        break;
      Views.erase(Iterator);
    }
  }

  template < class view > view* graphViewSequencesCollection::getView() 
  { 
    viewsList::iterator Iterator = Views.find_type<view>();
    return ( Iterator == Views.end() ) ? NULL : dynamic_cast<view*>(*Iterator);
  }

  template < class view > const view* graphViewSequencesCollection::getView() const 
  {
    return ( const_cast< graphViewSequencesCollection* >(this) )->getView<view>();
  }
  
  template < class view > void graphViewSequencesCollection::setViewVisible( bool Show )
  {
    graphViewSequence *View = getView<view>();
    if ( View != NULL )
      View->setVisible( Show );
  }

  template < class view > typename view::style graphViewSequencesCollection::getViewStyle() const
  { 
    const view *View = getView<view>();
    if ( View == NULL )
      throw std::runtime_error("No such view in collection");
    return View->getStyle(); 
  }

  template <class view> void graphViewSequencesCollection::setViewStyle( typename view::style Style )
  {
    view *View = getView<view>();
    if ( View == NULL )
      throw std::runtime_error("No such view in collection");
    View->setStyle(Style); 
  }

  // ============================================================

}

