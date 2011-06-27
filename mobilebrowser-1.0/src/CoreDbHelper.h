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
* File Name   : CoreDbHelper.h
*
* Created     : 15 June 2011
*
* Description :
*
* CoreDbHelper is the class which knows how the database has to be created
*
*--------------------------------------------------------
* 1.0 - First open source release
*
*/

#ifndef COREDBHELPER_H
#define COREDBHELPER_H

#include <QSqlDatabase>

#include "SqliteDbHelper.h"

class CoreDbHelper : public SqliteDbHelper
{
public:
    CoreDbHelper(const QString &filePath, const int requestedVersion, const QString &defaultConnName = QString() );
    virtual ~CoreDbHelper();

protected:
    virtual void onCreate(QSqlDatabase &database);
};

#endif // COREDBHELPER_H
