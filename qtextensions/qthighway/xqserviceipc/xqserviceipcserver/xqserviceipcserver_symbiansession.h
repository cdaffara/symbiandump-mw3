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
* Description:  IPC Server class using Symbian client-server
*
*/

#ifndef C_XQSERVICE_IPC_SYMBIAN_SESSION_H
#define C_XQSERVICE_IPC_SYMBIAN_SESSION_H

#include <QtCore/qobject.h>
#include <e32base.h>
#include <e32cmn.h> 
#include "xqserviceipcserversession.h"
#include "xqsharablefile.h"

namespace QtService
{

    // Utilities
    bool AdoptSharableFile(const RMessage2& aMsg, XQSharableFile *file);
    quint32 ClientCapabilities(const RMessage2& aMsg);
    
    class CServiceSymbianSession : public CSession2, public ServiceIPCSession
    {
    public:
    
        static CServiceSymbianSession* NewL(MServiceIPCObserver* aObserver);
    
        virtual ~CServiceSymbianSession();
    
        // From ServiceIPCSession
    protected:
    
        bool write(const QByteArray& aData);
    
        bool completeRequest();
    
        void close();
    
        // From CSession2
    protected:
    
        void ServiceL(const RMessage2& aMessage);
    
        void Disconnect(const RMessage2 &aMessage);
    
        // Service methods
    private:
    
        void handleRequestL(const RMessage2& aMessage);
    
        void handleGetBufferL(const RMessage2& aMessage);
    
        HBufC* ReadDesLC(const RMessage2& aMessage, TInt aMsgSlot);
    
        HBufC8* ReadDes8LC(const RMessage2& aMessage, TInt aMsgSlot);
    private:
    
        CServiceSymbianSession(MServiceIPCObserver* aObserver);
    
        void ConstructL();
    
    private:
        // data
        RMessage2 iMessage;
        QByteArray iData;
    };
}
#endif // C_XQSERVICE_IPC_SYMBIAN_SESSION_H
