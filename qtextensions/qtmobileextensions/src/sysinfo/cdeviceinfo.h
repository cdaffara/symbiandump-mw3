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

#ifndef CIMEIREADER_H
#define CIMEIREADER_H

#include <exception>
#include <e32base.h>
#include <etel3rdparty.h>

class CDeviceInfo : public CActive
{
public:
    enum States
    {
        EModel, 
        EManufacturer, 
        ESerialnumber,
    };
    
    static CDeviceInfo* NewL();
    static CDeviceInfo* NewLC();
    ~CDeviceInfo();

    TBuf<CTelephony::KPhoneSerialNumberSize> imei();
    TBuf<CTelephony::KPhoneModelIdSize> model();
    TBuf<CTelephony::KPhoneManufacturerIdSize> manufacturer();
    TBuf<CTelephony::KIMSISize> imsi();
    TUint batteryLevel();
    TInt32 signalStrength();
    
protected:  
    void DoCancel();
    void RunL();
    
private:
    CDeviceInfo();
    void ConstructL();

private:
    CTelephony*                 iTelephony;
        
    CTelephony::TBatteryInfoV1Pckg iBatteryInfoV1Pkg; 
    CTelephony::TBatteryInfoV1 iBatteryInfoV1;
    
    CTelephony::TPhoneIdV1      iPhoneIdV1;    
    CTelephony::TPhoneIdV1Pckg  iPhoneIdV1Pkg;
        
    CTelephony::TSignalStrengthV1Pckg iSignalStrengthV1Pckg; 
    CTelephony::TSignalStrengthV1 iSignalStrengthV1;
    
    CTelephony::TSubscriberIdV1Pckg iSubscriberIdV1Pckg;
    CTelephony::TSubscriberIdV1 iSubscriberIdV1; 
    
    CActiveSchedulerWait *iWait;
    
    bool phoneIdFetched;
};

#endif /* CIMEIREADER_H */
