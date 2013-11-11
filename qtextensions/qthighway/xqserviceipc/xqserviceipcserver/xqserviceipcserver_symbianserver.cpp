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
* Description:  Symbian implementation for IPC server
*
*/

#include "xqservicelog.h"

#include "xqserviceipcserver_symbianserver.h"
#include "xqserviceipcserver_symbiansession.h"

namespace QtService
{
// Server Security Policy
const TUint KServerRangeCount = 2;
const TInt KServerRanges[KServerRangeCount] = { 
                0, //range is [0-2)
                3 //range is [3-KMaxTInt] 
                };
const TUint8 KSeverElementsIndex[KServerRangeCount] = { 
                0,
                CPolicyServer::ENotSupported };

const CPolicyServer::TPolicyElement KServerPolicyElements[] = { {
                _INIT_SECURITY_POLICY_C1(ECapabilityWriteDeviceData),
                CPolicyServer::EFailClient } };

const CPolicyServer::TPolicy KServerPolicy = {
                CPolicyServer::EAlwaysPass, //specifies all connect attempts should pass
                KServerRangeCount, KServerRanges, KSeverElementsIndex,
                KServerPolicyElements };

enum
{
    EServerPriority = CActive::EPriorityStandard
};

// ======== MEMBER FUNCTIONS ========

/*!
    \class CServiceSymbianServer
    \brief Symbian client server implementation
*/

/*!
    Constructor.
*/
CServiceSymbianServer::CServiceSymbianServer() :
    CPolicyServer(EServerPriority, KServerPolicy)
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianServer::CServiceSymbianServer");
}

/*!
    2nd phased constructor.
*/
void CServiceSymbianServer::ConstructL()
{
}

/*!
    Two phased constructor.
*/
CServiceSymbianServer* CServiceSymbianServer::NewL()
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianServer::NewL");
    CServiceSymbianServer* self = new (ELeave) CServiceSymbianServer;
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
}

/*!
    Destructor.
*/
CServiceSymbianServer::~CServiceSymbianServer()
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianServer::~CServiceSymbianServer");
}

/*!
    Start listening for new service requests.
    \param aServerName Name of the server.
    \return true if successful.
*/
bool CServiceSymbianServer::listen(const QString& aServerName)
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianServer::listen");
    XQSERVICE_DEBUG_PRINT("aServerName: %s", qPrintable(aServerName));
    bool listening(true);

    // Needs to be here becuase Q_Ptr isonly set after constructor of public
    iObserver = Observer();
    TPtrC serverName(reinterpret_cast<const TUint16*> (aServerName.utf16()));
    TInt err=0;
    TRAP( err, StartL(serverName) );
    XQSERVICE_DEBUG_PRINT("listen status=%d", err);
    if (err != KErrNone) {
        listening = false;
    }
    // Complete the server rendezvous that th client started
    XQSERVICE_DEBUG_PRINT("CServiceSymbianServer::Rendezvouz");
    RProcess::Rendezvous(KErrNone);
    XQSERVICE_DEBUG_PRINT("listening: %d", listening);
    return listening;
}

/*!
    Shutdown the server and stop serving clients.
*/
void CServiceSymbianServer::disconnect()
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianServer::disconnect");
    // Symbian Servers do not have disconnect, 
    // the process has to exit
}

/*!
    Create a new session, derived from CPolicyServer.
    \param aVersion Version of the server.
    \param aMessage Message object.
*/
CSession2* CServiceSymbianServer::NewSessionL(const TVersion& aVersion,
                                              const RMessage2& aMessage) const
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianServer::NewSessionL");
    TVersion v(1, 0, 0);
    if (!User::QueryVersionSupported(v, aVersion)) {
        XQSERVICE_DEBUG_PRINT("Not supported version");
        User::Leave(KErrNotSupported);
    }

    // Create a new Symbian Session for the client
    return CServiceSymbianSession::NewL(iObserver);
}
}

// END OF FILE
