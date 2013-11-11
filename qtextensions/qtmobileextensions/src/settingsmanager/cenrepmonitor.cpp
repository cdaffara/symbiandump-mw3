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

#include "cenrepmonitor.h"
#include <centralrepository.h>

#include "xqsettingskey.h"
#include "settingshandlerbase.h"

CCenRepMonitor::CCenRepMonitor(CRepository& aRepository, const XQSettingsKey& aKey, XQSettingsManager::Type type, MSettingsHandlerObserver& aObserver)
    : CActive(EPriorityStandard), iRepository(aRepository), iKey(aKey), m_type(type), iObserver(aObserver)
    {
    CActiveScheduler::Add(this);
    }

CCenRepMonitor::~CCenRepMonitor()
    {
    Cancel();
    }

TInt CCenRepMonitor::StartMonitoring()
    {
    TInt ret = iRepository.NotifyRequest(iKey.key(), iStatus);
    SetActive();
    return ret;
    }

void CCenRepMonitor::DoCancel()
    {
    iRepository.NotifyCancel(iKey.key());
    }

void CCenRepMonitor::RunL()
    {
    TUint32 meta;
    //This trick is used to check whether the key no longer exists.
    TInt ret = iRepository.GetMeta(iKey.key(), meta);
    if (ret == KErrNone)
        {
        iRepository.NotifyRequest(iKey.key(), iStatus);
        SetActive();
        iObserver.valueChanged(iKey, m_type);
        }
    else if (ret == KErrNotFound)
        {
        iObserver.itemDeleted(iKey);
        }
    }
