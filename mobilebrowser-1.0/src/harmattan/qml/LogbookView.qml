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
* File Name   : LogbookView.qml
*
* Created     : 15 June 2011
*
* Description :
*
* The main logbook view, from here you can navigate
* through the browser history and the bookmarks
*
*--------------------------------------------------------
* 1.0 - First open source release
*
*/

import Qt 4.7

import "LogbookViewComponents"
import "common"

Item {
   id: logbookView

   width: parent ? parent.width : 480
   height: parent ? parent.height : 800

   // Tabs-Container
   Row {
      id: tabsContainer
     // anchors.top: parent.bottom
      anchors.bottom: tabs.top
      z: 0

      BookmarksListView {
         id: bookmarks
         width: logbookView.width
         height: logbookView.height - tabs.height
         model: bookmarksModel
         z: 1
      }

      MostVisitedListView {
         id: mostVisited
         width: logbookView.width
         height: logbookView.height - tabs.height
         model: mostVisitedModel
         z: 1
      }

      HistoryListView {
         id: history
         width: logbookView.width
         height: logbookView.height - tabs.height
         model: historyModel
         z: 1
      }

      states: [
      State {
         name: "bookmarksSelected"
         when: tabs.selectedTab == 0
         PropertyChanges { target: tabsContainer; x: 0; }
      },
      State {
         name: "mostVisitedSelected"
         when: tabs.selectedTab == 1
         PropertyChanges { target: tabsContainer; x: -logbookView.width; }
      },
      State {
         name: "historySelected"
         when: tabs.selectedTab == 2
         PropertyChanges { target: tabsContainer; x: -logbookView.width*2; }
      }
      ]

      transitions: [
      Transition {
         from: "*"
         to: "*"
         reversible: true
         PropertyAnimation { property: "x"; duration: 300; easing.type: "InOutQuad"; }
      }
      ]
   }

   // Tabs-Header
   TabBar {
      id: tabs
      z: 2
      anchors.bottom: parent.bottom
      anchors.left: parent.left
      anchors.right: parent.right
   }
}
