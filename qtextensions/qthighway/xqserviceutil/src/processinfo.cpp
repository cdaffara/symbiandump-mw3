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

#include "processinfo.h"

#include "xqservicelog.h"

#include <QCoreApplication>

#define TIMER_DELAY 3000000 // 3000000 microseconds == 3s

// static data initialization

ProcessInfoDestroyer *ProcessInfoDestroyer::instance = 0;
CProcessInfo::ProcessInfoMap CProcessInfo::iProcessInfoMap;

// ProcessInfoMap implementation

CProcessInfo::ProcessInfoMap::~ProcessInfoMap()
{
    CProcessInfo::Destroy();
    ProcessInfoDestroyer::disable();
}

// CProcessInfo implementation

CProcessInfo::CProcessInfo(const TUid& appUid):
        CActive(CActive::EPriorityStandard), 
        iAppUid(appUid)
{
    XQSERVICE_DEBUG_PRINT("CProcessInfo::CProcessInfo");
    
    CActiveScheduler::Add(this);
}

CProcessInfo::~CProcessInfo()
{
    XQSERVICE_DEBUG_PRINT("CProcessInfo::~CProcessInfo");
    
    // Cancel asynch request, normally it should be done in DoCancel() 
    // but we dont wont to cancel request when we cancel active object
    User::CancelMiscNotifier(iStatus);
    
    Cancel();
}

void CProcessInfo::Destroy()
{
    foreach (CProcessInfo* info, iProcessInfoMap.map.values())
        delete info;
    iProcessInfoMap.map.clear();
}

void CProcessInfo::AddProcessL(const TUid& appUid, RProcess& appProcess)
{
    XQSERVICE_DEBUG_PRINT("CProcessInfo::AddProcessL");
    
    CProcessInfo* self = new(ELeave) CProcessInfo(appUid);
    CleanupStack::PushL(self);
    self->ConstructL(appProcess);
    CleanupStack::Pop(self);
}

bool CProcessInfo::EnsureProcessCanStartL(const TUid& appUid)
{
    XQSERVICE_DEBUG_PRINT("CProcessInfo::EnsureProcessCanStartL");
  
    bool ret = true;
    
    CProcessInfo* previousProcess = iProcessInfoMap.map[appUid.iUid];
    if (previousProcess) {
        // Timer is for ensure that wait will end. 
        // There is possibility that destroying process notification could be lost.
        RTimer securityTimer;
        securityTimer.CreateLocal();
        CleanupClosePushL(securityTimer);
        
        TRequestStatus timerStatus;
        securityTimer.After(timerStatus, TIMER_DELAY);
        User::WaitForRequest(previousProcess->iStatus, timerStatus);
        
        if (previousProcess->iStatus == KRequestPending)
            ret = false;
        
        CleanupStack::PopAndDestroy();
        delete previousProcess;
        iProcessInfoMap.map.remove(appUid.iUid);
    }
    return ret;
}

void CProcessInfo::RunL()
{
    XQSERVICE_DEBUG_PRINT("CProcessInfo::RunL");
    
    iProcessInfoMap.map.remove(iAppUid.iUid);
    delete this;
}

void CProcessInfo::ConstructL(RProcess& appProcess)
{
    XQSERVICE_DEBUG_PRINT("CProcessInfo::ConstructL");
    
    SetActive();
    
    EnsureProcessCanStartL(iAppUid);
    iProcessInfoMap.map.insert(iAppUid.iUid, this);
    appProcess.NotifyDestruction(iStatus);
    
    ProcessInfoDestroyer::enable();
}

void CProcessInfo::DoCancel()
{
    XQSERVICE_DEBUG_PRINT("CProcessInfo::DoCancel");
    
    // Cancel asynch request, normally it should be done in DoCancel() 
    // but we dont wont to cancel request when we cancel active object.
    // Cancel asynch request is in ~CProcessInfo().
}

// ProcessInfoDestroyer implementation

void ProcessInfoDestroyer::enable()
{
    if (!instance)
        instance = new ProcessInfoDestroyer;
}

void ProcessInfoDestroyer::disable()
{
    if (instance) {
        delete instance;
        instance = 0;
    }
}

ProcessInfoDestroyer::ProcessInfoDestroyer()
{
    connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(destroy()));
}

void ProcessInfoDestroyer::destroy()
{
    CProcessInfo::Destroy();
}

