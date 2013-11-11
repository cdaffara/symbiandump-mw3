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

#ifndef XQCENTRALREPOSITORYUTILS_P_H
#define XQCENTRALREPOSITORYUTILS_P_H

#include "xqsettingsmanager.h"
#include "xqsettingskey.h"
#include "xqcentralrepositoryutils.h"

class XQSettingsManagerPrivate;
class XQCentralRepositorySearchCriteria;

class XQCentralRepositoryUtilsPrivate
{
public:
    XQCentralRepositoryUtilsPrivate(XQSettingsManager& settingsManager);
    ~XQCentralRepositoryUtilsPrivate();
    bool createKey(const XQCentralRepositorySettingsKey& key, const QVariant& value);
    bool deleteKey(const XQCentralRepositorySettingsKey& key);
    bool resetKey(const XQCentralRepositorySettingsKey& key);
    bool resetRepository(long int repositoryUid);
    QList<XQCentralRepositorySettingsKey> findKeys(const XQCentralRepositorySearchCriteria& criteria);

    bool startTransaction(long int repositoryUid, XQCentralRepositoryUtils::TransactionMode transactionMode);
    bool commitTransaction(long int repositoryUid);
    void cancelTransaction(long int repositoryUid);
    void failTransaction(long int repositoryUid);
    XQCentralRepositoryUtils::TransactionState transactionState(long int repositoryUid) const;

private:
    XQSettingsManagerPrivate& m_settingsManagerPrivate;
};

#endif //XQCENTRALREPOSITORYUTILS_P_H
