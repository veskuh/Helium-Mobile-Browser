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
#include "gconfsymbols_p.h"

#include <QtCore/qlibrary.h>
#include <QtDebug>

#ifdef Q_WS_MAEMO_5

QT_BEGIN_NAMESPACE

static QLibrary *qgconf_libgconf = 0;

bool qgconf_loadLibGConf()
{
    g_type_init();

    static volatile bool triedToLoadLibrary = false;

    QLibrary *&lib = qgconf_libgconf;
    if (triedToLoadLibrary)
        return lib && lib->isLoaded();

    lib = new QLibrary(QLatin1String("gconf-2"), 4);
    triedToLoadLibrary = true;

    if (lib->load() && lib->resolve("gconf_client_get_default"))
       return true;

    lib->unload();
    delete lib;
    lib = 0;
    return false;
}

void qgconf_unloadLibGConf()
{
    qgconf_libgconf->unload();
}

void *qgconf_resolve_me(const char *name)
{
    void *ptr = 0;
    if (!qgconf_loadLibGConf())
        qFatal("Cannot find libgconf-2 in your system to resolve symbol '%s'.", name);

    ptr = qgconf_libgconf->resolve(name);
    if (!ptr)
        qFatal("Cannot resolve '%s' in your libgconf-2.", name);

    return ptr;
}


QT_END_NAMESPACE

#endif //Q_WS_MAEMO_5
