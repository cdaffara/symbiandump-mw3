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

#ifndef XQSERVICEMANAGER_H
#define XQSERVICEMANAGER_H

#include <xqserviceglobal.h>
#include <xqaiwinterfacedescriptor.h>
#include <QString>
#include <QList>

class XQServiceManagerPrivate;

class XQSERVICEUTIL_EXPORT XQServiceManager : public QObject
    {
    Q_OBJECT
    public:
        XQServiceManager() ;
        virtual ~XQServiceManager() ;
        
        int startServer(const QString& service, bool embedded, int& applicationUid, quint64& processId);
        QList<XQAiwInterfaceDescriptor>  findInterfaces ( const QString &interfaceName ) const;
        QList<XQAiwInterfaceDescriptor>  findInterfaces ( const QString &serviceName, const QString &interfaceName ) const;
        QList<XQAiwInterfaceDescriptor>  findFirstInterface ( const QString &interfaceName ) const;
        QList<XQAiwInterfaceDescriptor>  findFirstInterface ( const QString &serviceName, const QString &interfaceName ) const;
        
        // Extended starter to allow passing XQAiwInterfaceDescriptor as user data
        // The user data has been attached by the XQServiceRequest
        int startServer(const QString& service, bool embedded, int& applicationUid, quint64& processId,
                        const void *userData);
        
        int latestError() const;
        bool isRunning(const XQAiwInterfaceDescriptor& implementation) const;
        
    private:   
   
    private:
        XQServiceManagerPrivate* d;
    };

#endif
