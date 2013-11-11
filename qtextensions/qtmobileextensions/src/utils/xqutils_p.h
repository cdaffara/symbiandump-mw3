/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, version 2.1 of the License.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, 
* see "http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html/".
*
* Description:
*
*/

#ifndef XQUTILS_P_H
#define XQUTILS_P_H

// INCLUDES
#include <exception>
#include <e32base.h>

// FORWARD DECLARATIONS
class XQUtils;
class CDocumentHandler;

// CLASS DECLARATION
class XQUtilsPrivate: public CBase
{
public:
    XQUtilsPrivate(XQUtils* utils);
    ~XQUtilsPrivate();

    bool launchFile(const QString& filename);
    void resetInactivityTime();

    static QString romRootPath();
    static QString phoneMemoryRootPath();
    static QString memoryCardRootPath();
    static QString gamesPath();
    static QString installsPath();
    static QString othersPath();
    static QString videosPath();
    static QString imagesPath();
    static QString picturesPath(); 
    static QString gmsPicturesPath();
    static QString mmsBackgroundImagesPath();
    static QString presenceLogosPath();
    static QString soundsPath();
    static QString digitalSoundsPath();
    static QString simpleSoundsPath();
    static QString imagesThumbnailPath();
    static QString memoryCardContactsPath();

    XQUtils::Error error() const;

private:
    CDocumentHandler* iDocHandler;
    XQUtils* q;
    int iError;
};

#endif /*XQUTILS_P_H*/

// End of file
