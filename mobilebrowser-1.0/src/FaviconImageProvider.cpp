#include "FaviconImageProvider.h"

#include <QSqlError>
#include <QVariant>
#include <QWebSettings>

#include "macros.h"

#define DB_CONN                  "faviconimageprovider_db_conn"
#define DEFAULT_FAVICON_PATH     ":/qmls/pics/bookmark-icon-30x30.png"

// NOTE The following SQL Query depends on the SQL Schema of the WebKit's Icon Database
#define SELECT_ICON_DATA_BY_URL_SQL    "SELECT data FROM PageUrl JOIN IconData ON PageUrl.iconID = IconData.iconID WHERE PageUrl.url = ?"

FaviconImageProvider::FaviconImageProvider() :
      QDeclarativeImageProvider(QDeclarativeImageProvider::Image),
      m_db(QSqlDatabase::addDatabase("QSQLITE", DB_CONN)),
      m_defaultFavicon(QImage(DEFAULT_FAVICON_PATH))
{
   QString faviconDbPath = QWebSettings::globalSettings()->iconDatabasePath();
   QDEBUG_EXP("FaviconImageProvider::FaviconImageProvider()","favicon db path:" << faviconDbPath);
   m_db.setDatabaseName(QWebSettings::globalSettings()->iconDatabasePath());
   m_db.open();
   m_query = QSqlQuery(m_db);
}

FaviconImageProvider::~FaviconImageProvider() {
   if ( m_db.isOpen() ) m_db.close();
}

QImage FaviconImageProvider::requestImage(const QString &id, QSize *size, const QSize &reqSize)
{
   Q_UNUSED(reqSize)
   QVERBOSE_EXP("FaviconImageProvider::request()", "id: " << id << " - width: " << size->width() << " height: " << size->height() << " - reqwidth: " << reqSize.width() << " reqheight: " << reqSize.height());

   m_query.prepare(SELECT_ICON_DATA_BY_URL_SQL);
   m_query.addBindValue(id);
   if ( m_query.exec() ) {
      if ( m_query.next() ) {
         QImage image;
         image.loadFromData(m_query.value(0).toByteArray());
         // Set the QImage size before returning the actual QImage
         if (size) *size = m_defaultFavicon.size();
         return image;
      } else {
         QVERBOSE_EXP("FaviconImageProvider::request()", "Sorry, no favicon for: " << id);
      }
   } else {
      QDEBUG_EXP("FaviconImageProvider::request()", "Query.exec() error: " << m_query.lastError().text());
   }

   // Set the QImage size before returning the actual QImage
   if (size) *size = m_defaultFavicon.size();
   return m_defaultFavicon;
}
