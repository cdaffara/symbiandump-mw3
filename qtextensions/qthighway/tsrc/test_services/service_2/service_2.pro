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
TARGET = service_2

symbian:TARGET.UID3 = 0xED698FE0

CONFIG += service

include(../src/test_services.pri)

SOURCES += main.cpp

HEADERS += ../../inc/testdata.h

SERVICE.FILE = service_conf.xml
SERVICE.OPTIONS = embeddable

RSS_RULES += \
  "datatype_list = " \
  "      {" \
  "      DATATYPE" \
  "          {" \
  "          priority = EDataTypePriorityNormal;" \
  "          type = \"text/plain\";" \
  "          }" \
  "      };" \