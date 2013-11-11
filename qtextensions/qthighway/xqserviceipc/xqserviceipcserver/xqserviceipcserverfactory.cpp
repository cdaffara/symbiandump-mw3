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
* Description:  Factory class to construct the server backend
*
*/

#include "xqservicelog.h"

#include "xqserviceipcserverfactory.h"
#ifdef __SYMBIAN32__
#include "xqserviceipcserver_symbianserver.h"
#include "xqserviceipcserver_apasymbianserver.h"
#endif // __SYMBIAN32__
namespace QtService
{

/*!
    \class ServiceFwIPCServerFactory
    \brief Factory class to construct IPC server backend
*/

/*!
    Destructor.
*/
ServiceFwIPCServerFactory::~ServiceFwIPCServerFactory()
{
    XQSERVICE_DEBUG_PRINT("ServiceFwIPCServerFactory::~ServiceFwIPCServerFactory");
}

/*!
    Constructor.
*/
ServiceFwIPCServerFactory::ServiceFwIPCServerFactory()
{
    XQSERVICE_DEBUG_PRINT("ServiceFwIPCServerFactory::ServiceFwIPCServerFactory");
}

/*!
    Construct a backend.
    \param aBackend Backend to construct.
    \param aParent Parent to this QObject.
*/
ServiceFwIPCServerPrivate* ServiceFwIPCServerFactory::createBackend(QObject* aParent,
                                                                    TServiceIPCBackends aBackend)
{
    XQSERVICE_DEBUG_PRINT("ServiceFwIPCServerFactory::createBackend");
    XQSERVICE_DEBUG_PRINT("aBackend: %d", aBackend);
    ServiceFwIPCServerPrivate* backend(NULL);
#ifdef __SYMBIAN32__
    // Symbian server is default
    if (aBackend == ESymbianServer || aBackend == EDefaultIPC) {
        TRAP_IGNORE( backend = CServiceSymbianServer::NewL(); );
	}
    if (aBackend == ESymbianApaServer) {
        TRAP_IGNORE( backend = CApaSymbianServer::NewL(); );
	}
#endif
    return backend;
    }
}
// END OF FILE
