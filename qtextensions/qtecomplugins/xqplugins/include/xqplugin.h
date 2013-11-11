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

#ifndef XQPLUGIN_H
#define XQPLUGIN_H

#include <xqpluginfactory.h>
#include <ecom/implementationproxy.h>

#  define XQ_PLUGIN_ECOM_HEADER(PLUGIN) \
        class C##PLUGIN##Factory : public CPluginFactory { \
        public: \
            static C##PLUGIN##Factory * NewL(); \
            QObject * constructPlugin(); \
        }; \


#  define XQ_PLUGIN_ECOM_IMPL(PLUGIN, PLUGINCLASS) \
    C##PLUGIN##Factory * C##PLUGIN##Factory::NewL() {return new(ELeave) C##PLUGIN##Factory;} \
    QObject* C##PLUGIN##Factory::constructPlugin(){ return new PLUGINCLASS;}

// convenience macro
#  define XQ_EXPORT_PLUGIN(PLUGIN, PLUGINCLASS) \
        XQ_EXPORT_PLUGIN2(PLUGIN, PLUGINCLASS)

#  define XQ_EXPORT_PLUGIN2(PLUGIN, PLUGINCLASS) \
        XQ_PLUGIN_ECOM_HEADER(PLUGIN) \
        XQ_PLUGIN_ECOM_IMPL(PLUGIN, PLUGINCLASS)

#endif // XQ_PLUGIN_H
