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

#include <QApplication>
#include "serviceclient.h"

int main(int argc, char **argv)
{
    // qInstallMsgHandler(XQSERVICEMESSAGEHANDLER);
    XQSERVICE_DEBUG_PRINT("ServiceClient::main");
    QApplication a( argc, argv );
    ServiceClient *cl = new ServiceClient();
    cl->show();
    int rv = a.exec();
    delete cl;
    return rv;
}

