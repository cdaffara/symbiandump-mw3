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

TEMPLATE=subdirs

CONFIG += ordered
SUBDIRS= \
         serviceapp \
         serviceclient \
         appmgrclient \
         appmgrclient2

# If Orbit configured 
#include($$[QMAKE_MKSPECS]/features/hb.prf) {
#SUBDIRS += \
#         hbserviceprovider \
#         hbserviceclient \
#         notifications 
#}

symbian:BLD_INF_RULES.prj_exports += "./rom/xqserviceexamples.iby $$CUSTOMER_VARIANT_APP_LAYER_IBY_EXPORT_PATH(xqserviceexamples.iby)"
