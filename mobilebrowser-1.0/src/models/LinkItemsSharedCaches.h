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
* File Name   : LinkItemsSharedCaches.h
*
* Created     : 15 June 2011
*
* Description :
*
* History and bookmarks cache to avoid continuous DB interaction
*
*--------------------------------------------------------
* 1.0 - First open source release
*
*/

#ifndef LINKITEMSSHAREDCACHES_H
#define LINKITEMSSHAREDCACHES_H

#include <QCache>

#include "LogbookLinkItems.h"

typedef QCache<linkItemId, HistoryItem> HistoryItemsCache;
typedef QCache<linkItemId, BookmarkItem> BookmarkItemsCache;

class LinkItemsSharedCaches
{
public:
   static HistoryItemsCache * acquireHistoryItemsCache();
   static BookmarkItemsCache * acquireBookmarkItemsCache();

   static void releaseHistoryItemsCache();
   static void releaseBookmarkItemsCache();

private:
   static HistoryItemsCache * historyItemsCache();
   static BookmarkItemsCache * bookmarkItemsCache();

   static int * historyItemsCacheRefCounter();
   static int * bookmarkItemsCacheRefCounter();

private:
   LinkItemsSharedCaches();
};

#endif // LINKITEMSSHAREDCACHES_H
