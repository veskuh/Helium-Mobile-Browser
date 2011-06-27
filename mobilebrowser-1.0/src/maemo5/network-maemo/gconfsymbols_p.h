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

#ifndef QGCONF_SYMBOLS_P_H
#define QGCONF_SYMBOLS_P_H

#include <QtCore/qglobal.h>

#ifdef Q_WS_MAEMO_5

#include <gconf/gconf.h>

QT_BEGIN_NAMESPACE

class GConf;
class GConfClient;

typedef enum {
    GCONF_CLIENT_PRELOAD_NONE,     /* don't preload anything */
    GCONF_CLIENT_PRELOAD_ONELEVEL, /* load entries directly under the directory. */
    GCONF_CLIENT_PRELOAD_RECURSIVE /* recurse the directory tree; possibly quite expensive! */
} GConfClientPreloadType;

typedef void (*GConfClientNotifyFunc)(GConfClient* client,
                                      guint cnxn_id,
                                      GConfEntry *entry,
                                      gpointer user_data);

# define DEFINEFUNC(ret, func, args, argcall, funcret)          \
    typedef ret (* _q_PTR_##func) args;                         \
    static inline ret q_##func args                             \
    {                                                           \
        static _q_PTR_##func ptr;                               \
        if (!ptr)                                               \
            ptr = (_q_PTR_##func) qgconf_resolve_me(#func);     \
        funcret ptr argcall;                                    \
    }

//Private Functions
void *qgconf_resolve_me(const char *name);

//Public Functions
bool qgconf_loadLibGConf();
void qgconf_unloadLibGConf();

DEFINEFUNC(GConfClient*, gconf_client_get_default, (),() ,return )
DEFINEFUNC(GConfValue*,  gconf_client_get, (GConfClient* a, const gchar* b, GError ** c), (a, b, c), return)
DEFINEFUNC(char* , gconf_client_get_string, (GConfClient* a, const char* b, GError ** c), (a, b, c), return)
DEFINEFUNC(gboolean, gconf_client_get_bool, (GConfClient* a, const char* b, GError ** c), (a, b, c), return)
DEFINEFUNC(gint, gconf_client_get_int, (GConfClient* a, const char* b, GError ** c), (a, b, c), return)
DEFINEFUNC(GSList*, gconf_value_get_list, (const GConfValue *a), (a), return)
DEFINEFUNC(gchar*, gconf_value_to_string, (GConfValue* a),(a),return)
DEFINEFUNC(void, gconf_client_add_dir, (GConfClient* a, const gchar*b, GConfClientPreloadType c, GError** d), (a, b, c, d), )
DEFINEFUNC(guint, gconf_client_notify_add, (GConfClient* a, const gchar* b, GConfClientNotifyFunc c, gpointer d, GFreeFunc e, GError** f),
           (a, b, c, d, e, f), return)

QT_END_NAMESPACE

#endif //Q_WS_MAEMO_5

#endif // QGCONF_SYMBOLS_P_H
