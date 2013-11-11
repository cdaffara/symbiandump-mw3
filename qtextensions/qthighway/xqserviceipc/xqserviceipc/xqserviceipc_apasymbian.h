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
* Description:  Symbian client side implementation for APA SERVER IPC
*
*/

#ifndef C_XQSERVICEIPC_APASYMBIAN_H
#define C_XQSERVICEIPC_APASYMBIAN_H

#include <xqserviceipc_p.h>
#include <xqsharablefile.h>
#include "xqserviceipc_apasymbiansession.h"

namespace QtService {

    // Sharable file utility
    bool TransferSharableFile(TIpcArgs *args, const XQSharableFile *file);
    
    class CApaSymbianIPC : public CActive, public ServiceFwIPCPrivate,
                           public MApaServerAppExitObserver
        {
    
    public:
    
        virtual ~CApaSymbianIPC();
    
        static CApaSymbianIPC* NewL();
    
        // From ServiceFwIPCPrivate
    protected:
    
        bool connect(const QString& aServerName);
    
        void disconnect();
    
        bool startServer(const QString& aServerName, const QString& aExeName, quint64& processId, int options = 0);

        bool sendSync(const QString& aRequestType, const QByteArray& aData);
    
        void sendAsync(const QString& aRequestType, const QByteArray& aData);
    
        void readAll(QByteArray& aArray);
        
        QByteArray readAll();
    
        bool waitForRead();
    
        // From CActive
    protected:
    
        void RunL();
        
        TInt RunError(TInt err);
        
        void DoCancel();
        
        // From MApaServerAppExitObserver
    protected:
        /**
        * HandleServerAppExit
        * Gets called when server application exits
        */
        void HandleServerAppExit( int aReason );   
        
    private:
    
        enum TState
            {
            ESend,
            ERead,
            EDone
            };
        
        void doReadAllL(QByteArray& aArray);
        
        QByteArray doReadAllL();
    
        int doMapErrors( TInt aError );
        
    private:
    
        CApaSymbianIPC();
        
        void ConstructL();
        
        void StartExitMonitor();
        
    private:
        RApaIPCSession iSession;
        HBufC8* iAsyncData;  // member variable to keep alive any data for async ops
        TPtrC8 iAsyncDataPtr;
        HBufC* iRequestData;
        TPtrC  iRequestDataPtr;
        TInt iDataSize;
        TState iState;
        /**
        * Exit monitor
        */
        CApaServerAppExitMonitor* iServerExitMonitor; 
        QByteArray* iArray; //not owed
        RBuf8 iBuf;
    };
}
#endif // C_XQSERVICEIPC_APASYMBIAN_H
