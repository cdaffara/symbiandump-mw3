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

#ifndef PUBSUBMONITOR_H
#define PUBSUBMONITOR_H

#include <exception>
#include <e32base.h>
#include <e32property.h>

#include "xqsettingskey.h"
#include "xqsettingsmanager.h"

class MSettingsHandlerObserver;

class CPubSubMonitor : public CActive
    { 
    public:
        CPubSubMonitor(const XQSettingsKey& aKey, XQSettingsManager::Type type, MSettingsHandlerObserver& aObserver);
        ~CPubSubMonitor();
        TInt StartMonitoring();
        
    private:
        void RunL();
        void DoCancel();
              
    private:
        RProperty iProperty;
        XQSettingsKey iKey;
        XQSettingsManager::Type m_type;
        MSettingsHandlerObserver& iObserver;
    };

#endif //PUBSUBMONITOR_H
