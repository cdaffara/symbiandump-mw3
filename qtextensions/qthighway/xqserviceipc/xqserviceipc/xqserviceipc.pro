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
# Description:  Service Framework IPC
#

TEMPLATE = lib
TARGET = xqserviceipcclient

XQSERVICE_ROOT=../..
        
include(api_headers.pri)
include($$XQSERVICE_ROOT/xqservicebase.pri)

QT -=gui
QT +=network

#Sources
HEADERS += xqserviceipcdefs.h \
           xqserviceipc_p.h \
           xqserviceipc.h \ 
           xqserviceipcfactory.h
SOURCES += xqserviceipc.cpp \ 
           xqserviceipcfactory.cpp


LIBS += -lxqserviceutil -lapparc -lefsrv

DEFINES += QT_MAKE_IPC_DLL

# Symbian Specifics
# Old UID3 102829E9
symbian {
    TARGET.UID3 = 0x2002871A
    TARGET.VID = VID_DEFAULT
    TARGET.CAPABILITY = CAP_GENERAL_DLL
    TARGET.EPOCALLOWDLLDATA = 1

    HEADERS += xqserviceipc_symbian.h \
               xqserviceipc_symbiansession.h \
               xqserviceipc_apasymbian.h \
               xqserviceipc_apasymbiansession.h
    SOURCES += xqserviceipc_symbian.cpp \
               xqserviceipc_symbiansession.cpp \
               xqserviceipc_apasymbian.cpp \
               xqserviceipc_apasymbiansession.cpp
#    BLD_INF_RULES.prj_exports += \
#        "xqserviceipcdefs.h       ..\..\inc\xqserviceipcdefs.h" \
#        "xqserviceipc.h           ..\..\inc\xqserviceipc.h" 
}
