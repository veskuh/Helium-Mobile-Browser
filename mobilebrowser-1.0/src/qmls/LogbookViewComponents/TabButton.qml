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
* File Name   : TabButton.qml
*
* Created     : 15 June 2011
*
*--------------------------------------------------------
* 1.0 - First open source release
*
*/

import Qt 4.7

Rectangle {
   id: tabButton
   property alias imageSource: icon.source
   property alias text: label.text
   property bool selected: false

   z: 0
   color: "transparent"
   state: "normal"

   Rectangle {
      id: tabButtonSelectedUnderlay
      anchors.fill: parent
      anchors.margins: 3
      color: "#fff"
      radius: 5
      z: 1
   }

   Image {
      id: icon
      anchors.horizontalCenter: parent.horizontalCenter
      anchors.top: parent.top
      anchors.topMargin: 5
      z: 2
   }
   Text {
      id: label
      color: "#fff"
      styleColor: "#000"
      style: Text.Sunken

      font.family: "Helvetica"
      font.pointSize: 12
      font.bold: true

      anchors.horizontalCenter: parent.horizontalCenter
      anchors.bottom: parent.bottom
      anchors.bottomMargin: 5
      z: 2
   }

   states: [
      State {
         name: "normal"
         when: !tabButton.selected
         PropertyChanges { target: tabButtonSelectedUnderlay; opacity: 0; }
      },
      State {
         name: "selected"
         when: tabButton.selected
         PropertyChanges { target: tabButtonSelectedUnderlay; opacity: 0.2; }
      }
   ]
}
