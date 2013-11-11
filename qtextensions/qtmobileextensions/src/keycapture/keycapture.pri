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

DIR_PRFX=.

HEADERS += \
  $${DIR_PRFX}/xqkeycapture.h \
  $${DIR_PRFX}/keycapture_p.h \
  $${DIR_PRFX}/txlogger.h

SOURCES += \
  $${DIR_PRFX}/xqkeycapture.cpp
  
symbian { 
    HEADERS += $${DIR_PRFX}/keycapture_s60_p.h \
                $${DIR_PRFX}/capturerequest_s60.h \
                $${DIR_PRFX}/keymapper.h \
                $${DIR_PRFX}/responsehandler.h \
                $${DIR_PRFX}/responsehandlerex.h \
                $${DIR_PRFX}/targetwrapper.h
    SOURCES += $${DIR_PRFX}/keycapture_s60_p.cpp \
                $${DIR_PRFX}/capturerequest_s60.cpp \
                $${DIR_PRFX}/keymapper.cpp \
                $${DIR_PRFX}/responsehandler.cpp \
                $${DIR_PRFX}/responsehandlerex.cpp \
                $${DIR_PRFX}/targetwrapper.cpp
} else { 
    HEADERS += $${DIR_PRFX}/keycapture_stub_p.h
    SOURCES += $${DIR_PRFX}/keycapture_stub_p.cpp
}
  
LIBS += -lws32 -lcone
