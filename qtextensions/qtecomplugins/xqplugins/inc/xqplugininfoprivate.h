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
* Description:  Private implementation of plugin description
*
*/

#ifndef XQPLUGININFOPRIVATE_H_
#define XQPLUGININFOPRIVATE_H_

#include <QString>
#include <QtGlobal>

class XQPluginInfoPrivate
{
public:
    XQPluginInfoPrivate();
    
    XQPluginInfoPrivate(const XQPluginInfoPrivate &pluginInfoPrivate);
    
    XQPluginInfoPrivate(quint32 id, quint32 version, const QString &dllName, const QString &opaqueData);
    
    ~XQPluginInfoPrivate();
    
    inline quint32 uid() const;
    
    inline quint32 version() const;
    
    inline const QString& dllName() const;
    
    inline const QString& opaqueData() const;
    
private:
    const quint32 iUid;
    const quint32 iVersion;
    const QString iDllName;
    const QString iOpaqueData;
    
};

quint32 XQPluginInfoPrivate::uid() const { return iUid; }

quint32 XQPluginInfoPrivate::version() const { return iVersion; }

const QString& XQPluginInfoPrivate::dllName() const { return iDllName; }

const QString& XQPluginInfoPrivate::opaqueData() const { return iOpaqueData; }

#endif /* QPLUGININFOPRIVATE_H_ */
