/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtNetwork module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights.  These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**
**
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/
#ifndef QGCONFBACKEND_P_H
#define QGCONFBACKEND_P_H

#include "gconfsymbols_p.h"
#include "QtCore/qvariant.h"
#include "QtCore/qmap.h"

#ifdef Q_WS_MAEMO_5

QT_BEGIN_NAMESPACE

typedef void (*slot)(QString& key, QVariant value);

class QGConfBackend
{
   typedef void (*callbackFunc)(GConfClient *a, guint b, GConfEntry *c, void *d);

public:
    QGConfBackend();
    ~QGConfBackend();

    //Read the value for the key stored in GConf
    QVariant getValue(const QString &key);

    //slotToCall is called when changes happen in the GConf dir.
    bool connect(const QString& dir, slot slotToCall);

    static QGConfBackend *self();

private:
    void watch(const QString& dir, callbackFunc cPtr);
    static void callback(GConfClient *a, guint b, GConfEntry *c, void *d);
    void callSlot(GConfEntry* entry);

    GConfClient *client;
    QMap<QString, slot> map;
};

QT_END_NAMESPACE

#endif //Q_WS_MAEMO_5

#endif //QGCONFBACKEND_P_H
