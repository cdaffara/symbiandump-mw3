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
TARGET = xqsystemtoneservice

symbian:defFilePath=..
symbian:TARGET.UID3=0x2002EA83
symbian:TARGET.EPOCALLOWDLLDATA=1

TARGET.CAPABILITY = CAP_GENERAL_DLL

DEFINES += __STS_QT_LIB_BUILD__
include(systemtoneservice.pri)

