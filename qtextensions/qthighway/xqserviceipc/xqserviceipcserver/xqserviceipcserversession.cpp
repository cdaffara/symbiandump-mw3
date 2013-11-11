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
* Description:  Session class for server side IPC communication
*
*/

#include "xqservicelog.h"

#include "xqserviceipcobserver.h"
#include "xqserviceipcserversession.h"

namespace QtService
{
/*!
    \class ServiceIPCSession
    \brief Session class to hold the state of each connected client
*/

/*!
    Constructor.
    \param aObserver Observer to the session.
*/
ServiceIPCSession::ServiceIPCSession(MServiceIPCObserver* aObserver) :
    iPendingRequest(false), iObserver(aObserver), iCurRequest(NULL)
{
    XQSERVICE_DEBUG_PRINT("ServiceIPCSession::ServiceIPCSession");
}

/*!
    Destructor.
*/
ServiceIPCSession::~ServiceIPCSession()
{
    XQSERVICE_DEBUG_PRINT("ServiceIPCSession::~ServiceIPCSession");
}

/*!
    \fn QtService::ServiceIPCSession::write( const QByteArray& aData )
 
    Write some data in response to a request.
    \param aData Some data to write as response.
    \return bool If write was successful.
*/

/*!
    \fn QtService::ServiceIPCSession::completeRequest()
 
    Complete a request.
    \return bool if request completed .
*/

/*!
    \fn QtService::ServiceIPCSession::close()
 
    Close a session and gracefully shutdown.
*/
}
// END OF FILE
