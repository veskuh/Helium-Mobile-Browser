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
* File Name   : FieldText.qml
*
* Created     : 15 June 2011
*
*--------------------------------------------------------
* 1.0 - First open source release
*
*/


import QtQuick 1.1
import com.nokia.meego 1.0

Item {
   id: fieldText
   height: 32
   property string text: ""
   property bool mouseGrabbed: false
   property bool loading: false

   signal editFinished
   signal editAborted
   signal editStarted
   signal reloadRequested
   signal stopRequested

   function updateFavIcon() {
      //console.log("fieldText.setFavIcon()");
      defaultFavIcon.opacity = 0;
      favIcon.opacity = 1;
   }

   function setDefaultFavIcon() {
      //console.log("fieldText.setDefaultFavIcon()");
      favIcon.opacity = 0;
      defaultFavIcon.opacity = 0.7;
   }

   // Set the "fieldText" to "editingUrl" state, and emit the "editStarted" signal
   function edit() {
      if (!mouseGrabbed) {
         fieldText.editStarted();
         fieldText.state = 'editingUrl';
         mouseGrabbed = true;
      }
   }

   // Emit the "editFinished" signal
   function finishEdit() {
      fieldText.state = '';
      fieldText.text = textEdit.text;
      mouseGrabbed = false;
      textEdit.selectAll();
      fieldText.editFinished();
   }

   // Set the "fieldText" to "<empty>" state, and emit the "editAborted" signal
   function abortEdit() {
      if (mouseGrabbed) {
         mouseGrabbed = false;
         textEdit.text = fieldText.text;
         fieldText.state = '';
         textEdit.selectAll();
      }
      fieldText.editAborted();
   }



   // TextInput box in which the User types the URL
   TextField {
      id: textEdit

      Binding {
         target: textEdit
         property: "text"
         value: fieldText.text
      }

      Image {
         id: favIcon
         height: parent.height-16
         width: height
         smooth: true; asynchronous: true;
         fillMode: Image.PreserveAspectFit
         anchors.verticalCenter: parent.verticalCenter
         anchors.left: parent.left
         anchors.leftMargin: 16
         opacity: 0;
         source: "image://favicons/"+webView.url
      }

      Image {
         id: defaultFavIcon
         height: parent.height-16
         width: height
         smooth: true
         fillMode: Image.PreserveAspectFit
         anchors.verticalCenter: parent.verticalCenter
         anchors.left: parent.left
         anchors.leftMargin: 16
         source: "qrc:/qmls/pics/default-favico-30x30.png"
         opacity: 1
      }

      Image {
         id: reloadIcon
         height: parent.height-16
         width: height
         smooth: true
         anchors.verticalCenter: parent.verticalCenter
         anchors.right: parent.right
         anchors.rightMargin: 16
         source: "qrc:/qmls/pics/reload-30x30.png"
         opacity: 0.7
         MouseArea {
            anchors.fill: parent
            onClicked: { fieldText.reloadRequested(); }
            onPressed: { parent.opacity = 1; }
            onReleased: { parent.opacity = 0.7; }
         }
      }

      Image {
         id: clearIcon
         height: parent.height-16
         width: height
         smooth: true
         anchors.verticalCenter: parent.verticalCenter
         anchors.right: parent.right
         anchors.rightMargin: 16
         source: "qrc:/qmls/pics/clear-30x30.png"
         opacity: 0
         MouseArea {
            anchors.fill: parent
            onClicked: { textEdit.text = ''; }
            onPressed: { parent.opacity = 1; }
            onReleased: { parent.opacity = 0.7; }
         }
      }
      Image {
         id: stopIcon
         height: parent.height-16
         width: height
         smooth: true
         anchors.verticalCenter: parent.verticalCenter
         anchors.right: parent.right
         anchors.rightMargin: 16
         source: "qrc:/qmls/pics/stop-30x30.png"
         opacity: 0
         MouseArea {
            anchors.fill: parent
            onClicked: { fieldText.stopRequested(); }
            onPressed: { parent.opacity = 1; }
            onReleased: { parent.opacity = 0.7; }
         }
      }


      platformStyle: TextFieldStyle { paddingRight: defaultFavIcon.width + 32 ; paddingLeft:reloadIcon.width + 32  }
      anchors.left: parent.left
      anchors.leftMargin: 4
      anchors.right: parent.right
      anchors.rightMargin: 4
      anchors.verticalCenter: parent.verticalCenter
      font.family: "Helvetica"
      font.bold: false
      font.pixelSize: 18
      Keys.onEscapePressed: { abortEdit(); }
      Keys.onEnterPressed: { finishEdit(); }
      Keys.onReturnPressed: { finishEdit(); }

      placeholderText: "Enter URL"

      onActiveFocusChanged: {
          if( textEdit.focus ) {
              edit();
          }

      }



   }





   states: [
   State {
      name: "editingUrl"
      PropertyChanges {
         target: clearIcon
         opacity: 0.7
      }
      PropertyChanges {
         target: reloadIcon
         opacity: 0
      }
      PropertyChanges {
         target: stopIcon
         opacity: 0
      }
      PropertyChanges {
         target: textEdit
         readOnly: false
         focus: true
      }
      StateChangeScript {
         name: "selectAll"
         script: { textEdit.selectAll(); }
      }
   },
   State {
      name: "loadingUrl"
      when: fieldText.loading
      PropertyChanges {
         target: clearIcon
         opacity: 0
      }
      PropertyChanges {
         target: reloadIcon
         opacity: 0
      }
      PropertyChanges {
         target: stopIcon
         opacity: 0.7
      }
   }
   ]

   transitions: [
   Transition {
      from: ""
      to: "*"
      reversible: true
      NumberAnimation {
         property: "opacity"
         duration: 200
      }
   }
   ]
}
