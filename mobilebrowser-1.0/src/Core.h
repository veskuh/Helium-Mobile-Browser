/*--------------------------------------------------------
* Module Name : Helium Mobile Browser
* Version : 1.0
*
* Software Name : Helium Mobile Browser
* Version : 1.0
*
* Copyright (c) 2010 - 2011 France Telecom
* This software is distributed under the LGPL v2.1 license,
* the text of which is available at http://www.gnu.org/licenses/lgpl-2.1.html
* or see the "LICENSE.txt" file for more details.
*
*--------------------------------------------------------
* File Name   : Core.h
*
* Created     : 15 June 2011
*
* Description :
*
* Core holds the browser business logic, which is driven by the internal
* state machine
*
*--------------------------------------------------------
* 1.0 - First open source release
*
*/

#ifndef CORE_H
#define CORE_H

#include <QObject>
#include <QStateMachine>
#include <QMetaType>
#include <QDeclarativeComponent>
#include <QGraphicsObject>
#include <QSettings>

#include "buildconfig.h"
#include "MainView.h"
#include "WebViewInterface.h"
#include "models/BookmarksListModel.h"
#include "models/HistoryListModel.h"
#include "models/MostVisitedListModel.h"
#include "Logbook.h"

class Core : public QObject, public Declarativable
{
   Q_OBJECT

   Q_PROPERTY(QString currentUrl READ currentUrl WRITE setCurrentUrl NOTIFY currentUrlChanged)

public:
   Core(MainView *mainView, QObject *parent = 0);
   virtual ~Core();

   /** (Gentlemen,) Start THIS Engine */
   void start();

// Implement Declarativable
   static const char * metaTypeName() { return "Core"; }
   static QString declarativeName() { return "appcore"; }

public:
   // Property methods
   /** Get the current URL */
   QString currentUrl();
   /** Set the current URL. */
   void setCurrentUrl(const QString &url, const bool &emitChanged = true);
   Q_INVOKABLE static QString fixUrl(const QString &urlString);

public slots:
   // Public Slots - Exposed to the QML Context
   void loadHomeUrl();
   void bookmarkCurrentUrl();
   void historyCurrentUrl();
   void showLogbookView();
   void showBrowserView();

   void deflateLogbookViewLater();
   void hideVkb();

private slots:
   void onEngineQuit();
   void onShowBrowserView();
   void onShowLogbookView();
   void onMainViewStatusChanged(QDeclarativeView::Status status);
   void onLogbookViewComponentStatusChanged(QDeclarativeComponent::Status status);

private slots:
   void inflateBookmarksListModel();
   void deflateBookmarksListModel();
   void inflateHistoryListModel();
   void deflateHistoryListModel();
   void inflateMostVisitedListModel();
   void deflateMostVisitedListModel();
   void inflateLogbookView();
   void deflateLogbookView();
   void addLogbookViewOnScene();
   void removeLogbookViewFromScene();

signals:
   void currentUrlChanged();
   void showingBrowserView();
   void showingLogbookView();

   // Private Signals
   void m_showLogbookView();
   void m_showBrowserView();

private:
   QStateMachine                       m_stateMachine;
   QString                             m_currentUrl;
   QSettings                           m_settings;
   MainView                            *m_mainView; //< Not Owned
   WebViewInterface                    *m_WebViewInterface; //< Owned
   BookmarksListModel                  *m_bookmarksModel; //< Owned, as a child QObject
   MostVisitedListModel                *m_mostVisitedModel; //< Owned, as a child QObject
   HistoryListModel                    *m_historyModel; //< Owned, as a child QObject
   QGraphicsObject                     *m_browserView; //< Not owned
   QDeclarativeComponent               *m_logbookViewComponent; //< Owned, as child QObject
   QGraphicsObject                     *m_logbookView; //< Owned
   Logbook                             *m_logbook; //< Owned
};

#endif // CORE_H
