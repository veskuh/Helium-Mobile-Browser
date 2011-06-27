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
* File Name   : Settings.h
*
* Created     : 15 June 2011
*
* Description :
*
* Collection of keys to be used with QSettings
*
*--------------------------------------------------------
* 1.0 - First open source release
*
*/

#ifndef SETTINGSKEYS_H
#define SETTINGSKEYS_H

#include <QSettings>

#define SETTINGS_KEY_INITIAL_HOME         "settings/initial_home"
#define SETTINGS_KEY_HOME_BOOKMARK_ID     "settings/home_bookmark_id"
#define SETTINGS_KEY_HISTORY_LENGTH_DAYS  "settings/history_length_days"
#define SETTINGS_KEY_WEBSTORAGE_ENABLED   "settings/webstorage_enabled"
#define SETTINGS_KEY_WEBSTORAGE_SIZE_MB   "settings/webstorage_size_MB"
#define SETTINGS_KEY_LOCATION_ENABLED     "settings/location_enabled"

#define CORE_DB_FILE_NAME                 "core.db"
#define CORE_DB_VERSION                   2

#define SETTINGS_KEY_WEB_AUTO_LOAD_IMAGES                       "websettings/auto_load_images"
#define SETTINGS_KEY_WEB_DNS_PREFETCH                           "websettings/dns_prefetch"
#define SETTINGS_KEY_WEB_JAVASCRIPT                             "websettings/javascript"
#define SETTINGS_KEY_WEB_JAVA                                   "websettings/java"
#define SETTINGS_KEY_WEB_PLUGINS                                "websettings/plugins"
#define SETTINGS_KEY_WEB_PRIVATE_BROWSING                       "websettings/private_browsing"
#define SETTINGS_KEY_WEB_JAVASCRIPT_CAN_OPEN_WINDOWS            "websettings/javascript_can_open_windows"
#define SETTINGS_KEY_WEB_JAVASCRIPT_CAN_ACCESS_CLIPBOARD        "websettings/javascript_can_access_clipboard"
#define SETTINGS_KEY_WEB_DEVELOPER_EXTRAS                       "websettings/developer_extras"
#define SETTINGS_KEY_WEB_SPATIAL_NAVIGATION                     "websettings/spatial_navigation"
#define SETTINGS_KEY_WEB_LINKS_INCLUDED_IN_FOCUS_CHAIN          "websettings/links_included_in_focus_chain"
#define SETTINGS_KEY_WEB_ZOOM_TEXT_ONLY                         "websettings/zoom_text_only"
#define SETTINGS_KEY_WEB_PRINT_ELEMENT_BACKGROUNDS              "websettings/print_element_backgrounds"
#define SETTINGS_KEY_WEB_OFFLINE_STORAGE_DATABASE               "websettings/offline_storage_database"
#define SETTINGS_KEY_WEB_OFFLINE_STORAGE_QUOTA                  "websettings/offline_storage_quota"
#define SETTINGS_KEY_WEB_OFFLINE_WEBAPP_CACHE                   "websettings/offline_webapp_cache"
#define SETTINGS_KEY_WEB_OFFLINE_WEBAPP_QUOTA                   "websettings/offline_webapp_quota"
#define SETTINGS_KEY_WEB_PERSISTENT_STORAGE                     "websettings/persistent_storage"
#define SETTINGS_KEY_WEB_LOCALSTORAGE                           "websettings/localstorage"
#define SETTINGS_KEY_WEB_LOCAL_CONTENT_CAN_ACCESS_REMOTE_URLS   "websettings/local_content_access_remote"
#define SETTINGS_KEY_WEB_LOCAL_CONTENT_CAN_ACCESS_LOCAL_URLS    "websettings/local_content_access_local"
#define SETTINGS_KEY_WEB_XSS_AUDITING                           "websettings/xxs_auditing"
#define SETTINGS_KEY_WEB_ACCELERATED_COMPOSITING                "websettings/accelerated_compositing"
//#define SETTINGS_KEY_WEB_WEBGL                                  "websettings/webgl"
#define SETTINGS_KEY_WEB_TILED_BACKING_STORE                    "websettings/tiled_backing_store"
#define SETTINGS_KEY_WEB_FRAME_FLATTENING                       "websettings/frame_flattening"
#define SETTINGS_KEY_WEB_SITE_SPECIFIC_QUIRKS                   "websettings/site_specific_quirks"
#define SETTINGS_KEY_WEB_MAX_PAGES_IN_CACHE                     "websettings/max_pages_in_cache"
#define SETTINGS_KEY_WEB_OBJ_CACHE_CAPACITY_MIN                 "websettings/obj_cache_capacity_min"
#define SETTINGS_KEY_WEB_OBJ_CACHE_CAPACITY_MAX                 "websettings/obj_cache_capacity_max"
#define SETTINGS_KEY_WEB_OBJ_CACHE_CAPACITY_OVERALL             "websettings/obj_cache_capacity_overall"

class Settings
{
public:
   static void initSettings();
   static void initWebSettings();
   static void reloadWebSettings();


   static QString dbFilePath();
   static int dbFileVersion();
};

#endif // SETTINGSKEYS_H
