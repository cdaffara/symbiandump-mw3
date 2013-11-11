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
* Description:  Service Framework IPC Factory
*
*/

#include "xqservicelog.h"

#include "xqserviceipcfactory.h"
#include "xqserviceipc_p.h"
#ifdef __SYMBIAN32__
#include "xqserviceipc_symbian.h"
#include "xqserviceipc_apasymbian.h"
#endif // __SYMBIAN32__
namespace QtService
{

/*!
    \class ServiceFwIPCFactory
    \brief Factory class for constructing the IPC backend
*/

/*!
    Constructor.
*/
ServiceFwIPCFactory::ServiceFwIPCFactory()
{
}

/*!
    Destructor.
*/
ServiceFwIPCFactory::~ServiceFwIPCFactory()
{
}

/*!
    Check to see if the particular IPC type is supported.
    \param aIPCType Type of the IPC.
    \return true If the IPC type is supported, false otherwise.
*/
bool ServiceFwIPCFactory::isIPCTypeSupported(TServiceIPCBackends aIPCType)
{
    XQSERVICE_DEBUG_PRINT("ServiceFwIPCFactory::isIPCTypeSupported");
    bool supported(false);
#ifdef __SYMBIAN32__
    if (aIPCType == ESymbianServer) {
        supported = true;
	}
    if (aIPCType == ESymbianApaServer) {
        supported = true;
    }    
#endif

    return supported;
}

/*!
    Create an instance of the Service IPC backend.
    \param aBackend Type of backend to create.
    \return ServiceFwIPCPrivate instance.
*/
ServiceFwIPCPrivate* ServiceFwIPCFactory::createBackend(TServiceIPCBackends aBackend)
{
    XQSERVICE_DEBUG_PRINT("ServiceFwIPCFactory::createBackend");
    ServiceFwIPCPrivate* backend(NULL);
#ifdef __SYMBIAN32__
    // Symbian server is default
    if (aBackend == ESymbianServer || aBackend == EDefaultIPC) {
        TRAP_IGNORE( backend = CServiceSymbianIPC::NewL() );
    }
    if (aBackend == ESymbianApaServer) {
        TRAP_IGNORE( backend = CApaSymbianIPC::NewL() );
    }
#endif
    return backend;
    }
}
// END OF FILE
