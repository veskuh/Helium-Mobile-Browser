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
* File Name   : LogbookListView.qml
*
* Created     : 15 June 2011
*
*--------------------------------------------------------
* 1.0 - First open source release
*
*/

import Qt 4.7

import "../common"

ListView {
// model: SET HERE THE MODEL
   cacheBuffer: 300
   pressDelay: 200

   Background { anchors.fill: parent; z: -1; }

   ScrollBar {
      verticalOrientation: true
      whenToShow: parent.moving
      positionInContent: parent.visibleArea.yPosition
      visibleContent: parent.visibleArea.heightRatio
      width: 8
      anchors.top: parent.top
      anchors.right: parent.right
      anchors.bottom: parent.bottom
      z: 2
   }

   VScrollDropShadow {
      x: -parent.contentX
      y: -parent.contentY
      width: parent.width
      height: parent.contentHeight
      z: 0
   }
}
