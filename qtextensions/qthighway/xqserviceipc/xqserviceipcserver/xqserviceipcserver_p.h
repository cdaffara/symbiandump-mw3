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
* Description:  Service Framework IPC Private implementation
*
*/

#ifndef XQ_SERVICE_FRAMEWORK_IPC_PRIVATE_SERVER_H
#define XQ_SERVICE_FRAMEWORK_IPC_PRIVATE_SERVER_H

#include <QtCore>
#include "xqserviceipcserver_p.h"
#include "xqserviceipcserver.h"

namespace QtService {
    
    class ServiceFwIPCServer;
    class ServiceIPCSession;
    class MServiceIPCObserver;
    
    /**
     *  Private implementation interface for service framework server
     *  This class is the abstract interface for all server backends
     */
    class ServiceFwIPCServerPrivate
    {
    public:
        /**
        * Virtual destructor
        */
        virtual ~ServiceFwIPCServerPrivate() {};

    public:
    
        /**
         * Start listening for new service requests
         * @param aServerName name of the server
         * @return true if listen was successful
         */
        virtual bool listen(const QString& aServerName) = 0;
    
        /**
         * Shutdown the server and stop serving clients 
         * @return void
         */
        virtual void disconnect() = 0;
    
    protected:
    
        /**
         * Get the server observer
         * @return MServiceIPCObserver* observer to this server
         */
        inline MServiceIPCObserver* Observer()
        {
            return q->iObserver;
        };
    
    protected:
        QList<ServiceIPCSession*> iSessions;
    
    private:
        friend class ServiceFwIPCServer;
        ServiceFwIPCServer* q;
    };

}
#endif // XQ_SERVICE_FRAMEWORK_IPC_PRIVATE_SERVER_H
