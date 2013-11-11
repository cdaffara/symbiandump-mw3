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
* Description: This class implements a plugin's description (metadata).
*
*/

#ifndef XQPLUGININFO_H_
#define XQPLUGININFO_H_

class XQPluginInfoPrivate;

#include <QtGlobal>

#ifdef BUILD_XQPLUGINS_DLL
#define DLL_EXPORT Q_DECL_EXPORT
#else
#define DLL_EXPORT Q_DECL_IMPORT
#endif

class DLL_EXPORT XQPluginInfo
    {
public:

    XQPluginInfo();
    XQPluginInfo(const XQPluginInfo &pluginInfo);
    XQPluginInfo(quint32 uid, quint32 version, const QString &dllName, const QString &opaqueData);
    ~XQPluginInfo();
    XQPluginInfo &operator=(const XQPluginInfo &other);
    quint32 uid() const;
    quint32 version() const;
    const QString& dllName() const;
    const QString& opaqueData() const;
    
private:

    XQPluginInfoPrivate *d_ptr;
    };
	
#endif /* XQPLUGININFO_H_ */
