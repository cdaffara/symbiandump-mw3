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

#ifndef SETTINGSMANAGER_GLOBAL_H
#define SETTINGSMANAGER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(XQSETTINGSMANAGER_LIBRARY)
#  define XQSETTINGSMANAGER_EXPORT Q_DECL_EXPORT
#else
#  if defined(XQSETTINGSMANAGER_NO_LIBRARY)
#    define XQSETTINGSMANAGER_EXPORT
#  else
#    define XQSETTINGSMANAGER_EXPORT Q_DECL_IMPORT
#  endif
#endif

#endif // SETTINGSMANAGER_GLOBAL_H
