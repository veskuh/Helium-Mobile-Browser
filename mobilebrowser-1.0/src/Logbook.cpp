#include "Logbook.h"

#include <QVariant>
#include <QSqlError>

#include "macros.h"
#include "Settings.h"

#define DB_CONN "logbook_db_conn"

#define INSERT_BOOKMARK_SQL        "INSERT INTO bookmark (url, title) VALUES (?, ?)"
#define UPDATE_BOOKMARK_SQL_BY_ID  "UPDATE bookmark SET url = ?, title = ? WHERE id = ?"
#define UPDATE_BOOKMARK_SQL_BY_URL "UPDATE bookmark SET title = ? WHERE url = ?"
#define DELETE_BOOKMARK_SQL_BY_URL "DELETE FROM bookmark WHERE url = ?"
#define DELETE_BOOKMARK_SQL_BY_ID  "DELETE FROM bookmark WHERE id = ?"
#define DELETE_BOOKMARK_ALL_SQL    "DELETE FROM bookmark"
#define SELECT_BOOKMARK_SQL_BY_URL "SELECT url, title, id FROM bookmark WHERE url = ?"
#define SELECT_BOOKMARK_SQL_BY_ID  "SELECT url, title, id FROM bookmark WHERE id = ?"
#define SELECT_BOOKMARK_SQL_AT     "SELECT url, title, id FROM bookmark ORDER BY lower(title) ASC, url ASC LIMIT 1 OFFSET ?"
#define SELECT_BOOKMARK_ID_SQL_AT  "SELECT id FROM bookmark ORDER BY lower(title) ASC, url ASC LIMIT 1 OFFSET ?"
#define SELECT_BOOKMARK_COUNT_SQL  "SELECT count(*) FROM bookmark"

#define INSERT_HISTORY_SQL         "INSERT INTO history (url, title) VALUES (?, ?)"
#define UPDATE_HISTORY_SQL_BY_ID   "UPDATE history SET url = ?, title = ?, load_count = load_count+1, last_visit_timestamp = datetime('now') WHERE id = ?"
#define UPDATE_HISTORY_SQL_BY_URL  "UPDATE history SET title = ?, load_count = load_count+1, last_visit_timestamp = datetime('now') WHERE url = ?"
#define UPDATE_HISTORY_LCOUNT_SQL  "UPDATE history SET load_count=load_count+1, last_visit_timestamp=datetime('now') WHERE url = ?"
#define DELETE_HISTORY_SQL_BY_ID   "DELETE FROM history WHERE id = ?"
#define DELETE_HISTORY_SQL_BY_URL  "DELETE FROM history WHERE url = ?"
#define DELETE_HISTORY_ALL_SQL     "DELETE FROM history"
#define DELETE_EXPIRED_HISTORY_SQL "DELETE FROM history WHERE last_visit_timestamp < datetime('now', ?, 'start of day')"
#define SELECT_HISTORY_SQL_BY_URL  "SELECT url, title, load_count, last_visit_timestamp, id FROM history WHERE url = ?"
#define SELECT_HISTORY_SQL_BY_ID   "SELECT url, title, load_count, last_visit_timestamp, id FROM history WHERE id = ?"
#define SELECT_HISTORY_SQL_AT      "SELECT url, title, load_count, last_visit_timestamp, id FROM history ORDER BY last_visit_timestamp DESC LIMIT 1 OFFSET ?"
#define SELECT_HISTORY_ID_SQL_AT   "SELECT id FROM history ORDER BY last_visit_timestamp DESC, lower(title) ASC, url ASC LIMIT 1 OFFSET ?"
#define SELECT_HISTORY_COUNT_SQL   "SELECT count(*) FROM history"

#define MOST_VISITED_LIMIT                      "10"
#define SELECT_MOST_VISITED_HISTORY_SQL_AT      "SELECT url, title, load_count, last_visit_timestamp, id FROM history ORDER BY load_count DESC, last_visit_timestamp DESC LIMIT 1 OFFSET ?"
#define SELECT_MOST_VISITED_HISTORY_ID_SQL_AT   "SELECT id FROM history ORDER BY load_count DESC, last_visit_timestamp DESC LIMIT 1 OFFSET ?"
#define SELECT_MOST_VISITED_HISTORY_COUNT_SQL   "SELECT min("MOST_VISITED_LIMIT", count(*)) FROM history ORDER BY load_count DESC, last_visit_timestamp DESC"

Logbook::Logbook(QObject *parent) :
      QObject(parent),
      m_dbHelper(new CoreDbHelper( Settings::dbFilePath(), Settings::dbFileVersion() )),
      m_db(m_dbHelper->open()),
      m_query(QSqlQuery(m_db))
{
   // TODO Initialize CoreDb with default values
}

Logbook::~Logbook() {
   QDEBUG("Logbook::~Logbook()");
   deleteExpiredHistories();
   m_query.clear();
   m_db.close();
   if ( m_dbHelper ) delete m_dbHelper;
}

BookmarkItem * Logbook::getBookmark(const linkItemId &bookmarkLinkItemId) {
   QVERBOSE_EXP("Logbook::getBookmark(id)", "id: " << bookmarkLinkItemId);
   BookmarkItem *result = NULL;

   m_query.prepare(SELECT_BOOKMARK_SQL_BY_ID);
   m_query.bindValue(0, bookmarkLinkItemId);
   if ( !m_query.exec() ) {
      QWARNING_EXP("Logbook::getBookmark(id)", "Query exec error: " << m_query.lastError().text());
   } else if ( m_query.next() ) {
      QVERBOSE_EXP("Logbook::getBookmark(id)", "Bookmark found.");
      result = new BookmarkItem(
            m_query.value(1).toString(), // title
            m_query.value(0).toString(), // url
            isHome(m_query.value(2).toULongLong()), // isHome
            m_query.value(2).toInt() // id
            );

      if ( m_query.next() ) {
         // Should never reach here!
         QWARNING_EXP("Logbook::getBookmark(id)", "More then 1 Bookmark with the same id?");
      }
   } else {
      QDEBUG_EXP("Logbook::getBookmark(id)", "No Bookmark found.");
   }

   return result;
}

BookmarkItem * Logbook::getBookmark(const QString &url) {
   QVERBOSE_EXP("Logbook::getBookmark(url)", "url: " << url);
   BookmarkItem *result = NULL;

   m_query.prepare(SELECT_BOOKMARK_SQL_BY_URL);
   m_query.bindValue(0, url);
   if ( !m_query.exec() ) {
      QWARNING_EXP("Logbook::getBookmark(url)", "Query exec error: " << m_query.lastError().text());
   } else if ( m_query.next() ) {
      QVERBOSE_EXP("Logbook::getBookmark(url)", "Bookmark found.");
      result = new BookmarkItem(
            m_query.value(1).toString(), // title
            m_query.value(0).toString(), // url
            isHome(m_query.value(2).toULongLong()), // isHome
            m_query.value(2).toInt() // id
            );

      if ( m_query.next() ) {
         // Should never reach here!
         QWARNING_EXP("Logbook::getBookmark(url)", "More then 1 Bookmark with the same URL?");
      }
   } else {
      QVERBOSE_EXP("Logbook::getBookmark(url)", "No Bookmark found.");
   }

   return result;
}

BookmarkItem * Logbook::getBookmarkAt(const int &at) {
   QVERBOSE_EXP("Logbook::getBookmarkAt(at)", "at: " << at);
   BookmarkItem *result = NULL;

   m_query.prepare(SELECT_BOOKMARK_SQL_AT);
   m_query.bindValue(0, at);
   if ( !m_query.exec() ) {
      QWARNING_EXP("Logbook::getBookmarkAt()", "Query exec error: " << m_query.lastError().text());
   } else if ( m_query.next() ) {
      result = new BookmarkItem(
            m_query.value(1).toString(), // title
            m_query.value(0).toString(), // url
            isHome(m_query.value(2).toULongLong()), // isHome
            m_query.value(2).toInt() // id
            );
   }

   return result;
}

linkItemId Logbook::getBookmarkIdAt(const int &at) {
   QVERBOSE_EXP("Logbook::getBookmarkIdAt(at)", "at: " << at);
   linkItemId result = LINK_ITEM_ID_UNDEFINED;

   m_query.prepare(SELECT_BOOKMARK_ID_SQL_AT);
   m_query.bindValue(0, at);
   if ( !m_query.exec() ) {
      QWARNING_EXP("Logbook::getBookmarkIdAt()", "Query exec error: " << m_query.lastError().text());
   } else if ( m_query.next() ) {
      result = m_query.value(0).toInt();
   }

   return result;
}

int Logbook::getBookmarksCount() {
   QVERBOSE("Logbook::getBookmarksCount()");
   if ( !m_query.exec(SELECT_BOOKMARK_COUNT_SQL) ) {
      QWARNING_EXP("Logbook::getBookmarksCount()", "Query exec error: " << m_query.lastError().text());
      return 0;
   } else if ( m_query.next() ) {
      return m_query.value(0).toInt();
   } else {
      QWARNING_EXP("Logbook::getBookmarksCount()", "No result when counting Bookmarks?");
      return 0;
   }
}

HistoryItem * Logbook::getHistory(const linkItemId &historyLinkItemId) {
   QVERBOSE_EXP("Logbook::getHistory(id)", "id: " << historyLinkItemId);
   HistoryItem *result = NULL;

   m_query.prepare(SELECT_HISTORY_SQL_BY_ID);
   m_query.bindValue(0, historyLinkItemId);
   if ( !m_query.exec() ) {
      QWARNING_EXP("Logbook::getHistory(id)", "Query exec error: " << m_query.lastError().text());
   } else if ( m_query.next() ) {
      QVERBOSE_EXP("Logbook::getHistory(id)", "History found.");
      result = new HistoryItem(
            m_query.value(1).toString(), // title
            m_query.value(0).toString(), // url
            m_query.value(2).toULongLong(), // load_count
            m_query.value(3).toDateTime(), // last_visit_timestamp
            m_query.value(4).toInt() // id
            );

      if ( m_query.next() ) {
         // Should never reach here!
         QWARNING_EXP("Logbook::getHistory(id)", "More then 1 History with the same Id?");
      }
   } else {
      QVERBOSE_EXP("Logbook::getHistory(id)", "No History found.");
   }

   return result;
}

HistoryItem * Logbook::getHistory(const QString &url) {
   QVERBOSE_EXP("Logbook::getHistory(url)", "url: " << url);
   HistoryItem *result = NULL;

   m_query.prepare(SELECT_HISTORY_SQL_BY_URL);
   m_query.bindValue(0, url);
   if ( !m_query.exec() ) {
      QWARNING_EXP("Logbook::getHistory(url)", "Query exec error: " << m_query.lastError().text());
   } else if ( m_query.next() ) {
      QVERBOSE_EXP("Logbook::getHistory(url)", "History found.");
      result = new HistoryItem(
            m_query.value(1).toString(), // title
            m_query.value(0).toString(), // url
            m_query.value(2).toULongLong(), // load_count
            m_query.value(3).toDateTime(), // last_visit_timestamp
            m_query.value(4).toInt() // id
            );

      if ( m_query.next() ) {
         // Should never reach here!
         QWARNING_EXP("Logbook::getHistory(url)", "More then 1 History with the same URL?");
      }
   } else {
      QVERBOSE_EXP("Logbook::getHistory(url)", "No History found.");
   }

   return result;
}

HistoryItem * Logbook::getHistoryAt(const int &at) {
   QVERBOSE_EXP("Logbook::getHistoryAt(at)", "at: " << at);
   HistoryItem *result = NULL;

   m_query.prepare(SELECT_HISTORY_SQL_AT);
   m_query.bindValue(0, at);
   if ( !m_query.exec() ) {
      QWARNING_EXP("Logbook::getHistoryAt()", "Query exec error: " << m_query.lastError().text());
   } else if ( m_query.next() ) {
      result = new HistoryItem(
            m_query.value(1).toString(), // title
            m_query.value(0).toString(), // url
            m_query.value(2).toULongLong(), // load_count
            m_query.value(3).toDateTime(), // last_visit_timestamp
            m_query.value(4).toInt() // id
            );
   }

   return result;
}

linkItemId Logbook::getHistoryIdAt(const int &at) {
   QVERBOSE_EXP("Logbook::getHistoryIdAt(at)", "at: " << at);
   linkItemId result = LINK_ITEM_ID_UNDEFINED;

   m_query.prepare(SELECT_HISTORY_ID_SQL_AT);
   m_query.bindValue(0, at);
   if ( !m_query.exec() ) {
      QWARNING_EXP("Logbook::getHistoryIdAt()", "Query exec error: " << m_query.lastError().text());
   } else if ( m_query.next() ) {
      result = m_query.value(0).toInt();
   }

   return result;
}

int Logbook::getHistoriesCount() {
   QVERBOSE("Logbook::getHistoriesCount()");
   if ( !m_query.exec(SELECT_HISTORY_COUNT_SQL) ) {
      QWARNING_EXP("Logbook::getHistoriesCount()", "Query exec error: " << m_query.lastError().text());
      return 0;
   } else if ( m_query.next() ) {
      return m_query.value(0).toInt();
   } else {
      QWARNING_EXP("Logbook::getHistoriesCount()", "No result when counting History?");
      return 0;
   }
}

HistoryItem * Logbook::getMostVisitedHistoryAt(const int &at) {
   QVERBOSE_EXP("Logbook::getMostVisitedHistoryAt(at)", "at: " << at);
   HistoryItem *result = NULL;

   m_query.prepare(SELECT_MOST_VISITED_HISTORY_SQL_AT);
   m_query.bindValue(0, at);
   if ( !m_query.exec() ) {
      QWARNING_EXP("Logbook::getMostVisitedHistoryAt()", "Query exec error: " << m_query.lastError().text());
   } else if ( m_query.next() ) {
      result = new HistoryItem(
            m_query.value(1).toString(), // title
            m_query.value(0).toString(), // url
            m_query.value(2).toULongLong(), // load_count
            m_query.value(3).toDateTime(), // last_visit_timestamp
            m_query.value(4).toInt() // id
            );
   }

   return result;
}

linkItemId Logbook::getMostVisitedHistoryIdAt(const int &at) {
   QVERBOSE_EXP("Logbook::getMostVisitedHistoryIdAt(at)", "at: " << at);
   linkItemId result = LINK_ITEM_ID_UNDEFINED;

   m_query.prepare(SELECT_MOST_VISITED_HISTORY_ID_SQL_AT);
   m_query.bindValue(0, at);
   if ( !m_query.exec() ) {
      QWARNING_EXP("Logbook::getMostVisitedHistoryIdAt()", "Query exec error: " << m_query.lastError().text());
   } else if ( m_query.next() ) {
      result = m_query.value(0).toInt();
   }

   return result;
}

int Logbook::getMostVisitedHistoriesCount() {
   QVERBOSE("Logbook::getMostVisitedHistoriesCount()");
   if ( !m_query.exec(SELECT_MOST_VISITED_HISTORY_COUNT_SQL) ) {
      QWARNING_EXP("Logbook::getMostVisitedHistoriesCount()", "Query exec error: " << m_query.lastError().text());
      return 0;
   } else if ( m_query.next() ) {
      return m_query.value(0).toInt();
   } else {
      QWARNING_EXP("Logbook::getMostVisitedHistoriesCount()", "No result when counting History?");
      return 0;
   }
}

bool Logbook::isHome(const BookmarkItem &item) {
   return isHome(item.id);
}

bool Logbook::isHome(const QString &url) {
   return isHome(getBookmark(url)->id);
}

bool Logbook::isHome(const linkItemId &bookmarkItemId) {
   return bookmarkItemId == m_settings.value(SETTINGS_KEY_HOME_BOOKMARK_ID).toInt() ? true : false;
}

BookmarkItem * Logbook::getHome() {
   QDEBUG("Logbook::getHome()");
   return getBookmark( m_settings.value( SETTINGS_KEY_HOME_BOOKMARK_ID ).toInt() );
}

// public slots:
void Logbook::addBookmark(const BookmarkItem &item) {
   addBookmark(item.title, item.url, item.id);
}

void Logbook::addBookmark(const QString &title, const QString &url, const linkItemId &bookmarkItemId) {
   QDEBUG_EXP("Logbook::addBookmark()", "title: " << title << " url: " << url << " id: " << bookmarkItemId);
   if ( bookmarkItemId != LINK_ITEM_ID_UNDEFINED ) { // Update Bookmark
      QDEBUG_EXP("Logbook::addBookmark()", "Provided Bookmark Id -> Updating");
      m_query.prepare(UPDATE_BOOKMARK_SQL_BY_ID);
      m_query.bindValue(0, url); m_query.bindValue(1, title); m_query.bindValue(2, bookmarkItemId);
   } else { // Insert Bookmark, or Update it
      if ( NULL != getBookmark(url) ) {
         QDEBUG_EXP("Logbook::addBookmark()", "Updating Bookmark");
         m_query.prepare(UPDATE_BOOKMARK_SQL_BY_URL);
         m_query.bindValue(0, title); m_query.bindValue(1, url);
      } else {
         QDEBUG_EXP("Logbook::addBookmark()", "Inserting Bookmark");
         m_query.prepare(INSERT_BOOKMARK_SQL);
         m_query.bindValue(0, url); m_query.bindValue(1, title);
      }
   }
   // Let's query!
   if ( !m_query.exec() ) {
      QWARNING_EXP("Logbook::addBookmark()", "Query exec error: " << m_query.lastError().text() << " Query: " << m_query.lastQuery());
   } else {
      emit bookmarkChanged( getBookmark(url)->id );
   }
}

void Logbook::addHistory(const HistoryItem &item) {
   addHistory(item.title, item.url, item.id);
}

void Logbook::addHistory(const QString &title, const QString &url, const linkItemId &historyItemId) {
   QDEBUG_EXP("Logbook::addHistory()", "title: " << title << " url: " << url << " id: " << historyItemId);
   if ( historyItemId != LINK_ITEM_ID_UNDEFINED ) { // Update history
      QDEBUG_EXP("Logbook::addHistory()", "Provided Bookmark Id -> Updating");
      m_query.prepare(UPDATE_HISTORY_SQL_BY_ID);
      m_query.bindValue(0, url); m_query.bindValue(1, title); m_query.bindValue(2, historyItemId);
   } else { // Insert History, or Update
      if ( NULL != getHistory(url) ) { // Update, there is already this URL
         QDEBUG_EXP("Logbook::addHistory()", "Updating History");
         m_query.prepare(UPDATE_HISTORY_SQL_BY_URL);
         m_query.bindValue(0, title); m_query.bindValue(1, url);
      } else { // Insert, never stored this URL
         QDEBUG_EXP("Logbook::addHistory()", "Inserting History");
         m_query.prepare(INSERT_HISTORY_SQL);
         m_query.bindValue(0, url); m_query.bindValue(1, title);
      }
   }
   // Let's query!
   if ( !m_query.exec() ) {
      QWARNING_EXP("Logbook::addHistory()", "Query exec error: " << m_query.lastError().text() << " Query: " << m_query.lastQuery());
   } else {
      emit historyChanged( getHistory(url)->id );
   }
}

void Logbook::updateHistoryCounter(const QString &url) {
   QDEBUG_EXP("Logbook::addHistory()", "url: " << url);
   m_query.prepare(UPDATE_HISTORY_LCOUNT_SQL);
   m_query.bindValue(0, url);
   // Let's query!
   if ( !m_query.exec() ) {
      QWARNING_EXP("Logbook::addHistory()", "Query exec error: " << m_query.lastError().text() << " Query: " << m_query.lastQuery());
   } else {
      emit historyChanged( getHistory(url)->id );
   }
}

void Logbook::deleteBookmark(const BookmarkItem &item) {
   if ( item.id != LINK_ITEM_ID_UNDEFINED ) {
      deleteBookmark(item.id);
   } else {
      deleteBookmark(item.url);
   }
}

void Logbook::deleteBookmark(const linkItemId &bookmarkItemId) {
   QDEBUG_EXP("Logbook::deleteBookmark(id)", "id: " << bookmarkItemId);
   m_query.prepare(DELETE_BOOKMARK_SQL_BY_ID);
   m_query.bindValue(0, bookmarkItemId);
   if ( !m_query.exec() ) {
      QWARNING_EXP("Logbook::deleteBookmark(id)", "Query exec error: " << m_query.lastError().text());
   } else {
      emit bookmarksChanged();
   }
}

void Logbook::deleteBookmark(const QString &url) {
   QDEBUG_EXP("Logbook::deleteBookmark(url)", "url: " << url);
   m_query.prepare(DELETE_BOOKMARK_SQL_BY_URL);
   m_query.bindValue(0, url);
   if ( !m_query.exec() ) {
      QDEBUG_EXP("Logbook::deleteBookmark(url)", "Query exec error: " << m_query.lastError().text());
   } else {
      emit bookmarksChanged();
   }
}

void Logbook::deleteBookmarks() {
   QDEBUG("Logbook::deleteBookmarks()");
   if ( !m_query.exec(DELETE_BOOKMARK_ALL_SQL) ) {
      QWARNING_EXP("Logbook::deleteBookmarks()", "Query exec error: " << m_query.lastError().text());
   } else {
      emit bookmarksChanged();
   }
}

void Logbook::deleteHistory(const HistoryItem &item) {
   if ( item.id != LINK_ITEM_ID_UNDEFINED ) {
      deleteHistory(item.id);
   } else {
      deleteHistory(item.url);
   }
}

void Logbook::deleteHistory(const linkItemId &historyItemId) {
   QDEBUG_EXP("Logbook::deleteHistory(id)", "id: " << historyItemId);
   m_query.prepare(DELETE_HISTORY_SQL_BY_ID);
   m_query.bindValue(0, historyItemId);
   if ( !m_query.exec() ) {
      QWARNING_EXP("Logbook::deleteHistory(id)", "Query exec error: " << m_query.lastError().text());
   } else {
      emit historiesChanged();
   }
}

void Logbook::deleteHistory(const QString &url) {
   QDEBUG_EXP("Logbook::deleteHistory(url)", "url: " << url);
   m_query.prepare(DELETE_HISTORY_SQL_BY_URL);
   m_query.bindValue(0, url);
   if ( !m_query.exec() ) {
      QWARNING_EXP("Logbook::deleteHistory(url)", "Query exec error: " << m_query.lastError().text());
   } else {
      emit historiesChanged();
   }
}

void Logbook::deleteHistories() {
   QDEBUG("Logbook::deleteHistories()");
   if ( !m_query.exec(DELETE_HISTORY_ALL_SQL) ) {
      QWARNING_EXP("Logbook::deleteHistories()", "Query exec error: " << m_query.lastError().text());
   } else {
      emit historiesChanged();
   }
}

void Logbook::deleteExpiredHistories() {
   QDEBUG("Logbook::deleteExpiredHistories()");
   m_query.prepare(DELETE_EXPIRED_HISTORY_SQL);
   m_query.addBindValue( QString("-%1 days").arg( m_settings.value(SETTINGS_KEY_HISTORY_LENGTH_DAYS).toString() ) );
   if ( !m_query.exec() ) {
      QWARNING_EXP("Logbook::deleteExpiredHistories()", "Query exec error: " << m_query.lastError().text());
   } else {
      QDEBUG_EXP("Logbook::deleteExpiredHistories()", "Expired Histories deleted: " << m_query.numRowsAffected());
   }
}

void Logbook::setHome(BookmarkItem &item) {
   QDEBUG_EXP("Logbook::setHome(id)", "title: " << item.title << " url: " << item.url << " id: " << item.id);
   if ( item.id != LINK_ITEM_ID_UNDEFINED ) {
      m_settings.setValue(SETTINGS_KEY_HOME_BOOKMARK_ID, item.id);
      item.isHome = true;
      emit homeChanged(item.id);
   }
}

void Logbook::setHome(const QString &url) {
   BookmarkItem *item = getBookmark(url);
   if ( !item ) {
      addBookmark(QString(), url);
      item = getBookmark(url);
   }
   if ( item ) { // Ensuring the item actually exists
      setHome(*item);
      delete item;
   }
}

void Logbook::setHome(const linkItemId &bookmarkItemId) {
   BookmarkItem *item = getBookmark(bookmarkItemId);
   if ( item ) { // Ensuring the item actually exists
      setHome(*item);
      delete item;
   }
}
