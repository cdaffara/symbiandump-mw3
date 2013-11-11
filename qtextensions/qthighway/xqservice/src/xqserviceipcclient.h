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

#ifndef XQSERVICECLIENT_H
#define XQSERVICECLIENT_H

#include <QtCore/qobject.h>

#include <xqserviceipcobserver.h>
#include <xqrequestinfo.h>

namespace QtService {
class MServiceIPCObserver;
class ServiceIPCRequest;
class ServiceFwIPCServer;
class ServiceFwIPC;
}
using namespace QtService;

#include <QtCore/qshareddata.h>
#include <QtCore/qregexp.h>

class QEventLoop;
class XQServiceChannel;
class XQServiceRequestCompletedAsync;
class XQServiceProvider;

#define	XQServiceCmd_Send		        	1
#define XQServiceCmd_ReturnValueDelivered   2

class XQServiceIpcClient : public QObject,
                           public MServiceIPCObserver
{
    Q_OBJECT
public:
    XQServiceIpcClient(const QString& ipcConName, bool isServer, 
                       bool isSync, XQServiceRequestCompletedAsync* rc,
                       const void *userData);
    ~XQServiceIpcClient();

    bool listen();
    bool connectToServer();

    bool sendChannelCommand(int cmd, const QString& ch);
    bool send(const QString& ch, const QString& msg, const QByteArray& data, QByteArray &retData,
              int cmd = XQServiceCmd_Send);
    bool cancelPendingSend(const QString& ch);
    
    bool isServer() const { return server; }

    int setCurrentRequest(ServiceIPCRequest* request);
    int setCurrentRequestAsync();
    bool completeRequest(int index, const QVariant& retValue);
    bool cancelRequest(ServiceIPCRequest* request);
    XQRequestInfo requestInfo() const;
    bool setRequestInfo(XQRequestInfo &info);
    
signals:

public slots:
    void disconnected();
    
private slots:
    // ServiceFwIPC error
    void clientError(int);
    void readyRead();
    void readDone();
    
private: // from MServiceIPCObserver
    bool handleRequest(ServiceIPCRequest *aRequest);
    void handleCancelRequest(ServiceIPCRequest* aRequest);
    void handleDeleteRequest(ServiceIPCRequest* aRequest);
    bool isEmbedded();
    
private:    
    void wait(int msec);
    ServiceIPCRequest *requestPtr(int index) const; 
    
private:
    bool server;
    bool synchronous;
    int retryCount;
    
    QEventLoop* sendSyncLoop;

    QString mIpcConName ;

    ServiceIPCRequest* cancelledRequest; 
    
    ServiceFwIPC* serviceIpc;
    ServiceFwIPCServer* serviceIpcServer;    
    
    static const int minPacketSize = 256;
    static const int retryToServerMax = 50;
    
    char outBuffer[minPacketSize];
    XQServiceRequestCompletedAsync* callBackRequestComplete;
    QObject* plugin;
    XQServiceProvider* localProvider;
	
	QByteArray iRetData;
    const void *mUserData;
    
    int lastId;
    QHash<int, ServiceIPCRequest*> requestsMap;
    
};

#endif
