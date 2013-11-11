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

load(qttest_p4)
SOURCES         += ../tst_xqpluginloader.cpp
TARGET  = ../tst_xqpluginloader
QT = core
HEADERS += ../theplugin/plugininterface.h

LIBS += -lxqplugins.dll

win32 {
  CONFIG(debug, debug|release) {
    TARGET = ../../debug/tst_qpluginloader
} else {
    TARGET = ../../release/tst_xqpluginloader
  }
}


wince*: {
   addFiles.sources = ../bin/*.dll
   addFiles.path = bin
   DEPLOYMENT += addFiles
}

symbian: {
   libDep.sources = xmylib.dll
   libDep.path = /sys/bin
   pluginDep.sources = xtheplugin.dll
   pluginDep.path = bin
   
   DEPLOYMENT += libDep pluginDep
   TARGET.UID3 = 0xE6E9EB85
}

