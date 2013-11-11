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

#ifndef XQCENTRALREPOSITORYSEARCHCRITERIA_H
#define XQCENTRALREPOSITORYSEARCHCRITERIA_H

#include <QVariant>
#include "settingsmanager_global.h"

class XQSETTINGSMANAGER_EXPORT XQCentralRepositorySearchCriteria
{
public:
    XQCentralRepositorySearchCriteria(long int repositoryUid,
        unsigned long int partialKey = 0, unsigned long int bitMask = 0);
    ~XQCentralRepositorySearchCriteria();
    void setValueCriteria(const QVariant& value, bool negativeCriteria = false);

    long int repositoryUid() const;
    unsigned long int partialKey() const;
    unsigned long int bitMask() const;
    const QVariant& value() const;
    bool negativeCriteria() const;

private:
    long int m_repositoryUid;
    unsigned long int m_partialKey;
    unsigned long int m_bitMask;
    QVariant m_value;
    bool m_negativeCriteria;
};

#endif //XQCENTRALREPOSITORYSEARCHCRITERIA_H
