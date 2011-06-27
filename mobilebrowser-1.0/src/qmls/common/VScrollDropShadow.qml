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
* File Name   : VScrollDropShadow.qml
*
* Created     : 15 June 2011
*
*--------------------------------------------------------
* 1.0 - First open source release
*
*/

import Qt 4.7

Item {
   Image {
      source: "qrc:/qmls/pics/softshadow-top.png"
      x: 0
      y: -16
      width: parent.width
      height: 16
   }
   Image {
      source: "qrc:/qmls/pics/softshadow-bottom.png"
      x: 0
      y: parent.height
      width: parent.width
      height: 16
   }
}
