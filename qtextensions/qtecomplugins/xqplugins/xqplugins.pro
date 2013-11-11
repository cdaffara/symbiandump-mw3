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
TARGET = 
TARGET.UID3=0x20022FCD

DEFINES += BUILD_XQPLUGINS_DLL

QT -= gui
LIBS += -lecom.dll
LIBS += -lcharconv.dll
INCLUDEPATH += $$MW_LAYER_SYSTEMINCLUDE
HEADERS = include/xqplugin.h \
    include/xqpluginloader.h \
    include/xqtecom.hrh \
    include/xqpluginfactory.h \
    include/xqplugininfo.h \
    inc/xqpluginloaderprivate.h \
    inc/xqpluginhandle.h \
    inc/xqplugininfoprivate.h
SOURCES = src/xqpluginfactory.cpp \
    src/xqpluginloader.cpp \
    src/xqpluginloaderprivate.cpp \
    src/xqpluginhandle.cpp \
    src/xqplugininfo.cpp \
    src/xqplugininfoprivate.cpp
symbian { 
    TARGET.CAPABILITY = All -TCB
    TARGET.EPOCALLOWDLLDATA = 1
    defFilePath=..

#    deploy.path = /
#    headers.path = epoc32/include/mw
#    headers.sources = include/xqpluginfactory.h \
#        include/xqplugin.h \
#    	include/xqplugininfo.h \
#        include/xqpluginloader.h \
#        include/xqtecom.hrh
#    DEPLOYMENT += headers

BLD_INF_RULES.prj_exports += "include/xqpluginfactory.h /epoc32/include/mw/xqpluginfactory.h"
BLD_INF_RULES.prj_exports += "include/xqplugin.h /epoc32/include/mw/xqplugin.h"
BLD_INF_RULES.prj_exports += "include/xqplugininfo.h /epoc32/include/mw/xqplugininfo.h"
BLD_INF_RULES.prj_exports += "include/xqpluginloader.h /epoc32/include/mw/xqpluginloader.h"
BLD_INF_RULES.prj_exports += "include/xqtecom.hrh /epoc32/include/mw/xqtecom.hrh"
}

