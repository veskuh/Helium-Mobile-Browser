#include "MostVisitedListModel.h"

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
#define ROLE_ID                  "id"
#define ROLE_ID_ID               0x1004

MostVisitedListModel::MostVisitedListModel() :
   QAbstractListModel(),
   m_logbook(NULL)
{
   m_roleNames.insert(ROLE_TITLE_ID,  ROLE_TITLE);
   m_roleNames.insert(ROLE_URL_ID,    ROLE_URL);
   m_roleNames.insert(ROLE_LAST_VISIT_ID, ROLE_LAST_VISIT);
   m_roleNames.insert(ROLE_ID_ID, ROLE_ID);

   init();
}

MostVisitedListModel::MostVisitedListModel(Logbook *logbook) :
   QAbstractListModel(),
   m_logbook(logbook)
{
   m_roleNames.insert(ROLE_TITLE_ID,  ROLE_TITLE);
   m_roleNames.insert(ROLE_URL_ID,    ROLE_URL);
   m_roleNames.insert(ROLE_LAST_VISIT_ID, ROLE_LAST_VISIT);
   m_roleNames.insert(ROLE_ID_ID, ROLE_ID);

   init();
}

MostVisitedListModel::MostVisitedListModel(const MostVisitedListModel &copy) :
   QAbstractListModel(),
   m_logbook(copy.m_logbook),
   m_roleNames(copy.m_roleNames)
{
   init();
}

MostVisitedListModel::~MostVisitedListModel() {
   // Nothing to do here
   QDEBUG("MostVisitedListModel::~MostVisitedListModel()");
   LinkItemsSharedCaches::releaseHistoryItemsCache();
}

// private:
void MostVisitedListModel::init() {
   QDEBUG("MostVisitedListModel::init()");
   // Register RoleNames for that this Model will "answer for"
   setRoleNames(m_roleNames);

   // Initialize caches
   m_mostVisitedItemsCache = LinkItemsSharedCaches::acquireHistoryItemsCache();
   m_mostVisitedItemsIdCache.setCapacity(ROW_ITEM_ID_CACHE_SIZE);

   if ( m_logbook ) {
      connect(m_logbook, SIGNAL(historiesChanged()), this, SLOT(onLogbookHistoriesChanged()));
      connect(m_logbook, SIGNAL(historyChanged(linkItemId)), this, SLOT(onLogbookHistoryChanged(linkItemId)));
   }

   m_currentRowCount = m_logbook->getMostVisitedHistoriesCount();
}

// public:
void MostVisitedListModel::setLogbook(Logbook *logbook) {
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
int MostVisitedListModel::rowCount(const QModelIndex &parent) const {
   Q_UNUSED(parent)
   QVERBOSE_EXP("MostVisitedListModel::rowCount()", m_currentRowCount);
   return m_currentRowCount;
}

QVariant MostVisitedListModel::data(const QModelIndex &index, int role) const {
   QVERBOSE_EXP("MostVisitedListModel::data()", "index.row: " << index.row() << " role: " << m_roleNames.value(role));
   HistoryItem * item = const_cast<MostVisitedListModel *>(this)->getCachedMostVisited(index.row());
   if ( item ) {
      switch( role ) {
         case ROLE_TITLE_ID: return QVariant(item->title);
         case ROLE_URL_ID: return QVariant(item->url);
         case ROLE_LAST_VISIT_ID: return QVariant(item->last_visit_datetime);
         case ROLE_ID_ID: return QVariant(item->id);
         default:
            QDEBUG_EXP("MostVisitedListModel::data", "Requested data for unknown role: " << role);
            return QVariant();
      }
   }
   else {
      QDEBUG_EXP("MostVisitedListModel::data", "Requested data for index.row() that doesn't exists: " << index.row());
      return QVariant();
   }
}

Qt::ItemFlags MostVisitedListModel::flags(const QModelIndex &index) const {
   Q_UNUSED(index)
   return (Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

QModelIndex MostVisitedListModel::index (int row, int column, const QModelIndex & parent) const {
   Q_UNUSED(parent)
   // Map row-position to the correspondent History Id
   return createIndex(const_cast<MostVisitedListModel *>(this)->getCachedMostVisitedId(row), column, (void *)NULL);
}

// private:
HistoryItem * MostVisitedListModel::getCachedMostVisited(const linkItemId &id) {
   HistoryItem *result = NULL;

   result = m_mostVisitedItemsCache->object(id);
   if ( !result ) {
      QVERBOSE_EXP("MostVisitedListModel::getCachedMostVisited(id)", "id: " << id << " - Cache Miss!");
      result = m_logbook->getHistory(id);
      if ( result ) {
         m_mostVisitedItemsCache->insert(id, result);
      }
   }

   return result;
}

linkItemId MostVisitedListModel::getCachedMostVisitedId(const int &row) {
   while(row > m_mostVisitedItemsIdCache.lastIndex()) {
      QVERBOSE_EXP("MostVisitedListModel::getCachedMostVisitedId()", "Cache miss! - Low bound");
      m_mostVisitedItemsIdCache.append(m_logbook->getMostVisitedHistoryIdAt(m_mostVisitedItemsIdCache.lastIndex()+1));
   }
   while(row < m_mostVisitedItemsIdCache.firstIndex()) {
      QVERBOSE_EXP("MostVisitedListModel::getCachedMostVisitedId()", "Cache miss! - High bound");
      m_mostVisitedItemsIdCache.prepend(m_logbook->getMostVisitedHistoryIdAt(m_mostVisitedItemsIdCache.firstIndex()-1));
   }

   return m_mostVisitedItemsIdCache.at(row);
}

void MostVisitedListModel::onLogbookHistoryChanged(const linkItemId &id) {
   m_mostVisitedItemsCache->insert(id, m_logbook->getHistory(id)); //< Update only the item that actually changed
   m_mostVisitedItemsIdCache.clear(); //< The (row,id) association needs to be regenerated
   m_currentRowCount = m_logbook->getMostVisitedHistoriesCount();
   reset(); //< Model Reset
}

void MostVisitedListModel::onLogbookHistoriesChanged() {
   m_mostVisitedItemsCache->clear(); //< The (id, HistoryItem) association needs to be regenerated
   m_mostVisitedItemsIdCache.clear(); //< The (row,id) association needs to be regenerated
   m_currentRowCount = m_logbook->getMostVisitedHistoriesCount();
   reset(); //< Model Reset
}
