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

#include <QApplication>
#include "serviceapp.h"

int main(int argc, char **argv)
{
    // qInstallMsgHandler(XQSERVICEMESSAGEHANDLER);
    XQSERVICE_DEBUG_PRINT("ServiceApp::main");
    XQSERVICE_DEBUG_PRINT("ServiceApp::main:isService (argv)=%d", XQServiceUtil::isService(argc,argv));
    XQSERVICE_DEBUG_PRINT("ServiceApp::main:interfaceName (argv)=%s", qPrintable(XQServiceUtil::interfaceName(argc,argv)));
    XQSERVICE_DEBUG_PRINT("ServiceApp::main:serviceName (argv)=%s", qPrintable(XQServiceUtil::serviceName(argc,argv)));
    XQSERVICE_DEBUG_PRINT("ServiceApp::main:isEmbbedded (argv)=%d", XQServiceUtil::isEmbedded(argc,argv));
    XQSERVICE_DEBUG_PRINT("ServiceApp::main:operationName (argv)=%s", qPrintable(XQServiceUtil::operationName(argc,argv)));
    
    QApplication a( argc, argv );

    XQSERVICE_DEBUG_PRINT("ServiceApp::main:isService =%d", XQServiceUtil::isService());
    XQSERVICE_DEBUG_PRINT("ServiceApp::main:interfaceName=%s", qPrintable(XQServiceUtil::interfaceName()));
    XQSERVICE_DEBUG_PRINT("ServiceApp::main:serviceName =%s", qPrintable(XQServiceUtil::serviceName()));
    XQSERVICE_DEBUG_PRINT("ServiceApp::main:isEmbbedded =%d", XQServiceUtil::isEmbedded());
    XQSERVICE_DEBUG_PRINT("ServiceApp::main:operationName =%s", qPrintable(XQServiceUtil::operationName()));
    
    ServiceApp *cl = new ServiceApp();
    cl->show();
    int rv = a.exec();
    delete cl;
    return rv;
}

