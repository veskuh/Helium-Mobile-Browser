#include "HistoryListModel.h"

#include <QList>
#include <QDate>
#include <QDateTime>

#include "macros.h"

#define ROW_ITEM_ID_CACHE_SIZE   50

#define ROLE_TITLE               "title"
#define ROLE_TITLE_ID            0x1000
#define ROLE_URL                 "url"
#define ROLE_URL_ID              0x1001
#define ROLE_LAST_VISIT          "lastVisit"
#define ROLE_LAST_VISIT_ID       0x1002
#define ROLE_LAST_VISIT_DAY      "lastVisitDay"
#define ROLE_LAST_VISIT_DAY_ID   0x1004
#define ROLE_ID                  "id"
#define ROLE_ID_ID               0x1008

HistoryListModel::HistoryListModel() :
   QAbstractListModel(),
   m_logbook(NULL)
{
   m_roleNames.insert(ROLE_TITLE_ID,  ROLE_TITLE);
   m_roleNames.insert(ROLE_URL_ID,    ROLE_URL);
   m_roleNames.insert(ROLE_LAST_VISIT_ID, ROLE_LAST_VISIT);
   m_roleNames.insert(ROLE_LAST_VISIT_DAY_ID, ROLE_LAST_VISIT_DAY);
   m_roleNames.insert(ROLE_ID_ID, ROLE_ID);

   init();
}

HistoryListModel::HistoryListModel(Logbook *logbook) :
   QAbstractListModel(),
   m_logbook(logbook)
{
   m_roleNames.insert(ROLE_TITLE_ID,  ROLE_TITLE);
   m_roleNames.insert(ROLE_URL_ID,    ROLE_URL);
   m_roleNames.insert(ROLE_LAST_VISIT_ID, ROLE_LAST_VISIT);
   m_roleNames.insert(ROLE_LAST_VISIT_DAY_ID, ROLE_LAST_VISIT_DAY);
   m_roleNames.insert(ROLE_ID_ID, ROLE_ID);

   init();
}

HistoryListModel::HistoryListModel(const HistoryListModel &copy) :
   QAbstractListModel(),
   m_logbook(copy.m_logbook),
   m_roleNames(copy.m_roleNames)
{
   init();
}

HistoryListModel::~HistoryListModel() {
   // Nothing to do here
   QDEBUG("HistoryListModel::~HistoryListModel()");
   LinkItemsSharedCaches::releaseHistoryItemsCache();
}

// private:
void HistoryListModel::init() {
   QDEBUG("HistoryListModel::init()");
   // Register RoleNames for that this Model will "answer for"
   setRoleNames(m_roleNames);

   // Initialize caches
   m_historyItemsCache = LinkItemsSharedCaches::acquireHistoryItemsCache();
   m_historyItemsIdCache.setCapacity(ROW_ITEM_ID_CACHE_SIZE);

   if ( m_logbook ) {
      connect(m_logbook, SIGNAL(historiesChanged()), this, SLOT(onLogbookHistoriesChanged()));
      connect(m_logbook, SIGNAL(historyChanged(linkItemId)), this, SLOT(onLogbookHistoryChanged(linkItemId)));
   }

   m_currentRowCount = m_logbook->getHistoriesCount(); //< Update RowCount
}

// public:
void HistoryListModel::setLogbook(Logbook *logbook) {
   if ( m_logbook != logbook ) {
      if ( m_logbook ) {
         disconnect(m_logbook, SIGNAL(historiesChanged()), this, SLOT(onLogbookHistoriesChanged()));
         disconnect(m_logbook, SIGNAL(historyChanged(linkItemId)), this, SLOT(onLogbookHistoryChanged(linkItemId)));
      }
      m_logbook = logbook;
      if ( m_logbook ) {
         connect(m_logbook, SIGNAL(historiesChanged()), this, SLOT(onLogbookHistoriesChanged()));
         connect(m_logbook, SIGNAL(historyChanged(linkItemId)), this, SLOT(onLogbookHistoryChanged(linkItemId)));
      }
   }
}

// public: Extend QAbstractListModel
int HistoryListModel::rowCount(const QModelIndex &parent) const {
   Q_UNUSED(parent)
   QVERBOSE_EXP("HistoryListModel::rowCount()", m_currentRowCount);
   return m_currentRowCount;
}

QVariant HistoryListModel::data(const QModelIndex &index, int role) const {
   QVERBOSE_EXP("HistoryListModel::data()", "index.row: " << index.row() << " role: " << m_roleNames.value(role));
   HistoryItem * item = const_cast<HistoryListModel *>(this)->getCachedHistory(index.row());
   if ( item ) {
      switch( role ) {
         case ROLE_TITLE_ID: return QVariant(item->title);
         case ROLE_URL_ID: return QVariant(item->url);
         case ROLE_LAST_VISIT_ID: return QVariant(item->last_visit_datetime);
         case ROLE_LAST_VISIT_DAY_ID: {
               if ( item->last_visit_datetime.date() == QDate::currentDate() )
                  return QVariant("Today");
               if ( item->last_visit_datetime.date() == QDate::currentDate().addDays(-1) )
                  return QVariant("Yesterday");
               return QVariant(item->last_visit_datetime.toString("ddd dd MMMM yyyy"));
         }
         case ROLE_ID_ID: return QVariant(item->id);
         default:
            QDEBUG_EXP("HistoryListModel::data", "Requested data for unknown role: " << role);
            return QVariant();
      }
   }
   else {
      QDEBUG_EXP("HistoryListModel::data", "Requested data for index.row() that doesn't exists: " << index.row());
      return QVariant();
   }
}

Qt::ItemFlags HistoryListModel::flags(const QModelIndex &index) const {
   Q_UNUSED(index)
   return (Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

QModelIndex HistoryListModel::index (int row, int column, const QModelIndex & parent) const {
   Q_UNUSED(parent)
   // Map row-position to the correspondent History Id
   return createIndex(const_cast<HistoryListModel *>(this)->getCachedHistoryId(row), column, (void *)NULL);
}

// public slots:
void HistoryListModel::remove(const linkItemId &historyItemId) {
   QDEBUG_EXP("HistoryListModel::remove(id)", "Id: " << historyItemId);
   m_logbook->deleteBookmark(historyItemId);
   m_historyItemsCache->remove(historyItemId);
   m_historyItemsIdCache.clear();
   --m_currentRowCount;
}

void HistoryListModel::removeAll() {
   QDEBUG("HistoryListModel::removeAll()");
   m_logbook->deleteHistories();
   m_historyItemsCache->clear();
   m_historyItemsIdCache.clear();
   m_currentRowCount = 0;
}

// private:
HistoryItem * HistoryListModel::getCachedHistory(const linkItemId &id) {
   HistoryItem *result = NULL;

   result = m_historyItemsCache->object(id);
   if ( !result ) {
      QVERBOSE_EXP("HistoryListModel::getCachedHistory(id)", "id: " << id << " - Cache Miss!");
      result = m_logbook->getHistory(id);
      if ( result ) {
         m_historyItemsCache->insert(id, result);
      }
   }

   return result;
}

linkItemId HistoryListModel::getCachedHistoryId(const int &row) {
   while(row > m_historyItemsIdCache.lastIndex()) {
      QVERBOSE_EXP("HistoryListModel::getCachedHistoryId()", "Cache miss! - Low bound");
      m_historyItemsIdCache.append(m_logbook->getHistoryIdAt(m_historyItemsIdCache.lastIndex()+1));
   }
   while(row < m_historyItemsIdCache.firstIndex()) {
      QVERBOSE_EXP("HistoryListModel::getCachedHistoryId()", "Cache miss! - High bound");
      m_historyItemsIdCache.prepend(m_logbook->getHistoryIdAt(m_historyItemsIdCache.firstIndex()-1));
   }

   return m_historyItemsIdCache.at(row);
}

void HistoryListModel::onLogbookHistoryChanged(const linkItemId &id) {
   m_historyItemsCache->insert(id, m_logbook->getHistory(id)); //< Update only the item that actually changed
   m_historyItemsIdCache.clear(); //< The (row,id) association needs to be regenerated
   m_currentRowCount = m_logbook->getHistoriesCount(); //< Update RowCount
   reset(); //< Model Reset
}

void HistoryListModel::onLogbookHistoriesChanged() {
   m_historyItemsCache->clear(); //< The (id, HistoryItem) association needs to be regenerated
   m_historyItemsIdCache.clear(); //< The (row,id) association needs to be regenerated
   m_currentRowCount = m_logbook->getHistoriesCount(); //< Update RowCount
   reset(); //< Model Reset
}
