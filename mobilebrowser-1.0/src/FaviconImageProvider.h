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
* File Name   : FaviconImageProvider.h
*
* Created     : 15 June 2011
*
* Description :
*
* Wrapper aroung Webkit's favicon database
*
*--------------------------------------------------------
* 1.0 - First open source release
*
*/

#ifndef FAVICONIMAGEPROVIDER_H
#define FAVICONIMAGEPROVIDER_H

#include <QDeclarativeImageProvider>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QImage>

/**
  Provider of Favicons.
  It's based on the WebKit Icon Database, and extracts favicons from there.

  Note: The Class depends on the SQL Schema of the WebKit's Icon Database. If it changes, this class should change.
  Info about Favicon: \link{http://en.wikipedia.org/wiki/Favicon}
  */
class FaviconImageProvider : public QDeclarativeImageProvider
{
public:
    FaviconImageProvider();
    virtual ~FaviconImageProvider();

    /**
      @param id It's expected to be a URL, usually previously "normalized" by a browser (core).
      @param size It's an Output Variable to return the size of the Image
      @param requested_size Allows to require an image of a particular size (ignored here)
    */
    virtual QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

private:
    QSqlDatabase     m_db;
    QSqlQuery        m_query;
    QImage           m_defaultFavicon;
};

#endif // FAVICONIMAGEPROVIDER_H
