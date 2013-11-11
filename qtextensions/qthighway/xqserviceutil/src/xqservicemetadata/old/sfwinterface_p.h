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

#ifndef XQSFWINTERFACE_H_
#define XQSFWINTERFACE_H_

#include <QString>
#include <QList>
#include <QStringList>
#include "xqqserviceglobal.h"

QT_BEGIN_NAMESPACE

class Q_SFW_EXPORT SFWInterface
{
public:

    SFWInterface(const QString& anInterfaceName);
    SFWInterface(const SFWInterface& other);
    virtual ~SFWInterface();

    SFWInterface &operator=(const SFWInterface&other);

    QString name() const;
    QString service() const;
    QString description() const;
    QStringList capabilities() const;
    QString version() const;

    void setDescription(const QString& aDescription);
    void setCapabilities(const QList<QString>& capabilities);
    void setVersion(const QString& version);
    void setVersion(int maj = 1, int min = 0);
    void setService(const QString &service);

private:
    QString interfaceName;
    QString interfaceDescription;
    QStringList interfaceCapabilities;
    QString interfaceVersion;
    QString serviceName;
};

QT_END_NAMESPACE

#endif /*XQSFWINTERFACE_H_*/
