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
* Description:  Implementation for a Symbian IPC Client
*
*/

#include "xqservicelog.h"

#include <QtCore/qobject.h>
#include <e32base.h>
#include <e32cmn.h>
#include "xqserviceipc_symbian.h"
#include "xqrequestutil.h"

namespace QtService
{
const TInt KIPCOperation = 0;
const TInt KIPCGetBuffer = 1;
const TInt KIPCCancel = 2;
const TInt KIPCOperationWithSharableFile = 3;  // Sharable file support

const TInt KServerMajorVersionNumber = 1;
const TInt KServerMinorVersionNumber = 0;
const TInt KServerBuildVersionNumber = 0;



/*!
    \class CServiceSymbianIPC
    \brief Symbian Client backend for the service IPC
*/

/*!
    Constructor.
*/
CServiceSymbianIPC::CServiceSymbianIPC() :
    CActive(CActive::EPriorityStandard), iDataSize(0)
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianIPC::CServiceSymbianIPC");
    CActiveScheduler::Add(this);
}

/*!
    Destructor.
*/
CServiceSymbianIPC::~CServiceSymbianIPC()
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianIPC::~CServiceSymbianIPC");
    Cancel();
    delete iAsyncData;
    delete iRequestData;
}

/*!
    2nd phased constructor.
*/
void CServiceSymbianIPC::ConstructL()
{
}

/*!
    Two Phased Constructor.
*/
CServiceSymbianIPC* CServiceSymbianIPC::NewL()
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianIPC::NewL");
    CServiceSymbianIPC* self = new (ELeave) CServiceSymbianIPC();
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
bool CServiceSymbianIPC::connect(const QString& aServerName)
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianIPC::connect");
    // Version informaton
    TVersion version(KServerMajorVersionNumber, 
                     KServerMinorVersionNumber, 
                     KServerBuildVersionNumber);
    TPtrC serverName(reinterpret_cast<const TUint16*> (aServerName.utf16()));
    TInt err = iSession.Connect(serverName, version);

    return (err == KErrNone);
}

/*!
    Disconnect from the server.
*/
void CServiceSymbianIPC::disconnect()
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianIPC::disconnect");
    Cancel();
    iSession.Close();
}

/*!
    Starts the service.
    \param aServerName Server name.
    \param aExeName Server executable name.
    \return true if successful.
*/
bool CServiceSymbianIPC::startServer(const QString& /*aServerName*/,
                                     const QString& aExeName,
                                     quint64& /*processId*/,
                                     int /*options*/)
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianIPC::startServer");
    TPtrC serverName(reinterpret_cast<const TUint16*> (aExeName.utf16()));
    TInt err = iSession.StartServer(serverName);
    return (err == KErrNone);
}

/*!
    Send a request synchronously.
    \param aRequestType Type of request to send to the server.
    \param aData Data to send to the server.
    \return true if successful.
*/
bool CServiceSymbianIPC::sendSync(const QString& aRequestType,
                                  const QByteArray& aData)
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianIPC::sendSync");
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
    
    XQSERVICE_DEBUG_PRINT("CServiceSymbianIPC::sendSync status%d", err);
    return (err == KErrNone);
}


/*!
    Read sync.
    \return Result of the read.
*/
QByteArray CServiceSymbianIPC::readAll()
{
// this is sync operation

    XQSERVICE_DEBUG_PRINT("CServiceSymbianIPC::readAll");
    QByteArray rtn;
    
    TInt err(KErrNone);
    TRAP( err, rtn = doReadAllL() );
    XQSERVICE_DEBUG_PRINT("CServiceSymbianIPC::readAll status=%d",err);
    
    if ( err )
        {
        emitError(err);
        }
    
    return rtn;
}

/*!
    Read sync.
*/
QByteArray CServiceSymbianIPC::doReadAllL()
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianIPC::doReadAllL");
    // Read the data via IPC
    //
    CBufBase* buf = CBufFlat::NewL(iDataSize);
    CleanupStack::PushL(buf);
    buf->ResizeL(iDataSize);
    TPtr8 ptr(buf->Ptr(0));
    iSession.SendReceiveL(KIPCGetBuffer, TIpcArgs(&ptr));

    QByteArray retData((char*) ptr.Ptr(), ptr.Length());

    CleanupStack::PopAndDestroy(buf);

    XQSERVICE_DEBUG_PRINT("CServiceSymbianIPC::doReadAllL DONE");
    // Deep copy, return variable is implicitly shared
    return retData;
}



/*!
    Send a request asynchronously.
    \param aRequestType Type of request to send to the server.
    \param aData Data to send to the server.
*/
void CServiceSymbianIPC::sendAsync(const QString& aRequestType,
                                   const QByteArray& aData)
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianIPC::sendAsync");
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
/*!
    Read async.
    \param aArray Array where result will be put.
*/
void CServiceSymbianIPC::readAll(QByteArray& aArray)
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianIPC::readAll");
	
    // this is async operation
    TInt err(KErrNone);
    TRAP(err, doReadAllL(aArray));
    XQSERVICE_DEBUG_PRINT("CServiceSymbianIPC::readAll status=%d", err);
    if (err)
        {
        emitError(err);
        }
    
}

/*!
    Reads all data pending in the buffer, leaves if an error occured.
    \param aArray QByteArray containing the result data.
*/
void CServiceSymbianIPC::doReadAllL(QByteArray& aArray)
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianIPC::doReadAllL");
    //shouldn't be active when this is called
    if ( IsActive() )
        {
        User::LeaveIfError( KErrGeneral );
        }  

    iArray = &aArray;
        
    iBuf.CreateL(iDataSize);
    
    iSession.SendReceive(KIPCGetBuffer, TIpcArgs(&iBuf), iStatus );
    
    iState = ERead;
    SetActive();
}


/*!
    Maps error codes from Symbian error codes to Service IPC error codes.
    \param aError Symbian error code.
    \return Mapped error code.
*/
int CServiceSymbianIPC::doMapErrors(TInt aError)
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianIPC::doMapErrors");
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
    case KErrServerBusy: {
        error = ServiceFwIPC::EConnectionError;
        break;
    }
                         
    case KErrArgument:
    {
        error = XQService::EArgumentError;
        break;
    }
        
    case KErrNoMemory: {
        error = ServiceFwIPC::EIPCError;
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
    Waits until data is available for reading 
    \return bool Always true, no need to wait
*/
bool CServiceSymbianIPC::waitForRead()
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianIPC::waitForRead");   
    // Symbian Client-server is blocking, so no need to wait for read
    return true;
}

/*!
    Active object callback.
*/
TInt CServiceSymbianIPC::RunError(TInt err)
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianIPC::error");
    emitError(doMapErrors(err));
    return KErrNone;
}

/*!
    Active object RunL() function.
*/
void CServiceSymbianIPC::RunL()
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianIPC::RunL");   
    TInt err = iStatus.Int();
    User::LeaveIfError(err);
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
    Active object cancel.
*/
void CServiceSymbianIPC::DoCancel()
{
    XQSERVICE_DEBUG_PRINT("CServiceSymbianIPC::DoCancel");
    TRAP_IGNORE(iSession.SendReceiveL(KIPCCancel));
    XQSERVICE_DEBUG_PRINT("CServiceSymbianIPC::DoCancel DONE");
}

// Sharable file utility
const TInt KRFsSlot = 2;
const TInt KRFileSlot = 3;

bool TransferSharableFile(TIpcArgs *args, const XQSharableFile *file)
{
    XQSERVICE_DEBUG_PRINT("TransferSharableFile");
    if (!file)
    {
        XQSERVICE_DEBUG_PRINT("\tNo file");
        return false;
    }
    
    bool ret=false;
    RFile handle;
    if (file->getHandle(handle))
    {
        XQSERVICE_DEBUG_PRINT("\thandle OK");
        TInt err = handle.TransferToServer(*args, KRFsSlot, KRFileSlot);
        XQSERVICE_DEBUG_PRINT("TransferSharableFile:status=%d", err);
        ret = (err == KErrNone);
    }
    else
    {
        XQSERVICE_DEBUG_PRINT("\thandle NOT OK");
    }
    
    return ret;
}


}
// END OF FILE
