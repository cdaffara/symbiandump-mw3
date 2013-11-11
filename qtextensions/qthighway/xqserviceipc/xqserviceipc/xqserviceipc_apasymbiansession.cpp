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
* Description:  
*
*/

#include "xqservicelog.h"

#include "xqserviceipc_apasymbiansession.h"

#include "xqserviceipcconst.h"
#include <QString>

#include <xqservicemanager.h>

#include <QString>

namespace QtService
{
// ============================== MEMBER FUNCTIONS ============================

/*!
    \class RApaIPCSession
    \brief Symbian class encapsulating RMessage2 interface
*/

/*!
    Send a message.
    \param aFunction Function code.
    \return message Completion code.
*/
TInt RApaIPCSession::SendReceiveL(TInt aFunction) const
{
    XQSERVICE_DEBUG_PRINT("RApaIPCSession::SendReceiveL(1)");
    return User::LeaveIfError(RSessionBase::SendReceive(aFunction));
}

/*!
    Send a message.
    \param aFunction Function code.
    \param aArgs Parameter to server.
    \return Message completion code.
*/
TInt RApaIPCSession::SendReceiveL(TInt aFunction, const TIpcArgs& aArgs) const
{
    XQSERVICE_DEBUG_PRINT("RApaIPCSession::SendReceiveL(2)");
    return User::LeaveIfError(RSessionBase::SendReceive(aFunction,aArgs));
}

/*!
    Send message asynchronously.
    \param aFunction Function code.
    \param aStatus The request status object used to contain the 
           completion status of the request.
*/
EXPORT_C void RApaIPCSession::SendReceive(TInt aFunction,
                                          TRequestStatus& aStatus) const
{
    XQSERVICE_DEBUG_PRINT("RApaIPCSession::SendReceive(1)");
    RSessionBase::SendReceive(aFunction,aStatus);
    XQSERVICE_DEBUG_PRINT("RApaIPCSession::SendReceive(1) done");
}

/*!
    Send message asynchronously.
    \param aFunction Function code.
    \param aArgs Parameter to server.
    \param aStatus The request status object used to contain the 
           completion status of the request.
*/
void RApaIPCSession::SendReceive(TInt aFunction,
                                       const TIpcArgs& aArgs,
                                       TRequestStatus& aStatus) const
{
    XQSERVICE_DEBUG_PRINT("RApaIPCSession::SendReceive(2)");
    RSessionBase::SendReceive(aFunction,aArgs,aStatus);
    XQSERVICE_DEBUG_PRINT("RApaIPCSession::SendReceive(2) done");
}

/*!
    Connect to server.
    \param aServer Server name.
    \param aVersion Version of the server.
    \return KErrNone on success, otherwise system error code.
*/
TInt RApaIPCSession::Connect(const TDesC& aServer, 
                                 const TVersion& aVersion)
{
    QString server = QString::fromUtf16(aServer.Ptr(), aServer.Length());
    XQSERVICE_DEBUG_PRINT("RApaIPCSession::Connect %s", qPrintable(server));
    iVersion = aVersion;
    return CreateSession(aServer, aVersion, 10);
}

/*!
    Get version info.
    \return Version info.
*/
TVersion RApaIPCSession::Version() const
{
    XQSERVICE_DEBUG_PRINT("RApaIPCSession::Version");
    return iVersion;
}

/*!
    Get service uid.
    \return Service uid.
*/
TUid RApaIPCSession::ServiceUid() const
{
    XQSERVICE_DEBUG_PRINT("RApaIPCSession::ServiceUid");
    return iUid;
}

/*!
    Set service uid.
    \param uid Service uid to set.
*/
void RApaIPCSession::setServiceUid(TInt uid)
{
    XQSERVICE_DEBUG_PRINT("RApaIPCSession::setServiceUid %x", uid);
    iUid.iUid = uid;
}

}

// END OF FILE
