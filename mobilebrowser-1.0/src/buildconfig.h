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
* File Name   : buildconfig.h
*
* Created     : 15 June 2011
*
* Description :
*
* Collection of compile time flag defines/macros
*
*--------------------------------------------------------
* 1.0 - First open source release
*
*/

#ifndef BUILDCONFIG_H
#define BUILDCONFIG_H

/** ------------------------------------- Proxy default Config */
//#define USE_NETWORK_PROXY //< Comment this to disable the usage of this Proxy
#define NETWORK_PROXY_HOSTNAME  ""
#define NETWORK_PROXY_PORT      3128

/** ------------------------------------- OpenGL default Config */
#define USE_OPENGL

/** ------------------------------------- Platform specific undefs */
// "Symbian" undefs
#if defined(Q_OS_SYMBIAN)
#undef USE_OPENGL
#undef USE_NETWORK_PROXY
#endif

// "Maemo 5" undefs
#if defined(Q_WS_MAEMO_5)
#undef USE_OPENGL
#undef USE_NETWORK_PROXY
#endif

// "Mac (out of Orange Labs office)" undefs
#if defined(Q_OS_MAC)
//#undef USE_OPENGL
#undef USE_NETWORK_PROXY
#endif

/** ------------------------------------- Compilation Warnings (for the developer to look after) */
#ifdef USE_NETWORK_PROXY
#warning "*** Using Hardcoded Proxy macros ***"
#endif
#ifndef USE_OPENGL
#warning "*** NOT using OpenGL ***"
#endif

/** ------------------------------------- App general constants */
#define APP_NAME                 "HeliumMobileBrowser"
#define APP_VERSION              "1.0.0"
#define ORG_NAME                 "OrangeLabsUK"
#define ORG_DOMAIN               "orange.com"

#endif // BUILDCONFIG_H
