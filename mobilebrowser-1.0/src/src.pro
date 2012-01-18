TARGET = helium-mobile-browser

TEMPLATE = app

QT += declarative \
    gui \
    core \
    network \
    webkit \
    opengl \
    sql

include(utility/utility.pri)
include(models/models.pri)

# Choose components to be used by default when building for harmattan
# for other platforms like QtSimulator and Nemo, we need to explicitly define QTCOMPONENTS=harmattan as parameter for qmake
contains(MEEGO_EDITION, harmattan) {
    QTCOMPONENTS_EDITION=harmattan
}


# For platforms with Harmattan QtComponents we use different main and resources
!contains(QTCOMPONENTS_EDITION,harmattan)  {
    SOURCES += main.cpp
}

SOURCES += Settings.cpp \
    MainView.cpp \
    Core.cpp \
    WebViewInterface.cpp \
    CoreDbHelper.cpp \
    Logbook.cpp \
    FaviconImageProvider.cpp


HEADERS += buildconfig.h \
    Settings.h \
    MainView.h \
    LinkItem.h \
    Core.h \
    WebViewInterface.h \
    CoreDbHelper.h \
    Logbook.h \
    LogbookLinkItems.h \
    FaviconImageProvider.h

# Used to see the QML files in the Project view of "Qt Creator"
OTHER_FILES += qmls/BrowserView.qml \
    qmls/common/VHScrollDropShadow.qml \
    qmls/common/VScrollDropShadow.qml \
    qmls/common/HScrollDropShadow.qml \
    qmls/common/ScrollBar.qml \
    qmls/BrowserViewComponents/FlickableWebView.qml \
    qmls/BrowserViewComponents/Header.qml \
    qmls/BrowserViewComponents/fieldtext/FieldText.qml \
    qmls/BrowserViewComponents/Footer.qml \
    qmls/BrowserViewComponents/FieldText.qml \
    qmls/LogbookView.qml \
    qmls/LogbookViewComponents/BookmarksListView.qml \
    qmls/LogbookViewComponents/HistoryListView.qml \
    qmls/LogbookViewComponents/MostVisitedListView.qml \
    qmls/LogbookViewComponents/TabButton.qml \
    qmls/LogbookViewComponents/TabBar.qml \
    qmls/common/EmbossedButton.qml \
    qmls/LogbookViewComponents/Header.qml \
    qmls/common/BottomDropShadow.qml \
    qmls/common/TopDropShadow.qml \
    qmls/common/Background.qml \
    qmls/common/BarIcon.qml \
    qmls/common/ToolTip.qml \
    qmls/common/FeedToolTip.qml \
    qmls/common/TemporaryMessageWithIcon.qml \
    qmls/LogbookViewComponents/LogbookListView.qml

# Resources - DON'T forget to include the QML files
!contains(QTCOMPONENTS_EDITION,harmattan)  {
 RESOURCES += qmls.qrc
}

include(symbian/symbian.pri)
include(maemo5/maemo5.pri)
include(harmattan/harmattan.pri)

# Desktop Specific
#!maemo5 && !symbian {
#   linux || win32 || macosx {
#      OBJECTS_DIR = temp
#      MOC_DIR = temp
#      UI_DIR = temp
#   }
#}


debug {
    message(Building in DEBUG mode)
    DEFINES += QT_FATAL_WARNINGS
    DEFINES += ENABLE_LOG
    #DEFINES += ENABLE_VERBOSE
    #DEFINES += INSTALL_MESSAGE_HANDLER
}

release {
    message(Building in RELEASE mode)
}

unix:!symbian {
    meego5 {
        target.path = /opt/usr/bin
    } else {
        target.path = /usr/local/bin
    }
    INSTALLS += target
}
