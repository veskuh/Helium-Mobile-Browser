# Maemo 5 specifics
INCLUDEPATH += $$PWD

maemo5 {
   QT += maemo5 dbus
   QMAKE_PKGCONFIG_REQUIRES += QtDBus

   HEADERS += \
      maemo5/Maemo5ProxySettingsReader.h

   SOURCES += \
      maemo5/Maemo5ProxySettingsReader.cpp

   #VARIABLES
   isEmpty(PREFIX) {
      PREFIX = /usr/local
   }
   BINDIR = $$PREFIX/bin
   DATADIR =$$PREFIX/share

   DEFINES += DATADIR=\\\"$$DATADIR\\\" PKGDATADIR=\\\"$$PKGDATADIR\\\"

   #MAKE INSTALL
   INSTALLS += target desktop icon16 icon32 icon48 icon64

   target.path = $$BINDIR

   desktop.path = $$DATADIR/applications/hildon
   desktop.files += $$PWD/$${TARGET}.desktop

   icon16.path = $$DATADIR/icons/hicolor/16x16/apps
   icon16.files += $$PWD/../images/icons/16x16/$${TARGET}.png

   icon32.path = $$DATADIR/icons/hicolor/32x32/apps
   icon32.files += $$PWD/../images/icons/32x32/$${TARGET}.png

   icon48.path = $$DATADIR/icons/hicolor/48x48/apps
   icon48.files += $$PWD/../images/icons/48x48/$${TARGET}.png

   icon64.path = $$DATADIR/icons/hicolor/64x64/apps
   icon64.files += $$PWD/../images/icons/64x64/$${TARGET}.png

   # Qt Network Maemo Module START
   # Note: This is taken almost as it is from the "x11-maemo" repo
   HEADERS += $$PWD/network-maemo/qmaemointernetconnectivity.h \
              $$PWD/network-maemo/qmaemointernetconnectivity_p.h \
              $$PWD/network-maemo/qgconfbackend_p.h \
              $$PWD/network-maemo/gconfsymbols_p.h

   SOURCES += $$PWD/network-maemo/qmaemointernetconnectivity.cpp \
              $$PWD/network-maemo/qgconfbackend.cpp \
              $$PWD/network-maemo/gconfsymbols.cpp

   CONFIG += link_pkgconfig
   PKGCONFIG_PRIVATE += gconf-2.0
   # Qt Network Maemo Module END
}
