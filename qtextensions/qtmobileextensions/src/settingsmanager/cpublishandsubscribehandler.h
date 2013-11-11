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

#ifndef PUBLISHANDSUBSCRIBEHANDLER_H
#define PUBLISHANDSUBSCRIBEHANDLER_H

#include <exception>
#include <e32base.h>

#include "settingshandlerbase.h"
#include "pubsubmonitor.h"
#include "xqpublishandsubscribesecuritypolicy.h"

class XQSettingsKey;

class CPublishAndSubscribeHandler : public CBase, public SettingsHandlerBase
{
public:
    static CPublishAndSubscribeHandler* NewL(TUid aUid);
    ~CPublishAndSubscribeHandler();

    TInt defineProperty(unsigned long key, XQSettingsManager::Type type);
    TInt defineProperty(unsigned long key, XQSettingsManager::Type type,
        const XQPublishAndSubscribeSecurityPolicy& readPolicy, const XQPublishAndSubscribeSecurityPolicy& writePolicy);
    TInt deleteProperty(unsigned long key);

private:
    CPublishAndSubscribeHandler(TUid aUid);
    void ConstructL();

protected:
    void setObserver(MSettingsHandlerObserver* observer);
    bool handleStartMonitoring(const XQSettingsKey& key, XQSettingsManager::Type type, MSettingsHandlerObserver& observer, TInt& error);
    bool handleStopMonitoring(const XQSettingsKey& key, TInt& error);

    TInt getValue(unsigned long key, TInt& value);
    TInt getValue(unsigned long key, TReal& value);
    void getValueL(unsigned long key, RBuf8& value);
    void getValueL(unsigned long key, RBuf16& value);

    TInt setValue(unsigned long key, const TInt& value);
    TInt setValue(unsigned long key, const TReal& value);
    TInt setValue(unsigned long key, const TDesC8& value);
    TInt setValue(unsigned long key, const TDesC16& value);

private:
    TSecurityPolicy symbianPolicy(const XQPublishAndSubscribeSecurityPolicy& policy);
    TCapability symbianCapability(const XQPublishAndSubscribeSecurityPolicy::Capability& capability);
private:
    MSettingsHandlerObserver* m_observer;
    TUid m_uid;
    QHash<unsigned long int, CPubSubMonitor*> m_monitors;
};

#endif //PUBLISHANDSUBSCRIBEHANDLER_H
