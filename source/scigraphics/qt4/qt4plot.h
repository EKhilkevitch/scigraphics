
#pragma once

// ============================================================

#include "scigraphics/plot.h"
#include "scigraphics/color.h"
#include "scigraphics/drawer.h"

#include <QLabel>
#include <QWidget>
#include <QGraphicsView>
#include <QColor>
#include <QFont>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QMouseEvent>
#include <QWheelEvent>

// ================================================================

class QPixmap;
class QGraphicsView;
class QGraphicsScene;
class QGraphicsPixmapItem;

class qt4plot;

// ================================================================

class qt4plotView : public QGraphicsView
{
    Q_OBJECT

    bool DragEnabled;

  protected:
    virtual void resizeEvent( QResizeEvent* )                   { emit resized(); }
    virtual void mousePressEvent( QMouseEvent *Event )          { emit mousePressed( Event ); }
    virtual void mouseMoveEvent( QMouseEvent *Event )           { emit mouseMoved( Event ); }
    virtual void mouseReleaseEvent( QMouseEvent *Event )        { emit mouseReleased( Event ); } 
    virtual void mouseDoubleClickEvent( QMouseEvent *Event )    { emit mouseDoubleClick( Event ); }
    virtual void keyReleaseEvent( QKeyEvent *Event )            { emit keyReleased( Event ); }
    virtual void wheelEvent( QWheelEvent* Event )               { emit mouseWheel( Event ); }
   
    virtual void dragMoveEvent(QDragMoveEvent *event);
    virtual void dragEnterEvent( QDragEnterEvent *Event );
    virtual void dropEvent( QDropEvent *Event );

  public:
    qt4plotView( QWidget* Parent = 0 ) : QGraphicsView(Parent), DragEnabled(false) { setMouseTracking(true); }
    qt4plotView( QGraphicsScene *Scene, QWidget *Parent = 0 ) : QGraphicsView(Scene,Parent), DragEnabled(false) {}

    void enableDrop( const bool Enable ) { setAcceptDrops(Enable); DragEnabled = Enable; }


  signals:
    void resized();

    void mousePressed( QMouseEvent *Event );
    void mouseMoved( QMouseEvent *Event );
    void mouseReleased( QMouseEvent *Event );
    void mouseDoubleClick( QMouseEvent *Event );
    void mouseWheel( QWheelEvent* Event );
    void keyReleased( QKeyEvent *Event );

    void dropped( QString );
};

// ----------------------------------------------------------------

class drawerQt : public scigraphics::drawer
{
  private:
    QWidget *const Parent;

    qt4plotView *View;
    QGraphicsScene *Scene;
    QPainter *Painter;
    QGraphicsPixmapItem *PixmapItem;
    QPixmap *PlotPixmap;

  public:
    static QColor colorQt( const scigraphics::color& Color )         { return QColor( Color.red(), Color.green(), Color.blue(), 0xFF - Color.transparency() ); }
    static QPoint pointQt( const scigraphics::wpoint& Point )        { return QPoint(Point.x(),Point.y()); }
    static QRect  rectangleQt( const scigraphics::wrectangle& Rect );
    static QPen   penQt( const scigraphics::lineStyle& Style );
    static QFont  fontQt( const scigraphics::textStyle &Style );
    static QBrush brushQt( const scigraphics::brushStyle &Style );
    static QPolygon polygonQt( const std::vector<scigraphics::wpoint> &Points );
   
  public:
    void drawLine( const scigraphics::wpoint &A, const scigraphics::wpoint &B, const scigraphics::lineStyle &Style );
    void drawRectangle( const scigraphics::wrectangle& Rectangle, const scigraphics::brushStyle& BrushStyle, const scigraphics::lineStyle &LineStyle );
    void drawPolygon( const std::vector<scigraphics::wpoint> &Points, const scigraphics::brushStyle& BrushStyle );
    void drawText( const std::string &Text, const scigraphics::wrectangle& Rectangle, const scigraphics::textStyle &Style, double RotAngle );
    void eraseRectangle( const scigraphics::wrectangle& Rectangle );
    void eraseAll();
    void flush();

    scigraphics::wcoord textWidth( const std::string &Text, const scigraphics::textStyle &Style );
    scigraphics::wcoord textHeight( const std::string &Text, const scigraphics::textStyle &Style );
    
    scigraphics::wcoord width()  const { return Scene == NULL ? 0 : Scene->width(); };
    scigraphics::wcoord height() const { return Scene == NULL ? 0 : Scene->height(); };

  public:
    drawerQt( QWidget *Parent );

    QGraphicsScene* scene() { return Scene; }
    qt4plotView* view() { return View; }
    QPainter* painter() { return Painter; }
};

// ----------------------------------------------------------------

class qt4plotMouseCallBack : public scigraphics::mouseCallBack
{
  protected:
    qt4plot& getQt4Plot(); 
    
    static bool isSelectionMouseAction( scigraphics::mouse::mouseActionHandler* );

  public:
    qt4plotMouseCallBack( qt4plot &Plot );

    void onPressed( scigraphics::mouse::mouseActionHandler* );
    void onMoved( scigraphics::mouse::mouseActionHandler* );
    void onRelease( scigraphics::mouse::mouseActionHandler* );
    void onWheel( scigraphics::mouse::mouseWheelHandler* );
};

// ----------------------------------------------------------------

class qt4plot : public QWidget, public scigraphics::plot
{
  Q_OBJECT

    friend class qt4plotMouseCallBack;

  protected:
    drawerQt* getDrawerQt() { return dynamic_cast<drawerQt*>( getDrawer() ); }

    virtual void resizeEvent( QResizeEvent* ) { resizePlot(); }

    int sceneShift() const { return 10; }
    void printTestCornerRectangles();

    static unsigned plotMouseModifiers( Qt::KeyboardModifiers Modifiers );
    static unsigned plotMouseButtons( const QMouseEvent *Event );
    static unsigned plotMouseButtons( QWheelEvent *Event ) { return plotMouseModifiers(Event->modifiers()); }
    static scigraphics::wpoint plotMousePoisition( const QMouseEvent *Event );
    static scigraphics::wpoint plotMousePoisition( const QWheelEvent *Event );

    static QPixmap createTranspPixmap( const int Width, const int Height );
    
    void emitPositionObtained( const QMouseEvent *Event );
    void emitPlotChangedByMouse();
    
  public:
    qt4plot( QWidget* Parent = NULL, Qt::WindowFlags Flags = 0 );
    virtual ~qt4plot() {};

    void resize( int w, int h ) { QWidget::resize( w, h ); resizePlot(); }
    void enableDrop( bool E ) { getDrawerQt()->view()->enableDrop(E); }

    void emitSelectionChanged() { emit selectionChanged(); }
    void emitSelectionChangingFinished() { emit selectionChangingFinished(); }
   
  protected slots:
    void resizePlot();

    void mousePressed( QMouseEvent *Event );
    void mouseMoved( QMouseEvent *Event );
    void mouseReleased( QMouseEvent *Event );
    void mouseDoubleClicked( QMouseEvent *Event );
    void mouseWheel( QWheelEvent *Event );

  public slots:
    void replot() { plot::replot(); }
    void setCrossCursor( bool Set );

  signals:
    void selectionChangingFinished();
    void selectionChanged();
    void dropped( QString );
    void positionObtained( double X, double Y );
    void plotChangedByMouse();
};

// ============================================================

