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

#ifndef XQAPPMGR_P_H
#define XQAPPMGR_P_H

#include<QString>
#include<QList>
#include<QUrl>
#include<QUrl>
#include<QFile>
#include<QMap>

#include <xqaiwrequest.h>
#include <xqaiwinterfacedescriptor.h>
#include <xqsharablefile.h>
#include <xqapplicationmanager.h>

class XQServiceManager;
class XQAiwUtils;
class XQSettingsManager;
class XQSettingsKey;

class XQApplicationManagerPrivate : public QObject
{
    Q_OBJECT
    
public:
    XQApplicationManagerPrivate();
    virtual ~XQApplicationManagerPrivate();
    
    XQAiwRequest* create( const QString &interface, const QString &operation, bool embedded = true);
    XQAiwRequest* create( const QString &service, const QString &interface, const QString &operation, bool embedded = true);
    XQAiwRequest* create( const XQAiwInterfaceDescriptor &implementation, const QString &operation, bool embedded = true);
    XQAiwRequest* create( const QUrl &uri, const XQAiwInterfaceDescriptor *implementation, bool embedded);
    XQAiwRequest* create( const QFile &file, const XQAiwInterfaceDescriptor *implementation, bool embedded);
    XQAiwRequest* create( const XQSharableFile &file, const XQAiwInterfaceDescriptor *implementation, bool embedded);

    QList<XQAiwInterfaceDescriptor> list(const QString &interface, const QString &operation);
    QList<XQAiwInterfaceDescriptor> list(const QString &service, const QString &interface, const QString &operation);
    QList<XQAiwInterfaceDescriptor> list(const QUrl &uri);
    QList<XQAiwInterfaceDescriptor> list(const QFile &file);
    QList<XQAiwInterfaceDescriptor> list(const XQSharableFile &file);

    bool hasCustomHandler(const QUrl &uri) const;
    
    int lastError() const;
    bool isRunning(const XQAiwInterfaceDescriptor& implementation) const;
    bool getDrmAttributes(const QFile &file, const QList<int> &attributeNames, QVariantList &attributeValues);
    bool getDrmAttributes(const XQSharableFile &file, const QList<int> &attributeNames, QVariantList &attributeValues);
    int status(const XQAiwInterfaceDescriptor& implementation);
    
    bool startNotifications(XQAiwInterfaceDescriptor& serviceImplDescriptor);
    bool stopNotifications(XQAiwInterfaceDescriptor& serviceImplDescriptor);

public slots:
    void valueChanged(const XQSettingsKey& key, const QVariant& value);
    void itemDeleted(const XQSettingsKey& key);

private:
    
    QList<XQAiwInterfaceDescriptor> listMimeHandlers(const QFile &file);
    QList<XQAiwInterfaceDescriptor> listMimeHandlers(const XQSharableFile &file);
    bool getAppDescriptor(const QUrl &uri, XQAiwInterfaceDescriptor *descriptor);
    QList<XQAiwInterfaceDescriptor> listFileHandlers(const QList<XQAiwInterfaceDescriptor> &mimeHandlers);
    
    XQSettingsManager *settingsManager();
    
private:
    XQApplicationManager *v_ptr;
    
    XQServiceManager * serviceMgr;
    XQAiwUtils * aiwUtilities;
    
    XQSettingsManager *settingsManagerInstance;
    
    QMap<quint32, XQAiwInterfaceDescriptor> descriptorsMap;
    
    friend class XQApplicationManager;
};

#endif
