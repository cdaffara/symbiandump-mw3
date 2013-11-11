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
* Description:  Service IPC Session
*
*/

#include <e32capability.h> 

#include "xqservicelog.h"

#include <xqserviceclientinfo.h>
#include "xqserviceipcobserver.h"
#include "xqserviceipcrequest.h"
#include "xqserviceipcserver_symbiansession.h"
#include "xqrequestutil.h"

namespace QtService
{

// Constants for the IPC operation id
const TInt KIPCNewOperation = 0;
const TInt KIPCGetData = 1;
const TInt KIPCCancelAsync = 2;
const TInt KIPCOperationWithSharableFile = 3;  // Sharable file support


/*!
    \class CServiceSymbianSession
    \brief Symbian Session class
*/

/*!
    Constructor.
    \param aObserver Observer to the server.
*/
CServiceSymbianSession::CServiceSymbianSession(MServiceIPCObserver* aObserver) :
    ServiceIPCSession(aObserver)
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianSession::CServiceSymbianSession");
}

/*!
    2nd phased constructor.
*/
void CServiceSymbianSession::ConstructL()
{
}

/*!
    Two-Phased Constructor.
    \param aObserver Observer to the server.
*/
CServiceSymbianSession* CServiceSymbianSession::NewL(MServiceIPCObserver* aObserver)
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianSession::NewL");
    CServiceSymbianSession* self =
                    new (ELeave) CServiceSymbianSession(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
}

/*!
    Destructor.
*/
CServiceSymbianSession::~CServiceSymbianSession()
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianSession::~CServiceSymbianSession");
    if (iCurRequest) {
        iObserver->handleDeleteRequest(iCurRequest); 
        delete iCurRequest;
    }
    iCurRequest = NULL;
}

/*!
    Write some data in response to a request.
    \param aData Some data to write as response.
    \return bool if write was successful.
*/
bool CServiceSymbianSession::write(const QByteArray& aData)
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianSession::write");
    XQSERVICE_DEBUG_PRINT("aData: %s", aData.constData());
    // Implicitly shared
    iData = aData;
    return false;
}

/*!
    Complete a Request.
    \return bool if request completed 
*/
bool CServiceSymbianSession::completeRequest()
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianSession::completeRequest");
    XQSERVICE_DEBUG_PRINT("iData: %s", iData.constData());
    TPtrC8 data(reinterpret_cast<const TUint8*> (iData.constData()), iData.length());
    iMessage.Complete(data.Length());
    return true;
}

/*!
    Close a session and gracefully shutdown.
*/
void CServiceSymbianSession::close()
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianSession::close");
    // Symbian doens't really do anything
}

/*!
    From CSession2.
    Service request.
    \param aMessage Message object.
*/
void CServiceSymbianSession::ServiceL(const RMessage2& aMessage)
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianSession::ServiceL");
    // Default ServiceErrorL() will complete the message if this method leaves
    TInt operation(aMessage.Function());
    XQSERVICE_DEBUG_PRINT("operation: %d", operation);
    switch (operation) {
    case KIPCNewOperation:
    case KIPCOperationWithSharableFile:
    {
        handleRequestL(aMessage);
        break;
    }
    case KIPCGetData: {
        handleGetBufferL(aMessage);
        break;
    }
    case KIPCCancelAsync: {
        if (iCurRequest) {
            // Inform also observer, that current pending  request is about to go
            iObserver->handleCancelRequest(iCurRequest);
            iCurRequest->completeRequest();
        }
        aMessage.Complete(KErrCancel);
        break;
    }
    default: {
        aMessage.Complete(KErrNotFound);
        break;
    }
    }
}

/*!
    From CSession2.
    Handle any disconnection from the client.
    \param aMessage Message object.
*/
void CServiceSymbianSession::Disconnect(const RMessage2 &aMessage)
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianSession::Disconnect");
    CSession2::Disconnect(aMessage);
}

/*!
    Handle an IPC request.
    \param aMessage Message object.
*/
void CServiceSymbianSession::handleRequestL(const RMessage2& aMessage)
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianSession::handleRequestL");
    // Store the message
    iMessage = aMessage;

    // Convert from Symbian to QT
    HBufC* request = ReadDesLC(aMessage, 0);
    HBufC8* data = ReadDes8LC(aMessage, 1);
           
    XQSharableFile *file = 0;
    if (aMessage.Function() == KIPCOperationWithSharableFile)
    {
        // Only one file support now !
        file = new XQSharableFile();
        AdoptSharableFile(aMessage, file);
    }
    
    // Shallow copy only, we want a deep copy
    QString d = QString::fromUtf16(request->Ptr(), request->Length());
    QString operation;
    operation += d;
    XQSERVICE_DEBUG_PRINT("operation: %s", qPrintable(operation));

    //QByteArray convertData;
    TPtr8 ptr8(data->Des());
    const char* ptrz = reinterpret_cast<const char*> (ptr8.PtrZ());
    //convertData.append(ptrz);
	QByteArray convertData(ptrz,data->Length());
	XQSERVICE_DEBUG_PRINT("convertData: %s", convertData.constData());

    // New request
    if (iCurRequest) {
        iObserver->handleDeleteRequest(iCurRequest); 
        delete iCurRequest;
    }
    iCurRequest = NULL;
    iCurRequest = new ServiceIPCRequest(this, 0, operation);
    iData.clear();

    // Get client info
    ClientInfo *client = new ClientInfo();
    client->setProcessId(aMessage.SecureId().iId);
    client->setVendorId(aMessage.VendorId().iId);
    RThread clientThread;
    aMessage.ClientL(clientThread);
    CleanupClosePushL(clientThread);
    RProcess clientProc;
    CleanupClosePushL(clientProc);
    clientThread.Process(clientProc);
    client->setName(QString::fromUtf16(clientProc.Name().Ptr(), 
                                       clientProc.Name().Length()));
    client->setCapabilities(ClientCapabilities(aMessage));
    CleanupStack::PopAndDestroy(2, &clientThread);

    // Set the picked sharable file if any
    if (file != 0)
    {
        // Support only one sharable file
        iCurRequest->addSharableFile(file, 0);
    }
    
    // Add data and callback to the observer
    // 
    iCurRequest->addRequestdata(convertData);
    iCurRequest->setClientInfo(client); // ownership passed
    iObserver->handleRequest(iCurRequest);

    CleanupStack::PopAndDestroy(2, request);
}

/*!
    Handle getting the result buffer.
    \param aMessage Message object.
*/
void CServiceSymbianSession::handleGetBufferL(const RMessage2& aMessage)
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianSession::handleGetBufferL");
    XQSERVICE_DEBUG_PRINT("iData: %s", iData.constData());
    TPtrC8 data(reinterpret_cast<const TUint8*> (iData.constData()), iData.length());
    TInt err = aMessage.Write(0, data);
    aMessage.Complete(err);
}

/*!
    Read a 16 bit descriptor from the message.
    \param aMessage Message to read from.
    \param aMsgSlot Slot to read from.
 */
HBufC* CServiceSymbianSession::ReadDesLC(const RMessage2& aMessage,
                                         TInt aMsgSlot)
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianSession::ReadDesLC");
    TInt length = aMessage.GetDesLengthL(aMsgSlot);
    HBufC* des = HBufC::NewLC(length);
    TPtr ptr = des->Des();
    aMessage.ReadL(aMsgSlot, ptr);
    return des;
}

/*!
    Read a 8 bit descriptor from the message.
    \param aMessage Message to read from.
    \param aMsgSlot Slot to read from.
*/
HBufC8* CServiceSymbianSession::ReadDes8LC(const RMessage2& aMessage,
                                           TInt aMsgSlot)
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianSession::ReadDes8LC");
    TInt length = aMessage.GetDesLengthL(aMsgSlot);
    HBufC8* des = HBufC8::NewLC(length + 1); // 1 more for null terminator
    TPtr8 ptr = des->Des();
    aMessage.ReadL(aMsgSlot, ptr);
    return des;
}

// Sharable file utility
const TInt KRFsSlot = 2;
const TInt KRFileSlot = 3;

bool AdoptSharableFile(const RMessage2& aMsg, XQSharableFile *file)
{
    XQSERVICE_DEBUG_PRINT("AdoptSharableFile");
    RFile handle;
    TInt err = handle.AdoptFromClient(aMsg, KRFsSlot, KRFileSlot);

    bool ret = (err == KErrNone);
   
     if (ret)
    {
        file->setHandle(handle);
    }
    
    // On error the handle remains invalid !
    return ret;
}

/*!
    Get client capabilities from the IPC request.
*/
quint32 ClientCapabilities(const RMessage2& aMsg)
{
    quint32 caps = 0;
    for(int i = 0; i < ECapability_Limit; i++)
    {
        if (aMsg.HasCapability(static_cast<TCapability>(i)))
        {
            caps |= 1 << i;
        }
    }

    return caps;
}

}


// END OF FILE

