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

#ifndef XQSETTINGSMANAGER_P_H
#define XQSETTINGSMANAGER_P_H

// INCLUDES
#include "xqsettingsmanager.h"
#include <QHash>

#include "xqsettingskey.h"
#include "settingshandlerbase.h"

// FORWARD DECLARATIONS
class QVariant;
class CCentralRepositoryHandler;
class CPublishAndSubscribeHandler;

// CLASS DECLARATION
class XQSettingsManagerPrivate
    : public CBase, public MSettingsHandlerObserver
{
public:
    XQSettingsManagerPrivate(XQSettingsManager* settingsmanager);
    ~XQSettingsManagerPrivate();

    QVariant readItemValue(const XQSettingsKey& key, XQSettingsManager::Type type);
    bool writeItemValue(const XQSettingsKey& key, const QVariant& value);
    bool startMonitoring(const XQSettingsKey& key, XQSettingsManager::Type type);
    bool stopMonitoring(const XQSettingsKey& key);

    XQSettingsManager::Error error() const;

    CCentralRepositoryHandler* cenRepHandlerInstance(long int repositoryUid);
    CPublishAndSubscribeHandler* pubSubHandlerInstance(long int categoryUid);

private:
    SettingsHandlerBase* handlerInstance(const XQSettingsKey& key);

protected:  //MSettingsHandlerObserver
    void valueChanged(const XQSettingsKey& key, XQSettingsManager::Type type);
    void itemDeleted(const XQSettingsKey& key);

private:
    friend class XQCentralRepositoryUtilsPrivate;
    friend class XQPublishAndSubscribeUtilsPrivate;
    XQSettingsManager* q;
    mutable int iError;
    QHash<long int, CCentralRepositoryHandler*> m_centralRepositoryHandlers;
    QHash<long int, CPublishAndSubscribeHandler*> m_publishAndSubscribeHandlers;
};

#endif /*XQSETTINGSMANAGER_P_H*/

// End of file
