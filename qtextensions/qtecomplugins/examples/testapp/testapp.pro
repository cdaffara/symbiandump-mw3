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

TEMPLATE = app
TARGET = 
TARGET.UID3 = 
CONFIG += qt

DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += /epoc32/include/domain \
               /epoc32/include/domain/middleware \
               /epoc32/include/domain/applications \
               /orbit/include \
               /epoc32/include/hb \
               /epoc32/include/hb/hbcore \
	       ../testplugin/inc

TARGET.CAPABILITY = ALL -TCB
TARGET.EPOCHEAPSIZE = 0x20000 0x1600000

LIBS += -lxqplugins

SOURCES += \
           src/main.cpp
          
HEADERS += 
		   

           

