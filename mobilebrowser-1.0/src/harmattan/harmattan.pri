contains(QTCOMPONENTS_EDITION,harmattan)  {
    INCLUDEPATH += $$PWD \
                   $$PWD/../src
    SOURCES += $$PWD/main.cpp

    # Speed up launching on MeeGo/Harmattan when using applauncherd daemon
    CONFIG += qdeclarative-boostable

    # Please do not modify the following two lines. Required for deployment.
    include($$PWD/qmlapplicationviewer/qmlapplicationviewer.pri)
    qtcAddDeployment()
RESOURCES += \
    $$PWD/res.qrc

OTHER_FILES += \
    $$PWD/qml/MainPage.qml \
    $$PWD/qml/main.qml \
    $$PWD/qml/LogbookView.qml \
    $$PWD/qml/Header.qml \
    $$PWD/qml/FlickableWebView.qml \
    $$PWD/qml/FieldText.qml \
    $$PWD/qml/common/VScrollDropShadow.qml \
    $$PWD/qml/common/VHScrollDropShadow.qml \
    $$PWD/qml/common/TopDropShadow.qml \
    $$PWD/qml/common/ToolTip.qml \
    $$PWD/qml/common/TemporaryMessageWithIcon.qml \
    $$PWD/qml/common/ScrollBar.qml \
    $$PWD/qml/common/HScrollDropShadow.qml \
    $$PWD/qml/common/EmbossedButton.qml \
    $$PWD/qml/common/BottomDropShadow.qml \
    $$PWD/qml/common/BarIcon.qml \
    $$PWD/qml/common/Background.qml \
    $$PWD/qml/LogbookViewComponents/TabButton.qml \
    $$PWD/qml/LogbookViewComponents/TabBar.qml \
    $$PWD/qml/LogbookViewComponents/MostVisitedListView.qml \
    $$PWD/qml/LogbookViewComponents/LogbookListView.qml \
    $$PWD/qml/LogbookViewComponents/HistoryListView.qml \
    $$PWD/qml/LogbookViewComponents/Header.qml \
    $$PWD/qml/LogbookViewComponents/BookmarksListView.qml
}

# HEADERS += $$PWD/Declarativable.h
