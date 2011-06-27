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
* File Name   : LogbookLinkItems.h
*
* Created     : 15 June 2011
*
* Description :
*
* Generic list item to be show in the logbook view
*
*--------------------------------------------------------
* 1.0 - First open source release
*
*/

#ifndef LOGBOOKLINKITEMS_H
#define LOGBOOKLINKITEMS_H

#include <QString>
#include <QDateTime>

#include "LinkItem.h"

/** Holds the information about one Bookmark. */
struct BookmarkItem : LinkItem
{
   bool isHome;

   BookmarkItem(const QString &title, const QString &url, const bool &isHome = false, const linkItemId &id = LINK_ITEM_ID_UNDEFINED) :
      LinkItem(title, url, id)
   {
      this->isHome = isHome;
   }

   BookmarkItem() :
      LinkItem()
   {
      isHome = false;
   }
};

/** Holds the information about one History item. */
struct HistoryItem: LinkItem
{
   QDateTime last_visit_datetime;
   quint64 load_count;

   HistoryItem(const QString &title, const QString &url, const quint64 &lcount = 0, const QDateTime &lvdatetime = QDateTime(), const linkItemId &id = LINK_ITEM_ID_UNDEFINED) :
      LinkItem(title, url, id)
   {
      this->last_visit_datetime = lvdatetime;
      this->load_count = lcount;
   }

   HistoryItem() :
      LinkItem()
   {
      last_visit_datetime = QDateTime();
      load_count = 0;
   }
};

#endif // LOGBOOKLINKITEMS_H
