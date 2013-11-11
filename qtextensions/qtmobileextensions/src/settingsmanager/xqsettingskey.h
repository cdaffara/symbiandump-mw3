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

#ifndef XQSETTINGSITEM_H
#define XQSETTINGSITEM_H

// INCLUDES
#include "settingsmanager_global.h"

// CLASS DECLARATION
class XQSETTINGSMANAGER_EXPORT XQSettingsKey
{
public:
    enum Target
    {
        TargetCentralRepository = 0,
        TargetPublishAndSubscribe
    };

    XQSettingsKey(XQSettingsKey::Target target, long int uid, unsigned long int key);
    ~XQSettingsKey();

    XQSettingsKey::Target target() const;
    long int uid() const;
    unsigned long int key() const;

protected:
    XQSettingsKey::Target m_target;
    long int m_uid;
    unsigned long int m_key;
};

class XQSETTINGSMANAGER_EXPORT XQCentralRepositorySettingsKey : public XQSettingsKey
{
public:
    XQCentralRepositorySettingsKey(long int repositoryUid, unsigned long int key);
    ~XQCentralRepositorySettingsKey();
};

class XQSETTINGSMANAGER_EXPORT XQPublishAndSubscribeSettingsKey : public XQSettingsKey
{
public:
    XQPublishAndSubscribeSettingsKey(long int categoryUid, unsigned long int key);
    ~XQPublishAndSubscribeSettingsKey();
};

#endif //XQSETTINGSITEM_H
