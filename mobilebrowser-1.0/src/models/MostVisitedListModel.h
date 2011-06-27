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
* File Name   : MostVisitedListModel.h
*
* Created     : 15 June 2011
*
* Description :
*
* Model providing user's most visited URLs
*
*--------------------------------------------------------
* 1.0 - First open source release
*
*/


#ifndef MOSTVISITEDLISTMODEL_H
#define MOSTVISITEDLISTMODEL_H

#include <QAbstractListModel>
#include <QModelIndex>
#include <QHash>
#include <QContiguousCache>
#include <QVariant>

#include "Logbook.h"
#include "Declarativable.h"
#include "LinkItemsSharedCaches.h"

/**
  A ListModel for MostVisited.
  */
class MostVisitedListModel : public QAbstractListModel, public Declarativable
{
   Q_OBJECT
public:
   MostVisitedListModel();
   MostVisitedListModel(Logbook *logbook);
   MostVisitedListModel(const MostVisitedListModel &copy);
   virtual ~MostVisitedListModel();

private:
   void init();

public:
   void setLogbook(Logbook *logbook);

public:
// Implement ListModel
   static const char * metaTypeName() { return "MostVisitedListModel"; }
   static QString declarativeName() { return "mostVisitedModel"; }

public:
// Extend QAbstractListModel
   int rowCount(const QModelIndex &parent = QModelIndex()) const;
   QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
   Qt::ItemFlags flags(const QModelIndex &index) const;
   QModelIndex index (int row, int column, const QModelIndex &parent = QModelIndex()) const;

private:
   HistoryItem * getCachedMostVisited(const linkItemId &mostVisitedItemId);
   linkItemId getCachedMostVisitedId(const int &row);

private slots:
   void onLogbookHistoryChanged(const linkItemId &id);
   void onLogbookHistoriesChanged();

private:
   Logbook                             *m_logbook;
   QHash<int, QByteArray>              m_roleNames;
   HistoryItemsCache                   *m_mostVisitedItemsCache;
   QContiguousCache<linkItemId>        m_mostVisitedItemsIdCache;
   int                                 m_currentRowCount;
};
Q_DECLARE_METATYPE(MostVisitedListModel)

#endif // MOSTVISITEDLISTMODEL_H
