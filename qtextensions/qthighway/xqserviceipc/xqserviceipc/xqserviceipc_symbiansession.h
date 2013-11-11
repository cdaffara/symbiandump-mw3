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
* Description:  Client session class to  server
*
*/

#ifndef C_XQSERVICEIPC_SYMBIANSESSION_H
#define C_XQSERVICEIPC_SYMBIANSESSION_H

#include <QtCore/qobject.h>
#include <e32std.h>
namespace QtService {
    
    class RServiceIPCSession : public RSessionBase
    {
    public:
        TInt Connect(const TDesC& aServer, const TVersion& aVersion);
    
        TVersion Version() const;
    
        TInt SendReceiveL(TInt aFunction) const;
    
        TInt SendReceiveL(TInt aFunction, const TIpcArgs& aArgs) const;
    
        void SendReceive(TInt aFunction, TRequestStatus& aStatus) const;
    
        void SendReceive(TInt aFunction,
                         const TIpcArgs& aArgs,
                         TRequestStatus& aStatus) const;
        
		TInt Send(TInt aFunction, const TIpcArgs& aArgs) const;
		
        TInt StartServer(const TDesC& aImage);
    
    private:
        TVersion iVersion;
    };
}
#endif // C_XQSERVICEIPC_SYMBIANSESSION_H
// End of file
