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

HEADERS += xqsettingskey.h \
    xqsettingsmanager.h \
    xqsettingsmanager_p.h \
    settingshandlerbase.h \
    ccentralrepositoryhandler.h \
    cpublishandsubscribehandler.h \
    cenrepmonitor.h \
    pubsubmonitor.h \
    xqcentralrepositoryutils.h \
    xqcentralrepositoryutils_p.h \
    xqpublishandsubscribeutils.h \
    xqpublishandsubscribeutils_p.h \
    xqcentralrepositorysearchcriteria.h \
    xqpublishandsubscribesecuritypolicy.h \

SOURCES += xqsettingskey.cpp \
    xqsettingsmanager.cpp \
    xqsettingsmanager_p.cpp \
    settingshandlerbase.cpp \
    ccentralrepositoryhandler.cpp \
    cpublishandsubscribehandler.cpp \
    cenrepmonitor.cpp \
    pubsubmonitor.cpp \
    xqcentralrepositoryutils.cpp \
    xqcentralrepositoryutils_p.cpp \
    xqpublishandsubscribeutils.cpp \
    xqpublishandsubscribeutils_p.cpp \
    xqcentralrepositorysearchcriteria.cpp \
    xqpublishandsubscribesecuritypolicy.cpp \

symbian:LIBS += -lcentralrepository \
    -lCommonEngine
