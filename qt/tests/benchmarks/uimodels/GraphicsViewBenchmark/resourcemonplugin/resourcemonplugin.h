/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the either Technology Preview License Agreement or the
** Beta Release License Agreement.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at http://qt.nokia.com/contact.
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef __RESOURCEMONPLUGIN_H__
#define __RESOURCEMONPLUGIN_H__

#include <QObject>
#include "resourcemoninterface.h"

#if defined (Q_OS_SYMBIAN)
#include <e32std.h>

class TTime;
class TTimeIntervalMicroSeconds;
#endif
#if defined Q_OS_LINUX
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <malloc.h>
#endif

class ResourceMonitorPlugin : public QObject, ResourceMonitorInterface
{
    Q_OBJECT
    Q_INTERFACES(ResourceMonitorInterface)
    
    public:
        ~ResourceMonitorPlugin();
        bool Prepare(QString applicationThreadName);
        ResourceMonitorInterface::CpuUsage CPULoad();
        ResourceMonitorInterface::MemoryAllocation MemoryLoad();
        
        void BeginMeasureMemoryLoad();
        ResourceMonitorInterface::MemoryAllocation EndMeasureMemoryLoad();
        
        void BeginMeasureCPULoad();
        ResourceMonitorInterface::CpuUsage EndMeasureCPULoad();
        
    private:
        ResourceMonitorInterface::MemoryAllocation iMemoryAtStart;
        
#if defined (Q_OS_SYMBIAN)
        TTime iPrevSampleTime;
        TTimeIntervalMicroSeconds iPrevSystemCpuTime;
        TTimeIntervalMicroSeconds iPrevAppCpuTime;
        RThread iSysThread;
        RThread iAppThread;
#endif
#if defined Q_OS_LINUX
    uint64_t iPrevTotalJiffies;
    uint64_t iPrevBusyJiffies;
    struct timeval iPrevAppCpuTime;
#endif
};

#endif //__RESOURCEMONPLUGIN_H__
