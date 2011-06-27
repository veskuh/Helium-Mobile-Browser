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

#ifndef QMAEMOINTERNETCONNECTIVITY_P_H
#define QMAEMOINTERNETCONNECTIVITY_P_H

#include "QtCore/qobject.h"
#include "QtDBus/qdbusinterface.h"
#include "qgconfbackend_p.h"

#ifdef Q_WS_MAEMO_5

QT_BEGIN_NAMESPACE

class QMaemoICPrivate : public QObject
{
    Q_OBJECT

    enum connectStatus {
        UNKNOWN = -1,
        DISCONNECTED = 0,
        CONNECTED = 1,
        DISCONNECTING = 2,
        CONNECTING = 3
        //SCAN_START
        //SCAN_STOP
    };

public:
    QMaemoICPrivate();
    ~QMaemoICPrivate();

    bool isConnected();
    bool isAutoConnect();

    //Show connection dialog.
    void connectionRequest();

    bool isHttpProxyUsed();
    QString currentAPName();
    QString lastAPName();

    QString lastAPid();

    //Called by the QGConfBackend instance when the proxy parameters change.
    static void proxySettingsChanged(QString& key, QVariant value);

private:
    void checkConnectionStatus();
    void readErrorDBusErrorMsg(const QDBusMessage& msg);

private Q_SLOTS:
    //Update the internal vars with the status_changed signals sent by the daemon via D-Bus
    void statusChangedSlot(QString IAPname, QString networkType,
			QString state, QString errorCode);

private:
   int connected;
   QString iap; //Internet Access Point
};

QT_END_NAMESPACE

#endif //Q_WS_MAEMO_5

#endif //QMAEMOINTERNETCONNECTIVITY_P_H
