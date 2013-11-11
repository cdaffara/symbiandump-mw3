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
* Description:  Class to represent a IPC request
*
*/

#ifndef XQ_SERVICEIPC_REQUEST_H
#define XQ_SERVICEIPC_REQUEST_H

#include <QtCore>
#include <QList>
#include <xqserviceipcdefs.h>
#include <xqrequestinfo.h>
#include <xqsharablefile.h>

namespace QtService {
        
    class ServiceIPCSession;
    class ClientInfo;
    
    class SFWIPCSRV_EXPORT ServiceIPCRequest : public QObject
    {
    public:
        virtual ~ServiceIPCRequest();
    
        ServiceIPCRequest(ServiceIPCSession* aSession,
                          qint64 aDataLength,
                          const QString& aRequestOp);
        
        ServiceIPCRequest(ServiceIPCRequest& aRequest);
        
        ServiceIPCRequest& operator=(ServiceIPCRequest& aRequest);
    
    public:
        // New function
    
        const QString& getOperation();
    
        const QByteArray& getData();
    
        bool write(const QByteArray& aData);
    
        bool completeRequest();
    
        bool addRequestdata(const QByteArray& aMoreData);

        void setClientInfo(ClientInfo *aClientInfo);

        ClientInfo* clientInfo();
        
        ServiceIPCSession* getSession();
        
        void setId(int id);
        
        int id() const;
        
        void setAsync(bool aAsync);
        
        bool isAsync() const;

        void setRequestInfo(XQRequestInfo *info);
        XQRequestInfo requestInfo() const;
        
        void addSharableFile(XQSharableFile *file, int index);
        XQSharableFile sharableFile(int index) const;
        
    private:
        ServiceIPCSession* iSession; // not owned
        ClientInfo *iClientInfo;  // owned
        QString iRequestOp;
        QByteArray iRequestData;
        qint64 iDatalength;
        int iId;
        bool iAsync;
        XQRequestInfo iRequestInfo;  // owned
        QList<XQSharableFile> iSharableFiles;  // owned
    };

}
#endif // XQ_SERVICEIPC_REQUEST_H
