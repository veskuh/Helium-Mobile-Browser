#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"
#include "Core.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());

    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    Core *core = new Core(viewer->getView());
    viewer->setMainQmlFile("qrc:/qmls/qml//main.qml");

    viewer->showExpanded();
    core->start();

    return app->exec();
}
