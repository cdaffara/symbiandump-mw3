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
TARGET = xqkeycapture

CONFIG += qt
QT        += core gui

DEFINES += XQKEYCAPTURE_LIBRARY

# for debug purposes
#DEFINES += ENABLETRACE
#DEFINES += TRACE_FILE
 
INCLUDEPATH += \ 
           /sf/mw/qt/include/Qt \
           . 

#INCLUDEPATH += /sf/mw/qt/src/gui/kernel/


LIBS += -lremconcoreapi \
        -lremconinterfacebase \
        -lremconextensionapi
symbian { 
    INCLUDEPATH += . /epoc32/include
    defFilePath=..
    TARGET.UID3 = 0x2002C342
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.EPOCHEAPSIZE = 0x020000 0x2000000
    TARGET.CAPABILITY = ALL -TCB
}

include(keycapture.pri)
