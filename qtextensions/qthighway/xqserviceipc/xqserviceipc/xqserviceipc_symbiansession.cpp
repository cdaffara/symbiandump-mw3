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
* Description:  implementation of playback session
*
*/

#include "xqservicelog.h"

#include "xqserviceipc_symbiansession.h"

namespace QtService
{
// ============================== MEMBER FUNCTIONS ============================

/*!
    \class RServiceIPCSession
    \brief Symbian class encapsulating RMessage2 interface
 */

/*!
    Send a message.
    \param aFunction Function code.
    \return Message completion code.
 */
TInt RServiceIPCSession::SendReceiveL(TInt aFunction) const
{
    XQSERVICE_DEBUG_PRINT("RServiceIPCSession::SendReceiveL(1)");
    return User::LeaveIfError(RSessionBase::SendReceive(aFunction));
}

TInt RServiceIPCSession::Send(TInt aFunction, const TIpcArgs& aArgs) const
{
    XQSERVICE_DEBUG_PRINT("RServiceIPCSession::Send");
    return RSessionBase::Send(aFunction, aArgs);
}

/*!
    Send a message.
    \param aFunction Function code.
    \param aArgs Parameter to server.
    \return Message completion code.
 */
TInt RServiceIPCSession::SendReceiveL(TInt aFunction, const TIpcArgs& aArgs) const
{
    XQSERVICE_DEBUG_PRINT("RServiceIPCSession::SendReceiveL(2)");
    return User::LeaveIfError(RSessionBase::SendReceive(aFunction, aArgs));
}

/*!
    Send message asynchronously.
    \param aFunction Function code.
    \param aStatus The request status object used to contain the 
                   completion status of the request.
*/
EXPORT_C void RServiceIPCSession::SendReceive(TInt aFunction,
                                              TRequestStatus& aStatus) const
{
    XQSERVICE_DEBUG_PRINT("RServiceIPCSession::SendReceive(1)");
    RSessionBase::SendReceive(aFunction, aStatus);
}

/*!
    Send message asynchronously.
    \param aFunction Function code.
    \param aArgs Parameter to server.
    \param aStatus The request status object used to contain the
                   completion status of the request.
*/
void RServiceIPCSession::SendReceive(TInt aFunction,
                                     const TIpcArgs& aArgs,
                                     TRequestStatus& aStatus) const
{
    XQSERVICE_DEBUG_PRINT("RServiceIPCSession::SendReceive(2)");
    RSessionBase::SendReceive(aFunction, aArgs, aStatus);
}

/*!
    Connect to server.
    \param aServer server name.
    \param aVersion version of the server.
    \return KErrNone success, otherwise system error code.
*/
TInt RServiceIPCSession::Connect(const TDesC& aServer, const TVersion& aVersion)
{
    QString server = QString::fromUtf16(aServer.Ptr(), aServer.Length());
    XQSERVICE_DEBUG_PRINT("RServiceIPCSession::Connect %s", qPrintable(server));
    iVersion = aVersion;
    return CreateSession(aServer, aVersion, 1);
}

/*!
    Get version info.
    \return Version info.
*/
TVersion RServiceIPCSession::Version() const
{
    XQSERVICE_DEBUG_PRINT("RServiceIPCSession::Version");
    return iVersion;
}

/*!
    Start the server .
    \param aImage Binary name to start.
    \return KErrNone if started properly.
*/
TInt RServiceIPCSession::StartServer(const TDesC& aImage)
{
    XQSERVICE_DEBUG_PRINT("RServiceIPCSession::StartServer");
    RProcess server;
    TInt ret = server.Create(aImage, KNullDesC);
    if (ret == KErrNone) {
        TRequestStatus status;
        XQSERVICE_DEBUG_PRINT("RServiceIPCSession::Rendezvouz");
        server.Rendezvous(status);
        if (status != KRequestPending)
            server.Kill(0);
        else
            server.Resume();
        User::WaitForRequest(status);
        XQSERVICE_DEBUG_PRINT("RServiceIPCSession::Rendezvouz done");
        ret = (server.ExitType() == EExitPanic) ? KErrGeneral : status.Int();
        server.Close();
    }
    return ret;
}

}

// END OF FILE
