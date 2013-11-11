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

#ifndef XQSYSINFOPRIVATE_H
#define XQSYSINFOPRIVATE_H

// INCLUDES
#include "xqsysinfo.h"
#include "xqsysinfo_p.h"

#include "networksignalmonitor.h"
#include "batterymonitor.h"

#include <etel3rdparty.h>

// FORWARD DECLARATIONS
class CDeviceInfo;

// CLASS DECLARATION
class XQSysInfoPrivate: public QObject, public CBase, public MNetworkSignalObserver, 
    public MBatteryObserver
{
    Q_OBJECT
public:
    XQSysInfoPrivate(XQSysInfo* sysInfo);
    ~XQSysInfoPrivate();
    
    XQSysInfo::Language currentLanguage() const;
    QString imei() const;
    QString imsi() const;
    QString softwareVersion() const;
    QString model() const;
    QString manufacturer() const;
    uint batteryLevel() const;
    int signalStrength() const;                  
    QString browserVersion() const;
    
    int memory() const;
    qlonglong diskSpace(XQSysInfo::Drive drive);
    bool isDiskSpaceCritical(XQSysInfo::Drive drive) const;
    bool isNetwork() const;
    
    static bool isSupported(int featureId);
    
    XQSysInfo::Error error() const;
    
private: // From MNetworkSignalObserver
    void SignalStatusL(TInt32 aStrength, TInt8 aBars);

private: // From MBatteryObserver
    void BatteryLevelL(TUint aChargeLevel, CTelephony::TBatteryStatus aBatteryStatus);
    
private:
    XQSysInfo* q;
    CDeviceInfo* iDeviceInfo;
    
    mutable int iError;
};

#endif /*XQSYSINFOPRIVATE_H*/

// End of file
