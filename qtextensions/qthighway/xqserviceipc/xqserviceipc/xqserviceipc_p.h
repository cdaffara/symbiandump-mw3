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
* Description:  Service Framework Secure IPC Private implementation base class
*
*/

#ifndef XQ_SERVICEIPC_PRIVATE_H
#define XQ_SERVICEIPC_PRIVATE_H

#include <QtCore>
#include <QByteArray>
#include "xqserviceipc.h"

namespace QtService {

    /*!
        \class ServiceFwIPCPrivate
        \brief Private implementation class for IPC
    */
    class ServiceFwIPC;
    class ServiceFwIPCPrivate
    {
    public:
        /*!
            Virtual destructor
        */
        virtual ~ServiceFwIPCPrivate() {};

    
        // Virtual functions 
    
        /*!
            Connect to the server.
            \param aServerName Name of the server to connect to.
            \return true if connected, false if not.
        */
        virtual bool connect(const QString& aServerName) = 0;
    
        /*!
            Disconnect from the server.
        */
        virtual void disconnect() = 0;
    
        /*!
            Starts the service.
            \note Refer to public API ServiceFwIPC.
        */
        virtual bool startServer(const QString& aServerName,
                                 const QString& aExeName,
                                 quint64& processId,
                                 int options = 0 ) = 0;
    
        /*!
            Send a request synchronously.
            \note Refer to public API ServiceFwIPC.
        */
        virtual bool sendSync(const QString& aRequestType,
                              const QByteArray& aData) = 0;
    
        /*!
            Send a request asynchronously.
            \note Refer to public API ServiceFwIPC.
        */
        virtual void sendAsync(const QString& aRequestType,
                               const QByteArray& aData) = 0;
    
        /*!
            Reads all data pending in the buffer.
            \note Refer to public API ServiceFwIPC.
        */
        virtual void readAll(QByteArray& aArray) = 0;
        
        virtual QByteArray readAll() = 0;
		
        /*!
            Waits until data is available for reading.
            \note Refer to public API ServiceFwIPC.
        */
        virtual bool waitForRead() = 0;

        virtual void setUserData(const void *data) {userData = data;};
        
    protected:
        // TODO: Move to inl file
        inline void emitReadyRead()
        {
            if (q->iAsyncRequestPending) {
                q->iAsyncRequestPending = false;
                emit q->readyRead();
            }
        }
    
        inline void emitError(int aError)
        {
            emit q->error(aError);
        }
        
        inline void emitReadDone()
        {
            emit q->ReadDone();
        }
        
        inline bool asyncPending()
        {
            return q->requestPending();
        }

    protected:
        const void *userData;  // not owned
        
    private:
        friend class ServiceFwIPC;
        ServiceFwIPC* q;  // not owned
    };

}
#endif // XQ_SERVICEIPC_PRIVATE_H
