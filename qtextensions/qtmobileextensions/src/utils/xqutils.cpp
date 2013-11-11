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

#include "xqutils.h"
#include "xqutils_p.h"

/*!
    \class XQUtils
    \brief The XQUtils is a utility class. The class constains some
    convenience functions e.g.  keeping the device backlight on, and launching files.
*/

/*!
    Constructs a XQUtils object with the given parent.
*/
XQUtils::XQUtils(QObject* parent)
 : QObject(parent), d(new XQUtilsPrivate(this))
{
}

/*!
    Destroys the XQUtils object.
*/
XQUtils::~XQUtils()
{
    delete d;
}

/*!
    \enum XQUtils::Error

    This enum defines the possible errors for an XQUtils object.
*/
/*! \var XQUtils::Error XQUtils::NoError
    No error occured.
*/
/*! \var XQUtils::Error XQUtils::OutOfMemoryError
    Not enough memory.
*/
/*! \var XQUtils::Error XQUtils::UserCancelledError
    User cancelled an operation.
*/
/*! \var XQUtils::Error XQUtils::UnknownError
    Unknown error.
*/

/*!
    Tries to launch a file in the appropriate application based on the file type.

    \param filename Path to the file
    \return If false is returned, an error has occurred. Call error() to the
    XQUtils::Error value that indicates which error occurred
    \sa error()
*/
bool XQUtils::launchFile(const QString& filename)
{
    return d->launchFile(filename);
}

/*!
    Resets the system inactivity timer. Calling this function regularly keeps
    the device backlight on.
*/
void XQUtils::resetInactivityTime()
{
    d->resetInactivityTime();
}

/*!
    Returns the root path in ROM.

    \return The root path in ROM.
*/
QString XQUtils::romRootPath()
{
    return XQUtilsPrivate::romRootPath();
}

/*!
    Returns the root path in Phone Memory.

    \return The root path in Phone Memory.
*/
QString XQUtils::phoneMemoryRootPath()
{
    return XQUtilsPrivate::phoneMemoryRootPath();
}

/*!
    Returns the root path in Memory Card.

    \return The root path in Memory Card.
*/
QString XQUtils::memoryCardRootPath()
{
    return XQUtilsPrivate::memoryCardRootPath();
}

/*!
    Returns the games path to be appended to a root path.

    \return The games path.
*/
QString XQUtils::gamesPath()
{
    return XQUtilsPrivate::gamesPath();
}

/*!
    Returns the installs path to be appended to a root path.

    \return The installs path.
*/
QString XQUtils::installsPath()
{
    return XQUtilsPrivate::installsPath();
}

/*!
    Returns the others path to be appended to a root path.

    \return The installs path.
*/
QString XQUtils::othersPath()
{
    return XQUtilsPrivate::othersPath();
}

/*!
    Returns the videos path to be appended to a root path.

    \return The videos path.
*/
QString XQUtils::videosPath()
{
    return XQUtilsPrivate::videosPath();
}

/*!
    Returns the images path to be appended to a root path.

    \return The images path.
*/
QString XQUtils::imagesPath()
{
    return XQUtilsPrivate::imagesPath();
}

/*!
    Returns the pictures path to be appended to a root path.

    \return The pictures path.
*/
QString XQUtils::picturesPath()
{
    return XQUtilsPrivate::picturesPath();
}

/*!
    Returns the GMS pictures path to be appended to a root path.

    \return The GMS pictures path.
*/
QString XQUtils::gmsPicturesPath()
{
    return XQUtilsPrivate::gmsPicturesPath();
}

/*!
    Returns the MMS background images path to be appended to a root path.

    \return The MMS background images path.
*/
QString XQUtils::mmsBackgroundImagesPath()
{
    return XQUtilsPrivate::mmsBackgroundImagesPath();
}

/*!
    Returns the presence logos path to be appended to a root path.

    \return The presence logos path.
*/
QString XQUtils::presenceLogosPath()
{
    return XQUtilsPrivate::presenceLogosPath();
}

/*!
    Returns the sounds path to be appended to a root path.

    \return The sounds path.
*/
QString XQUtils::soundsPath()
{
    return XQUtilsPrivate::soundsPath();
}

/*!
    Returns the digital sounds path to be appended to a root path.

    \return The digital sounds path.
*/
QString XQUtils::digitalSoundsPath()
{
    return XQUtilsPrivate::digitalSoundsPath();
}

/*!
    Returns the simple sounds path to be appended to a root path.

    \return The simple sound path.
*/
QString XQUtils::simpleSoundsPath()
{
    return XQUtilsPrivate::simpleSoundsPath();
}

/*!
    Returns a thumbnail images path. The thumbnail images
    directory exists under the same directory where the corresponding
    image is. Do not try to append this to a root directory.

    \return The thumbnail images path.
*/
QString XQUtils::imagesThumbnailPath()
{
    return XQUtilsPrivate::imagesThumbnailPath();
}

/*!
    Returns the full path of the contacts folder in 
    the memory card. The path also contains the drive letter.
    Do not try to append this to any root directory.

    \return The full path of the contacts folder in the memory card.
*/
QString XQUtils::memoryCardContactsPath()
{
    return XQUtilsPrivate::memoryCardContactsPath();
}

/*!
    Returns the type of error that occurred if the latest function call failed; otherwise returns NoError.

    \return Error code
*/
XQUtils::Error XQUtils::error() const
{
    return d->error();
}

// End of file
