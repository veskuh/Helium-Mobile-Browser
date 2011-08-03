#include <QApplication>
#include <QInputContextFactory>

#include "MainView.h"
#include "Core.h"
#include "macros.h"

#include "CoreDbHelper.h"

#if defined(Q_OS_SYMBIAN)
#include "sym_iap_util.h"
#endif

/** Message Handler for qDebug, qWarning, qCritical and qFatal messages
@param type Message Type
@param msg Message Body
*/
void messageHandler(QtMsgType type, const char *msg)
{
    switch (type)
    {
    case QtDebugMsg:
        fprintf(stderr, "OWRTTEST Debug: %s\n", msg);
        break;
    case QtWarningMsg:
        fprintf(stderr, "OWRTTEST Warning: %s\n", msg);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "OWRTTEST Critical: %s\n", msg);
        break;
    case QtFatalMsg:
        fprintf(stderr, "OWRTTEST Fatal: %s\n", msg);
        break;
    }
}

int main(int argc, char** argv) {
   // Register a new Message Handler for qDebug/qWarning/qCritical/qFatal
#ifdef INSTALL_MESSAGE_HANDLER
   qInstallMsgHandler(messageHandler);
#endif

   // Create the Application
   QApplication app(argc, argv);
   app.setApplicationName(APP_NAME);
   app.setApplicationVersion(APP_VERSION);
   app.setOrganizationName(ORG_NAME);
   app.setOrganizationDomain(ORG_DOMAIN);

   // For notifying VKB of orientation changes
   // this is meego specific and should be behind flag
   QInputContext *ic = QInputContextFactory::create("MInputContext", 0);
   if(ic) {
       app.setInputContext(ic);
   }

   // Init Resource Files
   Q_INIT_RESOURCE(qmls);

   // MainView, that will hold the QML UI
   MainView mainView;

#if defined(Q_OS_SYMBIAN)
   qt_SetDefaultIap(); // This will avoid the browser to keep asking for an IAP
#elif defined(Q_WS_MAEMO_5)
   mainView.setAttribute(Qt::WA_Maemo5NonComposited, true); //< This will avoid the use of Composite on this Parentless-Widget
   mainView.setAttribute(Qt::WA_Maemo5AutoOrientation, true); //< "Qt::WA_Maemo5PortraitOrientation" or "Qt::WA_Maemo5LandscapeOrientation" or "Qt::WA_Maemo5AutoOrientation"
#endif

#if defined(Q_OS_MAC) || defined(Q_OS_WIN32)
   //desktop build, just open a window
//   mainView.setWindowFlags(Qt::FramelessWindowHint);
   mainView.show();
#else
	//TODO recognise whether Q_OS_LINUX is defined and understand whether is meego or desktop linux
   //mobile builds, we want the whole screen!
   mainView.showFullScreen();
#endif


   // The Core of the Application
   Core core(&mainView);
   core.start();

   return app.exec();
}
