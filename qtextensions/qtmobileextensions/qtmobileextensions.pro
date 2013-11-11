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

TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = src/settingsmanager \
          src/sysinfo \
          src/utils \
          src/keycapture \
          src/systemtoneservice

symbian {
BLD_INF_RULES.prj_exports += "./src/settingsmanager/settingsmanager_global.h               $$MW_LAYER_PLATFORM_EXPORT_PATH(settingsmanager_global.h)"
BLD_INF_RULES.prj_exports += "./src/settingsmanager/xqcentralrepositorysearchcriteria.h    $$MW_LAYER_PLATFORM_EXPORT_PATH(xqcentralrepositorysearchcriteria.h)"
BLD_INF_RULES.prj_exports += "./include/XQCentralRepositorySearchCriteria                  $$MW_LAYER_PLATFORM_EXPORT_PATH(XQCentralRepositorySearchCriteria)"
BLD_INF_RULES.prj_exports += "./src/settingsmanager/xqcentralrepositoryutils.h             $$MW_LAYER_PLATFORM_EXPORT_PATH(xqcentralrepositoryutils.h)"
BLD_INF_RULES.prj_exports += "./include/XQCentralRepositoryUtils                           $$MW_LAYER_PLATFORM_EXPORT_PATH(XQCentralRepositoryUtils)"
BLD_INF_RULES.prj_exports += "./src/settingsmanager/xqpublishandsubscribesecuritypolicy.h  $$MW_LAYER_PLATFORM_EXPORT_PATH(xqpublishandsubscribesecuritypolicy.h)"
BLD_INF_RULES.prj_exports += "./include/XQPublishAndSubscribeSecurityPolicy                $$MW_LAYER_PLATFORM_EXPORT_PATH(XQPublishAndSubscribeSecurityPolicy)"
BLD_INF_RULES.prj_exports += "./src/settingsmanager/xqpublishandsubscribeutils.h           $$MW_LAYER_PLATFORM_EXPORT_PATH(xqpublishandsubscribeutils.h)"
BLD_INF_RULES.prj_exports += "./include/XQPublishAndSubscribeUtils                         $$MW_LAYER_PLATFORM_EXPORT_PATH(XQPublishAndSubscribeUtils)"
BLD_INF_RULES.prj_exports += "./src/settingsmanager/xqsettingskey.h                        $$MW_LAYER_PLATFORM_EXPORT_PATH(xqsettingskey.h)"
BLD_INF_RULES.prj_exports += "./include/XQCentralRepositorySettingsKey                     $$MW_LAYER_PLATFORM_EXPORT_PATH(XQCentralRepositorySettingsKey)"
BLD_INF_RULES.prj_exports += "./include/XQPublishAndSubscribeSettingsKey                   $$MW_LAYER_PLATFORM_EXPORT_PATH(XQPublishAndSubscribeSettingsKey)"
BLD_INF_RULES.prj_exports += "./include/XQSettingsKey                                      $$MW_LAYER_PLATFORM_EXPORT_PATH(XQSettingsKey)"
BLD_INF_RULES.prj_exports += "./src/settingsmanager/xqsettingsmanager.h                    $$MW_LAYER_PLATFORM_EXPORT_PATH(xqsettingsmanager.h)"
BLD_INF_RULES.prj_exports += "./include/XQSettingsManager                                  $$MW_LAYER_PLATFORM_EXPORT_PATH(XQSettingsManager)"
BLD_INF_RULES.prj_exports += "./src/sysinfo/sysinfo_global.h                               $$MW_LAYER_PLATFORM_EXPORT_PATH(sysinfo_global.h)"
BLD_INF_RULES.prj_exports += "./src/sysinfo/xqsysinfo.h                                    $$MW_LAYER_PLATFORM_EXPORT_PATH(xqsysinfo.h)"
BLD_INF_RULES.prj_exports += "./include/XQSysInfo                                          $$MW_LAYER_PLATFORM_EXPORT_PATH(XQSysInfo)"
BLD_INF_RULES.prj_exports += "./src/utils/utils_global.h                                   $$MW_LAYER_PLATFORM_EXPORT_PATH(utils_global.h)"
BLD_INF_RULES.prj_exports += "./src/utils/xqutils.h                                        $$MW_LAYER_PLATFORM_EXPORT_PATH(xqutils.h)"
BLD_INF_RULES.prj_exports += "./include/XQUtils                                            $$MW_LAYER_PLATFORM_EXPORT_PATH(XQUtils)"
BLD_INF_RULES.prj_exports += "./src/utils/xqconversions.h                                  $$MW_LAYER_PLATFORM_EXPORT_PATH(xqconversions.h)"
BLD_INF_RULES.prj_exports += "./include/XQConversions                                      $$MW_LAYER_PLATFORM_EXPORT_PATH(XQConversions)"
BLD_INF_RULES.prj_exports += "./src/keycapture/xqkeycapture.h                              $$MW_LAYER_PLATFORM_EXPORT_PATH(xqkeycapture.h)"
BLD_INF_RULES.prj_exports += "./include/XQKeyCapture                                       $$MW_LAYER_PLATFORM_EXPORT_PATH(XQKeyCapture)"
BLD_INF_RULES.prj_exports += "./src/systemtoneservice/xqsystemtoneservice.h                $$MW_LAYER_PLATFORM_EXPORT_PATH(xqsystemtoneservice.h)"
BLD_INF_RULES.prj_exports += "./include/XQSystemToneService                                $$MW_LAYER_PLATFORM_EXPORT_PATH(XQSystemToneService)"
BLD_INF_RULES.prj_exports += "./rom/qtmobileextensions.iby                                 $$CORE_MW_LAYER_IBY_EXPORT_PATH(qtmobileextensions.iby)"
}
