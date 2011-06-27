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
* File Name   : macros.h
*
* Created     : 15 June 2011
*
* Description :
*
* Logging macros
*
*--------------------------------------------------------
* 1.0 - First open source release
*
*/

#ifndef MACROS_H
#define MACROS_H

#include <QDebug>

#ifdef ENABLE_LOG    //< Put "DEFINES += ENABLE_LOG" in the .pro/.pri file to enable Logging.

#define QDEBUG_EXP(scope, exp) qDebug() << scope << " - " << exp
#define QDEBUG(scope) qDebug() << scope
#define QCRITICAL_EXP(scope, exp) qCritical() << scope << " - " << exp
#define QCRITICALE(scope) qCritical() << scope
#define QWARNING_EXP(scope, exp) qWarning() << scope << " - " << exp
#define QWARNING(scope) qWarning() << scope

#else

#define QDEBUG_EXP(scope, exp)
#define QDEBUG(scope)
#define QCRITICAL_EXP(scope, exp)
#define QCRITICAL(scope)
#define QWARNING_EXP(scope, exp)
#define QWARNING(scope)

#endif

#if defined(ENABLE_LOG) && defined(ENABLE_VERBOSE)

#define QVERBOSE_EXP(scope, exp) qDebug() << scope << " - " << exp << " (verbose)"
#define QVERBOSE(scope) qDebug() << scope << " (verbose)"

#else

#define QVERBOSE_EXP(scope, exp)
#define QVERBOSE(scope)

#endif

#endif // MACROS_H
