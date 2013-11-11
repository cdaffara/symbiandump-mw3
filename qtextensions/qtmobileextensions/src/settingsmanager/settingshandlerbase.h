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

#ifndef SETTINGSHANDLERBASE_H
#define SETTINGSHANDLERBASE_H

// INCLUDES
#include <QVariant>
#include "xqsettingsmanager.h"
#include <exception>
#include <e32base.h>

// FORWARD DECLARATIONS
class XQSettingsKey;

class MSettingsHandlerObserver
{
public:
    virtual void valueChanged(const XQSettingsKey& key, XQSettingsManager::Type type) = 0;
    virtual void itemDeleted(const XQSettingsKey& key) = 0;
};

class SettingsHandlerBase
{
public:
    virtual ~SettingsHandlerBase() {};
    virtual void setObserver(MSettingsHandlerObserver* observer) = 0;
    QVariant handleReadItemValue(const XQSettingsKey& key, XQSettingsManager::Type type, TInt& error);
    bool handleWriteItemValue(const XQSettingsKey& key, const QVariant& value, TInt& error);
    virtual bool handleStartMonitoring(const XQSettingsKey& key, XQSettingsManager::Type type, MSettingsHandlerObserver& observer, TInt& error) = 0;
    virtual bool handleStopMonitoring(const XQSettingsKey& key, TInt& error) = 0;

protected:
    virtual TInt getValue(unsigned long key, TInt& value) = 0;
    virtual TInt getValue(unsigned long key, TReal& value) = 0;
    virtual void getValueL(unsigned long key, RBuf8& value) = 0;
    virtual void getValueL(unsigned long key, RBuf16& value) = 0;

    virtual TInt setValue(unsigned long key, const TInt& value) = 0;
    virtual TInt setValue(unsigned long key, const TReal& value) = 0;
    virtual TInt setValue(unsigned long key, const TDesC8& value) = 0;
    virtual TInt setValue(unsigned long key, const TDesC16& value) = 0;
};

#endif //SETTINGSHANDLERBASE_H
