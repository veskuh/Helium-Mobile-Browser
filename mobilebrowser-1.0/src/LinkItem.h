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
* File Name   : LinkItem.h
*
* Created     : 15 June 2011
*
* Description :
*
* Generic list item
*
*--------------------------------------------------------
* 1.0 - First open source release
*
*/

#ifndef LINKITEM_H
#define LINKITEM_H

#include <QString>

typedef int linkItemId;
#define LINK_ITEM_ID_UNDEFINED   0

struct LinkItem
{
   QString title;
   QString url;
   linkItemId id;

   LinkItem(const QString &title, const QString &url, const linkItemId &id = LINK_ITEM_ID_UNDEFINED) {
      this->title = title; this->url = url; this->id = id;
   }
   LinkItem() { }
};

#endif // LINKITEM_H
