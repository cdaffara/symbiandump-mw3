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
* Description: This is a private implementation of a plugin's description.
*
*/

#include "xqplugininfoprivate.h"

/*!
  Default constructor.
*/
XQPluginInfoPrivate::XQPluginInfoPrivate()
:
iUid(),
iVersion(),
iDllName(), 
iOpaqueData()
{
}

/*!
  Constructor.
*/
XQPluginInfoPrivate::XQPluginInfoPrivate(
        const XQPluginInfoPrivate &pluginInfoPrivate)
:
iUid(pluginInfoPrivate.uid()),
iVersion(pluginInfoPrivate.version()),
iDllName(pluginInfoPrivate.dllName()), 
iOpaqueData(pluginInfoPrivate.opaqueData())
{
}

/*!
  Constructor.
*/
XQPluginInfoPrivate::XQPluginInfoPrivate(quint32 id, 
                                         quint32 version, 
                                         const QString &dllName, 
                                         const QString &opaqueData)
:
iUid(id),
iVersion(version),
iDllName(dllName), 
iOpaqueData(opaqueData)
{
}

/*!
  Destructor.
*/
XQPluginInfoPrivate::~XQPluginInfoPrivate()
{
}
