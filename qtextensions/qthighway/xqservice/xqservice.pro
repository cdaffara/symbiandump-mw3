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
TARGET   = xqservice
CONFIG  += dll warn_on

#QT -=gui # QAction used for AIW

# Try to put useful things in one place on win
win32:DESTDIR = ../../bin

DEFINES += XQ_BUILD_XQSERVICE_LIB
symbian {
	DEFINES += QT_S60_AIW
	TARGET.UID3 = 0x20022E72
	TARGET.EPOCSTACKSIZE = 0x5000
	TARGET.EPOCHEAPSIZE = "0x20000 0x1000000"
	TARGET.EPOCALLOWDLLDATA = 1
	TARGET.CAPABILITY = ALL -TCB
}

XQSERVICE_ROOT=..

include(src/api_headers.pri)
include(../xqservicebase.pri)
include(src/xqservice.pri)

LIBS+=-lxqserviceutil -lxqserviceipcclient -lxqserviceipcserver -lQtGui -lQtServiceFramework -lws32 -lefsrv -lapparc -lapgrfx
LIBS+=-lcaf
LIBS+=-lxqsettingsmanager

libFiles.sources = xqservice.dll xqserviceutil.dll
libFiles.path = "!:/sys/bin"
DEPLOYMENT += libFiles

