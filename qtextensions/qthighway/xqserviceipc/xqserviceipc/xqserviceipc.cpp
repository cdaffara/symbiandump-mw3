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
* Description:  IPC Client side implementation
*
*/

#include "xqservicelog.h"

#include "xqserviceipc.h"
#include "xqserviceipc_p.h"
#include "xqserviceipcfactory.h"

namespace QtService
{

/*!
    \class ServiceFwIPC
    \brief Public interface class for IPC operations
*/

/*!
    Destructor
*/
ServiceFwIPC::~ServiceFwIPC()
{
    XQSERVICE_DEBUG_PRINT("ServiceFwIPC::~ServiceFwIPC");
    delete d;
}

/*!
    Constructor.
    \param aParent Parent to this QObject
    \param aBackend IPC backend to use
*/
ServiceFwIPC::ServiceFwIPC(QObject* aParent, TServiceIPCBackends aBackend) :
                           QObject(aParent), iAsyncRequestPending(false)
{
    XQSERVICE_DEBUG_PRINT("ServiceFwIPC::ServiceFwIPC");
    // Private implementation pattern
    //
    d = ServiceFwIPCFactory::createBackend(aBackend);
    d->q = this;
}

/*!
    Connect to the server.
    \param aServerName name of the server to connect to.
    \return true if connected, false if not.
*/
bool ServiceFwIPC::connect(const QString& aServerName)
{
    XQSERVICE_DEBUG_PRINT("ServiceFwIPC::connect");
    return d->connect(aServerName);
}

/*!
    Disconnect from the server.
*/
void ServiceFwIPC::disconnect()
{
    XQSERVICE_DEBUG_PRINT("ServiceFwIPC::disconnect");
    d->disconnect();
}

/*!
    Starts the service.
    \param aServerName Name of the server.
    \param aExeName Executable of the server.
    \return true if connected.
*/
bool ServiceFwIPC::startServer(const QString& aServerName,
                                  const QString& aExeName,
                                  quint64& processId,
                                  ServiceIPCStartServerOptions options)
{
    XQSERVICE_DEBUG_PRINT("ServiceFwIPC::startServer");
    return d->startServer(aServerName, aExeName, processId, options);
}

/*!
    Send a request synchronously.
    \param aRequestType Name of the request.
    \param aData Data to send.
    \return true if sent successful, otherwise false.
*/
bool ServiceFwIPC::sendSync(const QString& aRequestType,
                            const QByteArray& aData)
{
    XQSERVICE_DEBUG_PRINT("ServiceFwIPC::sendSync");
#ifdef _DEBUG
    Q_ASSERT_X( aRequestType.contains(";") == false, "", "aRequestType cannot contain semicolons!" );
#endif // _DEBUG
    bool sent = d->sendSync(aRequestType, aData);
    if (sent) {
        sent = waitForRead();
    }
    return sent;
}


/*!
    Send a request asynchronously.
    \param aRequestType Name of the request.
    \param aData Data to send.
    \note Errors will be emitted via error() signal.
*/
void ServiceFwIPC::sendAsync(const QString& aRequestType,
                             const QByteArray& aData)
{
    XQSERVICE_DEBUG_PRINT("ServiceFwIPC::sendAsync");
#ifdef _DEBUG
    Q_ASSERT_X( aRequestType.contains(";") == false, "", "aRequestType cannot contain semicolons!" );
#endif // _DEBUG
    d->sendAsync(aRequestType, aData);
    iAsyncRequestPending = true;
}

/*!
    Asynchronous read.
    \param aArray Array where read data will be put.
*/
void ServiceFwIPC::readAll(QByteArray& aArray)
{
    XQSERVICE_DEBUG_PRINT("ServiceFwIPC::readAll(1)");
    d->readAll(aArray);
}

/*!
    Synchronous read.
    \return Array which where read data is put.
*/
QByteArray ServiceFwIPC::readAll()
{
    XQSERVICE_DEBUG_PRINT("ServiceFwIPC::readAll(2)");
    return d->readAll();
}

/*!
    Waits until data is available for reading.
    \return true if data is available for reading.
*/
bool ServiceFwIPC::waitForRead()
{
    XQSERVICE_DEBUG_PRINT("ServiceFwIPC::waitForRead");
    return d->waitForRead();
}

/*!
    Check if an async request is already pending.
    \return true if an async request is pending,
            false otherwise.
*/
bool ServiceFwIPC::requestPending()
{
    XQSERVICE_DEBUG_PRINT("ServiceFwIPC::requestPending");
    return iAsyncRequestPending;
}

void ServiceFwIPC::setUserData(const void *data)
{
    XQSERVICE_DEBUG_PRINT("ServiceFwIPC::setUserData: %x", (int)data);
    d->setUserData(data);
}

}



/*!
    \fn QtService::ServiceFwIPC::error( int aError )
 
    Signal emitted to handle any errors.
    \param aError error code
    \note For local socket implementation, the error can be interpreted 
          as QLocalSocket::LocalSocketError
*/

/*!
    \fn QtService::ServiceFwIPC::readyRead()

    Handle when a reply has been received for async requests.
    Emitted when the entire data packet has been received
*/

/*!
    \fn QtService::ServiceFwIPC::ReadDone()

    Emitted when reading has completed.
*/
// END OF FILE
