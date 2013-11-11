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

#include <DocumentHandler.h>
#include <apmstd.h>
#include <pathinfo.h>

XQUtilsPrivate::XQUtilsPrivate(XQUtils* utils): q(utils)
{
}

XQUtilsPrivate::~XQUtilsPrivate()
{
    delete iDocHandler;
}

bool XQUtilsPrivate::launchFile(const QString& filename)
{
    TRAP(iError,
        if (!iDocHandler)
        {
            iDocHandler = CDocumentHandler::NewL();
        }
        TPtrC16 textPtr(reinterpret_cast<const TUint16*>(filename.utf16()));
        TDataType dataType;
        User::LeaveIfError(iDocHandler->OpenFileEmbeddedL(textPtr, dataType));
    )
    return (iError == KErrNone);
}

void XQUtilsPrivate::resetInactivityTime()
{
    User::ResetInactivityTime();
}

QString XQUtilsPrivate::romRootPath()
{
    TPtrC romRootPath = PathInfo::RomRootPath();
    return QString::fromUtf16(romRootPath.Ptr(), romRootPath.Length());
}

QString XQUtilsPrivate::phoneMemoryRootPath()
{
    TPtrC phoneMemoryRootPath = PathInfo::PhoneMemoryRootPath();
    return QString::fromUtf16(phoneMemoryRootPath.Ptr(), phoneMemoryRootPath.Length());
}

QString XQUtilsPrivate::memoryCardRootPath()
{
    TPtrC memoryCardRootPath = PathInfo::MemoryCardRootPath();
    return QString::fromUtf16(memoryCardRootPath.Ptr(), memoryCardRootPath.Length());
}

QString XQUtilsPrivate::gamesPath()
{
    TPtrC gamesPath = PathInfo::GamesPath();
    return QString::fromUtf16(gamesPath.Ptr(), gamesPath.Length());
}

QString XQUtilsPrivate::installsPath()
{
    TPtrC installsPath = PathInfo::InstallsPath();
    return QString::fromUtf16(installsPath.Ptr(), installsPath.Length());
}

QString XQUtilsPrivate::othersPath()
{
    TPtrC othersPath = PathInfo::OthersPath();
    return QString::fromUtf16(othersPath.Ptr(), othersPath.Length());
}

QString XQUtilsPrivate::videosPath()
{
    TPtrC videosPath = PathInfo::VideosPath();
    return QString::fromUtf16(videosPath.Ptr(), videosPath.Length());
}

QString XQUtilsPrivate::imagesPath()
{
    TPtrC imagesPath = PathInfo::ImagesPath();
    return QString::fromUtf16(imagesPath.Ptr(), imagesPath.Length());
}

QString XQUtilsPrivate::picturesPath()
{
    TPtrC picturesPath = PathInfo::PicturesPath();
    return QString::fromUtf16(picturesPath.Ptr(), picturesPath.Length());
}

QString XQUtilsPrivate::gmsPicturesPath()
{
    TPtrC gmsPicturesPath = PathInfo::GmsPicturesPath();
    return QString::fromUtf16(gmsPicturesPath.Ptr(), gmsPicturesPath.Length());
}

QString XQUtilsPrivate::mmsBackgroundImagesPath()
{
    TPtrC mmsBackgroundImagesPath = PathInfo::MmsBackgroundImagesPath();
    return QString::fromUtf16(mmsBackgroundImagesPath.Ptr(), mmsBackgroundImagesPath.Length());
}

QString XQUtilsPrivate::presenceLogosPath()
{
    TPtrC presenceLogosPath = PathInfo::PresenceLogosPath();
    return QString::fromUtf16(presenceLogosPath.Ptr(), presenceLogosPath.Length());
}

QString XQUtilsPrivate::soundsPath()
{
    TPtrC soundsPath = PathInfo::SoundsPath();
    return QString::fromUtf16(soundsPath.Ptr(), soundsPath.Length());
}

QString XQUtilsPrivate::digitalSoundsPath()
{
    TPtrC digitalSoundsPath = PathInfo::DigitalSoundsPath();
    return QString::fromUtf16(digitalSoundsPath.Ptr(), digitalSoundsPath.Length());
}

QString XQUtilsPrivate::simpleSoundsPath()
{
    TPtrC simpleSoundsPath = PathInfo::SimpleSoundsPath();
    return QString::fromUtf16(simpleSoundsPath.Ptr(), simpleSoundsPath.Length());
}

QString XQUtilsPrivate::imagesThumbnailPath()
{
    TPtrC imagesThumbnailPath = PathInfo::ImagesThumbnailPath();
    return QString::fromUtf16(imagesThumbnailPath.Ptr(), imagesThumbnailPath.Length());
}

QString XQUtilsPrivate::memoryCardContactsPath()
{
    TPtrC memoryCardContactsPath = PathInfo::MemoryCardContactsPath();
    return QString::fromUtf16(memoryCardContactsPath.Ptr(), memoryCardContactsPath.Length());
}

XQUtils::Error XQUtilsPrivate::error() const
{
    switch (iError)
    {
        case KErrNone:
            return XQUtils::NoError;
        case KErrNoMemory:
            return XQUtils::OutOfMemoryError;
        case KUserCancel:
            return XQUtils::UserCancelledError;
        default:
            return XQUtils::UnknownError;
    }
}

// End of file
