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
* File Name   : Footer.qml
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
   id: footer

   signal bookmarkAdded
   signal lostFocus

   source: "qrc:/qmls/pics/bar-bg-10x80.png"
   border.top: 1
   border.bottom: 1
   opacity: 1
   width: parent.width
   height: 70
   anchors.bottom: parent.bottom

   // Back Button
   BarIcon {
      id: backbutton
      enabled: webView.back.enabled
      source: "qrc:/qmls/pics/back-30x30.png"
      onClicked: { webView.back.trigger(); }

      height: parent.height
      width: height
      anchors.left: parent.left
      anchors.leftMargin: 5
      anchors.verticalCenter: parent.verticalCenter
   }

   // Home Button
   BarIcon {
      id: homebutton
      enabled: true
      source: "qrc:/qmls/pics/home-30x30.png"
      onClicked: { if (appcore) appcore.loadHomeUrl(); }

      height: parent.height
      width: height
      anchors.left: backbutton.right
      anchors.leftMargin: 5
      anchors.verticalCenter: parent.verticalCenter
   }

   // Forward Button
   BarIcon {
      id: forwardbutton
      enabled: webView.forward.enabled
      source: "qrc:/qmls/pics/forward-30x30.png"
      onClicked: { webView.forward.trigger(); }

      height: parent.height
      width: height
      anchors.left: homebutton.right
      anchors.leftMargin: 5
      anchors.verticalCenter: parent.verticalCenter
   }

   // New Bookmark Button
   BarIcon {
      id: newbookmarkbutton
      source: "qrc:/qmls/pics/new-bookmark-30x30.png"
      onClicked: { footer.bookmarkAdded(); if (appcore) appcore.bookmarkCurrentUrl(); footer.lostFocus(); }

      height: parent.height
      width: height
      anchors.right: logbookviewbutton.left
      anchors.rightMargin: 5
      anchors.verticalCenter: parent.verticalCenter
   }

   // LogbookView Button
   BarIcon {
      id: logbookviewbutton
      source: "qrc:/qmls/pics/bookmarks-30x30.png"
      onClicked: { if (appcore) appcore.showLogbookView(); }

      height: parent.height
      width: height
      anchors.right: parent.right
      anchors.rightMargin: 5
      anchors.verticalCenter: parent.verticalCenter
   }
}
