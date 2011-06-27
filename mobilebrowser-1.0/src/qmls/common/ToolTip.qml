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
* File Name   : ToolTip.qml
*
* Created     : 15 June 2011
*
*--------------------------------------------------------
* 1.0 - First open source release
*
*/

import Qt 4.7

BorderImage {
   id: toolTip
   z: 5

   property variant attachTo
   property int attachToMargin: 10
   property int orientation: 0   // '0' is 'North', '1' is 'East', '2' is 'South', '3' is 'West'
   property bool transparent: false
   property int marginsForChilds: 10
   enabled: true
   opacity: 1

   // Orientation == '0'(North)
   anchors.top: (orientation == 0) ? attachTo.bottom : none
   anchors.topMargin: (orientation == 0) ? attachToMargin : 0
   // Orientation == '1'(East)
   anchors.right: (orientation == 1) ? attachTo.left: none
   anchors.rightMargin: (orientation == 1) ? attachToMargin : 0
   // Orientation == '2'(South)
   anchors.bottom: (orientation == 2) ? attachTo.top : none
   anchors.bottomMargin: (orientation == 2) ? attachToMargin : 0
   // Orientation == '3'(West)
   anchors.left: (orientation == 3) ? attachTo.right: none
   anchors.leftMargin: (orientation == 3) ? attachToMargin : 0

   // Orientation == '1'(East) or Orientation == '3'(West)
   anchors.verticalCenter: (orientation == 1 || orientation == 3) ? attachTo.verticalCenter : none
   // Orientation == '0'(North) or Orientation == '2'(South)
   anchors.horizontalCenter: (orientation == 0 || orientation == 2) ? attachTo.horizontalCenter : undefined

   source: transparent ? "qrc:/qmls/pics/tooltip-bg-trans-100x100.png" : "qrc:/qmls/pics/tooltip-bg-100x100.png"
   border.top: 10
   border.bottom: 10
   border.left: 10
   border.right: 10

   Image {
      source: (orientation == 0) ? "qrc:/qmls/pics/tip-center-top-99x50.png" :
         (orientation == 1) ? "qrc:/qmls/pics/tip-center-right-50x99.png" :
         (orientation == 2) ? "qrc:/qmls/pics/tip-center-bottom-99x50.png" :
         "qrc:/qmls/pics/tip-center-left-50x99.png";

      // Orientation == '1'(East) or Orientation == '3'(West)
      anchors.verticalCenter: (parent.orientation == 1 || parent.orientation == 3) ? parent.verticalCenter : none
      // Orientation == '0'(North) or Orientation == '2'(South)
      anchors.horizontalCenter: (parent.orientation == 0 || parent.orientation == 2) ? parent.horizontalCenter : none

      // Orientation == '0'(North)
      anchors.bottom: (parent.orientation == 0) ? parent.top : none
      // Orientation == '1'(East)
      anchors.left: (parent.orientation == 1) ? parent.right : none
      // Orientation == '2'(South)
      anchors.top: (parent.orientation == 2) ? parent.bottom : none
      // Orientation == '3'(West)
      anchors.right: (parent.orientation == 3) ? parent.left : none

      // Sizing based on Orientation
      width: (parent.orientation == 0 || parent.orientation == 2) ? 60 : 30
      height: (parent.orientation == 0 || parent.orientation == 2) ? 30 : 60

      smooth: true
   }

//   effect: DropShadow { blurRadius: 20; offset.x: 0; offset.y: 0; }

   states: [
      State {
         name: "disabled"; when: !toolTip.enabled;
         PropertyChanges {
            target: toolTip;
            opacity: 0;
            // Orientation == '0' - 'North'
            anchors.topMargin: (toolTip.orientation == 0) ? -toolTip.attachToMargin : 0;
            // Orientation == '1' - 'East'
            anchors.rightMargin: (toolTip.orientation == 1) ? -toolTip.attachToMargin : 0;
            // Orientation == '2' - 'South'
            anchors.bottomMargin: (toolTip.orientation == 2) ? -toolTip.attachToMargin : 0;
            // Orientation == '3' - 'West'
            anchors.leftMargin: (toolTip.orientation == 3) ? -toolTip.attachToMargin : 0;
         }
      },
      State {
         name: "enabled"; when: toolTip.enabled;
         PropertyChanges {
            target: toolTip;
            opacity: 1;
            // Orientation == '0' - 'North'
            anchors.topMargin: (toolTip.orientation == 0) ? toolTip.attachToMargin : 0;
            // Orientation == '1' - 'East'
            anchors.rightMargin: (toolTip.orientation == 1) ? toolTip.attachToMargin : 0;
            // Orientation == '2' - 'South'
            anchors.bottomMargin: (toolTip.orientation == 2) ? toolTip.attachToMargin : 0;
            // Orientation == '3' - 'West'
            anchors.leftMargin: (toolTip.orientation == 3) ? toolTip.attachToMargin : 0;
         }
      }
   ]

   transitions: [
      Transition {
         from: "disabled"; to: "enabled"; reversible: true;
         NumberAnimation {
            target: toolTip; duration: 200;
            properties: "opacity, anchors.topMargin, anchors.rightMargin, anchors.bottomMargin, anchors.leftMargin";
         }
      }
   ]

   onFocusChanged: { if ( !focus ) { enabled = false; } }
}
