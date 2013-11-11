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

#ifndef CENTRALREPOSITORYHANDLER_H
#define CENTRALREPOSITORYHANDLER_H

#include <exception>
#include <e32base.h>
#include <centralrepository.h>

#include "settingshandlerbase.h"
#include "cenrepmonitor.h"

class XQSettingsKey;
class CRepository;

class CCentralRepositoryHandler : public CBase, public SettingsHandlerBase
{
public:
    static CCentralRepositoryHandler* NewL(TUid aUid);
    ~CCentralRepositoryHandler();

    TInt createKey(unsigned long key, const TInt& value);
    TInt createKey(unsigned long key, const TReal& value);
    TInt createKey(unsigned long key, const TDesC8& value);
    TInt createKey(unsigned long key, const TDesC16& value);

    TInt deleteKey(unsigned long key);

    TInt resetKey(unsigned long key);
    TInt resetRepository();

    TInt findKeyL(unsigned long partialKey, TUint32 mask, RArray<TUint32>& aFoundKeys);
    TInt findKeyL(unsigned long partialKey, TUint32 mask, const TInt& value, bool negative, RArray<TUint32>& aFoundKeys);
    TInt findKeyL(unsigned long partialKey, TUint32 mask, const TReal& value, bool negative, RArray<TUint32>& aFoundKeys);
    TInt findKeyL(unsigned long partialKey, TUint32 mask, const TDesC8& value, bool negative, RArray<TUint32>& aFoundKeys);
    TInt findKeyL(unsigned long partialKey, TUint32 mask, const TDesC16& value, bool negative, RArray<TUint32>& aFoundKeys);

    TInt startTransaction(CRepository::TTransactionMode transactionMode);
    TInt commitTransaction();
    void cancelTransaction();
    void failTransaction();
    TInt transactionState() const;

private:
    CCentralRepositoryHandler(TUid aUid);
    void ConstructL();

protected:  //From SettingsHandlerBase
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
    TUid m_uid;
    CRepository* iRepository;
    MSettingsHandlerObserver* m_observer;
    QHash<unsigned long int, CCenRepMonitor*> m_monitors;
};

#endif //CENTRALREPOSITORYHANDLER_H
