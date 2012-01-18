# Helium Mobile Browser #
An experimental Mobile Browser, built on the top of Qt, QML, WebKit - HTML5 enabled.

## Background ##
Orange Labs UK has worked for long now in the area of Mobile Web Technologies, producing a number of internal projects and documents in the area.
The advent of HTML5, CSS3, Javascript and all the related technologies is making the Mobile Browser world move at an unprecedented speed.
We believe that Mobile Browsers will enable to deliver to Mobile device a new, richer class of WebApps. These WebApps will enjoy an everyday-smaller gap compared to all-native applications.

## Objective ##
Build a WebKit based browser using Qt and all the cutting edge related technologies (like QML).
The work takes into focus 3 areas:

1. **Browser Chrome**, focused on User Experience and Usability
2. **Browser Core**, focused on the WebCore functionalities that are already in WebKit and, possibly, building in advance some of them
3. **Browser Extensions**, focused on designing a Framework to implement mini-in-browser apps, to deliver small, focused services

## High-level TODOs ##

* Document properly the C++ code. Documentation is a bit too "sparse".
    * Need to learn the Doxygen tags used by the Qt guys - I know, I should have already done that
* Tune the performances: there are so many different optimization in `QWebSettings` that can be applied
* Produce a Symbian Build: for now we have only tested on Maemo5/N900

## Directory Strucure ##

* `assets/` - It's sort of workspace of the graphic assets of the project
* `builds/` - Stable builds, divided by Target Platform (Maemo5, Symbian, ...)
* `mobilebrowser-1.0/` - contains the actual source code of the project
* `screenshots/` - Screenshots, organized by date

## Building for Harmattan QtComponents ##
If building with QtSDK 1.1.4 or newer for Harmattan (N9) Helium the build compiles use of QtComponents by default.
You can run QtComponents version in QtSimulator by giving QTCOMPONENTS_EDITION=harmattan as additional argument to qmake and in simulator selecting
harmattan as edition for the components.

# Credits #
This project started from the `[qt-prefix]/demos/declarative/webbrowser/` ([see on qt.gitorious.org](http://qt.gitorious.org/qt/qt/trees/4.7/demos/declarative/webbrowser)), then evolved into what is today.

THANKS to the [Qt](http://labs.trolltech.com/)/[Nokia](http://qt.nokia.com/) guys for the amazing job they do with the Framework and the tons of high quality, inspiring examples and demos always provided.

Thanks to the Nokia Meego Developer Edition team: http://wiki.meego.com/ARM/N900#MeeGo_1.2_Community_Edition_for_Nokia_N900
	- expecially Vesku Hartikainen http://twitter.com/#!/veskuh for his contributions and interest in the project

Currently the maintainer of the project is me, Luca Colantonio lucabox@gmail.com https://twitter.com/#!/lucabox

A BIG credit goes also to the one who started the project: Ivan De Marino detronizator@gmail.com https://twitter.com/#!/detronizator

Some of the icons found under assets/ are taken from http://glyphish.com (released with Creative Commons Attribution)

