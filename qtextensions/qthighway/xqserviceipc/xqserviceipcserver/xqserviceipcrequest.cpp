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
* Description:  IPC request class
*
*/

#include "xqservicelog.h"

#include <xqserviceclientinfo.h>
#include "xqserviceipcrequest.h"
#include "xqserviceipcserversession.h"
#include "xqrequestutil.h"

namespace QtService
{
/*!
    \class ServiceIPCRequest
    \brief Class to encapsulate a service request
*/

/*!
    Constructor.
    \param aSession Session associated with this request.
    \param aDataLength Amount of data to be received in this request.
    \param aRequestOp Operation name.
*/
ServiceIPCRequest::ServiceIPCRequest(ServiceIPCSession* aSession,
                                     qint64 aDataLength,
                                     const QString& aRequestOp) :
    QObject(NULL),
    iSession(aSession), 
    iClientInfo(NULL),
    iRequestOp(aRequestOp), 
    iDatalength(aDataLength),
    iId(-1),
    iAsync(false)
{
    XQSERVICE_DEBUG_PRINT("ServiceIPCRequest::ServiceIPCRequest(1)");
    XQSERVICE_DEBUG_PRINT("aDataLength: %d", aDataLength);
    XQSERVICE_DEBUG_PRINT("aRequestOp: %s", qPrintable(aRequestOp));
}

/*!
    Copy Constructor.
    \param aRequest Request to be copied.
*/
ServiceIPCRequest::ServiceIPCRequest(ServiceIPCRequest& aRequest)
{
    XQSERVICE_DEBUG_PRINT("ServiceIPCRequest::ServiceIPCRequest(2)");
    //session
    iSession = aRequest.getSession();
    
    //client info
    iClientInfo = new ClientInfo();
    iClientInfo->setName((aRequest.clientInfo())->name());
    iClientInfo->setProcessId(aRequest.clientInfo()->processId());
    iClientInfo->setVendorId(aRequest.clientInfo()->vendorId());
    
    // request operation
    iRequestOp = aRequest.getOperation();
    
    //data part
    iRequestData = aRequest.getData();
    iDatalength = iRequestData.length();

    // Request options
    iRequestInfo = aRequest.requestInfo();

}

/*!
    Destructor.
*/
ServiceIPCRequest::~ServiceIPCRequest()
{
    XQSERVICE_DEBUG_PRINT("ServiceIPCRequest::~ServiceIPCRequest");
    delete iClientInfo;
}

/*!
    Assignment operator.
    \param aRequest Request to be assigned.
    \return ServiceIPCRequest assigned request.
*/
ServiceIPCRequest& ServiceIPCRequest::operator=(ServiceIPCRequest& aRequest)
{
    XQSERVICE_DEBUG_PRINT("ServiceIPCRequest::operator=");
    if (this != &aRequest) {
        //session
        iSession = aRequest.getSession();
        
        //client info
        ClientInfo* info = new ClientInfo();
        info->setName((aRequest.clientInfo())->name());
        info->setProcessId(aRequest.clientInfo()->processId());
        info->setVendorId(aRequest.clientInfo()->vendorId());
        delete iClientInfo;
        iClientInfo = info;    
        
        // request operation
        iRequestOp = aRequest.getOperation();
            
        //data part
        iRequestData = aRequest.getData();
        iDatalength = iRequestData.length();

        // Request options
        iRequestInfo = aRequest.requestInfo();
        
    }
    return *this;
}

/*!
    Get the requested operation.
    \return QString operation ID.
*/
const QString& ServiceIPCRequest::getOperation()
{
    XQSERVICE_DEBUG_PRINT("ServiceIPCRequest::getOperation");
    XQSERVICE_DEBUG_PRINT("iRequestOp: %s", qPrintable(iRequestOp));
    return iRequestOp;
}

/*!
    Get the requested data.
    \return QByteArray data for this operation.
*/
const QByteArray& ServiceIPCRequest::getData()
{
    XQSERVICE_DEBUG_PRINT("ServiceIPCRequest::getData");
    XQSERVICE_DEBUG_PRINT("iRequestData: %s", iRequestData.constData());
    return iRequestData;
}

/*!
    Write some data to the request.
    \param aData Data to write to the socket.
*/
bool ServiceIPCRequest::write(const QByteArray& aData)
{
    XQSERVICE_DEBUG_PRINT("ServiceIPCRequest::write");
    XQSERVICE_DEBUG_PRINT("aData: %s", aData.constData());
    // Do we want to buffer the writes?
    return iSession->write(aData);
}

/*!
    Complete the request.
    \return true if request completed successfully.
*/
bool ServiceIPCRequest::completeRequest()
{
    XQSERVICE_DEBUG_PRINT("ServiceIPCRequest::completeRequest");
    return iSession->completeRequest();
}

/*!
    Append more data when creating the request.
    \param aMoreData Data to be appended to the request.
    \return true if iDataLength now equals the full length.
*/
bool ServiceIPCRequest::addRequestdata(const QByteArray& aMoreData)
{
    XQSERVICE_DEBUG_PRINT("ServiceIPCRequest::addRequestdata");
    XQSERVICE_DEBUG_PRINT("aMoreData: %s", aMoreData.constData());
    iRequestData.append(aMoreData);
    return (iRequestData.length() == iDatalength);
}

/*!
    Sets the client info. Onwership of the object is passed in.
    \param aClientInfo Client information.
*/
void ServiceIPCRequest::setClientInfo(ClientInfo *aClientInfo)
{
    XQSERVICE_DEBUG_PRINT("ServiceIPCRequest::setClientInfo");
    delete iClientInfo;
    iClientInfo = aClientInfo;

    // Fill in the implicit info generated by the server
    iRequestInfo.setInfo(XQServiceUtils::InfoSID, iClientInfo->processId());
    iRequestInfo.setInfo(XQServiceUtils::InfoVID, iClientInfo->vendorId());
    iRequestInfo.setInfo(XQServiceUtils::InfoCap, iClientInfo->capabilities());
    
}

/*!
    Gets the client info.
    \return Client Information object, NULL if none is available.
*/
ClientInfo* ServiceIPCRequest::clientInfo()
{
    XQSERVICE_DEBUG_PRINT("ServiceIPCRequest::clientInfo");
    return iClientInfo;
}

/*!
    Gets the session.
    \return ServiceIPCSession, NULL if none is available.
*/
ServiceIPCSession* ServiceIPCRequest::getSession()
{
    XQSERVICE_DEBUG_PRINT("ServiceIPCRequest::getSession");
    return iSession;
}

/*!
    Sets id of the request.
    \param aId Identifier of the request.
*/
void ServiceIPCRequest::setId(int aId)
{
    XQSERVICE_DEBUG_PRINT("ServiceIPCRequest::setId");
    XQSERVICE_DEBUG_PRINT("\t aId = %d", aId);
    iId = aId;
}

/*!
    Returns id of the request.
    \return Id of the request.
*/
int ServiceIPCRequest::id() const
{
    XQSERVICE_DEBUG_PRINT("ServiceIPCRequest::id");
    XQSERVICE_DEBUG_PRINT("\t iId = %d", iId);
    return iId;
}

/*!
    Sets asynchcronous flag to true or false.
    \param aAsync Value of the async flag.
*/
void ServiceIPCRequest::setAsync(bool aAsync)
{
    XQSERVICE_DEBUG_PRINT("ServiceIPCRequest::setAsync");
    XQSERVICE_DEBUG_PRINT("\t iAsync = %d", aAsync);
    iAsync = aAsync;
}

/*!
    Returns async flag.
    \return True if the request is asynchronous. False, if synchronous.
*/
bool ServiceIPCRequest::isAsync() const
{
    XQSERVICE_DEBUG_PRINT("ServiceIPCRequest::getAsync");
    XQSERVICE_DEBUG_PRINT("\t iAsync = %d", iAsync);
    return iAsync;
}

/*!
    Set request info passed along with the request.
    \param info Info to be passed with the request.
*/
void ServiceIPCRequest::setRequestInfo(XQRequestInfo *info)
{
    XQSERVICE_DEBUG_PRINT("ServiceIPCRequest::setRequestInfo");
    if (info)
    {
        iRequestInfo = *info;
        // Restore the overridden id value
        if (iClientInfo)
        {
            // Fill in the implicit info generated by the server
            XQSERVICE_DEBUG_PRINT("ServiceIPCRequest::setRequestInfo fill from clientInfo");
            iRequestInfo.setInfo(XQServiceUtils::InfoSID, iClientInfo->processId());
            iRequestInfo.setInfo(XQServiceUtils::InfoVID, iClientInfo->vendorId());
            iRequestInfo.setInfo(XQServiceUtils::InfoCap, iClientInfo->capabilities());
            iRequestInfo.setInfo(XQServiceUtils::InfoId, id());
        }
    }
}

/*!
    Get info added to the request.
    \return Info to be passed with the request.
*/
XQRequestInfo ServiceIPCRequest::requestInfo() const
{
    XQSERVICE_DEBUG_PRINT("ServiceIPCRequest::requestInfo");
    return iRequestInfo;
}

/*!
    Add sharable file to be passed with the request.
    \param file Sharable file to be added to the request.
    \param index Currently not used.
*/
void ServiceIPCRequest::addSharableFile(XQSharableFile *file, int index)
{
    XQSERVICE_DEBUG_PRINT("ServiceIPCRequest::setSharableFile");
    if (file != NULL)
        iSharableFiles.append(*file);
}

/*!
    Get sharable file from request.
    \param index Index of the sharable file to get.
*/
XQSharableFile ServiceIPCRequest::sharableFile(int index) const
{
    XQSERVICE_DEBUG_PRINT("ServiceIPCRequest::setSharableFile");
    if (index >= 0 && index < iSharableFiles.size())
    {
        XQSharableFile file = iSharableFiles.at(index);
        XQSERVICE_DEBUG_PRINT("ServiceIPCRequest::setSharableFile valid=%d", file.isValid());
        return file;
    }
    return XQSharableFile();
}


}
// END OF FILE
