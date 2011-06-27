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
* File Name   : WebViewInterface.h
*
* Created     : 15 June 2011
*
* Description :
*
* Interface with the QML WebView
*
*--------------------------------------------------------
* 1.0 - First open source release
*
*/


#ifndef WEBVIEWINTERFACE_H
#define WEBVIEWINTERFACE_H

#include <QObject>
#include <QGraphicsObject>
#include <QPixmap>
#include <QUrl>

class WebViewInterface
{
public:
   WebViewInterface(const QGraphicsObject *root, const QString &webViewObjectName);

public:
   QString title();
   QUrl url();
   QPixmap icon();
   bool renderingEnabled();

private:
   QObject *m_webView;
};

#endif // WEBVIEWINTERFACE_H
