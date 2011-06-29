#include "Core.h"

#include <QApplication>
#include <QState>
#include <QDeclarativeEngine>
#include <QDeclarativeItem>
#include <QDeclarativeComponent>
#include <QDeclarativeContext>
#include <QDeclarativeError>
#include <QMetaType>
#include <QTimer>
#include <QList>
#include <QDesktopWidget>
#include <QInputContext>

#include "macros.h"
#include "Settings.h"
#include "FaviconImageProvider.h"
#include "OrientationFollower.h"

#define ENGINE_BASEURL                    "qrc:/qmls/"
#define BROWSERVIEW_QML                   "BrowserView.qml"
#define BROWSERVIEW_QML_WEBVIEW_OBJ_NAME  "webView"
#define LOGBOOKVIEW_QML                   "LogbookView.qml"
#define DEFAULT_WIDTH                     800
#define DEFAULT_HEIGHT                    480

// public:
Core::Core(MainView *mainView, QObject *parent) :
      QObject(parent),
      m_mainView(mainView),
      m_WebViewInterface(NULL),
      m_bookmarksModel(NULL),
      m_mostVisitedModel(NULL),
      m_historyModel(NULL),
      m_logbookViewComponent(NULL),
      m_logbookView(NULL),
      m_logbook(new Logbook())
{

   // Let's start tracking orientation
   OrientationFollower * orientation = new OrientationFollower(this);

   QDeclarativeContext *context = mainView->rootContext();

#if defined(Q_OS_MAC) || defined(Q_OS_WIN32)
   //desktop build, defaults
   context->setContextProperty("screenWidth", DEFAULT_WIDTH);
   context->setContextProperty("screenHeight", DEFAULT_HEIGHT);
#else
   //TODO recognise whether Q_OS_LINUX is defined and understand whether is meego or desktop linux
   //mobile builds, we want the whole screen!
   context->setContextProperty("screenWidth", qApp->desktop()->width());
   context->setContextProperty("screenHeight", qApp->desktop()->height());
#endif

   context->setContextProperty("Orientation", orientation);
   context->setContextProperty("mainWindow", mainView);

   // Initialize Settings
   Settings::initSettings();
   Settings::initWebSettings();

   // Set the Current URL, using Settings
   setCurrentUrl(m_settings.value(SETTINGS_KEY_INITIAL_HOME).toString());

   // Listen for MainView status changes
   connect(m_mainView, SIGNAL(statusChanged(QDeclarativeView::Status)), this, SLOT(onMainViewStatusChanged(QDeclarativeView::Status)));

   // Populating the StateMachine
   QState *showBrowserViewState = new QState();
   QState *showLogbookViewState = new QState();
   // Transitions
   showBrowserViewState->addTransition(this, SIGNAL(m_showLogbookView()), showLogbookViewState);
   showLogbookViewState->addTransition(this, SIGNAL(m_showBrowserView()), showBrowserViewState);
   showLogbookViewState->addTransition(this, SIGNAL(currentUrlChanged()), showBrowserViewState);
   // Action to do on entering States
   connect(showBrowserViewState, SIGNAL(entered()), this, SLOT(onShowBrowserView()));
   connect(showBrowserViewState, SIGNAL(entered()), this, SIGNAL(showingBrowserView()));
   connect(showLogbookViewState, SIGNAL(entered()), this, SLOT(onShowLogbookView()));
   connect(showLogbookViewState, SIGNAL(entered()), this, SIGNAL(showingLogbookView()));
   // Adding the States to the State Machine
   m_stateMachine.addState(showBrowserViewState);
   m_stateMachine.addState(showLogbookViewState);
   m_stateMachine.setInitialState(showBrowserViewState);

   // Pre-inflate the BookmarksListModel and the HistoryListModel
   inflateBookmarksListModel();
   inflateMostVisitedListModel();
   inflateHistoryListModel();

   // Connect signal "QDeclarativeEngine::quit()" to the proper slot
   connect(m_mainView->engine(), SIGNAL(quit()), this, SLOT(onEngineQuit()));

   // Register BookmarksListModel MetaType
   qRegisterMetaType<BookmarksListModel>(BookmarksListModel::metaTypeName());
   // Register HistoryListModel MetaType
   qRegisterMetaType<HistoryListModel>(HistoryListModel::metaTypeName());

   // Injecting the Core into the QML Root Context
   m_mainView->engine()->rootContext()->setContextProperty(Core::declarativeName(), this);

   // Setting the BaseURL in the Engine: this will be used as "base" from Components while importing other Components
   m_mainView->engine()->setBaseUrl(QUrl(ENGINE_BASEURL));

   // Register an Image Provider for Favicons
   m_mainView->engine()->addImageProvider("favicons", new FaviconImageProvider);

   // Load the Home URL
   loadHomeUrl();
}

Core::~Core() {
   if ( m_WebViewInterface )
      delete m_WebViewInterface;
   if ( m_logbookView )
      delete m_logbookView;
   if ( m_logbook )
      delete m_logbook;
}

void Core::start() {
   QDEBUG("Core::start()");
   m_stateMachine.start();
}

// public:
QString Core::currentUrl() { return m_currentUrl; }

void Core::setCurrentUrl(const QString &url, const bool &emitChanged) {
   QDEBUG_EXP("Core::currentUrlChanged()", "old: " << m_currentUrl << " new: " << url);
   m_currentUrl = url;
   if ( emitChanged ) { emit currentUrlChanged(); }
}

QString Core::fixUrl(const QString &urlString) {
   QDEBUG_EXP("Core::fixUrl()", "Url to fix: " << urlString);

   if ( urlString.isEmpty() || urlString.isNull() ) { return urlString; }
   if ( urlString.at(0) == '/' ) { return QString("file://").append(urlString); }
   if ( urlString.indexOf(':') < 0 ) {
      if ( urlString.indexOf('.') < 0 || urlString.indexOf(' ') >= 0 ) {
         // Fall back to a search engine; hard-coded Google
         return QString("http://www.google.fr/search?q=").append(urlString);
      } else {
         return QString("http://").append(urlString);
      }
   }

   return urlString;
}

// public slots:
void Core::loadHomeUrl() {
   BookmarkItem *home = m_logbook->getHome();
   if ( home ) {
      setCurrentUrl(home->url);
      delete home;
   } else {
      setCurrentUrl(m_settings.value(SETTINGS_KEY_INITIAL_HOME).toString());
   }
}

void Core::bookmarkCurrentUrl() {
   m_logbook->addBookmark(m_WebViewInterface->title(), m_WebViewInterface->url().toEncoded());
}

void Core::historyCurrentUrl() {
   m_logbook->addHistory(m_WebViewInterface->title(), m_WebViewInterface->url().toEncoded());
}

void Core::showBrowserView() {
   QDEBUG("Core::showBrowserView()");
   emit m_showBrowserView();
}

void Core::showLogbookView() {
   QDEBUG("Core::showLogbookView()");
   emit m_showLogbookView();
}

// private slots:
void Core::onEngineQuit() {
   QDEBUG("Core::onEngineQuit()");
   qApp->quit();
}

void Core::onShowBrowserView() {
   QDEBUG("Core::onShowBrowserView()");
   // Set BrowserView as the "rootItem" on the Scene
   if ( m_mainView->source().isEmpty() ) {
      m_mainView->setSource(QUrl(BROWSERVIEW_QML));
      m_browserView = m_mainView->rootObject();
   }

   removeLogbookViewFromScene();
//   inflateBookmarksListModel();
//   inflateMostVisitedListModel();
//   inflateHistoryListModel();
//   m_mainView->setSource(QUrl(LOGBOOKVIEW_QML));
}

void Core::deflateLogbookViewLater() {
    QDEBUG("Core::deflateLogbookViewLater()");
    QTimer::singleShot(0, this, SLOT(deflateLogbookView()));
    //QMetaObject::invokeMethod( this, "deflateLogbookView", Qt::QueuedConnection );
}

void Core::onShowLogbookView() {
   QDEBUG("Core::onShowLogbookView()");
   // Ensure the BookmarksListModel is available
   inflateBookmarksListModel();
   // Ensure the MostVisitedListModel is available
   inflateMostVisitedListModel();
   // Ensure the HistoryListModel is available
   inflateHistoryListModel();
   // Ensure the LogbookView is available and add to Scene
   inflateLogbookView();
   addLogbookViewOnScene();
}

void Core::onMainViewStatusChanged(QDeclarativeView::Status status) {
   switch(status) {
   case QDeclarativeView::Null: {
         QDEBUG_EXP("Core::onMainViewStatusChanged()", "QDeclarativeView::Null (0)"); break;
      }
   case QDeclarativeView::Ready: {
         QDEBUG_EXP("Core::onMainViewStatusChanged()", "QDeclarativeView::Ready (1)");
         // "Grab" the QML WebView into a Native Interface
         m_WebViewInterface = new WebViewInterface(m_mainView->rootObject(), BROWSERVIEW_QML_WEBVIEW_OBJ_NAME);
         break;
      }
   case QDeclarativeView::Loading: {
         QDEBUG_EXP("Core::onMainViewStatusChanged()", "QDeclarativeView::Loading (2)"); break;
      }
   case QDeclarativeView::Error: {
         QDEBUG_EXP("Core::onMainViewStatusChanged()", "QDeclarativeView::Error (3)"); break;
         QList<QDeclarativeError> errors = m_mainView->errors();
         for ( QList<QDeclarativeError>::iterator i = errors.begin(); i != errors.end(); ++i ) {
            QDEBUG((*i).url() << ":" << (*i).line() << ":" << (*i).column() << ": " << (*i).description());
         }
      }
   }
}

void Core::onLogbookViewComponentStatusChanged(QDeclarativeComponent::Status status) {
   switch(status) {
   case QDeclarativeComponent::Null: {
         QDEBUG_EXP("Core::onComponentStatusChanged()", "QDeclarativeComponent::Null (0)"); break;
      }
   case QDeclarativeComponent::Ready: {
         QDEBUG_EXP("Core::onComponentStatusChanged()", "QDeclarativeComponent::Ready (1)"); break;
      }
   case QDeclarativeComponent::Loading: {
         QDEBUG_EXP("Core::onComponentStatusChanged()", "QDeclarativeComponent::Loading (2)"); break;
      }
   case QDeclarativeComponent::Error: {
         QDEBUG_EXP("Core::onComponentStatusChanged()", "QDeclarativeComponent::Error (3)");
         QList<QDeclarativeError> errors = m_logbookViewComponent->errors();
         for ( QList<QDeclarativeError>::iterator i = errors.begin(); i != errors.end(); ++i ) {
            QDEBUG((*i).url() << ":" << (*i).line() << ":" << (*i).column() << ": " << (*i).description());
         }
         break;
      }
   }
}

void Core::inflateBookmarksListModel() {
   if ( !m_bookmarksModel ) {
      QDEBUG("Core::inflateBookmarksListModel()");
      // Instantiate a BookmarksListModel
      m_bookmarksModel = new BookmarksListModel(m_logbook);
      m_bookmarksModel->setParent(this);
      if ( !m_bookmarksModel ) qCritical() << "CoreQmlView - Can't create BookmarksListModel";
      // Add the BookmarksListModel to the Root Context
      m_mainView->engine()->rootContext()->setContextProperty(BookmarksListModel::declarativeName(), m_bookmarksModel);
   }
}

void Core::deflateBookmarksListModel() {
   if ( m_bookmarksModel ) {
      QDEBUG("Core::deflateBookmarksListModel()");
      // Remove the BookmarksListModel from the Root Context
      m_mainView->engine()->rootContext()->setContextProperty(BookmarksListModel::declarativeName(), NULL);
      delete m_bookmarksModel;
      m_bookmarksModel = NULL;
   }
}

void Core::inflateHistoryListModel() {
   if ( !m_historyModel ) {
      QDEBUG("Core::inflateHistoryListModel()");
      // Instantiate a HistoryListModel
      m_historyModel = new HistoryListModel(m_logbook);
      m_historyModel->setParent(this);
      if ( !m_historyModel ) qCritical() << "CoreQmlView - Can't create HistoryListModel";
      // Add the HistoryListModel to the Root Context
      m_mainView->engine()->rootContext()->setContextProperty(HistoryListModel::declarativeName(), m_historyModel);
   }
}

void Core::deflateHistoryListModel() {
   if ( m_historyModel ) {
      QDEBUG("Core::deflateHistoryListModel()");
      // Remove the HistoryListModel from the Root Context
      m_mainView->engine()->rootContext()->setContextProperty(HistoryListModel::declarativeName(), NULL);
      delete m_historyModel;
      m_historyModel = NULL;
   }
}

void Core::inflateMostVisitedListModel() {
   if ( !m_mostVisitedModel ) {
      QDEBUG("Core::inflaMostVisitedListModel()");
      // Instantiate a MostVisitedListModel
      m_mostVisitedModel = new MostVisitedListModel(m_logbook);
      m_mostVisitedModel->setParent(this);
      if ( !m_mostVisitedModel ) qCritical() << "CoreQmlView - Can't create MostVisitedListModel";
      // Add the MostVisitedListModel to the Root Context
      m_mainView->engine()->rootContext()->setContextProperty(MostVisitedListModel::declarativeName(), m_mostVisitedModel);
   }
}

void Core::deflateMostVisitedListModel() {
   if ( m_mostVisitedModel ) {
      QDEBUG("Core::deflateMostVisitedListModel()");
      // Remove the MostVisitedListModel from the Root Context
      m_mainView->engine()->rootContext()->setContextProperty(MostVisitedListModel::declarativeName(), NULL);
      delete m_mostVisitedModel;
      m_mostVisitedModel = NULL;
   }
}

void Core::inflateLogbookView() {
   if ( !m_logbookViewComponent ) {
      QDEBUG_EXP("Core::inflateLogbookView()", "Inflating Component");
      m_logbookViewComponent = new QDeclarativeComponent(m_mainView->engine(), QUrl(LOGBOOKVIEW_QML), this);
      connect(m_logbookViewComponent, SIGNAL(statusChanged(QDeclarativeComponent::Status)), this, SLOT(onLogbookViewComponentStatusChanged(QDeclarativeComponent::Status)));
   }
   if ( !m_logbookView ) {
      QDEBUG_EXP("Core::inflateLogbookView()", "Inflating View");
      m_logbookView = static_cast<QGraphicsObject *>( m_logbookViewComponent->create() );
   }
}

void Core::deflateLogbookView() {
   if ( m_logbookView ) {
      QDEBUG_EXP("Core::deflateLogbookView()", "Deflating View");
      m_mainView->scene()->removeItem(m_logbookView);
      delete m_logbookView;
      m_logbookView = NULL;
   }
}

void Core::addLogbookViewOnScene() {
   if ( m_logbookView )
      m_logbookView->setParentItem(m_browserView);
}

void Core::removeLogbookViewFromScene() {
   if ( m_logbookView )
      m_logbookView->setParent( NULL );
}


void Core::hideVkb() {
   QInputContext *inputContext = qApp->inputContext();
   if (!inputContext) {
       // Not cool
       return;
   }

   QEvent request(QEvent::CloseSoftwareInputPanel);
   inputContext->filterEvent(&request);
   inputContext->reset();
}
