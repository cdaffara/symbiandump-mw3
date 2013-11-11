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
* Description:  Observer class used for callback of async requests
*
*/

#ifndef M_XQSERVICE_IPC_OBSERVER
#define M_XQSERVICE_IPC_OBSERVER

#include <QtCore>

namespace QtService {
    
    class ServiceIPCRequest;
    
    /*!
        \brief Interface Observer class to handle IPC related events
    */
    class MServiceIPCObserver
    {
    public:

        /*!
            Handle a new request.
            \param aRequest Request object, ownership NOT passed.
            \return true if handled.
        */
        virtual bool handleRequest(ServiceIPCRequest *aRequest) = 0;
    
        /*!
            Handle cancellation of the given request object.
            \param aRequest Request object that is going to be cancelled, ownership NOT passed.
        */
        virtual void handleCancelRequest(ServiceIPCRequest *aRequest) = 0;

        /*!
            About to delete the given request object. Observer can make necessary cleanup.
            \param aRequest Request object that is going to be cancelled, ownership NOT passed.
                            Object will be deleted after callback returns.
        */
        virtual void handleDeleteRequest(ServiceIPCRequest *aRequest) = 0;
        
    };

}
#endif // M_XQSERVICE_IPC_OBSERVER
