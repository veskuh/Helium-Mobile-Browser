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
* File Name   : Maemo5ProxySettingsReader.h
*
* Created     : 15 June 2011
*
* Description :
*
* Utility class to get Maemo5 proxy settings
*
*--------------------------------------------------------
* 1.0 - First open source release
*
*/

#ifndef MAEMO5PROXYSETTINGSREADER_H
#define MAEMO5PROXYSETTINGSREADER_H

#include "network-maemo/qgconfbackend_p.h"

class Maemo5ProxySettingsReader
{
public:
   Maemo5ProxySettingsReader();
   virtual ~Maemo5ProxySettingsReader();

   bool isHttpProxySet();
   QString httpProxyHost();
   int httpProxyPort();
   bool isHttpProxyAuthenticationSet();
   QString httpProxyUser();
   QString httpProxyPassword();
   bool isProxyConfigurationModeAuto();
   bool isProxyConfigurationModeManual();
   QString proxyAutoConfigurationUrl();

private:
   QGConfBackend * m_gconfBackend;
};

#endif // MAEMO5PROXYSETTINGSREADER_H
