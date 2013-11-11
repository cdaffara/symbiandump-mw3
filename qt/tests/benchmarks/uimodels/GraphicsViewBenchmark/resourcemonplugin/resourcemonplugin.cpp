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

#include <QtGui>
#include "resourcemonplugin.h"

#if defined Q_OS_SYMBIAN
#include <HAL.H>
#include <hal_data.h>
#endif

#if defined Q_OS_LINUX
// Parses first, cpu summary line and calculates total and busy jiffies
static int get_cpu_load(uint64_t *t, uint64_t *b)
{
    uint64_t user, nice, sys, idle, iowait, irq, softirq, steal;
    char cpu[20];
    int n;
    FILE *f;

    f = fopen("/proc/stat", "r");
    if (!f)
        return -1;

    n = fscanf(f, "%s %llu %llu %llu %llu %llu %llu %llu %llu", cpu,
               &user, &nice, &sys, &idle, &iowait, &irq, &softirq, &steal);
    if (n != 9 || strcmp(cpu, "cpu")) {	
        fclose(f);
        return -1;
    }

    fclose(f);

    *t = user + nice + sys + idle + iowait + irq + softirq + steal;
    *b = *t - idle - iowait;
    return 0;
}
#endif

ResourceMonitorPlugin::~ResourceMonitorPlugin()
{
#if defined Q_OS_SYMBIAN
    iSysThread.Close();
    iAppThread.Close();
#endif
}

bool ResourceMonitorPlugin::Prepare(QString applicationThreadName)
{
bool result = false;
#if defined Q_OS_SYMBIAN
    //Locate ekern null thread
    TFindProcess kernelProc(_L("ekern.exe*"));
    TFullName kernelProcName;
    
    if (kernelProc.Next(kernelProcName) == KErrNone)
        {
        kernelProcName.Append(_L("::Null"));
        
        TFindThread kernelNullThread(kernelProcName);
        TFullName kernelThreadName;
        
        if (kernelNullThread.Next(kernelThreadName) == KErrNone)
            {
            if (iSysThread.Open(kernelThreadName) == KErrNone)
                {
                //Set prev values so first call to CPU usage won't fail at first call
                iSysThread.GetCpuTime(iPrevSystemCpuTime);
                iPrevSampleTime.HomeTime();
                result =  true;
                }
            }
        }
    
    //Locate app thread, append asterisk if needed
    if(applicationThreadName.indexOf("*") == -1)
        applicationThreadName.append('*');
    
    TPtrC appName (reinterpret_cast<const TText*>(applicationThreadName.constData()),applicationThreadName.length());
    TFindThread appThread(appName);
    TFullName appThreadFullName;
    
    if(appThread.Next(appThreadFullName) == KErrNone)
        {
        if(iAppThread.Open(appThreadFullName) == KErrNone)
            {
            //Get CPU Time for app thread
            iAppThread.GetCpuTime(iPrevAppCpuTime);
            result = result && true;
            }
        }
#endif
#if defined Q_OS_LINUX
    Q_UNUSED(applicationThreadName);

    //Get resource usage for the process
    struct rusage usage;
    int res = getrusage(RUSAGE_SELF, &usage);
    if (res == -1) {
        return false;
    }

    //Init CPU jiffies
    res = get_cpu_load(&iPrevTotalJiffies, &iPrevBusyJiffies);
    if (res == -1) {
        return false;
    }

    //Init process CPU time, system functions time and user code time altogether
    timeradd(&(usage.ru_stime), &(usage.ru_utime), &iPrevAppCpuTime);

    result = true;
#endif
    return result;
}

ResourceMonitorInterface::CpuUsage ResourceMonitorPlugin::CPULoad()
{
    ResourceMonitorInterface::CpuUsage cpuUsage;
#if defined Q_OS_SYMBIAN
    //Get current time
    TTime currentTime;
    currentTime.HomeTime(); 
        
    TTimeIntervalMicroSeconds systemCpuTime;
    TTimeIntervalMicroSeconds appCpuTime;
    
    //Get CPU times
    TInt result = iSysThread.GetCpuTime(systemCpuTime);
    result = iAppThread.GetCpuTime(appCpuTime);
    
    //Calculate Total CPU usage
    TTimeIntervalMicroSeconds totalDelta = currentTime.MicroSecondsFrom(iPrevSampleTime);
    if(totalDelta.Int64() == 0)
        return cpuUsage; //return empty cpuUsage
    
    TInt64 totalUsage = ( systemCpuTime.Int64() - iPrevSystemCpuTime.Int64() ) * 1000000 / totalDelta.Int64();
    TInt64 appUsage = ( appCpuTime.Int64() - iPrevAppCpuTime.Int64() ) * 1000000 / totalDelta.Int64();
    
    //Convert usage (us) to percentage
    cpuUsage.systemUsage =  100.0 - ( totalUsage / 10000.0);
    cpuUsage.appTreadUsage = (appUsage / 10000.0);
    
    //update prev values
    iPrevSampleTime = currentTime;
    iPrevSystemCpuTime = systemCpuTime;
    iPrevAppCpuTime = appCpuTime;
#endif
#if defined Q_OS_LINUX
    struct rusage usage;
    int res = getrusage(RUSAGE_SELF, &usage);
    if (res == -1) {
        return cpuUsage;
    }

    uint64_t totalJiffies, busyJiffies;
    res = get_cpu_load(&totalJiffies, &busyJiffies);
    if (res == -1) {
        return cpuUsage;
    }

    struct timeval appTime;
    timeradd(&(usage.ru_stime), &(usage.ru_utime), &appTime);

    struct timeval appDelta;
    timersub(&appTime, &iPrevAppCpuTime, &appDelta);

    //"systemUsage" means here busy time percentage in relation to CPUs summary
    cpuUsage.systemUsage = (busyJiffies - iPrevBusyJiffies) * 100.0 / (totalJiffies - iPrevTotalJiffies);
    //"appTreadUsage" means here calling process time percentage in relation to CPUs summary
    cpuUsage.appTreadUsage = (appDelta.tv_sec*10000.0 + appDelta.tv_usec/100.0) / (totalJiffies - iPrevTotalJiffies);

    iPrevBusyJiffies = busyJiffies;
    iPrevTotalJiffies = totalJiffies;
    iPrevAppCpuTime = appTime;
#endif
    return cpuUsage;
}

ResourceMonitorInterface::MemoryAllocation ResourceMonitorPlugin::MemoryLoad()
{
    ResourceMonitorInterface::MemoryAllocation allocation;
#if defined Q_OS_SYMBIAN
    TInt allocInCells = 0;
    TInt numberOfCells = 0;
    TInt largestFreeBlock = 0;
    TInt availableInHeap = 0;
    TInt totalMemory = 0;
    TInt availableSysMemory = 0;
    
    numberOfCells = User::AllocSize(allocInCells);
    availableInHeap = User::Available(largestFreeBlock);
    
    TInt result = KErrNone;
    result = HAL::Get(HALData::EMemoryRAMFree, availableSysMemory);
    result = HAL::Get(HALData::EMemoryRAM, totalMemory);
    
    allocation.allocatedInAppThread = allocInCells;
    allocation.numberOfAllocatedCellsInAppThread = numberOfCells;
    allocation.availableMemoryInAppThreadHeap = availableInHeap;
    allocation.availableMemoryInSystem = availableSysMemory;
    allocation.totalMemoryInSystem = totalMemory;
#endif
#if defined Q_OS_LINUX
    struct mallinfo info = mallinfo();
    allocation.allocatedInAppThread = info.uordblks; //bytes
    allocation.numberOfAllocatedCellsInAppThread = info.ordblks; //free chunks
    allocation.availableMemoryInAppThreadHeap = info.fordblks; //bytes
    allocation.availableMemoryInSystem = (long long)sysconf(_SC_AVPHYS_PAGES) * (long long)sysconf(_SC_PAGESIZE); //bytes
    allocation.totalMemoryInSystem = (long long)sysconf(_SC_PHYS_PAGES) * (long long)sysconf(_SC_PAGESIZE); //bytes
#endif
    return allocation;
}

void ResourceMonitorPlugin::BeginMeasureMemoryLoad()
{
    //Store current memory values.
    iMemoryAtStart = MemoryLoad();
}

ResourceMonitorInterface::MemoryAllocation ResourceMonitorPlugin::EndMeasureMemoryLoad()
{
    //Get current memory consumption
    ResourceMonitorInterface::MemoryAllocation current = MemoryLoad();
    //Calculate delta values for heap allocation and heap cell allocation
    current.allocatedInAppThread = current.allocatedInAppThread - iMemoryAtStart.allocatedInAppThread;
    current.numberOfAllocatedCellsInAppThread = current.numberOfAllocatedCellsInAppThread - iMemoryAtStart.numberOfAllocatedCellsInAppThread;
#if defined Q_OS_LINUX
    current.numberOfAllocatedCellsInAppThread = -current.numberOfAllocatedCellsInAppThread;
#endif
    return current;
}

void ResourceMonitorPlugin::BeginMeasureCPULoad()
{
    //Reset CPU load measuring
    CPULoad();
}

ResourceMonitorInterface::CpuUsage ResourceMonitorPlugin::EndMeasureCPULoad()
{
    //return CPU load
  return CPULoad();  
}

Q_EXPORT_PLUGIN2(resourcemonplugin, ResourceMonitorPlugin);
