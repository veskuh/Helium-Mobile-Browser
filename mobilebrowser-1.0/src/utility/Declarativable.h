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
* File Name   : Declarativable.h
*
* Created     : 15 June 2011
*
* Description :
*
* Utility class to make a class QML friendly
*
*--------------------------------------------------------
* 1.0 - First open source release
*
*/

#ifndef DECLARATIVABLE_H
#define DECLARATIVABLE_H

#include <QString>

/**
  Subclass this class to implement a Declarativable class.
  In this way the developer of the implemening class, will have to provide some methods used in the QML space.
  For the way it's written, it will enforce the need to implement the methods provided.
  */
class Declarativable
{
public:
   /** Used with qRegisterMetaType(const char *typeName). */
   static const char * metaTypeName();
   /** Used with QDeclarativeContext::setContextProperty(const QString &name, const QVariant &value).
     It's going to be the name of the ListModel withint the QML space
   */
   static QString declarativeName();
};

#endif // DECLARATIVABLE_H
