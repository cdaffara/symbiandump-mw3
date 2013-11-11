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

#include "pubsubmonitor.h"
#include "settingshandlerbase.h"

CPubSubMonitor::CPubSubMonitor(const XQSettingsKey& aKey, XQSettingsManager::Type type, MSettingsHandlerObserver& aObserver) 
    : CActive(EPriorityStandard), iKey(aKey), m_type(type), iObserver(aObserver)
    {
    CActiveScheduler::Add(this);
    }

CPubSubMonitor::~CPubSubMonitor()
    {
    Cancel();
    }

TInt CPubSubMonitor::StartMonitoring()
    {
    TUid uid;
    uid.iUid = iKey.uid();
    TInt err = iProperty.Attach(uid, iKey.key());
    if (err == KErrNone)
        {
        iProperty.Subscribe(iStatus);
        SetActive();
        }
    return err;
    }

void CPubSubMonitor::DoCancel()
    {
    iProperty.Cancel();
    }

void CPubSubMonitor::RunL()
    {
    if (iStatus == KErrNone)
        {
        iProperty.Subscribe(iStatus);
        SetActive();
        iObserver.valueChanged(iKey, m_type);
        }
    else if (iStatus == KErrNotFound)
        {
        iObserver.itemDeleted(iKey);
        }
    }
