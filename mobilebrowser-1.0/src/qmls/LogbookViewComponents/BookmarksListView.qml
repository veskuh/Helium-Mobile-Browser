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
* File Name   : BookmarksListView.qml
*
* Created     : 15 June 2011
*
*--------------------------------------------------------
* 1.0 - First open source release
*
*/

import Qt 4.7

import "../common"

LogbookListView {
   delegate: delegateComponent

   // Define a delegate component.  A component will be
   // instantiated for each visible item in the list.
   Component {
      id: delegateComponent

      Rectangle {
         id: delegateUi
         width: (parent) ? parent.width : 800
         height: 70
         color: "#fff"

         BorderImage {
            source: "qrc:/qmls/pics/url-list-bg-10x60.png"
            border.top: 1
            anchors.fill: parent
         }

         Rectangle {
            id: delegateIcon
            height: parent.height-10
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.margins: 2
            width: height

            color: "#ddd"
            radius: 5

            Image {
               id: bookmarkIcon
               anchors.centerIn: parent
               width: 30
               height: 30
               smooth: true; asynchronous: true;
               source: "image://favicons/"+model.url
            }

            Image {
               id: isHomeIcon
               source: "qrc:/qmls/pics/home-icon-32x32.png"
               opacity: model.isHome ? 1 : 0
               anchors.top: parent.top
               anchors.topMargin: -3
               anchors.right: parent.right
               anchors.rightMargin: -3
               width: 25
               height: 25
               smooth: true
               z: 4
            }
         }

         Column {
            anchors.left: delegateIcon.right
            anchors.leftMargin: 3
            anchors.right: parent.right
            anchors.rightMargin: 3
            anchors.verticalCenter: parent.verticalCenter
            Text {
               id: delegateTitle
               font.pointSize: 16
               font.bold: true
               color: "#000000"
               text: model.title
               elide: Text.ElideRight
               width: parent.width
            }
            Text {
               id: delegateUrl
               text: model.url
               font.pointSize: 14
               color: "#555555"
               elide: Text.ElideRight
               width: parent.width
            }
         }

         Rectangle {
            id: delegateUiHover
            opacity: 0
            anchors.fill: parent
            color: "#FF6600"
         }

         MouseArea {
            anchors.fill: parent
            onPressed: { delegateUiHover.opacity = 0.4; }
            onReleased: { if ( appcore ) { appcore.currentUrl = model.url; } delegateUiHover.opacity = 0; }
            onExited: { delegateUiHover.opacity = 0; }
         }
      }
   }

   section.property: "title"
   section.criteria: ViewSection.FirstCharacter
   section.delegate: Background {
      width: (parent) ? parent.width : 480
      height: 30
      Text {
         text: section; font.pointSize: 14; font.bold: true; color: "#fff"; style: Text.Sunken; styleColor: "#000";
         anchors.left: parent.left; anchors.leftMargin: 4; height: parent.height; verticalAlignment: 'AlignVCenter'
      }
   }
}
