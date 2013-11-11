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

#ifndef XQUTILS
#define XQUTILS

// INCLUDES
#include <QObject>
#include "utils_global.h"

// FORWARD DECLARATIONS
class XQUtilsPrivate;

// CLASS DECLARATION
class XQUTILS_EXPORT XQUtils : public QObject
{
     Q_OBJECT

public:
    enum Error
    {
        NoError = 0,
        OutOfMemoryError,
        UserCancelledError,
        UnknownError = -1
    };

    XQUtils(QObject* parent = 0);
    ~XQUtils();

    bool launchFile(const QString& filename);

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

public Q_SLOTS:
    void resetInactivityTime();

private:
    XQUtilsPrivate* d;
};

#endif // XQUTILS

// End of file
