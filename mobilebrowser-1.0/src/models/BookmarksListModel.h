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
* File Name   : BookmarksListModel.h
*
* Created     : 15 June 2011
*
* Description :
*
* Model providing user's bookmarks
*
*--------------------------------------------------------
* 1.0 - First open source release
*
*/


#ifndef BOOKMARKSLISTMODEL_H
#define BOOKMARKSLISTMODEL_H

#include <QAbstractListModel>
#include <QModelIndex>
#include <QHash>
#include <QContiguousCache>
#include <QVariant>

#include "Logbook.h"
#include "Declarativable.h"
#include "LinkItemsSharedCaches.h"

/**
  A ListModel for Bookmarks.
  */
class BookmarksListModel : public QAbstractListModel, public Declarativable
{
   Q_OBJECT
public:
   BookmarksListModel();
   BookmarksListModel(Logbook *logbook);
   BookmarksListModel(const BookmarksListModel &copy);
   virtual ~BookmarksListModel();

private:
   void init();

public:
   void setLogbook(Logbook *logbook);

public:
// Implement Declarativable
   static const char * metaTypeName() { return "BookmarksListModel"; }
   static QString declarativeName() { return "bookmarksModel"; }

public:
// Extend QAbstractListModel
   int rowCount(const QModelIndex &parent = QModelIndex()) const;
   QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
   Qt::ItemFlags flags(const QModelIndex &index) const;
   QModelIndex index (int row, int column, const QModelIndex &parent = QModelIndex()) const;

public slots:
   void remove(const linkItemId &bookmarkItemId);
   void removeAll();

private:
   BookmarkItem * getCachedBookmark(const linkItemId &id);
   linkItemId getCachedBookmarkId(const int &row);

private slots:
   void onLogbookBookmarkChanged(const linkItemId &id);
   void onLogbookBookmarksChanged();

private:
   Logbook                             *m_logbook;
   QHash<int, QByteArray>              m_roleNames;
   BookmarkItemsCache                  *m_bookmarkItemsCache;
   QContiguousCache<linkItemId>        m_bookmarkItemsIdCache;
   int                                 m_currentRowCount;
};
Q_DECLARE_METATYPE(BookmarksListModel)

#endif // BOOKMARKSLISTMODEL_H
