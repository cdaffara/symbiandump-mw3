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

#ifndef NETWORKSIGNALMONITOR_H
#define NETWORKSIGNALMONITOR_H

#include <exception>
#include <etel3rdparty.h> 
 
class MNetworkSignalObserver
    {
    public:
        virtual void SignalStatusL( TInt32 aStrength, TInt8 aBars ) = 0;
    };
      
class CNetworkSignalMonitor : public CActive
    { 
public:
    CNetworkSignalMonitor( MNetworkSignalObserver& aObserver );
    void ConstructL();
    ~CNetworkSignalMonitor();
    
private:
    void RunL();
    void DoCancel();
    
private:
    MNetworkSignalObserver& iObserver;
    CTelephony* iTelephony;
    CTelephony::TSignalStrengthV1 iSigStrengthV1;
    CTelephony::TSignalStrengthV1Pckg iSigStrengthV1Pckg;
    };

#endif /* NETWORKCSIGNALMONITOR_H */
