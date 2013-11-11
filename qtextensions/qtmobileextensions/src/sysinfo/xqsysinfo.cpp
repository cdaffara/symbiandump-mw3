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

#include "xqsysinfo.h"
#include "xqsysinfo_p.h"

/*!
    \class XQSysInfo

    \brief The XQSysInfo class is used to retrieve information about the phone (e.g. software version, free memory, IMEI, etc.)
*/

/*!
    Constructs a XQSysInfo object with the given parent.
*/
XQSysInfo::XQSysInfo(QObject* parent):
    QObject(parent), d(new XQSysInfoPrivate(this))
{
}

/*!
    Destroys the XQSysInfo object.
*/
XQSysInfo::~XQSysInfo()
{
    delete d;
}

/*!
    \enum XQSysInfo::Error
    This enum defines the possible errors for a XQSysInfo object.
*/
/*! \var XQSysInfo::Error XQSysInfo::NoError
    No error occured.
*/
/*! \var XQSysInfo::Error XQSysInfo::OutOfMemoryError
    Not enough memory.
*/
/*! \var XQSysInfo::Error XQSysInfo::IncorrectDriveError
    Wrong drive letter.
*/
/*! \var XQSysInfo::Error XQSysInfo::DriveNotFoundError
    Drive cannot be found.
*/
/*! \var XQSysInfo::Error XQSysInfo::UnknownError
    Unknown error.
*/

/*!
    \enum XQSysInfo::Drive

    This enum defines the possible drive letters to be used.
*/
/*! \var XQSysInfo::Drive XQSysInfo::DriveA 
    Drive A
*/
/*! \var XQSysInfo::Drive XQSysInfo::DriveB
    Drive B
*/
/*! \var XQSysInfo::Drive XQSysInfo::DriveC
    Drive C
*/
/*! \var XQSysInfo::Drive XQSysInfo::DriveD
    Drive D
*/
/*! \var XQSysInfo::Drive XQSysInfo::DriveE
    Drive E
*/
/*! \var XQSysInfo::Drive XQSysInfo::DriveF
    Drive F
*/
/*! \var XQSysInfo::Drive XQSysInfo::DriveG
    Drive G
*/
/*! \var XQSysInfo::Drive XQSysInfo::DriveH
    Drive H
*/
/*! \var XQSysInfo::Drive XQSysInfo::DriveI
    Drive I
*/
/*! \var XQSysInfo::Drive XQSysInfo::DriveJ
    Drive J
*/
/*! \var XQSysInfo::Drive XQSysInfo::DriveK
    Drive K
*/
/*! \var XQSysInfo::Drive XQSysInfo::DriveL
    Drive L
*/
/*! \var XQSysInfo::Drive XQSysInfo::DriveM
    Drive M
*/
/*! \var XQSysInfo::Drive XQSysInfo::DriveN 
    Drive N
*/
/*! \var XQSysInfo::Drive XQSysInfo::DriveO
    Drive O
*/
/*! \var XQSysInfo::Drive XQSysInfo::DriveP
    Drive P
*/
/*! \var XQSysInfo::Drive XQSysInfo::DriveQ
    Drive Q
*/
/*! \var XQSysInfo::Drive XQSysInfo::DriveR
    Drive R
*/
/*! \var XQSysInfo::Drive XQSysInfo::DriveS
    Drive S
*/
/*! \var XQSysInfo::Drive XQSysInfo::DriveT
    Drive T
*/
/*! \var XQSysInfo::Drive XQSysInfo::DriveU
    Drive U
*/
/*! \var XQSysInfo::Drive XQSysInfo::DriveV
    Drive V
*/
/*! \var XQSysInfo::Drive XQSysInfo::DriveW    
    Drive W
*/
/*! \var XQSysInfo::Drive XQSysInfo::DriveX
    Drive X
*/
/*! \var XQSysInfo::Drive XQSysInfo::DriveY
    Drive Y
*/
/*! \var XQSysInfo::Drive XQSysInfo::DriveZ
    Drive Z
*/

/*! 
    \enum XQSysInfo:Language
    This enum defines the languages that are available
    
*/        

/*!

    \return Current language of the phone UI
*/
XQSysInfo::Language XQSysInfo::currentLanguage() const
{
    return d->currentLanguage();
}

/*!

    \return International Mobile Equipment Identity (IMEI) unique identifier for the device. 
*/
QString XQSysInfo::imei() const
{
    return d->imei();
}

/*!

    \return International Mobile Subscriber Identity (IMSI) from the device's SIM card, 
*/
QString XQSysInfo::imsi() const
{
    return d->imsi();
}

/*!

    \return Firmware version of the device
*/
QString XQSysInfo::softwareVersion() const
{
    return d->softwareVersion();
}

/*!

    \return Phone model
*/
QString XQSysInfo::model() const
{
    return d->model();
}

/*!
    \return Manufacturer of the phone
*/
QString XQSysInfo::manufacturer() const
{
    return d->manufacturer();
}

/*!
    \return Current battery level as a percentage, i.e. 0 to 100%
*/
uint XQSysInfo::batteryLevel() const
{
    return d->batteryLevel();
}

/*!
    \return Network signal strength as a percentage, i.e. 0 to 100%
*/
int XQSysInfo::signalStrength() const
{
    return d->signalStrength();
}

/*!
    NOTE: No implementation in the alpha release

    \return Version string for the device's browser
*/
QString XQSysInfo::browserVersion() const
{
    return d->browserVersion();
}

/*!

    \param drive Drive letter as enumeration
    \return Available disk space in bytes on the given drive
*/
qlonglong XQSysInfo::diskSpace(XQSysInfo::Drive drive) const
{
    return d->diskSpace(drive);
}

/*!
    Helper function to check if the disk space on the given drive is in critical condition, i.e. very low

    \param drive Drive letter as enumeration
    \return True, if memory is low on the given device, otherwise false
*/
bool XQSysInfo::isDiskSpaceCritical(XQSysInfo::Drive drive) const
{
    return d->isDiskSpaceCritical(drive);
}

/*!
    Free RAM memory in bytes

    \return Free RAM memory in bytes
*/
int XQSysInfo::memory() const
{
    return d->memory();
}

/*!
    Checks if the specific feature is supported in the environment. See
    <a href="http://library.forum.nokia.com/topic/S60_5th_Edition_Cpp_Developers_Library/GUID-759FBC7F-5384-4487-8457-A8D4B76F6AA6/html/featureinfo_8h.html">
    featureinfo.h File Reference</a>
    in the Symbian S60 SDK for a list of the feature ids. //insp: major, description of the error checking missing
    
    \param featureId Feature id
     \return True, if the feature is supported, otherwise false
*/
bool XQSysInfo::isSupported(int featureId)
{
    return XQSysInfoPrivate::isSupported(featureId);
}
/*!
    \return Error code indicating the current error level
*/
XQSysInfo::Error XQSysInfo::error() const
{
    return d->error();
}

/*!
    Checks if the network is available

    \return True, if the network is available, otherwise false
*/
bool XQSysInfo::isNetwork() const
{
    return d->isNetwork();
}

/*!
    \fn void XQSysInfo::networkSignalChanged(ulong signalStrength)
    
    This signal is emitted when the signal strength changes.

    \param signalStrength New signal strength value
    \sa signalStrength()
*/

/*!
    \fn void XQSysInfo::batteryLevelChanged(uint batteryLevel)

    This signal is emitted when the battery level changes.

    \param batteryLevel New battery level
    \sa batteryLevel()
*/
