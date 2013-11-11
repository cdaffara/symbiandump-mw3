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

#include <batterymonitor.h>

CBatteryMonitor::~CBatteryMonitor()
{
    Cancel();
    delete iTelephony;
}
 
void CBatteryMonitor::ConstructL()
{
    iTelephony = CTelephony::NewL();
    iTelephony->GetBatteryInfo( iStatus, iBatteryInfoV1Pckg );
    SetActive();
}
 
CBatteryMonitor::CBatteryMonitor( MBatteryObserver& aObserver )
    : CActive( EPriorityStandard ), iObserver( aObserver ), iBatteryInfoV1Pckg( iBatteryInfoV1 )
{
    CActiveScheduler::Add(this);
}
 
void CBatteryMonitor::RunL()
{
    iObserver.BatteryLevelL( iBatteryInfoV1.iChargeLevel, iBatteryInfoV1.iStatus );
    iTelephony->NotifyChange( iStatus, CTelephony::EBatteryInfoChange, iBatteryInfoV1Pckg );   
    SetActive();
}
 
void CBatteryMonitor::DoCancel()
{
    iTelephony->CancelAsync( CTelephony::EBatteryInfoChangeCancel );
}
