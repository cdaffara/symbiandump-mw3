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
#include <xqserviceutil.h>

#include <hbapplication.h>
#include <hbserviceprovidermainwindow.h>

int main(int argc, char **argv)
{
    // qInstallMsgHandler(XQSERVICEMESSAGEHANDLER);
    XQSERVICE_DEBUG_PRINT("HbServiceProvider::main");

    XQSERVICE_DEBUG_PRINT("HbServiceProvider::main:isService (argv)=%d", XQServiceUtil::isService(argc,argv));
    XQSERVICE_DEBUG_PRINT("HbServiceProvider::main:interfaceName (argv)=%s", qPrintable(XQServiceUtil::interfaceName(argc,argv)));
    XQSERVICE_DEBUG_PRINT("HbServiceProvider::main:serviceName (argv)=%s", qPrintable(XQServiceUtil::serviceName(argc,argv)));
    XQSERVICE_DEBUG_PRINT("HbServiceProvider::main:isEmbbedded (argv)=%d", XQServiceUtil::isEmbedded(argc,argv));
    XQSERVICE_DEBUG_PRINT("HbServiceProvider::main:operationName (argv)=%s", qPrintable(XQServiceUtil::operationName(argc,argv)));
    
    HbApplication app( argc, argv ); 

    XQSERVICE_DEBUG_PRINT("HbServiceProvider::main:isService =%d", XQServiceUtil::isService());
    XQSERVICE_DEBUG_PRINT("HbServiceProvider::main:interfaceName=%s", qPrintable(XQServiceUtil::interfaceName()));
    XQSERVICE_DEBUG_PRINT("HbServiceProvider::main:serviceName =%s", qPrintable(XQServiceUtil::serviceName()));
    XQSERVICE_DEBUG_PRINT("HbServiceProvider::main:isEmbbedded =%d", XQServiceUtil::isEmbedded());
    XQSERVICE_DEBUG_PRINT("HbServiceProvider::main:operationName =%s", qPrintable(XQServiceUtil::operationName()));

    // Let's figure out why application was started?
    if(app.activateReason()== Hb::ActivationReasonActivity )
    {
        QString id = app.activateId();
        QVariant data = app.activateData();
        XQSERVICE_DEBUG_PRINT("HbServiceProvider::main Hb::ActivationReasonActivity=%s,data=%s",
                              qPrintable(id), qPrintable(data.toString()));
        QVariantHash params = app.activateParams();
        QHashIterator<QString, QVariant> i(params);
        while (i.hasNext())
        {
            i.next();
            XQSERVICE_DEBUG_PRINT("HbServiceProvider::param key=%s,value=%s",
                                  qPrintable(i.key()), qPrintable(i.value().toString()));
        }        
    }
    else if (app.activateReason() == Hb::ActivationReasonNormal)
    {
        XQSERVICE_DEBUG_PRINT("HbServiceProvider::main Hb::ActivationReasonNormal");
    }
    else if(app.activateReason() == Hb::ActivationReasonService)
    {
        // Does not come here: Bug MPAN-84QL7V entered for this
        XQSERVICE_DEBUG_PRINT("HbServiceProvider::main Hb::ActivationReasonService");
    }
    else
    {
        // This applicatiob does not support services
        XQSERVICE_DEBUG_PRINT("HbServiceProvider::main UNKNOWN reason");
    }
    
    HbServiceProviderMainWindow mainWindow;

    mainWindow.show();

    return app.exec();     
}

