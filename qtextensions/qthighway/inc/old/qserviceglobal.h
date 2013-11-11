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

#ifndef QSERVICEFRAMEWORKGLOBAL_H
#define QSERVICEFRAMEWORKGLOBAL_H

#include <QtCore/qglobal.h>

#if defined(Q_OS_WIN)
#  if defined(QT_NODLL)
#    undef QT_MAKEDLL
#    undef QT_DLL
#  elif defined(QT_MAKEDLL)
#    if defined(QT_DLL)
#      undef QT_DLL
#    endif
#    if defined(QT_BUILD_SFW_LIB)
#      define Q_SFW_EXPORT Q_DECL_EXPORT
#    else
#      define Q_SFW_EXPORT Q_DECL_IMPORT
#    endif
#  elif defined(QT_DLL) /* use a Qt DLL library */
#    define Q_SFW_EXPORT Q_DECL_IMPORT
#  endif
#else
#endif

#if !defined(Q_SFW_EXPORT)
#  if defined(QT_SHARED)
#    define Q_SFW_EXPORT Q_DECL_EXPORT
#  else
#    define Q_SFW_EXPORT
#  endif
#endif

QT_BEGIN_HEADER
QT_BEGIN_NAMESPACE
//TODO remove
static const char DefaultVersion[]          = "-1"; //Get lastest version, when the default value being set

QT_END_NAMESPACE
QT_END_HEADER

#endif // QSERVICEFRAMEWORKGLOBAL_H
