/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Service framework IPC Server declaration
*
*/

#ifndef XQ_SERVICE_FRAMEWORK_IPC_SERVER_H
#define XQ_SERVICE_FRAMEWORK_IPC_SERVER_H

#include <QtCore>
#include <xqserviceipcdefs.h>

namespace QtService {
    
    // Forward Declarations
    class ServiceFwIPCServerPrivate;
    class MServiceIPCObserver;
    
    class SFWIPCSRV_EXPORT ServiceFwIPCServer : public QObject
    {
    public:
    
        ServiceFwIPCServer(MServiceIPCObserver* aObserver,
                           QObject* aParent,
                           TServiceIPCBackends aBackend = EDefaultIPC);
    
        virtual ~ServiceFwIPCServer();
    
    // APIs provided
    public:
    
        bool listen(const QString& aServerName);
    
        void disconnect();
    
    protected:
        // data
        MServiceIPCObserver* iObserver;
    private:
        friend class ServiceFwIPCServerPrivate;
        ServiceFwIPCServerPrivate* d;
    };

}
#endif // XQ_SERVICE_FRAMEWORK_IPC_SERVER_H
