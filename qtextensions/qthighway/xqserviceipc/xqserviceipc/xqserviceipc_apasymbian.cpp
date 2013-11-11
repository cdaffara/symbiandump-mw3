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
* Description:  Implementation for a Symbian APA SERVER IPC Client
*
*/

#include "xqservicelog.h"

#include <QtCore/qobject.h>
#include <e32base.h>
#include <e32cmn.h>
#include <apaserverapp.h>
#include <coemain.h>
#include "xqserviceipc_apasymbian.h"
#include "xqrequestutil.h"

#include <xqservicemanager.h>

namespace QtService
{
const TInt KIPCOperation = RApaAppServiceBase::KServiceCmdBase;
const TInt KIPCGetBuffer = KIPCOperation+1;
const TInt KIPCOperationWithSharableFile = KIPCOperation+2;  // Sharable file support

const TInt KServerMajorVersionNumber = 1;
const TInt KServerMinorVersionNumber = 0;
const TInt KServerBuildVersionNumber = 0;


/*!
    \class CApaSymbianIPC
    \brief Symbian Client backend for the service IPC
*/

/*!
    Constructor.
*/
CApaSymbianIPC::CApaSymbianIPC() :
    CActive(CActive::EPriorityStandard), iDataSize(0)
{
    XQSERVICE_DEBUG_PRINT("CApaSymbianIPC::CApaSymbianIPC");
    CActiveScheduler::Add(this);
}

/*!
    Destructor.
*/
CApaSymbianIPC::~CApaSymbianIPC()
{
    XQSERVICE_DEBUG_PRINT("CApaSymbianIPC::~CApaSymbianIPC");
    Cancel();
    delete iAsyncData;
    delete iRequestData;
}

/*!
    2nd phased constructor.
*/
void CApaSymbianIPC::ConstructL()
{
    XQSERVICE_DEBUG_PRINT("CApaSymbianIPC::ConstructL");
	iServerExitMonitor = NULL;
}

/*!
    Two Phased Constructor.
*/
CApaSymbianIPC* CApaSymbianIPC::NewL()
{
    XQSERVICE_DEBUG_PRINT("CApaSymbianIPC::NewL");
    CApaSymbianIPC* self = new(ELeave) CApaSymbianIPC();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
}

/*!
    Connect to the server.
    \param aServerName Name of the server to connect to.
    \return true if connected, false if not.
*/
bool CApaSymbianIPC::connect( const QString& aServerName )
{
    XQSERVICE_DEBUG_PRINT("CApaSymbianIPC::connect");
    // Version informaton
    TVersion version(KServerMajorVersionNumber, 
                     KServerMinorVersionNumber, 
                     KServerBuildVersionNumber);
    TPtrC serverName(reinterpret_cast<const TUint16*> (aServerName.utf16()));
    TInt err = iSession.Connect(serverName, version);

    if (!err) {
        StartExitMonitor();
    }
    
    return (err == KErrNone);
}

/*!
    Disconnect from the server.
*/
void CApaSymbianIPC::disconnect()
{
    XQSERVICE_DEBUG_PRINT("CApaSymbianIPC::disconnect");
    // Note !
    // iServerExitMonitor->Cancel will cause request cancellation 
    // in CApaServerSymbianSession::ServiceL() with error code KErrCancel
    if (iServerExitMonitor) {
        iServerExitMonitor->Cancel();
        delete iServerExitMonitor;
        iServerExitMonitor = NULL;
    }
    iSession.Close();
}

/*!
    Starts the service.
    \param aServerName Server name.
    \param aExeName Server executable name.
    \return true if start was successful.
*/
bool CApaSymbianIPC::startServer( const QString& aServerName, 
                                  const QString& /*aExeName*/, 
                                  quint64& processId,
                                  int options)
{
    XQSERVICE_DEBUG_PRINT("CApaSymbianIPC::startServer");
    XQServiceManager mng;
    bool embedded = (options == ServiceFwIPC::EStartInEmbeddedMode) ? true : false;
    int appUid = 0;
    int ret=0;
    if (userData == NULL)
    {
        XQSERVICE_DEBUG_PRINT("CApaSymbianIPC::No user data in start");
        ret = mng.startServer(aServerName,embedded,appUid,processId);
    }
    else
    {
        XQSERVICE_DEBUG_PRINT("CApaSymbianIPC::Apply user data in start");
        ret = mng.startServer(aServerName,embedded,appUid,processId, userData);
    }
    
    if (ret)
        {
        emitError(ret);
        }
    iSession.setServiceUid(appUid);
    return (ret == KErrNone);
}

/*!
    Send a request synchronously.
    \param aRequestType Type of request to send to the server.
    \param aData Data to send to the server.
    \return true if send was successful.
*/
bool CApaSymbianIPC::sendSync( const QString& aRequestType, const QByteArray& aData )
{
    XQSERVICE_DEBUG_PRINT("CApaSymbianIPC::sendSync");
    // Convert from QString to TPtr
    TPtrC request(reinterpret_cast<const TUint16*> (aRequestType.utf16()));
    TPtrC8 data(reinterpret_cast<const TUint8*> (aData.constData()), aData.length());

    // Send data, 0 is new op
    TInt err=KErrNone;
    TInt dataSize=0;
    TIpcArgs args(&request, &data);
    TInt cmd = KIPCOperation;
    if (userData != 0)
    {
        XQRequestUtil *util = (XQRequestUtil *)userData;
        // Only the first transferred
        if (TransferSharableFile(&args, util->getSharableFile(0)))
        {
            cmd = KIPCOperationWithSharableFile;
        }
    }
    
    TRAP( err, dataSize = iSession.SendReceiveL(cmd,args) );

    // map return value
    if (err == KErrNone) {
        iDataSize = dataSize;
    }
    
    XQSERVICE_DEBUG_PRINT("CApaSymbianIPC::sendSync status=%s", err);
    
    return (err == KErrNone);
}


/*!
    Read sync.
    \return Result of read as QByteArray.
*/
QByteArray CApaSymbianIPC::readAll()
{
    // this is sync operation
    XQSERVICE_DEBUG_PRINT("CApaSymbianIPC::readAll");
    QByteArray rtn;
    TInt err(KErrNone);
    TRAP( err, rtn = doReadAllL() );
    XQSERVICE_DEBUG_PRINT("CApaSymbianIPC::readAll status=%d",err);
    if ( err )
        {
        emitError(err);
        }
    return rtn;
}

/**
* read sync
*/
QByteArray CApaSymbianIPC::doReadAllL()
{
    XQSERVICE_DEBUG_PRINT("CApaSymbianIPC::doReadAllL");
    // Read the data via IPC
    CBufBase* buf = CBufFlat::NewL(iDataSize);
    CleanupStack::PushL(buf);
    buf->ResizeL(iDataSize);
    TPtr8 ptr(buf->Ptr(0));
    iSession.SendReceiveL(KIPCGetBuffer, TIpcArgs(&ptr));

    QByteArray retData((char*) ptr.Ptr(), ptr.Length());

    CleanupStack::PopAndDestroy(buf);

    XQSERVICE_DEBUG_PRINT("CApaSymbianIPC::doReadAllL DONE");
    
    // Deep copy, return variable is implicitly shared
    return retData;
}



/*!
    Send a request asynchronously.
    \param aRequestType Type of request to send to the server.
    \param aData Data to send to the server.
*/
void CApaSymbianIPC::sendAsync(const QString& aRequestType,
							   const QByteArray& aData )
{
    XQSERVICE_DEBUG_PRINT("CApaSymbianIPC::sendAsync");
    delete iRequestData;
    iRequestData = NULL;
    TPtrC request(reinterpret_cast<const TUint16*> (aRequestType.utf16()));
    iRequestData = request.Alloc();
    delete iAsyncData;
    iAsyncData = NULL;
    TPtrC8 data(reinterpret_cast<const TUint8*> (aData.constData()), aData.length());
    iAsyncData = data.Alloc();

    // Send data
    iRequestDataPtr.Set(*iRequestData);
    iAsyncDataPtr.Set(*iAsyncData);
    TIpcArgs args(&iRequestDataPtr, &iAsyncDataPtr);

    TInt cmd = KIPCOperation;
    if (userData != 0)
    {
        XQRequestUtil *util = (XQRequestUtil *)userData;
        // Only the first transferred
        if (TransferSharableFile(&args, util->getSharableFile(0)))
        {
            cmd = KIPCOperationWithSharableFile;
        }
    }
    
    iSession.SendReceive(cmd, args, iStatus);
    iState = ESend;
    SetActive();
}

/*
 * read async
 */
void CApaSymbianIPC::readAll(QByteArray& aArray)
{
    XQSERVICE_DEBUG_PRINT("CApaSymbianIPC::readAll");

	// this is async operation
    TInt err(KErrNone);
    TRAP(err, doReadAllL(aArray ));
    XQSERVICE_DEBUG_PRINT("CApaSymbianIPC::readAll status=%d",err);
    if (err)
        {
        emitError(err);
        }
    
}

/*!
    Reads all data pending in the buffer, leaves if an error occured
    \return QByteArray Containing the result data.
*/
void CApaSymbianIPC::doReadAllL(QByteArray& aArray)
{
    XQSERVICE_DEBUG_PRINT("CApaSymbianIPC::doReadAllL");
    
	//shouldn't be active when this is called
    if (IsActive())
        {
        User::LeaveIfError(KErrGeneral);
        }
	
    iArray = &aArray;
          
    
    iBuf.CreateL(iDataSize);
    
    iSession.SendReceive(KIPCGetBuffer, TIpcArgs(&iBuf), iStatus);
    
    iState = ERead;
    SetActive();
}


/*!
    Maps error codes from Symbian error codes to Service IPC error codes
    \param aError Symbian error code.
    \return Mapped error code.
*/
int CApaSymbianIPC::doMapErrors(TInt aError)
{
    XQSERVICE_DEBUG_PRINT("CApaSymbianIPC::doMapErrors");
    int error(ERROR_NoError);
    switch (aError) {
    case KErrNone: {
        error = ERROR_NoError;
        break;
    }
    case KErrPermissionDenied:
    case KErrServerTerminated: {
        error = ServiceFwIPC::EConnectionClosed;
        break;
    }
                               
    case KErrServerBusy:
    case KErrCouldNotConnect:
    {
        error = ServiceFwIPC::EConnectionError;
        break;
    }
    case KErrArgument:
    {
        error = ServiceFwIPC::EArgumentError;
        break;
    }
        
    case KErrNoMemory: {
        error = ServiceFwIPC::EIPCError;
        break;
    }
    case KErrNotFound: {
        error = ServiceFwIPC::EServerNotFound;
        break;
    }
    default: {
        error = ServiceFwIPC::EUnknownError;
        break;
    }
    }
    return error;
}
/*!
    Waits until data is available for reading.
    \return bool always true, no need to wait.
*/
bool CApaSymbianIPC::waitForRead()
{
    XQSERVICE_DEBUG_PRINT("CApaSymbianIPC::waitForRead");
    // Symbian Client-server is blocking, so no need to wait for read
    return true;
}

/*!
    Active object callback
*/
TInt CApaSymbianIPC::RunError(TInt err)
{
    XQSERVICE_DEBUG_PRINT("CApaSymbianIPC::error %d", err);
    emitError(doMapErrors(err));
    
    return KErrNone;
}

/*!
    Active object RunL function.
*/
void CApaSymbianIPC::RunL()
{
    int err = iStatus.Int();
    XQSERVICE_DEBUG_PRINT("CApaSymbianIPC::RunL %d", err);
    User::LeaveIfError( err );
    //todo: here the err is used to return le lenght of the data i would change this
    // Callback to observers
    //
    switch (iState)
        {
        case ESend:
            {
            //here the err variable contains the length of retdata
            if (err >= KErrNone)
                {
                iDataSize = err;
                emitReadyRead();
                }
            
            break;
            }
        case ERead:
            {
            QByteArray retData((char*) iBuf.Ptr(), iBuf.Length());
            //pass the value back to upper layer
            *iArray = retData;

            delete iRequestData;
            iRequestData = NULL;
            delete iAsyncData;
            iAsyncData = NULL;
            iRequestDataPtr.Set(KNullDesC);
            iAsyncDataPtr.Set(KNullDesC8);

            emitReadDone();

            break;
            }
        }
}

/*!
    Active object cancel
*/
void CApaSymbianIPC::DoCancel()
{
    XQSERVICE_DEBUG_PRINT("CApaSymbianIPC::DoCancel");
    // We can't cancel in the IPC design.
 
}

void CApaSymbianIPC::StartExitMonitor()
{
    XQSERVICE_DEBUG_PRINT("CApaSymbianIPC::StartExitMonitor");
    if (iServerExitMonitor == NULL)
        {
        TInt err(KErrNone);
        TRAP( err, iServerExitMonitor = CApaServerAppExitMonitor::NewL(iSession,
                                                                        *this,
                                                                        CActive::EPriorityStandard ));    
        XQSERVICE_DEBUG_PRINT("CApaSymbianIPC::StartExitMonitor status=%d",err);
        }
}

/*!
    HandleServerAppExit
*/
void CApaSymbianIPC::HandleServerAppExit(int aReason)
{
    XQSERVICE_DEBUG_PRINT("CApaSymbianIPC::HandleServerAppExit");
    if (iServerExitMonitor) {
        iServerExitMonitor->Cancel();
        delete iServerExitMonitor;
        iServerExitMonitor = NULL;
    }
    emitError(doMapErrors(aReason));
}



} // namespace QtService
// END OF FILE
