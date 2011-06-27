#include "BookmarksListModel.h"

#include <QList>

#include "macros.h"

#define ROW_ITEM_ID_CACHE_SIZE   30

#define ROLE_TITLE      "title"
#define ROLE_TITLE_ID   0x1000
#define ROLE_URL        "url"
#define ROLE_URL_ID     0x1001
#define ROLE_ISHOME     "isHome"
#define ROLE_ISHOME_ID  0x1002
#define ROLE_ID         "id"
#define ROLE_ID_ID      0x1004

BookmarksListModel::BookmarksListModel() :
   QAbstractListModel(),
   m_logbook(NULL)
{
   m_roleNames.insert(ROLE_TITLE_ID,  ROLE_TITLE);
   m_roleNames.insert(ROLE_URL_ID,    ROLE_URL);
   m_roleNames.insert(ROLE_ISHOME_ID, ROLE_ISHOME);
   m_roleNames.insert(ROLE_ID_ID, ROLE_ID);

   init();
}

BookmarksListModel::BookmarksListModel(Logbook *logbook) :
   QAbstractListModel(),
   m_logbook(logbook)
{
   m_roleNames.insert(ROLE_TITLE_ID,  ROLE_TITLE);
   m_roleNames.insert(ROLE_URL_ID,    ROLE_URL);
   m_roleNames.insert(ROLE_ISHOME_ID, ROLE_ISHOME);
   m_roleNames.insert(ROLE_ID_ID, ROLE_ID);

   init();
}

BookmarksListModel::BookmarksListModel(const BookmarksListModel &copy) :
   QAbstractListModel(),
   m_logbook(copy.m_logbook),
   m_roleNames(copy.m_roleNames)
{
   init();
}

BookmarksListModel::~BookmarksListModel() {
   // Nothing to do here
   QDEBUG("BookmarksListModel::~BookmarksListModel()");
   LinkItemsSharedCaches::releaseBookmarkItemsCache();
}

// private:
void BookmarksListModel::init() {
   QDEBUG("BookmarksListModel::init()");
   // Register RoleNames for that this Model will "answer for"
   setRoleNames(m_roleNames);

   // Initialize caches
   m_bookmarkItemsCache = LinkItemsSharedCaches::acquireBookmarkItemsCache();
   m_bookmarkItemsIdCache.setCapacity(ROW_ITEM_ID_CACHE_SIZE);

   if ( m_logbook ) {
      // Clear BookmarkItemsIdCache when the Logbook's Bookmarks change:
      // this is because the association "(row, id)" might change.
      connect(m_logbook, SIGNAL(bookmarksChanged()), this, SLOT(onLogbookBookmarksChanged()));
      connect(m_logbook, SIGNAL(bookmarkChanged(linkItemId)), this, SLOT(onLogbookBookmarkChanged(linkItemId)));
   }

   m_currentRowCount = m_logbook->getBookmarksCount(); //< Update RowCount
}

// public:
void BookmarksListModel::setLogbook(Logbook *logbook) {
   if ( m_logbook != logbook ) {
      if ( m_logbook ) {
         disconnect(m_logbook, SIGNAL(bookmarksChanged()), this, SLOT(onLogbookBookmarksChanged()));
         disconnect(m_logbook, SIGNAL(bookmarkChanged(linkItemId)), this, SLOT(onLogbookBookmarkChanged(linkItemId)));
      }
      m_logbook = logbook;
      if ( m_logbook ) {
         connect(m_logbook, SIGNAL(bookmarksChanged()), this, SLOT(onLogbookBookmarksChanged()));
         connect(m_logbook, SIGNAL(bookmarkChanged(linkItemId)), this, SLOT(onLogbookBookmarkChanged(linkItemId)));
      }
   }
}

// public: Extend QAbstractListModel
int BookmarksListModel::rowCount(const QModelIndex &parent) const {
   Q_UNUSED(parent)
   QVERBOSE_EXP("BookmarksListModel::rowCount()", m_currentRowCount);
   return m_currentRowCount;
}

QVariant BookmarksListModel::data(const QModelIndex &index, int role) const {
   QVERBOSE_EXP("BookmarksListModel::data()", "index.row: " << index.row() << " role: " << m_roleNames.value(role));
   BookmarkItem * item = const_cast<BookmarksListModel *>(this)->getCachedBookmark(index.row());
   if ( item ) {
      switch( role ) {
         case ROLE_TITLE_ID: return QVariant(item->title);
         case ROLE_URL_ID: return QVariant(item->url);
         case ROLE_ISHOME_ID: return QVariant(m_logbook->isHome(item->id));
         case ROLE_ID_ID: return QVariant(item->id);
         default:
            QDEBUG_EXP("BookmarksListModel::data", "Requested data for unknown role: " << role);
            return QVariant();
      }
   } else {
      QDEBUG_EXP("BookmarksListModel::data", "Requested data for index.row() that doesn't exists: " << index.row());
      return QVariant();
   }
}

Qt::ItemFlags BookmarksListModel::flags(const QModelIndex &index) const {
   Q_UNUSED(index)
   return (Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

QModelIndex BookmarksListModel::index (int row, int column, const QModelIndex & parent) const {
   Q_UNUSED(parent)
   // Map row-position to the correspondent Bookmark Id
   return createIndex(const_cast<BookmarksListModel *>(this)->getCachedBookmarkId(row), column, (void *)NULL);
}

// public slots:
void BookmarksListModel::remove(const linkItemId &bookmarkItemId) {
   QDEBUG_EXP("BookmarksListModel::remove(id)", "Id: " << bookmarkItemId);
   m_logbook->deleteBookmark(bookmarkItemId);

   m_bookmarkItemsCache->remove(bookmarkItemId);
   m_bookmarkItemsIdCache.clear();

   --m_currentRowCount; //< Update RowCount
}

void BookmarksListModel::removeAll() {
   QDEBUG("BookmarksListModel::removeAll()");
   m_logbook->deleteBookmarks();

   m_bookmarkItemsCache->clear();
   m_bookmarkItemsIdCache.clear();

   m_currentRowCount = 0; //< Update RowCount
}

// private:
BookmarkItem * BookmarksListModel::getCachedBookmark(const linkItemId &id) {
   BookmarkItem *result = NULL;

   result = m_bookmarkItemsCache->object(id);
   if ( !result ) {
      QVERBOSE_EXP("BookmarksListModel::getCachedBookmark(id)", "id: " << id << " - Cache Miss!");
      result = m_logbook->getBookmark(id);
      if ( result ) {
         m_bookmarkItemsCache->insert(id, result);
      }
   }

   return result;
}

linkItemId BookmarksListModel::getCachedBookmarkId(const int &row) {
   while(row > m_bookmarkItemsIdCache.lastIndex()) {
      QVERBOSE_EXP("BookmarksListModel::getCachedBookmarkId()", "Cache miss! - Low bound");
      m_bookmarkItemsIdCache.append(m_logbook->getBookmarkIdAt(m_bookmarkItemsIdCache.lastIndex()+1));
   }
   while(row < m_bookmarkItemsIdCache.firstIndex()) {
      QVERBOSE_EXP("BookmarksListModel::getCachedBookmarkId()", "Cache miss! - High bound");
      m_bookmarkItemsIdCache.prepend(m_logbook->getBookmarkIdAt(m_bookmarkItemsIdCache.firstIndex()-1));
   }

   return m_bookmarkItemsIdCache.at(row);
}

void BookmarksListModel::onLogbookBookmarkChanged(const linkItemId &id) {
   Q_UNUSED(id)
   m_bookmarkItemsCache->insert(id, m_logbook->getBookmark(id)); //< Update only the item that actually changed
   m_bookmarkItemsIdCache.clear(); //< The (row,id) association needs to be regenerated
   m_currentRowCount = m_logbook->getBookmarksCount(); //< Update RowCount
   reset(); //< Model Reset - The View will redraw
}

void BookmarksListModel::onLogbookBookmarksChanged() {
   m_bookmarkItemsCache->clear();
   m_bookmarkItemsIdCache.clear(); //< The (row,id) association needs to be regenerated
   m_currentRowCount = m_logbook->getBookmarksCount(); //< Update RowCount
   reset(); //< Model Reset - The View will redraw
}
