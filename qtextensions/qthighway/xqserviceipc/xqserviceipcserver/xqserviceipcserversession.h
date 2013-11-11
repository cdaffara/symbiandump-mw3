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
* Description:  Session class 
*
*/

#ifndef XQ_SERVICE_IPC_SESSION_H
#define XQ_SERVICE_IPC_SESSION_H

#include <QtCore>

namespace QtService {
    
    class MServiceIPCObserver;
    class ServiceIPCRequest;
    
    class ServiceIPCSession : public QObject
    {
    Q_OBJECT
    public:
    
        ServiceIPCSession(MServiceIPCObserver* aObserver);
    
        virtual ~ServiceIPCSession();
    
        virtual bool write(const QByteArray& aData) = 0;
    
        virtual bool completeRequest() = 0;
    
        virtual void close() = 0;
    
    protected:
        // data
        bool iPendingRequest;
        MServiceIPCObserver* iObserver;
        ServiceIPCRequest* iCurRequest;
    };

}
#endif // XQ_SERVICE_IPC_SESSION_H
