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

#include "xqservicelog.h"
#include <hbapplication.h>
#include "hbserviceclientmainwindow.h"

int main(int argc, char **argv)
{
    // qInstallMsgHandler(XQSERVICEMESSAGEHANDLER);
    XQSERVICE_DEBUG_PRINT("HbServiceClient::main");
    HbApplication app( argc, argv ); 

    
    HbServiceClientMainWindow mainWindow;

    // Let's figure out why application was started?
    if(app.activateReason()== Hb::ActivationReasonActivity )
    {
        QString id = app.activateId();
        QVariant data = app.activateData();
        XQSERVICE_DEBUG_PRINT("HbServiceClient::main Hb::ActivationReasonActivity=%s,data=%s",
                             qPrintable(id), qPrintable(data.toString()));
        QVariantHash params = app.activateParams();
        QHashIterator<QString, QVariant> i(params);
        while (i.hasNext())
        {
            i.next();
            XQSERVICE_DEBUG_PRINT("HbServiceClient::param key=%s,value=%s",
                                  qPrintable(i.key()), qPrintable(i.value().toString()));
        }        
    }
    else if (app.activateReason() == Hb::ActivationReasonNormal)
    {
        XQSERVICE_DEBUG_PRINT("HbServiceClient::main Hb::ActivationReasonNormal");
    }
    else if(app.activateReason() == Hb::ActivationReasonService)
    {
        // This applicatiob does not support services
        XQSERVICE_DEBUG_PRINT("HbServiceClient::main Hb::ActivationReasonService");
    }
    else
    {
        // This applicatiob does not support services
        XQSERVICE_DEBUG_PRINT("HbServiceClient::main UNKNOWN reason");
    }
        
    
    mainWindow.show();

    return app.exec();     
}

