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
* File Name   : MainView.h
*
* Created     : 15 June 2011
*
* Description :
*
* the main QDeclarativeView
*
*--------------------------------------------------------
* 1.0 - First open source release
*
*/

#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QDeclarativeView>

class MainView : public QDeclarativeView
{
   Q_OBJECT
public:
   MainView(QWidget *parent = 0);
};

#endif // MAINVIEW_H
