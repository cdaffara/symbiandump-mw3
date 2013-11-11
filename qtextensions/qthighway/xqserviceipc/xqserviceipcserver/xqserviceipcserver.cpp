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
* Description:  Service Framework IPC Server
*
*/

#include "xqservicelog.h"

#include "xqserviceipcserver.h"
#include "xqserviceipcserver_p.h"
#include "xqserviceipcserverfactory.h"
#include "xqserviceipcobserver.h"

namespace QtService
{
/*!
    \class ServiceFwIPCServer
    \brief Service Framework IPC Server public class
*/

/*!
    Constructor.
    \param aObserver Engine observer to call back to handle new requests.
    \param aParent QObject's parent.
    \param aBackend Backend to instantiate.
*/
ServiceFwIPCServer::ServiceFwIPCServer( MServiceIPCObserver* aObserver, 
                                        QObject* aParent, 
                                        TServiceIPCBackends aBackend ) :
                                        QObject(aParent ),
                                        iObserver( aObserver )
{
    XQSERVICE_DEBUG_PRINT("ServiceFwIPCServer::ServiceFwIPCServer");
    XQSERVICE_DEBUG_PRINT("aBackend: %d", aBackend);
    d = ServiceFwIPCServerFactory::createBackend( aParent,aBackend );
    d->q = this;
}

/*!
    Destructor.
*/
ServiceFwIPCServer::~ServiceFwIPCServer()
{
    XQSERVICE_DEBUG_PRINT("ServiceFwIPCServer::~ServiceFwIPCServer");
    delete d;
}

/*!
    Start listening for new service requests.
    \param aServerName Name of the server.
    \return true if connected.
*/
bool ServiceFwIPCServer::listen(const QString& aServerName)
{
    XQSERVICE_DEBUG_PRINT("aServerName: %s", qPrintable(aServerName));
    XQSERVICE_DEBUG_PRINT("ServiceFwIPCServer::listen");
    return d->listen(aServerName);
}

/*!
    Shutdown the server and stop serving clients.
*/
void ServiceFwIPCServer::disconnect()
{
    XQSERVICE_DEBUG_PRINT("ServiceFwIPCServer::disconnect");
    d->disconnect();
}
}
// END OF FILE
