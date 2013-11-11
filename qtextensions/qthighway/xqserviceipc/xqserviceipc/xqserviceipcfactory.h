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
* Description:  Factory class to construct the proper IPC backend
*
*/

#ifndef XQ_SERVICE_FW_IPC_FACTORY
#define XQ_SERVICE_FW_IPC_FACTORY

#include <QtCore>
#include "xqserviceipcdefs.h"

namespace QtService {
    
    class ServiceFwIPCPrivate;
    
    class SFWIPC_EXPORT ServiceFwIPCFactory : public QObject
    {
    public:
    
        ServiceFwIPCFactory();
    
        virtual ~ServiceFwIPCFactory();
    
    public:
    
        static bool isIPCTypeSupported(TServiceIPCBackends aIPCType);
    
        static ServiceFwIPCPrivate* createBackend(TServiceIPCBackends aBackend =
                                                                     EDefaultIPC);
    
    private:
        // data
    
    };

}
#endif // XQ_SERVICE_FW_IPC_FACTORY
