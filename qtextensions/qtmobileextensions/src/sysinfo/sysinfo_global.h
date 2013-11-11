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

#ifndef SYSINFO_GLOBAL_H
#define SYSINFO_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(XQSYSINFO_LIBRARY)
#  define XQSYSINFO_EXPORT Q_DECL_EXPORT
#else
#  if defined(XQSYSINFO_NO_LIBRARY)
#    define XQSYSINFO_EXPORT
#  else
#    define XQSYSINFO_EXPORT Q_DECL_IMPORT
#  endif
#endif

#endif // SYSINFO_GLOBAL_H
