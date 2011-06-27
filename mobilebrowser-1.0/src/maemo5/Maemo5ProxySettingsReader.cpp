#include "Maemo5ProxySettingsReader.h"

/*
=== GCONF most important Proxy Configuration Keys ===
- /system/http_proxy/use_http_proxy            boolean
- /system/http_proxy/use_authentication        boolean
- /system/http_proxy/host                      string
- /system/http_proxy/authentication_user       string
- /system/http_proxy/authentication_password   string
- /system/http_proxy/port                      int
- /system/proxy/socks_host                     string
- /system/proxy/mode                           string
- /system/proxy/ftp_host                       string
- /system/proxy/secure_host                    string
- /system/proxy/socks_port                     int
- /system/proxy/ftp_port                       int
- /system/proxy/secure_port                    int
- /system/proxy/no_proxy_for                   list
- /system/proxy/gopher_host                    string
- /system/proxy/gopher_port                    int
- /system/proxy/autoconfig_url                 string
*/

#define GCONF_HTTPPROXY_SET            "/system/http_proxy/use_http_proxy"          //< Boolean
#define GCONF_HTTPPROXY_HOST           "/system/http_proxy/host"                    //< String
#define GCONF_HTTPPROXY_PORT           "/system/http_proxy/port"                    //< Integer
#define GCONF_HTTPPROXY_AUTH_SET       "/system/http_proxy/use_authentication"      //< Boolean
#define GCONF_HTTPPROXY_AUTH_USER      "/system/http_proxy/authentication_user"     //< String
#define GCONF_HTTPPROXY_AUTH_PASSWORD  "/system/http_proxy/authentication_password" //< String
#define GCONF_PROXY_MODE               "/system/proxy/mode"                         //< String {'none' | 'manual' | 'auto' }
#define GCONF_PROXY_AUTOCONFIG_URL     "/system/proxy/autoconfig_url"               //< String

#include <QVariant>

Maemo5ProxySettingsReader::Maemo5ProxySettingsReader() :
    m_gconfBackend(new QGConfBackend())
{
}

Maemo5ProxySettingsReader::~Maemo5ProxySettingsReader() {
   delete m_gconfBackend;
}

bool Maemo5ProxySettingsReader::isHttpProxySet() {
   return m_gconfBackend->getValue(GCONF_HTTPPROXY_SET).toBool();
}

QString Maemo5ProxySettingsReader::httpProxyHost() {
   return m_gconfBackend->getValue(GCONF_HTTPPROXY_HOST).toString();
}

int Maemo5ProxySettingsReader::httpProxyPort() {
   return m_gconfBackend->getValue(GCONF_HTTPPROXY_PORT).toInt();
}

bool Maemo5ProxySettingsReader::isHttpProxyAuthenticationSet() {
   return m_gconfBackend->getValue(GCONF_HTTPPROXY_AUTH_SET).toBool();
}

QString Maemo5ProxySettingsReader::httpProxyUser() {
   return m_gconfBackend->getValue(GCONF_HTTPPROXY_AUTH_USER).toString();
}

QString Maemo5ProxySettingsReader::httpProxyPassword() {
   return m_gconfBackend->getValue(GCONF_HTTPPROXY_AUTH_PASSWORD).toString();
}

bool Maemo5ProxySettingsReader::isProxyConfigurationModeAuto() {
   return m_gconfBackend->getValue(GCONF_PROXY_MODE).toString() == "auto" ? true : false;
}

bool Maemo5ProxySettingsReader::isProxyConfigurationModeManual() {
   return m_gconfBackend->getValue(GCONF_PROXY_MODE).toString() == "manual" ? true : false;
}

QString Maemo5ProxySettingsReader::proxyAutoConfigurationUrl() {
   return m_gconfBackend->getValue(GCONF_PROXY_AUTOCONFIG_URL).toString();
}
