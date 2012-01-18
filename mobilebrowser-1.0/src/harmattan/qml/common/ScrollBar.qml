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
* File Name   : ScrollBar.qml
*
* Created     : 15 June 2011
*
*--------------------------------------------------------
* 1.0 - First open source release
*
*/

import Qt 4.7

Item {
    id: scrollBar
    // The properties that define the scrollbar's state.
    // positionInContent and visibleContent are in the range 0.0 - 1.0.  They are relative to the
    // height of the page, i.e. a visibleContent of 0.5 means that you can see 50%
    // of the height of the view.
    // The equation "positionInContent + visibleContent == 1" must be ALWAYS TRUE.
    // orientation can be either 'Vertical' or 'Horizontal'
    property real positionInContent
    property real visibleContent
    property bool verticalOrientation: true
    property bool whenToShow: false
    opacity: 0

    // Size the bar to the required size, depending upon the orientation.
    Rectangle {
        opacity: parent.visibleContent < 1 ? 0.7 : 0
        color: "#555555"
        border.width: 1
        border.color: "#cccccc"
        radius: scrollBar.verticalOrientation ? (width/2 - 1) : (height/2 - 1)
        x: scrollBar.verticalOrientation ? 1 : (scrollBar.positionInContent * (scrollBar.width-2) + 1)
        y: scrollBar.verticalOrientation ? (scrollBar.positionInContent * (scrollBar.height-2) + 1) : 1
        width: scrollBar.verticalOrientation ? (scrollBar.width-2) : Math.max( height, (scrollBar.visibleContent * (scrollBar.width-2)) )
        height: scrollBar.verticalOrientation ? Math.max( width, (scrollBar.visibleContent * (scrollBar.height-2)) ) : (scrollBar.height-2)
    }

    states: [
       State {
          name: "ShowBars"
          when: scrollBar.whenToShow
          PropertyChanges { target: scrollBar; opacity: 1 }
       }
    ]
    transitions: [
       Transition {
          from: "*"
          to: "ShowBars"
          reversible: true
          PropertyAnimation { property: "opacity"; duration: 200; }
       }
    ]
}
