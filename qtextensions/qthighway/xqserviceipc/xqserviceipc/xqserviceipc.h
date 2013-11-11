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
* Description:  Service framework IPC Client side API
*
*/

#ifndef XQ_SERVICEIPC_CLIENT_H
#define XQ_SERVICEIPC_CLIENT_H

#include <QtCore>
#include "xqserviceipcdefs.h"
#include <xqserviceerrdefs.h>

namespace QtService {
    
    class ServiceFwIPCPrivate;
    
    class SFWIPC_EXPORT ServiceFwIPC : public QObject
    {
    Q_OBJECT
    public:

        //! ServiceFwIPC::ServiceIPCErrors
        /*!
         This enum describes the errors that may be returned by the IPC.
         */
        enum ServiceIPCErrors
            {
            EConnectionError = IPC_ERR_START_VALUE, /*!< Error in IPC Connection */
            EConnectionClosed,                      /*!< IPC Connection is closed */
            EServerNotFound,                        /*!< Can not find server */
            EIPCError,                              /*!< Known IPC error defined by SDK */
            EUnknownError,                          /*!< Unknown IPC error */
            ERequestPending,                        /*!< Already pending request */
            EMessageNotFound,                       /*!< Message slot not found */
            EArgumentError                          /*!< Argument error */
            };

        enum ServiceIPCStartServerOptions
            {
            EStartInNormalMode = 0,
            EStartInEmbeddedMode = 1
            };

    public:
        virtual ~ServiceFwIPC();
    
        ServiceFwIPC(QObject* aParent, TServiceIPCBackends aBackend = EDefaultIPC);
    
    public:
        bool connect(const QString& aServerName);
    
        void disconnect();
    
        bool startServer(const QString& aServerName, const QString& aExeName, quint64& processId, ServiceIPCStartServerOptions options = EStartInNormalMode);
    
        bool sendSync(const QString& aRequestType, const QByteArray& aData);
    
        void sendAsync(const QString& aRequestType, const QByteArray& aData);
        
	        
        //async
        void readAll(QByteArray& aArray);
        
        //sync
        QByteArray readAll();
    
        bool requestPending();

        void setUserData(const void *data);
    
    private:
        bool waitForRead();
    
        signals:
    
        void error(int aError);
    
        void readyRead();
        
        void ReadDone();
    
        // Member Variables
    protected:
        bool iAsyncRequestPending;

        friend class ServiceFwIPCPrivate;
        ServiceFwIPCPrivate* d;
    };

}
#endif // XQ_SERVICEIPC_CLIENT_H
