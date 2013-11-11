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
TARGET = xqserviceipcserver

XQSERVICE_ROOT=../..
        
include(api_headers.pri)
include($$XQSERVICE_ROOT/xqservicebase.pri)

QT +=gui
QT +=network

#Sources
HEADERS += xqserviceipcserverfactory.h \
           xqserviceipcserver.h \
           xqserviceipcserver_p.h \ 
           xqserviceipcobserver.h \
           xqserviceipcserversession.h \
           xqserviceipcrequest.h

SOURCES += xqserviceipcserverfactory.cpp \
           xqserviceipcserver.cpp \
           xqserviceipcserversession.cpp \
           xqserviceipcrequest.cpp
           
DEFINES += QT_MAKE_IPC_SERVER_DLL

# Symbian Specifics
# Old UID3 102829E8
symbian {
    TARGET.UID3 = 0x2002871B
    TARGET.VID = VID_DEFAULT
    TARGET.CAPABILITY = CAP_GENERAL_DLL
    TARGET.EPOCALLOWDLLDATA = 1

    HEADERS += xqserviceipcserver_symbianserver.h \
               xqserviceipcserver_symbiansession.h \
               xqserviceipcserver_apasymbianserver.h \
               xqserviceipcserver_apasymbiansession.h
    SOURCES += xqserviceipcserver_symbianserver.cpp \
               xqserviceipcserver_symbiansession.cpp \
               xqserviceipcserver_apasymbianserver.cpp \
               xqserviceipcserver_apasymbiansession.cpp

#    BLD_INF_RULES.prj_exports += \
#        "xqserviceipcserver.h         ..\..\inc\xqserviceipcserver.h" \
#        "xqserviceipcobserver.h       ..\..\inc\xqserviceipcobserver.h" \
#        "xqserviceipcserversession.h  ..\..\inc\xqserviceipcserversession.h" \
#        "xqserviceipcrequest.h        ..\..\inc\xqserviceipcrequest.h" 
    
    LIBS += -lapgrfx -lws32 -lapparc -lxqserviceutil -lefsrv -lxqsettingsmanager
}
