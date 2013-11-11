#
# Copyright (c) 2010 Nokia Corporation and/or its subsidiary(-ies).
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
TEMPLATE    = lib
CONFIG     += plugin hb
TARGET      = qhbstyle
win32 {
    debug:DESTDIR = ../debug/styles/
    release:DESTDIR = ../release/styles/
} else {
    DESTDIR = $$[QT_INSTALL_PLUGINS]/styles
}

symbian:BLD_INF_RULES.prj_exports += "./rom/qhbstyle.iby $$CORE_MW_LAYER_IBY_EXPORT_PATH(qhbstyle.iby)"

include(qhbstyle.pri)


symbian: {
    load(data_caging_paths)
    styleplugin.sources = qhbstyle.dll
    styleplugin.path = $$QT_PLUGINS_BASE_DIR/styles
    DEPLOYMENT += styleplugin
}

symbian {
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.UID3 = 0x2002FF8B
    TARGET.EPOCALLOWDLLDATA = 1
    TARGET.CAPABILITY = ALL -TCB
}
