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
* File Name   : Header.qml
*
* Created     : 15 June 2011
*
*--------------------------------------------------------
* 1.0 - First open source release
*
*/

import Qt 4.7

import "../common"

BorderImage {
   id: header

   source: "qrc:/qmls/pics/bar-bg-10x80.png"
   border.top: 1
   border.bottom: 1
   smooth: true

   height: 70

   BottomDropShadow { width: parent.width; anchors.top: parent.bottom; z: 2; }

   Text {
      text: "Logbook"
      color: "#fff"
      styleColor: "#000"
      style: Text.Sunken

      font.family: "Helvetica"
      font.pointSize: 22
      font.bold: true

      anchors.centerIn: parent
   }

   EmbossedButton {
      id: doneButton
      text: "Done"
      width: 100; height: parent.height-20;
      baseColor: "#333"

      anchors.right: parent.right
      anchors.rightMargin: 5
      anchors.verticalCenter: parent.verticalCenter

      onClicked: { if (appcore) { appcore.showBrowserView(); } }
   }
}
