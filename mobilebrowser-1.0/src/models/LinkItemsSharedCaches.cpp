#include "LinkItemsSharedCaches.h"

#define CACHE_MAX_COST  30

// public:
HistoryItemsCache * LinkItemsSharedCaches::acquireHistoryItemsCache() {
   ++(*LinkItemsSharedCaches::historyItemsCacheRefCounter());
   return LinkItemsSharedCaches::historyItemsCache();
}

BookmarkItemsCache * LinkItemsSharedCaches::acquireBookmarkItemsCache() {
   ++(*LinkItemsSharedCaches::bookmarkItemsCacheRefCounter());
   return LinkItemsSharedCaches::bookmarkItemsCache();
}

void LinkItemsSharedCaches::releaseHistoryItemsCache() {
   --(*LinkItemsSharedCaches::historyItemsCacheRefCounter());
   if ( (*LinkItemsSharedCaches::historyItemsCacheRefCounter()) == 0 ) {
      delete LinkItemsSharedCaches::historyItemsCache();
   }
}

void LinkItemsSharedCaches::releaseBookmarkItemsCache() {
   --(*LinkItemsSharedCaches::bookmarkItemsCacheRefCounter());
   if ( (*LinkItemsSharedCaches::bookmarkItemsCacheRefCounter()) == 0 ) {
      delete LinkItemsSharedCaches::bookmarkItemsCache();
   }
}

// private:
HistoryItemsCache * LinkItemsSharedCaches::historyItemsCache() {
   static HistoryItemsCache *hiCache = new HistoryItemsCache(CACHE_MAX_COST);
   return hiCache;
}

BookmarkItemsCache * LinkItemsSharedCaches::bookmarkItemsCache() {
   static BookmarkItemsCache *biCache = new BookmarkItemsCache(CACHE_MAX_COST);
   return biCache;
}

int * LinkItemsSharedCaches::historyItemsCacheRefCounter() {
   static int * hiRefCounter = new int(0);
   return hiRefCounter;
}

int * LinkItemsSharedCaches::bookmarkItemsCacheRefCounter() {
   static int * biRefCounter = new int(0);
   return biRefCounter;
}

LinkItemsSharedCaches::LinkItemsSharedCaches() {
   // Nothing to do here. This class can't be instantiated.
}
