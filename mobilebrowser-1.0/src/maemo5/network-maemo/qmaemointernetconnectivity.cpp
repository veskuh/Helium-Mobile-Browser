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

/*
    QMaemoInternetConnectivity

    Missing supports:
     - Switch to WLAN if available
*/

#include "qmaemointernetconnectivity.h"
#include "qmaemointernetconnectivity_p.h"

#ifdef Q_WS_MAEMO_5

#include <QtDebug>

#include <icd/osso-ic-dbus.h>
#include <icd/osso-ic-ui-dbus.h>

#define QMCDEBUG if (MIC_DEBUG) qDebug()

QT_BEGIN_NAMESPACE

Q_GLOBAL_STATIC(QMaemoICPrivate, maemoICInstance);

static bool MIC_DEBUG = false;

bool QMaemoInternetConnectivity::isConnected()
{
    return maemoICInstance()->isConnected();
}

/* not used ATM
static QString currentAPName() {
    return maemoICInstance()->currentAPName();
}
*/

void QMaemoInternetConnectivity::connectionRequest()
{
    return maemoICInstance()->connectionRequest();
}

bool QMaemoInternetConnectivity::isHttpProxyUsed()
{
    return maemoICInstance()->isHttpProxyUsed();
}

/*!
    \class QMaemoInternetConnectivity
    \since 4.5maemo
    \internal

    \brief The QMaemoInternetConnectivity TODO

    \reentrant
    \ingroup maemo
    \inmodule QtNetwork

    QMaemoInternetConnectivity TODO
*/

QMaemoICPrivate::QMaemoICPrivate()
    : connected(UNKNOWN)
{
    //QT_MIC_DEBUG set on debugging messages
    MIC_DEBUG = !qgetenv("QT_MIC_DEBUG").isEmpty();

    //Check for debug
    if (!QDBusConnection::systemBus().isConnected()) {
        qFatal("Cannot connect to the D-BUS session bus.");
    }

    QDBusConnection::systemBus().connect(QLatin1String(ICD_DBUS_SERVICE), QLatin1String(ICD_DBUS_PATH),
            QLatin1String(ICD_DBUS_INTERFACE), QLatin1String(ICD_STATUS_CHANGED_SIG),
            this, SLOT(statusChangedSlot(QString, QString, QString, QString) ));

    //Watching for changes in the proxy settings
	QString httpProxyDir = QLatin1String("/system/http_proxy");
    if (!QGConfBackend::self()->connect(httpProxyDir, proxySettingsChanged))
        qWarning() << "QMaemoICPrivate fails to watch for GConf changes in " << httpProxyDir;

}

QMaemoICPrivate::~QMaemoICPrivate()
{
}

bool QMaemoICPrivate::isAutoConnect()
{
    const QString auto_connect =  QGConfBackend::self()->getValue(
            QLatin1String("/system/osso/connectivity/network_type/auto_connect")).toString();

    QMCDEBUG << "auto_connect:" << auto_connect;

    int search_interval = QGConfBackend::self()->getValue(
            QLatin1String("/system/osso/connectivity/network_type/search_interval")).toInt();
    QMCDEBUG << "search_interval:" << search_interval;

    // Connect automatically:
    // Always ask -> empty
    // WLAN -> WLAN_INFRA
    // Phone -> DUM_... etc.
    // Any Connection -> *
    if (auto_connect.isEmpty())
        return false;

    if (search_interval == 0)
        return false;

    return true;
}

bool QMaemoICPrivate::isConnected()
{
    QMCDEBUG << "isConnected() :" << connected;
    if (connected == UNKNOWN)
        checkConnectionStatus();

    switch (connected) {
    case CONNECTED:
    case CONNECTING:
        return true;
    }
    return false;
}

void QMaemoICPrivate::connectionRequest()
{
    if (isConnected()) {
        QMCDEBUG << "The device is already connected";
        return;
    }

    QDBusMessage reply;

    if (!isAutoConnect()) {
        QMCDEBUG << "auto connect disabled. User have to choose the AP.";

        QDBusMessage showConnectionDialogRequest = QDBusMessage::createMethodCall(
                QLatin1String(ICD_UI_DBUS_SERVICE), QLatin1String(ICD_UI_DBUS_PATH),
                QLatin1String(ICD_UI_DBUS_INTERFACE), QLatin1String(ICD_UI_SHOW_CONNDLG_REQ));
        showConnectionDialogRequest << false;

        reply = QDBusConnection::systemBus().call(showConnectionDialogRequest);
    } else {
        //Check last used network
        //NOTE: Chinook needs lastApName
        const QString lastNetwork = lastAPid();
        QMCDEBUG << "Last used network is " << lastNetwork;
        if (lastNetwork.isEmpty()) {
            //Show the the Access point list to the user
            QDBusMessage showConnectionDialogRequest = QDBusMessage::createMethodCall(
                    QLatin1String(ICD_UI_DBUS_SERVICE), QLatin1String(ICD_UI_DBUS_PATH),
                    QLatin1String(ICD_UI_DBUS_INTERFACE), QLatin1String(ICD_UI_SHOW_CONNDLG_REQ));
            showConnectionDialogRequest << false;

            reply = QDBusConnection::systemBus().call(showConnectionDialogRequest);
        } else {
            //Connect to the last Access Point used
            quint32 i = 0;
            QDBusMessage connectRequest = QDBusMessage::createMethodCall(
                QLatin1String(ICD_DBUS_SERVICE), QLatin1String(ICD_DBUS_PATH),
                QLatin1String(ICD_DBUS_INTERFACE), QLatin1String(ICD_CONNECT_REQ));
            connectRequest << lastNetwork << i;
            reply = QDBusConnection::systemBus().call(connectRequest);
        }
    }

    if (reply.type() == QDBusMessage::ErrorMessage) {
        readErrorDBusErrorMsg(reply);
    }
}

bool QMaemoICPrivate::isHttpProxyUsed()
{
    if (connected != CONNECTED)
        return false;
    return QGConfBackend::self()->getValue(QLatin1String("/system/http_proxy/use_http_proxy")).toBool();
}

QString QMaemoICPrivate::currentAPName()
{
    if (connected != CONNECTED)
        return QString();
    QString gconfKey = iap;
    gconfKey.prepend(QLatin1String("/system/osso/connectivity/IAP/"));
    gconfKey.append(QLatin1String("/name"));
    return QGConfBackend::self()->getValue(gconfKey).toString();
}

QString QMaemoICPrivate::lastAPName()
{
    QString gconfKey = lastAPid();
    if (gconfKey.isEmpty())
        return QString();
    gconfKey.prepend(QLatin1String("/system/osso/connectivity/IAP/"));
    gconfKey.append(QLatin1String("/name"));
    return QGConfBackend::self()->getValue(gconfKey).toString();
}

QString QMaemoICPrivate::lastAPid()
{
    return QGConfBackend::self()->getValue(QLatin1String("/system/osso/connectivity/IAP/last_used_network")).toString();
}

//Called by the QGConfBackend instance when the proxy parameters change.
void QMaemoICPrivate::proxySettingsChanged(QString& key, QVariant value)
{
    QMCDEBUG << "PROXY SETTINGS CHANGED" << key << value;
    //emit qmic->proxySettingsChangedSig(key, value);
}

void QMaemoICPrivate::checkConnectionStatus()
{
    QMCDEBUG << "checkConnectionStatus()";

    QDBusMessage reply = QDBusConnection::systemBus().call(QDBusMessage::createMethodCall(
                QLatin1String(ICD_DBUS_SERVICE), QLatin1String(ICD_DBUS_PATH),
                QLatin1String(ICD_DBUS_INTERFACE), QLatin1String(ICD_GET_STATE_REQ)));

    if (reply.type() == QDBusMessage::ErrorMessage) {
        readErrorDBusErrorMsg(reply);
        connected = UNKNOWN;
        return;
    }

    const QList<QVariant> values = reply.arguments();
    QMCDEBUG << " get state req has returned:" << values;

    connected = DISCONNECTED;
    int n = values.value(0).toInt();
    if (n) {
        QMCDEBUG << "status:" << n;
        connected = CONNECTED;
    }
}

void QMaemoICPrivate::readErrorDBusErrorMsg(const QDBusMessage& msg)
{
    qWarning() << "QMaemoInternetConnectivity has received an error message." << endl
               << "Name: " << msg.errorName() << endl
               << "Message: " << msg.errorMessage();
    qWarning() << "NOTE: Please ignore this error message if you are running your application in scratchbox";
}

void QMaemoICPrivate::statusChangedSlot(QString IAPname, QString networkType, QString state, QString errorCode)
{
    QMCDEBUG << "STATUS CHANGED" << IAPname << networkType << state << errorCode;
    Q_UNUSED(networkType); //TODO

    iap = IAPname;

    if (!errorCode.isEmpty()) {
        connected= UNKNOWN;
        qWarning() << errorCode;
    } else if (state == QLatin1String("CONNECTED")) {
        connected = CONNECTED;
        //emit icStatusChanged();
    } else if (state == QLatin1String("CONNECTING")) {
        connected = CONNECTING;
    } else if (state == QLatin1String("IDLE") && errorCode.isEmpty()) {
        connected = DISCONNECTED;
        //emit icStatusChanged();
    } else if (state == QLatin1String("DISCONNECTING")) {
        connected = DISCONNECTING;
    }
    QMCDEBUG << "CHECK connected=" << connected;
}

QT_END_NAMESPACE

#endif //Q_WS_MAEMO_5
