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

#ifndef XQREQUESTUTIL_H
#define XQREQUESTUTIL_H

#include <QList>
#include <QVariant>

#include <xqserviceglobal.h>
#include <xqaiwinterfacedescriptor.h>
#include "xqrequestinfo.h"
#include "xqsharablefile.h"


//
// XQRequestUtil is internal helper class for starting QtHigway service app
// and for request handling
//
// Option key names for XQRequestInfo

namespace XQServiceUtils
{
    static const char * OptEmbedded= "XQEmb";    // Option Embedded (off=Non-Embedded)
    static const char * OptBackground= "XQBg";   // Option Service to Background (missing=no changes in Z-order)
    static const char * OptForeground= "XQFg";   // Set service app to foreground (missing=no changes in Z-order)
    static const char * OptSynchronous= "XQSync"; // Option Syncronous / Asynchronous call
    static const char * InfoSID= "XQSid"; // Client secure ID
    static const char * InfoVID= "XQVid"; // Client vendor ID
    static const char * InfoCap= "XQCap"; //Client cap
    static const char * InfoId= "XQId"; // Request ID
    
    // Startup arguments
    static const char * StartupArgEmbedded = "embedded=yes";
    static const char * StartupArgService = "service=yes";
    static const char * StartupArgInterfaceName = "intf=";
    static const char * StartupArgOperationName = "oper=";
    static const char * StartupArgServiceName = "xqsn=";
    
    // Observes if client is still running and close embedded view when client closed
    XQSERVICEUTIL_EXPORT void closeWhenClientClosed();
}


class XQSERVICEUTIL_EXPORT XQRequestUtil
{
    public:
        XQRequestUtil() ;
        virtual ~XQRequestUtil() ;

    // Helper to set the option OptSynchronous 
    void setSynchronous(bool on);
    void setSecurityInfo(const quint32 sid, const quint32 vid, const quint32 caps );
    const XQSharableFile *getSharableFile(int index) const;
    static int mapError(int error);
    static QString channelName(const QString &connectionName);
        
    public:
        XQAiwInterfaceDescriptor mDescriptor;  // Contains e.g. service and interface name 
        XQRequestInfo mInfo;
        QList<XQSharableFile> mSharableFileArgs;   // Use list even though one file possible to transfer
        QString mOperation;  // Operation (message) wanted
};

#endif
