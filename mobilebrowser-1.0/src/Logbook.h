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
* File Name   : Logbook.h
*
* Created     : 15 June 2011
*
* Description :
*
* Logbook is the class providing info about all the history and bookmarks
* colleted through the lifetime of the browser
*
*--------------------------------------------------------
* 1.0 - First open source release
*
*/

#ifndef LOGBOOK_H
#define LOGBOOK_H

#include <QObject>
#include <QList>
#include <QDateTime>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSettings>

#include "LogbookLinkItems.h"
#include "CoreDbHelper.h"

class Logbook : public QObject
{
   Q_OBJECT
public:
   Logbook(QObject *parent = 0);
   virtual ~Logbook();

   /** Return a pointer to a BookmarkItem if it exists, otherwise NULL.
     Ownership of this object is passed to the receiving code.
     @param bookmarkLinkItemId
   */
   BookmarkItem * getBookmark(const linkItemId &bookmarkLinkItemId);
   /** Returns a pointer to a BookmarkItem if it exists, otherwise NULL.
     Ownership of this object is passed to the receiving code.
     @param url
   */
   BookmarkItem * getBookmark(const QString &url);
   /** Returns a pointer to a BookmarkItem if it exists, otherwise NULL.
     The BookmarkItem is choosen based on it's position in the Logbook (i.e. "ORDER BY title ASC").
     Ownership of this object is passed to the receiving code.
     @param at
   */
   BookmarkItem * getBookmarkAt(const int &at);
   /** Returns a pointer to a BookmarkItem's Id if it exists, otherwise NULL.
     The BookmarkItem is choosen based on it's position in the Logbook (i.e. "ORDER BY title ASC").
     Ownership of this object is passed to the receiving code.
     @param at
   */
   linkItemId getBookmarkIdAt(const int &at);

   /** Return the number of Bookmarks saved/available. */
   int getBookmarksCount();

   /** Return a pointer to a HistoryItem if it exists, otherwise NULL.
     Ownership of this object is passed to the receiving code.
     @param historyLinkItemId
   */
   HistoryItem * getHistory(const linkItemId &historyLinkItemId);
   /** Return a pointer to a HistoryItem if it exists, otherwise NULL.
     Ownership of this object is passed to the receiving code.
     @param url
   */
   HistoryItem * getHistory(const QString &url);

   /** Returns a pointer to a HistoryItem if it exists, otherwise NULL.
     The HistoryItem is choosen based on it's position in the Logbook (i.e. "ORDER BY last_visit_timestamp DESC").
     Ownership of this object is passed to the receiving code.
     @param at
   */
   HistoryItem * getHistoryAt(const int &at);
   /** Returns a pointer to a HistoryItem's Id if it exists, otherwise NULL.
     The HistoryItem is choosen based on it's position in the Logbook (i.e. "ORDER BY last_visit_timestamp DESC").
     Ownership of this object is passed to the receiving code.
     @param at
   */
   linkItemId getHistoryIdAt(const int &at);

   /** Return the number of History(ies) saved/available. */
   int getHistoriesCount();

   /** Returns a pointer to a Most Visited HistoryItem if it exists, otherwise NULL.
     The HistoryItem is choosen based on it's position in the Logbook (i.e. "ORDER BY load_count DESC").
     Ownership of this object is passed to the receiving code.
     @param at
   */
   HistoryItem * getMostVisitedHistoryAt(const int &at);
   /** Returns a pointer to a Most Visited HistoryItem's Id if it exists, otherwise NULL.
     The HistoryItem is choosen based on it's position in the Logbook (i.e. "ORDER BY load_count DESC").
     Ownership of this object is passed to the receiving code.
     @param at
   */
   linkItemId getMostVisitedHistoryIdAt(const int &at);
   /** Return the number of Most Visited History(ies) saved/available. */
   int getMostVisitedHistoriesCount();

   /** Check if a BookmarkItem is set as the current Homepage.
     @param item
     @return "true" if it is the Homepage, "false" otherwise
   */
   bool isHome(const BookmarkItem &item);
   /** Check if a Url is set as the current Homepage.
     @param url
     @return "true" if it is the Homepage, "false" otherwise
   */
   bool isHome(const QString &url);
   /** Check if the Id of a BookmarkItem is set as the current Homepage.
     @param bookmarkItemId
     @return "true" if it is the Homepage, "false" otherwise
   */
   bool isHome(const linkItemId &bookmarkItemId);

   /** Return the current Homepage BookmarkItem.
     @return A pointer to the BookmarkItem representing the current Homepage. "NULL" if it doesn't exist.
   */
   BookmarkItem * getHome();

public slots:
   /** Add or Replace a Bookmark.
     @param item BookmarkItem, previously constructed/retrieved, that is going to be stored/replaced.
   */
   void addBookmark(const BookmarkItem &item);
   /** Add or Replace a Bookmark.
     It uses the UNIQUITY of "url" and "bookmarkItemId" to identify if it's a new item or not.
     @param title URL Title. Can be an null/empty QString.
     @param url In case this url already exists in the Database, it will work as a replace.
     @param bookmarkItemId Database ID of this item, if it was previously created. In this case it will work as a replace.
   */
   void addBookmark(const QString &title, const QString &url, const linkItemId &bookmarkItemId = LINK_ITEM_ID_UNDEFINED);

   /** Add or Replace a History.
     @param item HistoryItem, previously constructed/retrieved, that is going to be stored/replaced.
   */
   void addHistory(const HistoryItem &item);
   /** Add or Replace a History.
     It uses the UNIQUITY of "url" and "historyItemId" to identify if it's a new item or not.
     @param title URL Title. Can be an null/empty QString.
     @param url In case this url already exists in the Database, it will work as a replace.
     @param historyItemId Database ID of this item, if it was previously created. In this case it will work as a replace.
   */
   void addHistory(const QString &title, const QString &url, const linkItemId &historyItemId = LINK_ITEM_ID_UNDEFINED);
   /** Update History counter
     @param url URL of which to increase the visit-counter.
   */
   void updateHistoryCounter(const QString &url);

   /** Delete a BookmarkItem */
   void deleteBookmark(const BookmarkItem &item);
   /** Delete a BookmarkItem. It uses the UNIQUITY of the database "id" field. */
   void deleteBookmark(const linkItemId &bookmarkItemId);
   /** Delete a BookmarkItem. It uses the UNIQUITY of the "url" field. */
   void deleteBookmark(const QString &url);
   /** Delete all Bookmarks */
   void deleteBookmarks();

   /** Delete a HistoryItem */
   void deleteHistory(const HistoryItem &item);
   /** Delete a HistoryItem. It uses the UNIQUITY of the database "id" field. */
   void deleteHistory(const linkItemId &historyItemId);
   /** Delete a HistoryItem. It uses the UNIQUITY of the database "url" field. */
   void deleteHistory(const QString &url);
   /** Delete all Histories */
   void deleteHistories();
   /** Delete all the Histories that are older then a User-selected limit.
     The QSettings value "SETTINGS_KEY_HISTORY_LENGTH_DAYS" holds the limit to apply. */
   void deleteExpiredHistories();

   /** Set a new Homepage.
     It works only if the BookmarkItem provided is valid (generated previously by this class).
     @param item
   */
   void setHome(BookmarkItem &item);
   /** Set a new Homepage.
     It also creates a new, title-less Bookmark, if this URL was not already stored within the Bookmarks.
     @param url
   */
   void setHome(const QString &url);
   /** Set a new Homepage.
     It works only if the Id provided is valid (generated previously from this class).
     @param bookmarkItemId
   */
   void setHome(const linkItemId &bookmarkItemId);

signals:
   void bookmarksChanged();
   void bookmarkChanged(const linkItemId &bookmarkItemId);
   void historiesChanged();
   void historyChanged(const linkItemId &historyItemId);
   void homeChanged(const linkItemId &bookmarkItemId);

private:
   CoreDbHelper   *m_dbHelper;
   QSqlDatabase   m_db;
   QSqlQuery      m_query;
   QSettings      m_settings;
};

#endif // LOGBOOK_H
