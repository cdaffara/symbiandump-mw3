/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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

#include "xqserviceipcclient.h"
#include "xqservicechannel.h"
#include "xqservicethreaddata.h"
#include "xqrequestutil.h"

#include <xqserviceutil.h>
#include <xqserviceipc.h>
#include <xqserviceipcserver.h>
#include <xqserviceipcrequest.h>
#include <QProcess>
#ifdef QT_S60_AIW_PLUGIN
#include <xqplugin.h>
#include <xqpluginloader.h>
#include <xqplugininfo.h>
#endif
#include <QList>
#include <xqserviceprovider.h>
#include <e32err.h>

#include <xqsharablefile.h>

struct XQServicePacketHeader
{
    int totalLength;
    int command;
    int chLength;
    int msgLength;
    int dataLength;
};

XQServiceIpcClient::XQServiceIpcClient(const QString& ipcConName, bool isServer, 
                                       bool isSync, XQServiceRequestCompletedAsync* rc,
                                      const void *userData)
    : QObject(), cancelledRequest(NULL), serviceIpc(NULL), serviceIpcServer(NULL), callBackRequestComplete(rc),
      mUserData(userData)  // User data can be NULL !
{
    XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::XQServiceIpcClient");
    XQSERVICE_DEBUG_PRINT("ipcConName: %s, isServer: %d, isSync: %d", qPrintable(ipcConName), isServer, isSync);
    XQSERVICE_DEBUG_PRINT("userData: %x, (int)userData)");

	
	mIpcConName = ipcConName;
    server = isServer;
    synchronous = isSync ;
    sendSyncLoop = NULL;
	
	// Incomplete in-process plugin support (just null data members)
    plugin=NULL;
    localProvider=NULL;
    lastId = 0;  // Start IDs from 1
    
#ifdef QT_S60_AIW_PLUGIN
    QList<XQPluginInfo> impls;  
    XQPluginLoader pluginLoader;
         
    pluginLoader.listImplementations(ipcConName, impls);
    if (impls.count()) {
        pluginLoader.setUid(impls.at(0).uid()); 
		// Use the very first plugin found, otherwise impl. ui need to be passed here
		plugin = pluginLoader.instance();
    }
#endif
}

XQServiceIpcClient::~XQServiceIpcClient()
{
    XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::~XQServiceIpcClient");
#ifdef QT_S60_AIW_PLUGIN
	delete plugin;
    delete localProvider;
#endif
//    disconnected();
}

bool XQServiceIpcClient::listen()
{
    XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::listen,isServer?=%d", server);
#    
#ifdef QT_S60_AIW_PLUGIN
    if (plugin) return true;
#endif

    if (server) {
        serviceIpcServer = new ServiceFwIPCServer(this, this, ESymbianApaServer);
        bool embedded = XQServiceUtil::isEmbedded();
        XQSERVICE_DEBUG_PRINT("embedded: %d", embedded);
        QString conName = mIpcConName;
        if (embedded) {
            // For embedded launch ass the server app ID to connection name
            // The client side will check the same embedded options and use the
            // same pattern
            quint64 processId = qApp->applicationPid();
            conName = mIpcConName + "." + QString::number(processId);
        }
        XQSERVICE_DEBUG_PRINT("conName: %s", qPrintable(conName));
        return serviceIpcServer->listen(conName);
    }
    XQSERVICE_DEBUG_PRINT("No server");
    return false;
}

bool XQServiceIpcClient::connectToServer()
{
    XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::connectToServer, isServer?=%d", server);
#ifdef QT_S60_AIW_PLUGIN
	if (plugin)
        {
        localProvider= new XQServiceProvider( mIpcConName, NULL);
        localProvider->SetPlugin(plugin);
        // localProvider->publishAll();  
        return true;
        }
#endif

    // Attension.
    // The 'mIpcConName' may contai the unique session identifier to separate connections using the same
    // By default server name is the same as the channel name.
    // When embedded launch, we add the server process ID to name to make it unique
    QString serverName = XQRequestUtil::channelName(mIpcConName);
    
    if (!serviceIpc) {
        XQSERVICE_DEBUG_PRINT("New serviceIpc:mIpcConName=%s,serverName=%s",
                              qPrintable(mIpcConName), qPrintable(serverName));
        
        serviceIpc = new ServiceFwIPC(this, ESymbianApaServer);
        serviceIpc->setUserData(mUserData); // Attach user data, if any, to request
        const XQRequestUtil *util = static_cast<const XQRequestUtil*>(mUserData);
        bool embedded = util->mInfo.isEmbedded();
        
        connect(serviceIpc, SIGNAL(error(int)), this, SLOT(clientError(int)));
        connect(serviceIpc, SIGNAL(readyRead()), this, SLOT(readyRead()));
        XQSERVICE_DEBUG_PRINT("\tembedded: %d", embedded);
        if (embedded) {
            quint64 processId=0;
            // Embedded server launch.
            // Server executable is always started with common name.
            // The server has to add the it's process ID to server names when creating Symbian server names to
            // be connected to. That's how client and server can establish unique connection.
            // 
            bool ret = serviceIpc->startServer(serverName,"", processId, ServiceFwIPC::EStartInEmbeddedMode);
            XQSERVICE_DEBUG_PRINT("ret: %d", ret);
            if (ret && (processId > 0)) {
                // 
                // Start application in embedded mode. Add process ID to server name to make
                // server connection unique.
                serverName  = serverName  + "." + QString::number(processId);
                XQSERVICE_DEBUG_PRINT("Try connect to embedded service: %s", qPrintable(serverName));
                retryCount = 0;
                while (!serviceIpc->connect(serverName) && retryCount < retryToServerMax) {
                    XQSERVICE_DEBUG_PRINT("retryCount: %d", retryCount+1);
                    ++retryCount;
                    wait(200);
                }
                if (retryCount == retryToServerMax) {
                    XQSERVICE_DEBUG_PRINT("Couldn't connect to embedded server");
                    XQService::serviceThreadData()->setLatestError(ServiceFwIPC::EConnectionError);  // Set error also
                    processId = 0;
                }
            }
            if (!processId) {
                XQSERVICE_WARNING_PRINT("Could not connect to embedded service %s", qPrintable(serverName));
                delete serviceIpc;
                serviceIpc = NULL;
                return false;                
            }
            XQSERVICE_DEBUG_PRINT("Embedded connection created");
        }
        else {
            // Not embedded 
            XQSERVICE_DEBUG_PRINT("Use existing serviceIpc:mIpcConName=%s, serverName=%s",
                                  qPrintable(mIpcConName), qPrintable(serverName));
            if (!serviceIpc->connect(serverName)) {
                XQSERVICE_DEBUG_PRINT("Trying to start server %s", qPrintable(serverName));
                quint64 processId=0;
                bool ret=serviceIpc->startServer(serverName,"",processId);
                XQSERVICE_DEBUG_PRINT("starServer ret=%d", ret);
                if (ret && (processId > 0)) {
                    retryCount = 0;
                    while (!serviceIpc->connect(serverName) && retryCount < retryToServerMax) {
                        XQSERVICE_DEBUG_PRINT("retryCount: %d", retryCount+1);
                        ++retryCount;
                        wait(200);
                    }
                    if (retryCount == retryToServerMax) {
                        XQSERVICE_DEBUG_PRINT("Couldn't connect to server");
                        XQService::serviceThreadData()->setLatestError(ServiceFwIPC::EConnectionError);  // Set error also
                        processId = 0;
                    }
                }
                if (!processId) {
                    XQSERVICE_WARNING_PRINT("Could not connect to the service %s", qPrintable(serverName));
                    delete serviceIpc;
                    serviceIpc = NULL;
                    return false;                
                }
            }
        XQSERVICE_DEBUG_PRINT("Connection created");
        }
    }
    return true;
}

bool XQServiceIpcClient::handleRequest( ServiceIPCRequest *aRequest )
{
    XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::handleRequest,isServer?=%d", server);
	XQService::serviceThreadData()->setLatestError(KErrNone);
    bool result(true);
    int index = setCurrentRequest(aRequest);
    XQSERVICE_DEBUG_PRINT("index: %d", index);
    const QString oper = aRequest->getOperation();
    XQSERVICE_DEBUG_PRINT("oper: %s", qPrintable(oper));
    const QByteArray packet=aRequest->getData();
    const char *startPtr = packet.constData();
    XQSERVICE_DEBUG_PRINT("packet: %s", packet.constData());
    
    
    // We have a full packet to be processed.  Parse the command
    // and the channel name, but nothing else just yet.
    XQServicePacketHeader *header = (XQServicePacketHeader *)startPtr;
    int command = header->command;
    XQSERVICE_DEBUG_PRINT("command: %d", command);
    QString channel;
    const char *ptr = startPtr + sizeof(XQServicePacketHeader);
    if (header->chLength > 0) {
        channel = QString::fromUtf16
            ((const ushort *)ptr, header->chLength);
        ptr += header->chLength * 2;
    }
    XQSERVICE_DEBUG_PRINT("channel: %s", qPrintable(channel));
    // Parse the rest of the packet now that we know we need it.
    QString msg;
    QByteArray data;
    if (header->msgLength > 0) {
        msg = QString::fromUtf16
            ((const ushort *)ptr, header->msgLength);
        ptr += header->msgLength * 2;
    }
    XQSERVICE_DEBUG_PRINT("msg: %s", qPrintable(msg));
    if (header->dataLength > 0) {
        data = QByteArray ((const char *)ptr, header->dataLength);
        ptr += header->dataLength;
    }
    XQSERVICE_DEBUG_PRINT("data: %s", data.constData());
    QVariant ret;
    // Processing command on server side.
    if (command == XQServiceCmd_Send) {    
        //Only support 1 sharable file, so index is 0
        ret=XQServiceChannel::sendLocally(channel, msg, data, aRequest->sharableFile(0) );
    }
    else if (command == XQServiceCmd_ReturnValueDelivered) {
       XQServiceChannel::sendCommand(channel,XQServiceChannel::ReturnValueDelivered);
    }

    if (XQService::serviceThreadData()->latestError() || 
        !aRequest->isAsync()) {
        ret=completeRequest(index,ret);
    }
    XQSERVICE_DEBUG_PRINT("ret: %d", result);
    return result;
}

/*!
 * From MServiceIPCObserver
 * \see MServiceIPCObserver::handleCancelRequest( ServiceIPCRequest *aRequest )
 */
void XQServiceIpcClient::handleCancelRequest(ServiceIPCRequest* aRequest)
{
    XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::handleCancelRequest isServer?=%d", server);
    if (server)
    {
        // Save the request to be cancelled if service provider wants to as
        // XQRequestInfo for details
        // Valid only upon sendCommand call
        cancelledRequest = aRequest; 
        
        //Attention! At the moment in server side channel name and connection name are the same
        // it might be that in the future will be different then this is not valid anymore.
        XQServiceChannel::sendCommand(mIpcConName,XQServiceChannel::ClientDisconnected);

        // Remember to reset immediatelly
        cancelledRequest = 0;
        
        cancelRequest(aRequest);
    }
}

/*
* From MServiceIPCObserver
* \see MServiceIPCObserver::handleDeleteRequest( ServiceIPCRequest *aRequest )
*/
void XQServiceIpcClient::handleDeleteRequest(ServiceIPCRequest* aRequest)
{
    XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::handleDeleteRequest isServer?=%d, reqId=", server);
    if (server)
    {
        cancelRequest(aRequest);
    }
}

bool XQServiceIpcClient::cancelRequest(ServiceIPCRequest* aRequest) {
    bool ret(false);
    
    if (aRequest == NULL)
        return ret;
    
    if (server) {
        XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::cancelRequest, isServer?=%d, reqId=%d", server, aRequest->id());
        if (requestsMap.contains(aRequest->id())) {
            requestsMap.take(aRequest->id());  // Use "take" not to delete the request !!
            ret = true;
        } else {
            ret = false;
        }
        XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::cancelRequest, ret=%d", ret);
    }
    return ret;
}

bool XQServiceIpcClient::sendChannelCommand(int cmd, const QString& ch)
{
    XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::sendChannelCommand, isServer?=%d", server);
    XQSERVICE_DEBUG_PRINT("cmd: %d, ch: %s", cmd, qPrintable(ch));
    if (!connectToServer()){
        XQSERVICE_DEBUG_PRINT("Couldn't connect to the server");
        return false;
    }
    int len = ch.length() * 2 + sizeof(XQServicePacketHeader);
    XQSERVICE_DEBUG_PRINT("cmd: %d", len);
    int writelen;
    char *buf;
    bool freeBuf = false;
    if (len <= minPacketSize) {
        buf = outBuffer;
        memset(buf + len, 0, minPacketSize - len);
        writelen = minPacketSize;
    } else {
        buf = new char [len];
        writelen = len;
        freeBuf = true;
    }
    XQSERVICE_DEBUG_PRINT("writelen: %d", writelen);
    XQServicePacketHeader *header = (XQServicePacketHeader *)buf;
    header->command = cmd;
    header->totalLength = len;
    header->chLength = ch.length();
    header->msgLength = 0;
    header->dataLength = 0;
    char *ptr = buf + sizeof(XQServicePacketHeader);
    memcpy(ptr, ch.constData(), ch.length() * 2);
    QByteArray sndBuf(buf,writelen);
    XQSERVICE_DEBUG_PRINT("sndBuf: %s", sndBuf.constData());
	
    bool ret = serviceIpc->sendSync("sendChannelCommand",sndBuf);   
    
    if (freeBuf)
        delete[] buf;
    
    XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::sendChannelCommand: ret=%d", ret);
    return ret;
}

bool XQServiceIpcClient::send(const QString& ch, 
                              const QString& msg, 
                              const QByteArray& data, 
                              QByteArray &retData, 
                              int cmd)
{
    XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::send, isServer?=%d", server);
    
    // Attension. The 'ch' name may contain unique session identifier to separate requests going
    // the same channel. Before real IPC calls need to get the normalized channel name.
    // The 'mIpcConName' contains the same session identifier to separate connections using the same
    // channel name.
    QString channel = XQRequestUtil::channelName(ch);
    
    XQSERVICE_DEBUG_PRINT("\tchannel: %s, msg: %s", qPrintable(channel), qPrintable(msg));
    XQSERVICE_DEBUG_PRINT("\tdata: %s, cmd: %d", data.constData(), cmd);
    
    XQService::serviceThreadData()->setLatestError(KErrNone);
    if (!connectToServer()){
        XQSERVICE_DEBUG_PRINT("\tCouldn't connect to the server");
        return false;
	}

#ifdef QT_S60_AIW_PLUGIN
    if (plugin) {     
        QVariant ret=XQServiceChannel::sendLocally(channel, msg, data);     
        retData = XQServiceThreadData::serializeRetData(ret, XQService::serviceThreadData()->latestError());
        return true;
        }
#endif
    int len = channel.length() * 2 + msg.length() * 2 + data.length();
    len += sizeof(XQServicePacketHeader);
    XQSERVICE_DEBUG_PRINT("\tcmd: %d", len);
    int writelen;
    char *buf;
    bool freeBuf = false;
    if (len <= minPacketSize) {
        buf = outBuffer;
        memset(buf + len, 0, minPacketSize - len);
        writelen = minPacketSize;
    } else {
        buf = new char [len];
        writelen = len;
        freeBuf = true;
    }
    XQSERVICE_DEBUG_PRINT("\twritelen: %d", writelen);
    XQServicePacketHeader *header = (XQServicePacketHeader *)buf;
    header->command = cmd;
    header->totalLength = len;
    header->chLength = channel.length();
    header->msgLength = msg.length();
    header->dataLength = data.length();
    char *ptr = buf + sizeof(XQServicePacketHeader);
    memcpy(ptr, channel.constData(), channel.length() * 2);
    ptr += channel.length() * 2;
    memcpy(ptr, msg.constData(), msg.length() * 2);
    ptr += msg.length() * 2;
    memcpy(ptr, data.constData(), data.length());
    QByteArray sndBuf(buf,writelen);
    XQSERVICE_DEBUG_PRINT("\tsndBuf: %s", sndBuf.constData());
    bool ret = true;
    XQSERVICE_DEBUG_PRINT("\tsynchronous: %d", synchronous);
    
    if (synchronous) {
       
        ret=serviceIpc->sendSync("send",sndBuf);
        if (ret) {
            retData=serviceIpc->readAll();
            XQSERVICE_DEBUG_PRINT("\t readAll done, error=%d", XQService::serviceThreadData()->latestError());
            if (!XQService::serviceThreadData()->latestError())
            {
                // No point to send channel command on error. Error could be also
                // caused by server exit without completing the actual request
                sendChannelCommand(XQServiceCmd_ReturnValueDelivered,channel);
            }
            else
                ret = false;
        }
    }
    else {
        // At the moment we can not have multiple send async 
        if (serviceIpc->requestPending()) {
            XQSERVICE_DEBUG_PRINT("Request already pending");
            XQService::serviceThreadData()->setLatestError(ServiceFwIPC::ERequestPending);  // maparnan
            ret = false ;
        }
        else {
            serviceIpc->sendAsync("send",sndBuf);
            ret = true;
        }
    }
    if (freeBuf)
        delete[] buf;
    XQSERVICE_DEBUG_PRINT("\tret: %d", ret);
    return ret;
}


/*!
 * This method cancels requests.
 */
bool XQServiceIpcClient::cancelPendingSend(const QString& ch)
{
    Q_UNUSED(ch);  // 
    XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::cancelPendingSend, isServer?=%d", server);
    if (serviceIpc) {
        // Close the client connection silently
        disconnect(serviceIpc, SIGNAL(error(int)), this, SLOT(clientError(int)));
        XQService::serviceThreadData()->closeClientConnection(mIpcConName);
        // No callback wanted any more
        callBackRequestComplete = NULL;
    }

    return true;
}

void XQServiceIpcClient::disconnected()
{
    XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::disconnected START");
    XQSERVICE_DEBUG_PRINT("\t server: %d, lastId=%d", server, lastId);
    if (server) {
        // Closing down IPC
        ServiceIPCRequest* request = NULL;

        //
        // Go through all requests and send disconnected error to them
        //
        QHashIterator<int, ServiceIPCRequest*> iter(requestsMap);
        while (iter.hasNext()) {
            iter.next();
            int reqId = iter.key();
            request=iter.value();
            XQSERVICE_DEBUG_PRINT("\t request iter: id=%d", reqId);
            XQSERVICE_DEBUG_PRINT("\t request iter requestAsync=%d", request->isAsync());
            if (request->isAsync()) {
                QVariant ret;
                // Consider server side end as connection closure.
                XQService::serviceThreadData()->setLatestError(ServiceFwIPC::EConnectionClosed);
                completeRequest(reqId, ret);
                // In disconnnect phase let's wait a bit in order to be sure 
                // That completeRequest and application exit notification event goes in the client side
                wait(200);
            }
        }
        if (serviceIpcServer) {
            serviceIpcServer->disconnect();
            delete serviceIpcServer;
            serviceIpcServer = NULL;
            XQSERVICE_DEBUG_PRINT("\tXQServiceIpcClient deleteLater");
            wait(200);
            XQSERVICE_DEBUG_PRINT("\tXQServiceIpcClient deleteLater over");
        }
    } else {
        if (sendSyncLoop && sendSyncLoop->isRunning()) {
            XQSERVICE_DEBUG_PRINT("Quit sendSyncLoop");
            sendSyncLoop->quit();
        }
        if (serviceIpc) {
            XQSERVICE_DEBUG_PRINT("Disconnect serviceIpc");
            serviceIpc->disconnect();
            delete serviceIpc;
            serviceIpc = NULL;
        }
    }
    deleteLater();
    XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::disconnected END");
}

void XQServiceIpcClient::clientError(int error)
{
    XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::clientError, isServer?=%d", server);
    XQSERVICE_DEBUG_PRINT("error: %d", error);

    
    if (serviceIpc) {
        XQService::serviceThreadData()->setLatestError(error);
        disconnect(serviceIpc, SIGNAL(error(int)), this, SLOT(clientError(int)));
        //disconnected();
        XQService::serviceThreadData()->closeClientConnection(mIpcConName);
    }
    
    //complete the client request with error value
    if (callBackRequestComplete) {
        XQSERVICE_DEBUG_PRINT("requestErrorAsync mapped error=%d", error);
       callBackRequestComplete->requestErrorAsync(error);
    }       
    XQSERVICE_DEBUG_PRINT("clientError end mapped error=%d", error);
}

/**
* Async read operation
*/

void XQServiceIpcClient::readyRead()
{
    XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::readyRead, isServer?=%d", server);

    // Clear error
    XQService::serviceThreadData()->setLatestError(KErrNone);
    
    //in case it has been connected before
    //this prevents calling twice of the callback functions
    disconnect(serviceIpc, SIGNAL(ReadDone()), this, SLOT(readDone()));
    connect(serviceIpc, SIGNAL(ReadDone()), this, SLOT(readDone()));
	serviceIpc->readAll( iRetData );
}


/**
* readDone, send return value back to client
*/
void XQServiceIpcClient::readDone()
    {
    XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::readDone");
    QVariant retValue = XQServiceThreadData::deserializeRetData(iRetData);

#ifdef XQSERVICE_DEBUG
    QString s = retValue.toString();
    int len=s.length();
    XQSERVICE_DEBUG_PRINT("retValue: type=%s,len=%d,value(max.1024)=%s",
                          retValue.typeName(),len,qPrintable(s.left(1024)));
#endif
    int err = XQService::serviceThreadData()->latestError();
    XQSERVICE_DEBUG_PRINT("err: %d", err);

    if (err)
        {
        XQSERVICE_DEBUG_PRINT("there is error!");
        //emitError(err);
        clientError(err);
        }
    else if (iRetData.length())
        {
        XQSERVICE_DEBUG_PRINT("there is return data");
        if (callBackRequestComplete && 
//         !retValue.isNull() && (retValue.type() != QVariant::Invalid))  maparnan
           retValue.isValid())
            {
            XQSERVICE_DEBUG_PRINT("before compelete async request");
            
            //should this send before compete the request ?
            //Attention ! Map mIpcConName name may contain unique identifier to separate connections using the same
            //            channel name. So need to get channel name.
            QString channel = XQRequestUtil::channelName(mIpcConName);
            sendChannelCommand(XQServiceCmd_ReturnValueDelivered, channel);
            
            callBackRequestComplete->requestCompletedAsync( retValue );
            XQSERVICE_DEBUG_PRINT("After complete async request");
            }        
        else
            {
            clientError( KErrUnknown );
            }
        //attention at the moment channel name and connection name are the same
        // it might be that in the future will be different then this is not valid anymore.
        //sendChannelCommand(XQServiceCmd_ReturnValueDelivered,mIpcConName);
        }
    else
        {
        //err is KErrNone but no return value
        //reading failed 
        clientError( KErrUnknown );
        }
    }


int XQServiceIpcClient::setCurrentRequest(ServiceIPCRequest* request)
{
    XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::setCurrentRequest START");
    XQSERVICE_DEBUG_PRINT("\t isServer=%d", server);
    int id = -1;
    if (request) {
        lastId = lastId + 1;
        XQSERVICE_DEBUG_PRINT("\t new id=%d assigned to current request", lastId);
        request->setAsync(false);
        request->setId(lastId);
        requestsMap.insert(lastId, request);
        id = lastId;
    } else {
        XQSERVICE_DEBUG_PRINT("\t request was NULL");
    }
    XQSERVICE_DEBUG_PRINT("\t returning id=%d", id);
    XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::setCurrentRequest END");
    return id;
}

//
// This function need to be called during a slot call before returning from
// the slot.
// The lastId might change after returning from the slot call as
// other possible requests may arrive
//
int XQServiceIpcClient::setCurrentRequestAsync()
{
    XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::setCurrentRequestAsync");
    XQSERVICE_DEBUG_PRINT("\t isServer=%d", server);
    ServiceIPCRequest* request = requestPtr(lastId);
    int id = -1;

    if (request) {
        request->setAsync(true);
        id = request->id();
    }
    
    XQSERVICE_DEBUG_PRINT("\t returning request's id=%d", id);
    return id;
}

bool XQServiceIpcClient::completeRequest(int index, const QVariant& retValue)
{
    XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::completeRequest START");
    XQSERVICE_DEBUG_PRINT("\t isServer=%d", server);
    XQSERVICE_DEBUG_PRINT("\t index=%d", index);
#ifdef XQSERVICE_DEBUG
    QString s = retValue.toString();
    int len=s.length();
    XQSERVICE_DEBUG_PRINT("retValue: type=%s,len=%d,value(max.1024)=%s",
                          retValue.typeName(),len,qPrintable(s.left(1024)));
#endif

    ServiceIPCRequest* request = requestPtr(index);
    if (!request){
        XQSERVICE_DEBUG_PRINT("\t request = NULL");
        XQSERVICE_DEBUG_PRINT("\t return false");
        XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::completeRequest END (1)");
        return false;
    }
#ifdef QT_S60_AIW_PLUGIN
    if (plugin) {
        if (callBackRequestComplete && 
                !retValue.isNull() && (retValue.type() != QVariant::Invalid))
                {
                callBackRequestComplete->requestCompletedAsync(retValue);
                }        
        XQSERVICE_DEBUG_PRINT("\t return true");
        XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::completeRequest END (2)");
        return true;
    }
#endif    
    QByteArray array = XQServiceThreadData::serializeRetData(retValue,
                            XQService::serviceThreadData()->latestError() );
    XQSERVICE_DEBUG_PRINT("\t array: %s", array.constData());
    XQService::serviceThreadData()->setLatestError(KErrNone);
    request->write(array);
    bool ret = request->completeRequest();
    // cancelRequest(request);
    XQSERVICE_DEBUG_PRINT("\t return %d", ret);
    XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::completeRequest END (3)");
    return ret;
}


//
// This function need to be called during a slot call before returning from
// the slot.
// The lastId might change after returning from the slot call as
// other possible requests may arrive
//
XQRequestInfo XQServiceIpcClient::requestInfo() const
{
    XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::requestInfo");
    XQSERVICE_DEBUG_PRINT("\t isServer=%d", server);
    ServiceIPCRequest* request = requestPtr(lastId);
    
    if (request) {
        return request->requestInfo();
    } 
    return XQRequestInfo();
}

//
// This internal function need to be called before a slot call to set the request info
// The provider can then call requestInfo() to get the data.
// The lastId might change after returning from the slot call as
// other possible requests may arrive
//
bool XQServiceIpcClient::setRequestInfo(XQRequestInfo &info)
{
    XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::setRequestInfo");
    XQSERVICE_DEBUG_PRINT("\t isServer=%d", server);
    ServiceIPCRequest* request = requestPtr(lastId);

    if (request) {
        request->setRequestInfo(&info);
    } 
    return request != NULL;
}


// in disconnnect phase let's wait a bit in order to be sure 
// that completeRequest and application exit notification event goes in the client side
void XQServiceIpcClient::wait(int msec)
{
    XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::wait, isServer?=%d", server);

    if (server)
        return;

    User::After(1000 * msec); //Contribution from Seppo

    /*
    if (!QCoreApplication::instance())
    {
        User::After(1000 * msec); //Contribution from Seppo
        return;
    }

    QTime t1 = QTime::currentTime();
    QEventLoop w = QEventLoop();
    XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::wait start");
    QTimer::singleShot(msec, &w, SLOT(quit()));
    w.exec();
    QTime t2 = QTime::currentTime();
    XQSERVICE_DEBUG_PRINT("XQServiceIpcClient::wait end elapsed=%d", t1.msecsTo(t2));
    */
}

//
// Returns the handle of the current request
//
ServiceIPCRequest *XQServiceIpcClient::requestPtr(int index) const
{
    ServiceIPCRequest* request = NULL;

    // If request is being cancelled (saved by handleCancelRequest()) use it's id instead
    // By that way service provider can access the XQRequestInfo of the cancelled request
    // Upon handling clientDisconnected
    if (cancelledRequest)
    {
        index = cancelledRequest->id();
        XQSERVICE_DEBUG_PRINT("\t Cancelled request id=%d", index);
    }
    
    if (requestsMap.contains(index)) {
        XQSERVICE_DEBUG_PRINT("\t request having id=%d FOUND", index);
        request = requestsMap[index];
    } else {
        XQSERVICE_DEBUG_PRINT("\t request having id=%d NOT FOUND", index);
    }
    
    return request;
    
}
