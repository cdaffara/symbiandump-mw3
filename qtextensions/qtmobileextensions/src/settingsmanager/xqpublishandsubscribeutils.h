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

#ifndef XQCENTRALREPOSITORYUTILS_H
#define XQCENTRALREPOSITORYUTILS_H

#include <QObject>

#include "xqsettingsmanager.h"
#include "settingsmanager_global.h"

class XQPublishAndSubscribeUtilsPrivate;
class XQPublishAndSubscribeSettingsKey;
class XQPublishAndSubscribeSecurityPolicy;

class XQSETTINGSMANAGER_EXPORT XQPublishAndSubscribeUtils : public QObject
{
public:
    XQPublishAndSubscribeUtils(XQSettingsManager& settingsManager, QObject* parent = 0);
    ~XQPublishAndSubscribeUtils();
    bool defineProperty(const XQPublishAndSubscribeSettingsKey& key, XQSettingsManager::Type type);
    bool defineProperty(const XQPublishAndSubscribeSettingsKey& key, XQSettingsManager::Type type,
        const XQPublishAndSubscribeSecurityPolicy& readPolicy, const XQPublishAndSubscribeSecurityPolicy& writePolicy);
    bool deleteProperty(const XQPublishAndSubscribeSettingsKey& key);

private:
    XQPublishAndSubscribeUtilsPrivate* d;
};

#endif //XQCENTRALREPOSITORYUTILS_H
