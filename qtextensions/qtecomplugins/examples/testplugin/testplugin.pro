#
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
# All rights reserved.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, version 2.1 of the License.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, 
# see "http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html/".
#
# Description:
#

TEMPLATE = lib
CONFIG += qt hb ecomplugin
TARGET = testplugin

TARGET.UID3 = 0xE1253163

SERVICE.INTERFACE_NAME = org.nokia.mmdt.TestPlugin/1.0
SERVICE.CONFIGURATION = "<t>0x10000000</t><p>0x30000001</p><i>PluginPriorityNormal</i><f>0x00000003</f>"

INCLUDEPATH    += inc

HEADERS         = inc/testplugin.h \
		  inc/testplugininterface.h

SOURCES         = src/testplugin.cpp

TARGET.CAPABILITY = All -Tcb
symbian:TARGET.EPOCALLOWDLLDATA = 1
symbian:TARGETTYPE_OVERRIDE = PLUGIN
#MMP_RULES+=EXPORTUNFROZEN
