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

HEADERS += \
    src/xqserviceadaptor.h \
    src/xqservicechannel.h \
    src/xqserviceipcmarshal.h\
    src/xqservicerequest.h\
    src/xqserviceprovider.h\
    src/xqserviceipcclient.h \
    src/xqservicethreaddata.h\
    src/xqaiwglobal.h\
    src/xqaiwrequest.h\
    src/xqaiwrequestdriver.h \
    src/xqaiwservicedriver.h \
    src/xqaiwfiledriver.h \
    src/xqaiwuridriver.h \
    src/xqappmgr.h \
    src/xqappmgr_p.h \
    src/xqaiwutils.h

SOURCES += \
    src/xqserviceadaptor.cpp \
    src/xqservicechannel.cpp \
    src/xqserviceipcmarshal.cpp\
    src/xqservicerequest.cpp\
    src/xqserviceprovider.cpp\
    src/xqserviceipcclient.cpp \
    src/xqservicethreaddata.cpp\
    src/xqaiwrequest.cpp\
    src/xqappmgr.cpp \
    src/xqappmgr_p.cpp \
    src/xqaiwservicedriver.cpp \
    src/xqaiwfiledriver.cpp \
    src/xqaiwuridriver.cpp \
    src/xqaiwutils.cpp

LIBS+=-lapmime
