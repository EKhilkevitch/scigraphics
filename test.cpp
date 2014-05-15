
#include <list>

class x;

class n {};

template <class T> class tmpl 
{
  private:
    std::list<T*> List;

  public:
    tmpl() {}
    tmpl( T *a ) {}
    void f( n *a ) { if ( dynamic_cast<T*>(a) != 0 ) List.push_back( 0 ); }
};

class tmplX : public tmpl<x>
{
  public:

};

int main()
{
  tmplX TMPLX;
  return 0;
};

