#include "MainView.h"

#include "buildconfig.h"

#include <QGLWidget>

MainView::MainView(QWidget *parent) :
      QDeclarativeView(parent)
{
   // Visual initialization
#ifdef USE_OPENGL
   QGLFormat format = QGLFormat::defaultFormat();
   #ifdef Q_WS_MAC
      format.setSampleBuffers(true);
   #else
      format.setSampleBuffers(false);
   #endif
   QGLWidget *glWidget = new QGLWidget(format, this);
   setViewport(glWidget);
   setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
#else
   setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
#endif

   setAttribute(Qt::WA_OpaquePaintEvent);
   setAttribute(Qt::WA_NoSystemBackground);
   setResizeMode(QDeclarativeView::SizeRootObjectToView);
   setFocus();
}
