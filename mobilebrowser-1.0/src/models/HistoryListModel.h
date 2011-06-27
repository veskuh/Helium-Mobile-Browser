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
* File Name   : HistoryListModel.h
*
* Created     : 15 June 2011
*
* Description :
*
* Model providing user's browsing history
*
*--------------------------------------------------------
* 1.0 - First open source release
*
*/

#ifndef HISTORYLISTMODEL_H
#define HISTORYLISTMODEL_H

#include <QAbstractListModel>
#include <QModelIndex>
#include <QHash>
#include <QContiguousCache>
#include <QVariant>

#include "Logbook.h"
#include "Declarativable.h"
#include "LinkItemsSharedCaches.h"

/**
  A ListModel for History.
  */
class HistoryListModel : public QAbstractListModel, public Declarativable
{
   Q_OBJECT
public:
   HistoryListModel();
   HistoryListModel(Logbook *logbook);
   HistoryListModel(const HistoryListModel &copy);
   virtual ~HistoryListModel();

private:
   void init();

public:
   void setLogbook(Logbook *logbook);

public:
// Implement ListModel
   static const char * metaTypeName() { return "HistoryListModel"; }
   static QString declarativeName() { return "historyModel"; }

public:
// Extend QAbstractListModel
   int rowCount(const QModelIndex &parent = QModelIndex()) const;
   QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
   Qt::ItemFlags flags(const QModelIndex &index) const;
   QModelIndex index (int row, int column, const QModelIndex &parent = QModelIndex()) const;

public slots:
   void remove(const linkItemId &historyItemId);
   void removeAll();

private:
   HistoryItem * getCachedHistory(const linkItemId &id);
   linkItemId getCachedHistoryId(const int &row);

private slots:
   void onLogbookHistoryChanged(const linkItemId &id);
   void onLogbookHistoriesChanged();

private:
   Logbook                             *m_logbook;
   QHash<int, QByteArray>              m_roleNames;
   HistoryItemsCache                   *m_historyItemsCache;
   QContiguousCache<linkItemId>        m_historyItemsIdCache;
   int                                 m_currentRowCount;
};
Q_DECLARE_METATYPE(HistoryListModel)

#endif // HISTORYLISTMODEL_H
