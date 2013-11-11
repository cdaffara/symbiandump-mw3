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

CONFIG += qt ordered

TEMPLATE = subdirs

SUBDIRS = xqplugins

symbian:BLD_INF_RULES.prj_exports += "./rom/qtecomplugins.iby $$CORE_MW_LAYER_IBY_EXPORT_PATH(qtecomplugins.iby)"

defineReplace(myNativePath) {
    win32:!win32-g++ {
        unixstyle = false
    } else:win32-g++:isEmpty(QMAKE_SH) {
        unixstyle = false
    } else {
        unixstyle = true
    }
    $$unixstyle {
        return($$quote(\"$$replace(1, "\\", "/")\"))
    } else {
        return($$quote(\"$$replace(1, "/", "\\")\"))
    }
}

ecompluginprfdir = "mkspecs\features\\"
ecompluginprffile = "ecomplugin.prf"
ecompluginprfdestdir = "$$[QMAKE_MKSPECS]\features"

tooldir = "supplements\xqecom\\"
toolfile = "xqecom.py"
tooldestdir = "$$[QT_INSTALL_BINS]"

installprf.commands += $${QMAKE_COPY} $$myNativePath($$ecompluginprfdir$$ecompluginprffile) $$myNativePath($$ecompluginprfdestdir)
installtool.commands += $${QMAKE_COPY} $$myNativePath($$tooldir$$toolfile) $$myNativePath($$tooldestdir)
QMAKE_EXTRA_TARGETS += installtool installprf

cleanprf.commands += \$\(DEL_FILE\) $$myNativePath($$ecompluginprfdestdir\\$$ecompluginprffile)
cleantool.commands += \$\(DEL_FILE\) $$myNativePath($$tooldestdir\\$$toolfile)
QMAKE_EXTRA_TARGETS += cleantool cleanprf
