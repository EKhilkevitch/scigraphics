
#pragma once

// ============================================================

#include "graphics/plot.h"
#include "graphics/color.h"
#include "graphics/drawer.h"

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

class drawerQt : public graphics::drawer
{
  private:
    QWidget *const Parent;

    qt4plotView *View;
    QGraphicsScene *Scene;
    QPainter *Painter;
    QGraphicsPixmapItem *PixmapItem;
    QPixmap *PlotPixmap;

  public:
    static QColor colorQt( const graphics::color& Color )         { return QColor( Color.red(), Color.green(), Color.blue(), 0xFF - Color.transparency() ); }
    static QPoint pointQt( const graphics::wpoint& Point )        { return QPoint(Point.x(),Point.y()); }
    static QRect  rectangleQt( const graphics::wrectangle& Rect );
    static QPen   penQt( const graphics::lineStyle& Style );
    static QFont  fontQt( const graphics::textStyle &Style );
    static QBrush brushQt( const graphics::brushStyle &Style );
    static QPolygon polygonQt( const std::vector<graphics::wpoint> &Points );
   
  public:
    void drawLine( const graphics::wpoint &A, const graphics::wpoint &B, const graphics::lineStyle &Style );
    void drawRectangle( const graphics::wrectangle& Rectangle, const graphics::brushStyle& BrushStyle, const graphics::lineStyle &LineStyle );
    void drawPolygon( const std::vector<graphics::wpoint> &Points, const graphics::brushStyle& BrushStyle );
    void drawText( const std::string &Text, const graphics::wrectangle& Rectangle, const graphics::textStyle &Style, double RotAngle );
    void eraseRectangle( const graphics::wrectangle& Rectangle );
    void eraseAll();
    void flush();

    graphics::wcoord textWidth( const std::string &Text, const graphics::textStyle &Style );
    graphics::wcoord textHeight( const std::string &Text, const graphics::textStyle &Style );
    
    graphics::wcoord width()  const { return Scene == NULL ? 0 : Scene->width(); };
    graphics::wcoord height() const { return Scene == NULL ? 0 : Scene->height(); };

  public:
    drawerQt( QWidget *Parent );

    QGraphicsScene* scene() { return Scene; }
    qt4plotView* view() { return View; }
    QPainter* painter() { return Painter; }
};

// ----------------------------------------------------------------

class qt4plotMouseCallBack : public graphics::mouseCallBack
{
  protected:
    qt4plot& getQt4Plot(); 
    
    static bool isSelectionMouseAction( graphics::mouse::mouseActionHandler* );

  public:
    qt4plotMouseCallBack( qt4plot &Plot );

    void onPressed( graphics::mouse::mouseActionHandler* );
    void onMoved( graphics::mouse::mouseActionHandler* );
    void onRelease( graphics::mouse::mouseActionHandler* );
    void onWheel( graphics::mouse::mouseWheelHandler* );
};

// ----------------------------------------------------------------

class qt4plot : public QWidget, public graphics::plot
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
    static graphics::wpoint plotMousePoisition( const QMouseEvent *Event );
    static graphics::wpoint plotMousePoisition( const QWheelEvent *Event );

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

