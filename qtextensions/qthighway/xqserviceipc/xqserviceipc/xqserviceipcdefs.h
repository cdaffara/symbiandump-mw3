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

#ifndef XQSERVICEIPCDEFS_H
#define XQSERVICEIPCDEFS_H

#include <Qt/qglobal.h>

namespace QtService {

#ifdef QT_MAKE_IPC_DLL
    # define SFWIPC_EXPORT Q_DECL_EXPORT
#else
    # define SFWIPC_EXPORT Q_DECL_IMPORT
#endif

#ifdef QT_MAKE_IPC_SERVER_DLL
    # define SFWIPCSRV_EXPORT Q_DECL_EXPORT
#else
    # define SFWIPCSRV_EXPORT Q_DECL_IMPORT
#endif

enum TServiceIPCBackends
    {
    EDefaultIPC, 
    ESymbianServer, 
    ESymbianApaServer,
    };

static const char FUNCTIONSEM[] = "_funclock";
static const char SERVERSEM[] = "_serverlock";
static const char SERVERNOTSTARTED[] = "0";
static const char SERVERSTARTED[] = "1";

}
#endif // XQSERVICEIPCDEFS_H
