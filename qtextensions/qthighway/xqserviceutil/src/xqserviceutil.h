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

#ifndef XQSERVICEUTIL_H
#define XQSERVICEUTIL_H

#include <xqserviceglobal.h>

/*!
    \brief Provides static utility functions for service provider (used also by QtHighway impl.)
*/
namespace XQServiceUtil
{
    // Utility to set started application to background
    XQSERVICEUTIL_EXPORT void toBackground(bool value);
    
    // Utilities to extract startup command line arguments
    XQSERVICEUTIL_EXPORT bool isEmbedded();  // 
    XQSERVICEUTIL_EXPORT bool isService();
    XQSERVICEUTIL_EXPORT QString interfaceName();
    XQSERVICEUTIL_EXPORT QString operationName();
    XQSERVICEUTIL_EXPORT bool isEmbedded(int argc, char **argv);
    XQSERVICEUTIL_EXPORT bool isService(int argc, char **argv);
    XQSERVICEUTIL_EXPORT QString interfaceName(int argc, char **argv);
    XQSERVICEUTIL_EXPORT QString operationName(int argc, char **argv);
    XQSERVICEUTIL_EXPORT QString serviceName();
    XQSERVICEUTIL_EXPORT QString serviceName(int argc, char **argv);
    XQSERVICEUTIL_EXPORT quint32 serviceIdFromName(const char *serviceName);
}

#endif
