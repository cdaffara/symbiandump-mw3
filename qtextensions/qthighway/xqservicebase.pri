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

# Still sorting out the Qt caps..
#symbian:TARGET.CAPABILITY = All -Tcb -DRM -AllFiles -DiskAdmin
symbian:TARGET.CAPABILITY = All -Tcb

DEPENDPATH += .

INCLUDEPATH += .
INCLUDEPATH += $$XQSERVICE_ROOT/inc
INCLUDEPATH += $$APP_LAYER_SYSTEMINCLUDE

CONFIG(release):XQSERVICE_OUTPUT_DIR=$$XQSERVICE_ROOT/build/$$TARGET/rel
CONFIG(debug):XQSERVICE_OUTPUT_DIR=$$XQSERVICE_ROOT/build/$$TARGET/deb
     
OBJECTS_DIR = $$XQSERVICE_OUTPUT_DIR/obj
DESTDIR = $$XQSERVICE_OUTPUT_DIR/bin
MOC_DIR=$$XQSERVICE_OUTPUT_DIR/moc
RCC_DIR=$$XQSERVICE_OUTPUT_DIR/rcc
TEMPDIR=$$XQSERVICE_OUTPUT_DIR/tmp
QMAKE_RPATHDIR = $$DESTDIR $$QMAKE_RPATHDIR
QMAKE_LIBDIR = $$DESTDIR $$QMAKE_LIBDIR

symbian {

defFilePath=..
deploy.path = /
# This is for new exporting system coming in garden
for(header_public, XQSERVICE_API_HEADERS):BLD_INF_RULES.prj_exports += "$$header_public $$MW_LAYER_PUBLIC_EXPORT_PATH($$basename(header_public))"
for(header_internal, XQSERVICE_API_HEADERS_INTERNAL):BLD_INF_RULES.prj_exports += "$$header_internal |../inc/$$basename(header_internal)"
for(header_platform, XQSERVICE_API_HEADERS_PLATFORM):BLD_INF_RULES.prj_exports += "$$header_platform $$MW_LAYER_PLATFORM_EXPORT_PATH($$basename(header_platform))"
}
