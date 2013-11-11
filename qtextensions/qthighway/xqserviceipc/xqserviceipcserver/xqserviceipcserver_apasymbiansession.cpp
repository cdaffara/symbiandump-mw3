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

#include "xqservicelog.h"

#include <xqserviceclientinfo.h>
#include "xqserviceipcobserver.h"
#include "xqserviceipcrequest.h"
#include "xqserviceipcserver_apasymbiansession.h"
#include "xqrequestutil.h"

namespace QtService
{

// Constants for the IPC operation id
const TInt KIPCOperation = RApaAppServiceBase::KServiceCmdBase;
const TInt KIPCGetBuffer = KIPCOperation+1;
const TInt KIPCOperationWithSharableFile = KIPCOperation+2;  // Sharable file support


/*!
    \class CApaServerSymbianSession
    \brief Symbian Session class
*/

/*!
    Constructor.
    \param aObserver Observer to the server.
*/
CApaServerSymbianSession::CApaServerSymbianSession(MServiceIPCObserver* aObserver) :
    ServiceIPCSession(aObserver)
{
    XQSERVICE_DEBUG_PRINT("CApaServerSymbianSession::CApaServerSymbianSession");
}

/*!
    2nd phased constructor.
*/
void CApaServerSymbianSession::ConstructL()
{
}

/*!
    Two-Phased Constructor.
    \param aObserver Observer to the server.
*/
CApaServerSymbianSession* CApaServerSymbianSession::NewL(MServiceIPCObserver* aObserver)
{
    XQSERVICE_DEBUG_PRINT("CApaServerSymbianSession::NewL");
    CApaServerSymbianSession* self = 
	                   new( ELeave ) CApaServerSymbianSession(aObserver);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
}

/*!
    Destructor.
*/
CApaServerSymbianSession::~CApaServerSymbianSession()
{
    XQSERVICE_DEBUG_PRINT("CApaServerSymbianSession::~CApaServerSymbianSession");
    if (iCurRequest) {
        iObserver->handleDeleteRequest(iCurRequest); 
        delete iCurRequest;
    }
    iCurRequest = NULL;
}

/*!
    Write some data in response to a request.
    \param aData Some data to write as response.
    \return bool If write was successful.
*/
bool CApaServerSymbianSession::write( const QByteArray& aData )
{
    XQSERVICE_DEBUG_PRINT("CApaServerSymbianSession::write");
    XQSERVICE_DEBUG_PRINT("aData: %s,length=%d", aData.constData(), aData.length());
    // Implicitly shared
    iData = aData;
    return false;
}

/*!
    Complete a request.
    \return true if request completed 
*/
bool CApaServerSymbianSession::completeRequest()
{
    XQSERVICE_DEBUG_PRINT("CApaServerSymbianSession::completeRequest");
    TPtrC8 data(reinterpret_cast<const TUint8*> (iData.constData()), iData.length());    
    XQSERVICE_DEBUG_PRINT("data: %s, length=%d", iData.constData(), data.Length() );
    iMessage.Complete(data.Length());
    if (iCurRequest) {
        iObserver->handleDeleteRequest(iCurRequest); 
        delete iCurRequest;
    }
    iCurRequest = NULL;
    return true;
}

/*!
    Close a session and gracefully shutdown.
*/
void CApaServerSymbianSession::close()
{
    XQSERVICE_DEBUG_PRINT("CApaServerSymbianSession::close");
    // Symbian doens't really do anything
}

/*!
    From CSession2. Service request.
    \param aMessage Message object.
*/
void CApaServerSymbianSession::ServiceL(const RMessage2& aMessage)
{
    XQSERVICE_DEBUG_PRINT("CApaServerSymbianSession::ServiceL");
    // Default ServiceErrorL() will complete the message if this method leaves
    TInt operation(aMessage.Function());
    XQSERVICE_DEBUG_PRINT("\toperation: %d", operation);
    switch (operation) {
    case KIPCOperation:
    case KIPCOperationWithSharableFile:
    {
    
        XQSERVICE_DEBUG_PRINT("CApaServerSymbianSession::ServiceL handleRequest");
        handleRequestL(aMessage);
        XQSERVICE_DEBUG_PRINT("CApaServerSymbianSession::ServiceL handleRequest end");
        //and here
        break;
    }
    case KIPCGetBuffer: {
        XQSERVICE_DEBUG_PRINT("CApaServerSymbianSession::ServiceL handleGetBufferL");
        handleGetBufferL(aMessage);
        XQSERVICE_DEBUG_PRINT("CApaServerSymbianSession::ServiceL handleGetBufferL end");
        break;
    }
                        
    default: {
    XQSERVICE_DEBUG_PRINT("CApaServerSymbianSession::ServiceL default");
        if (operation == 2 && iCurRequest)  // maparnan
        {
            // Need to complete the pending request when handling the 
            // the EApaAppServerCancelNotifyServerExit (2) from the client
            // It is too late to complete that in session disconnect
            XQSERVICE_DEBUG_PRINT("CApaServerSymbianSession::ServiceL request cancelled");
            iMessage.Complete(KErrCancel);
        }
        
        CApaAppServiceBase::ServiceL( aMessage );
        //aMessage.Complete(KErrNotFound);
        XQSERVICE_DEBUG_PRINT("CApaServerSymbianSession::ServiceL default end");    
        break;
    }
    }
}

/*!
    From CSession2.
    Handle any disconnection from the client.
    \param aMessage Message Object.
*/
void CApaServerSymbianSession::Disconnect(const RMessage2 &aMessage)
{
    XQSERVICE_DEBUG_PRINT("CApaServerSymbianSession::Disconnect");
    if (iCurRequest) {
         // Inform also observer, that current pending  request is about to go
        iObserver->handleCancelRequest(iCurRequest); 
    }
    CApaAppServiceBase::Disconnect(aMessage);
}

/*!
    Handle an IPC request.
    \param aMessage Message Object.
*/
void CApaServerSymbianSession::handleRequestL( const RMessage2& aMessage )
{
    XQSERVICE_DEBUG_PRINT("CApaServerSymbianSession::handleRequestL");
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

    /*
    //QByteArray convertData;
    TPtr8 ptr8(data->Des());
    const char* ptrz = reinterpret_cast<const char*> (ptr8.PtrZ());
    //convertData.append(ptrz);
	QByteArray convertData(ptrz,data->Length());
    */
    QByteArray convertData( reinterpret_cast<const char*>(data->Ptr()), data->Length() );
    XQSERVICE_DEBUG_PRINT("convertData: %s", convertData.constData());
    
	// New request
    Q_ASSERT(!iCurRequest);
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
    User::LeaveIfError( clientThread.Process(clientProc) );
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

    //
    // Add data and callback to the observer
    // 
    iCurRequest->addRequestdata(convertData);
    iCurRequest->setClientInfo(client); // ownership passed
    iObserver->handleRequest(iCurRequest);

    CleanupStack::PopAndDestroy(2, request);
}

/*!
    Handle getting the result buffer.
    \param aMessage Message Object.
*/
void CApaServerSymbianSession::handleGetBufferL( const RMessage2& aMessage )
{
    XQSERVICE_DEBUG_PRINT("CApaServerSymbianSession::handleGetBufferL");
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
HBufC* CApaServerSymbianSession::ReadDesLC(const RMessage2& aMessage,
                                           TInt aMsgSlot)
{
    XQSERVICE_DEBUG_PRINT("CApaServerSymbianSession::ReadDesLC");
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
HBufC8* CApaServerSymbianSession::ReadDes8LC(const RMessage2& aMessage,
                                             TInt aMsgSlot)
{
    XQSERVICE_DEBUG_PRINT("CApaServerSymbianSession::ReadDes8LC");
    TInt length = aMessage.GetDesLengthL(aMsgSlot);
    HBufC8* des = HBufC8::NewLC(length + 1); // 1 more for null terminator
    TPtr8 ptr = des->Des();
    aMessage.ReadL(aMsgSlot, ptr);
    return des;
}

}

// END OF FILE

