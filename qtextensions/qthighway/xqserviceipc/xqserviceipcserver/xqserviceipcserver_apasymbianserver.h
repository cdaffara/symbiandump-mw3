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
* Description:  Symbian implementation for IPC backend
*
*/

#ifndef C_XQSERVICEIPCSERVER_APA_SYMBIAN_SERVER_H
#define C_XQSERVICEIPCSERVER_APA_SYMBIAN_SERVER_H

#include <QtCore/qobject.h>
#include <e32base.h>
#include <QtCore>
#include "xqserviceipcserver.h"
#include "xqserviceipcserver_p.h"

#include <apaserverapp.h>

#include <XQSettingsManager>

class CApaServerSymbianSession;
class XQPublishAndSubscribeKey;

namespace QtService {
    class MServiceIPCObserver;
    
    class CApaSymbianServer : public CApaAppServer,
                      public ServiceFwIPCServerPrivate 
    {
    public:
    
        static CApaSymbianServer* NewL();
    
        virtual ~CApaSymbianServer();

        CPolicyServer::TCustomResult CreateServiceSecurityCheckL(
                                                                TUid aServiceType,
                                                                const RMessage2& aMsg,
                                                                TInt& aAction,
                                                                TSecurityInfo& aMissing );

        // From ServiceFwIPCServerPrivate
    protected:
    
        bool listen(const QString& aServerName);
    
        void disconnect();
    
        // From CPolicyServer
    protected:
    
        CSession2* NewSessionL(const TVersion& aVersion,
                               const RMessage2& aMessage) const;
    
    private:
    
        CApaSymbianServer();
    
        void ConstructL();
 
    private:
        // data
        MServiceIPCObserver* iObserver;
        
        XQSettingsManager settingsManager;
        XQPublishAndSubscribeSettingsKey *stateNotifyKey;
        
    };
}
#endif // C_XQSERVICEIPCSERVER_APA_SYMBIAN_SERVER_H
