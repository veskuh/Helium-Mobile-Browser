#include "Settings.h"

#include <QVariant>
#include <QDir>
#include <QDesktopServices>
#include <QNetworkProxy>
#include <QWebSettings>

#include "buildconfig.h"
#include "macros.h"

#ifdef Q_WS_MAEMO_5
#include "maemo5/Maemo5ProxySettingsReader.h"
#endif

#define SETTINGS_KEY_ALREADY_INITIALIZED    "settings/already_initialized"
#define SETTINGS_KEY_WEB_ALREADY_INITIALIZED    "websettings/already_initialized"

void Settings::initSettings() {
   static bool initGlobalSettingsDone = false;
   QSettings settings;

   if ( !initGlobalSettingsDone && !settings.value(SETTINGS_KEY_ALREADY_INITIALIZED, false).toBool() ) {
      QDEBUG("Settings::initGlobalSettings()");
      settings.setValue(SETTINGS_KEY_INITIAL_HOME,          "http://www.google.com/");
      settings.setValue(SETTINGS_KEY_HOME_BOOKMARK_ID,      1);
      settings.setValue(SETTINGS_KEY_HISTORY_LENGTH_DAYS,   30);
      settings.setValue(SETTINGS_KEY_WEBSTORAGE_ENABLED,    true);
      settings.setValue(SETTINGS_KEY_WEBSTORAGE_SIZE_MB,    10);
      settings.setValue(SETTINGS_KEY_LOCATION_ENABLED,      true);

      // Remember to do it only once
      settings.setValue(SETTINGS_KEY_ALREADY_INITIALIZED, true);
   }

   #if defined(USE_NETWORK_PROXY)
      // Setting up the Network Proxy
      QDEBUG_EXP("Settings::init()", "Using USE_NETWORK_PROXY");
      QNetworkProxy::setApplicationProxy(QNetworkProxy(
            QNetworkProxy::HttpProxy,
            NETWORK_PROXY_HOSTNAME,
            NETWORK_PROXY_PORT));
   #elif defined(Q_WS_MAEMO_5)
      Maemo5ProxySettingsReader reader;
      if ( reader.isProxyConfigurationModeAuto() ) {
         // Proxy Configuration Mode on 'AUTO'
         QDEBUG("[Maemo5]" << "Proxy Configuration Mode: AUTO.");
         QWARNING("[Maemo5]" << "TODO / NOT SUPPORTED YET");
         // TODO
      } else if ( reader.isProxyConfigurationModeManual() && reader.isHttpProxySet() ) {
         // Proxy Configuration Mode on 'MANUAL'
         QNetworkProxy proxyConf(QNetworkProxy::HttpProxy);
         QDEBUG("[Maemo5]" << "Proxy Configuration Mode: MANUAL.");
         QDEBUG("[Maemo5]" << "HTTP Proxy Host:" << reader.httpProxyHost());
         QDEBUG("[Maemo5]" << "HTTP Proxy Port:" << reader.httpProxyPort());
         proxyConf.setHostName(reader.httpProxyHost());
         proxyConf.setPort(reader.httpProxyPort());

         if ( reader.isHttpProxyAuthenticationSet() ) {
            // Username and Password required
            QDEBUG("[Maemo5]" << "HTTP Proxy Authentication is Set");
            QDEBUG("[Maemo5]" << "HTTP Proxy Authentication Username:"
                   << reader.httpProxyUser());
            QDEBUG("[Maemo5]" << "HTTP Proxy Authentication Password:"
                   << reader.httpProxyPassword());
            proxyConf.setUser(reader.httpProxyUser());
            proxyConf.setPassword(reader.httpProxyPassword());
         }

         // Set the manual configuration
         QNetworkProxy::setApplicationProxy(proxyConf);
      } else {
         QDEBUG("[Maemo5]" << "Proxy Configuration Mode: NONE.");
      }
   #endif

   // Avoiding a second initialization
   initGlobalSettingsDone = true;
}

void Settings::initWebSettings() {
   static bool initGlobalWebSettingsDone = false;
   QSettings settings;

   if ( !initGlobalWebSettingsDone && !settings.value(SETTINGS_KEY_WEB_ALREADY_INITIALIZED, false).toBool() ) {
      QDEBUG("Settings::initGlobalWebSettings()");
      settings.setValue(SETTINGS_KEY_WEB_AUTO_LOAD_IMAGES, true);
      settings.setValue(SETTINGS_KEY_WEB_DNS_PREFETCH, true);
      settings.setValue(SETTINGS_KEY_WEB_JAVASCRIPT, true);
      settings.setValue(SETTINGS_KEY_WEB_JAVA, false);
      settings.setValue(SETTINGS_KEY_WEB_PLUGINS, true);
      settings.setValue(SETTINGS_KEY_WEB_PRIVATE_BROWSING, false);
      settings.setValue(SETTINGS_KEY_WEB_JAVASCRIPT_CAN_OPEN_WINDOWS, false);
      settings.setValue(SETTINGS_KEY_WEB_JAVASCRIPT_CAN_ACCESS_CLIPBOARD, true);
      settings.setValue(SETTINGS_KEY_WEB_DEVELOPER_EXTRAS, false);
      settings.setValue(SETTINGS_KEY_WEB_SPATIAL_NAVIGATION, true);
      settings.setValue(SETTINGS_KEY_WEB_LINKS_INCLUDED_IN_FOCUS_CHAIN, true);
      settings.setValue(SETTINGS_KEY_WEB_ZOOM_TEXT_ONLY, false);
      settings.setValue(SETTINGS_KEY_WEB_PRINT_ELEMENT_BACKGROUNDS, true);
      settings.setValue(SETTINGS_KEY_WEB_OFFLINE_STORAGE_DATABASE, true);
      settings.setValue(SETTINGS_KEY_WEB_OFFLINE_STORAGE_QUOTA, 5 * 1024 * 1024); //< 5MB
      settings.setValue(SETTINGS_KEY_WEB_OFFLINE_WEBAPP_CACHE, true);
      settings.setValue(SETTINGS_KEY_WEB_OFFLINE_WEBAPP_QUOTA, 5 * 1020 * 1024); //< 5MB
      settings.setValue(SETTINGS_KEY_WEB_PERSISTENT_STORAGE, true);
      settings.setValue(SETTINGS_KEY_WEB_LOCALSTORAGE, true);
      settings.setValue(SETTINGS_KEY_WEB_LOCAL_CONTENT_CAN_ACCESS_REMOTE_URLS, false);
      settings.setValue(SETTINGS_KEY_WEB_LOCAL_CONTENT_CAN_ACCESS_LOCAL_URLS, false);
      settings.setValue(SETTINGS_KEY_WEB_XSS_AUDITING, false);
      settings.setValue(SETTINGS_KEY_WEB_ACCELERATED_COMPOSITING, true);
//      settings.setValue(SETTINGS_KEY_WEB_WEBGL, true);
      settings.setValue(SETTINGS_KEY_WEB_TILED_BACKING_STORE, true);
      settings.setValue(SETTINGS_KEY_WEB_FRAME_FLATTENING, true);
      settings.setValue(SETTINGS_KEY_WEB_SITE_SPECIFIC_QUIRKS, true);
      settings.setValue(SETTINGS_KEY_WEB_MAX_PAGES_IN_CACHE, 5);
      settings.setValue(SETTINGS_KEY_WEB_OBJ_CACHE_CAPACITY_MIN, 128);
      settings.setValue(SETTINGS_KEY_WEB_OBJ_CACHE_CAPACITY_MAX, 256);
      settings.setValue(SETTINGS_KEY_WEB_OBJ_CACHE_CAPACITY_OVERALL, 1024);
      // Remember to do it only once
      settings.setValue(SETTINGS_KEY_WEB_ALREADY_INITIALIZED, true);
   }
   // Avoiding a second initialization
   initGlobalWebSettingsDone = true;
   Settings::reloadWebSettings();
}

void Settings::reloadWebSettings() {
   // WebKit Global WebSettings related initialization
   QSettings settings;
   QWebSettings *globalWebSettings = QWebSettings::globalSettings();

   globalWebSettings->setAttribute(QWebSettings::AutoLoadImages, settings.value(SETTINGS_KEY_WEB_AUTO_LOAD_IMAGES).toBool());
   globalWebSettings->setAttribute(QWebSettings::DnsPrefetchEnabled, settings.value(SETTINGS_KEY_WEB_DNS_PREFETCH).toBool());
   globalWebSettings->setAttribute(QWebSettings::JavascriptEnabled, settings.value(SETTINGS_KEY_WEB_JAVASCRIPT).toBool());
   globalWebSettings->setAttribute(QWebSettings::JavaEnabled, settings.value(SETTINGS_KEY_WEB_JAVA).toBool());
   globalWebSettings->setAttribute(QWebSettings::PluginsEnabled, settings.value(SETTINGS_KEY_WEB_PLUGINS).toBool());
   globalWebSettings->setAttribute(QWebSettings::PrivateBrowsingEnabled, settings.value(SETTINGS_KEY_WEB_PRIVATE_BROWSING).toBool());
   globalWebSettings->setAttribute(QWebSettings::JavascriptCanOpenWindows, settings.value(SETTINGS_KEY_WEB_JAVASCRIPT_CAN_OPEN_WINDOWS).toBool());
   globalWebSettings->setAttribute(QWebSettings::JavascriptCanAccessClipboard, settings.value(SETTINGS_KEY_WEB_JAVASCRIPT_CAN_ACCESS_CLIPBOARD).toBool());
   globalWebSettings->setAttribute(QWebSettings::DeveloperExtrasEnabled, settings.value(SETTINGS_KEY_WEB_DEVELOPER_EXTRAS).toBool());
   globalWebSettings->setAttribute(QWebSettings::SpatialNavigationEnabled, settings.value(SETTINGS_KEY_WEB_SPATIAL_NAVIGATION).toBool());
   globalWebSettings->setAttribute(QWebSettings::LinksIncludedInFocusChain, settings.value(SETTINGS_KEY_WEB_LINKS_INCLUDED_IN_FOCUS_CHAIN).toBool());
   globalWebSettings->setAttribute(QWebSettings::ZoomTextOnly, settings.value(SETTINGS_KEY_WEB_ZOOM_TEXT_ONLY).toBool());
   globalWebSettings->setAttribute(QWebSettings::PrintElementBackgrounds, settings.value(SETTINGS_KEY_WEB_PRINT_ELEMENT_BACKGROUNDS).toBool());
   globalWebSettings->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, settings.value(SETTINGS_KEY_WEB_OFFLINE_STORAGE_DATABASE).toBool());
   globalWebSettings->setOfflineStorageDefaultQuota(settings.value(SETTINGS_KEY_WEB_OFFLINE_STORAGE_QUOTA).toInt());
   globalWebSettings->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, settings.value(SETTINGS_KEY_WEB_OFFLINE_WEBAPP_CACHE).toBool());
   globalWebSettings->setOfflineWebApplicationCacheQuota(settings.value(SETTINGS_KEY_WEB_OFFLINE_WEBAPP_QUOTA).toInt());
   if ( settings.value(SETTINGS_KEY_WEB_PERSISTENT_STORAGE).toBool() ) {
        QDEBUG_EXP("Settings::reloadWebSettings()", "enabling PersistentStorage..." );
        QWebSettings::enablePersistentStorage();
   }
   globalWebSettings->setAttribute(QWebSettings::LocalStorageEnabled, settings.value(SETTINGS_KEY_WEB_LOCALSTORAGE).toBool());
   globalWebSettings->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, settings.value(SETTINGS_KEY_WEB_LOCAL_CONTENT_CAN_ACCESS_REMOTE_URLS).toBool());
   globalWebSettings->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, settings.value(SETTINGS_KEY_WEB_LOCAL_CONTENT_CAN_ACCESS_LOCAL_URLS).toBool());

#if defined(Q_WS_MAEMO_5)
   globalWebSettings->setAttribute(QWebSettings::XSSAuditingEnabled, settings.value(SETTINGS_KEY_WEB_XSS_AUDITING).toBool());
#else
   globalWebSettings->setAttribute(QWebSettings::XSSAuditingEnabled, settings.value(SETTINGS_KEY_WEB_XSS_AUDITING).toBool());
#endif

   globalWebSettings->setAttribute(QWebSettings::AcceleratedCompositingEnabled, settings.value(SETTINGS_KEY_WEB_ACCELERATED_COMPOSITING).toBool());
//   globalWebSettings->setAttribute(QWebSettings::WebGLEnabled, settings.value(SETTINGS_KEY_WEB_WEBGL).toBool());
   globalWebSettings->setAttribute(QWebSettings::TiledBackingStoreEnabled, settings.value(SETTINGS_KEY_WEB_TILED_BACKING_STORE).toBool());
   globalWebSettings->setAttribute(QWebSettings::FrameFlatteningEnabled, settings.value(SETTINGS_KEY_WEB_FRAME_FLATTENING).toBool());
   globalWebSettings->setAttribute(QWebSettings::SiteSpecificQuirksEnabled, settings.value(SETTINGS_KEY_WEB_SITE_SPECIFIC_QUIRKS).toBool());
   globalWebSettings->setMaximumPagesInCache(settings.value(SETTINGS_KEY_WEB_MAX_PAGES_IN_CACHE).toInt());
   globalWebSettings->setObjectCacheCapacities(
      settings.value(SETTINGS_KEY_WEB_OBJ_CACHE_CAPACITY_MIN).toInt(),
      settings.value(SETTINGS_KEY_WEB_OBJ_CACHE_CAPACITY_MAX).toInt(),
      settings.value(SETTINGS_KEY_WEB_OBJ_CACHE_CAPACITY_OVERALL).toInt());
}


QString Settings::dbFilePath() {
    QDir dir;
    dir.mkpath(QDesktopServices::storageLocation( QDesktopServices::DataLocation));
    QString dbFileLocation = QDir(QDesktopServices::storageLocation( QDesktopServices::DataLocation) ).absoluteFilePath( CORE_DB_FILE_NAME );
#if defined(Q_WS_MAEMO_5) || defined(Q_OS_LINUX)

#elif defined(Q_OS_WIN32) || defined(Q_OS_SYMBIAN)
    dbFileLocation.replace("/","\\\\");
#endif
    QDEBUG_EXP("Settings::dbFilePath()", dbFileLocation );
    return dbFileLocation;
}

int Settings::dbFileVersion() {
    return CORE_DB_VERSION;
}
