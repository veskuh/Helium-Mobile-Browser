#include "CoreDbHelper.h"

#include <QSqlQuery>
#include <QSqlError>

#include "macros.h"

#define CORE_DB_FILEPATH   "core.db"
#define CORE_DB_VERSION    2

#define CREATE_TABLE_BOOKMARK "CREATE TABLE IF NOT EXISTS " \
      "bookmark ( " \
         "url TEXT NOT NULL, " \
         "title TEXT DEFAULT NULL, " \
         "id INTEGER PRIMARY KEY, " \
         "UNIQUE (url)" \
      ")"

#define CREATE_TABLE_HISTORY "CREATE TABLE IF NOT EXISTS " \
      "history ( " \
         "url TEXT NOT NULL, " \
         "title TEXT DEFAULT NULL, " \
         "last_visit_timestamp INTEGER DEFAULT (datetime('now')), " \
         "load_count INTEGER DEFAULT 1, " \
         "id INTEGER PRIMARY KEY, " \
         "UNIQUE (url)" \
      ")"

CoreDbHelper::CoreDbHelper(const QString &filePath, const int requestedVersion, const QString &defaultConnName ) :
    SqliteDbHelper( filePath, requestedVersion, defaultConnName )
{
   // Nothing to do, all we need is nicely handled from the super-class, for now.
   QDEBUG_EXP("CoreDbHelper::CoreDbHelper()", "DB storage path: "       << filePath );
   QDEBUG_EXP("CoreDbHelper::CoreDbHelper()", "DB version: "            << requestedVersion );
   QDEBUG_EXP("CoreDbHelper::CoreDbHelper()", "DB default connection: " << defaultConnName );
}

CoreDbHelper::~CoreDbHelper() {
   QDEBUG("CoreDbHelper::~CoreDbHelper()");
}

void CoreDbHelper::onCreate(QSqlDatabase &database) {
   database.transaction();
   QSqlQuery query(database);
   if ( !query.exec(CREATE_TABLE_BOOKMARK) ) {
      QWARNING_EXP("CoreDbHelper::onCreate()", query.lastError().text());
   }
   if ( !query.exec(CREATE_TABLE_HISTORY) ) {
      QWARNING_EXP("CoreDbHelper::onCreate()", query.lastError().text());
   }
   if ( !database.commit() ) {
      QDEBUG_EXP("CoreDbHelper::onCreate()", "DB: " << database.databaseName() << " - " << query.lastError().text());
      QDEBUG_EXP("CoreDbHelper::onCreate()", "Last Query: " << query.lastQuery());
      QWARNING_EXP("CoreDbHelper::onCreate()", "Couldn't create tables");
   } else {
      QDEBUG_EXP("CoreDbHelper::onCreate()", "Done.");
   }
}
