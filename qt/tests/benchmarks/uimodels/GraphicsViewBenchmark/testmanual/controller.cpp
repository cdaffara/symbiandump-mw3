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

#include <QDebug>
#include <QApplication>
#include <QDir>
#include <QPluginLoader>

#include "controller.h"
#include "itemrecyclinglist.h"
#include "simplelist.h"
#include "menu.h"
#include "theme.h"
#include "button.h"
#include "filllists.h"

static const int ScrollStep = 36;
static const int ScrollDuration = 10000;

Controller::Controller(MainView *mw, Settings &settings, QObject *parent)
    : QObject(parent),
    m_mainView(mw),
    m_currentListType(Recycling),
    m_createListMenuItem(0),
    m_subTreeMenuItem(0),
    m_dummyGen(),
    m_scrollDirection(1),
    m_testDuration(),
    m_settings(settings),
    m_resMon(0)
{
    Menu *menu = m_mainView->menu();
    if (menu) {
        QString listText = "Create " + newListName(Simple);
        m_createListMenuItem = menu->addMenuItem(listText, this, SLOT(toggleListType()));
        menu->addMenuItem("Change Next Theme", this, SLOT(changeTheme()));
#if (QT_VERSION >= 0x040600)
        m_subTreeMenuItem = menu->addMenuItem("Enable SubtreeCache", this, SLOT(toggleListItemCaching()));
#endif
        menu->addMenuItem("Auto scroll", this, SLOT(startAutoScroll()));
        menu->addMenuItem("Rotate", this, SLOT(changeRotation()));
        menu->addMenuItem("One/Two columns", this, SLOT(toggleColumns()));
        menu->addMenuItem("Quit", this, SLOT(quit()) );

        if(loadPlugin())
            m_resMon->Prepare("graphicsviewbenchmark");
        
        toggleListType();
    }

#if (QT_VERSION >= 0x040600)
    if (m_subTreeMenuItem &&
        settings.options().testFlag(Settings::UseListItemCache))
        m_subTreeMenuItem->setText("Disable SubtreeCache");
#endif

    connect(this, SIGNAL(doAutoScroll()),
            this, SLOT(autoScroll()), Qt::QueuedConnection);
}


Controller::~Controller()
{
    delete m_resMon;
}

#if (QT_VERSION >= 0x040600)
void Controller::toggleListItemCaching()
{
    if (!m_subTreeMenuItem || !m_mainView)
        return;

    bool enabled = false;

    if (m_currentListType == Simple) {
        SimpleList *list = static_cast<SimpleList *>(
                m_mainView->testWidget());
        list->setListItemCaching(!list->listItemCaching());
        enabled = list->listItemCaching();
    } else {
        ItemRecyclingList *list = static_cast<ItemRecyclingList *>(
                m_mainView->testWidget());
        list->setListItemCaching(!list->listItemCaching());
        enabled = list->listItemCaching();
    }

    if (enabled) 
        m_subTreeMenuItem->setText(tr("Disable list item caching"));
    else 
        m_subTreeMenuItem->setText(tr("Enabled list item caching"));
}
#endif

void Controller::toggleListType()
{
    if (!m_mainView)
        return;

    if (m_currentListType == Simple) {
#if (QT_VERSION >= 0x040600)
        bool caching = false;
#endif
        SimpleList *oldList = static_cast<SimpleList *>(
                m_mainView->takeTestWidget());
        if (oldList) {
#if (QT_VERSION >= 0x040600)
            caching = oldList->listItemCaching();
#endif
            delete oldList, oldList = 0;
        }
        
        //CPU/MEM Begin
        if(m_resMon && !m_settings.options().testFlag(Settings::NoResourceUsage)) {
            m_resMon->BeginMeasureCPULoad();
            m_resMon->BeginMeasureMemoryLoad();
        }
        
        ItemRecyclingList *list = new ItemRecyclingList();
        m_dummyGen.Reset();
        fillRecyclingList(m_dummyGen, m_settings.listItemCount(), list);
        
#if (QT_VERSION >= 0x040600)
        list->setListItemCaching(caching);
#endif
        m_currentListType = Recycling;
        m_mainView->setTestWidget(list);
        
        if(m_resMon && !m_settings.options().testFlag(Settings::NoResourceUsage)) {
            //CPU/MEM End
            ResourceMonitorInterface::CpuUsage cpuUsage = m_resMon->EndMeasureCPULoad();
            ResourceMonitorInterface::MemoryAllocation memory = m_resMon->EndMeasureMemoryLoad();
            QString cpuLoad = QString("List Construction CPU usage: %1% application thread CPU use, %2% total CPU use ")
            .arg(cpuUsage.appTreadUsage)
            .arg(cpuUsage.systemUsage);
            qDebug() << cpuLoad;
            QString memoryLoad = QString("List Construction Memory Usage: %1 allocated in cells in app heap,"
                    "%2 cells in use in app heap, %3 available in app heap, %4 RAM available in system, %5 RAM in System")
                    .arg(memory.allocatedInAppThread)
                    .arg(memory.numberOfAllocatedCellsInAppThread)
                    .arg(memory.availableMemoryInAppThreadHeap)
                    .arg(memory.availableMemoryInSystem)
                    .arg(memory.totalMemoryInSystem);
            qDebug() << memoryLoad;
        }
    } else {
#if (QT_VERSION >= 0x040600)
        bool caching = false;
#endif
        ItemRecyclingList *oldList = static_cast<ItemRecyclingList *>(
                m_mainView->takeTestWidget());
        if (oldList) {
#if (QT_VERSION >= 0x040600)
            caching = oldList->listItemCaching();
#endif
            delete oldList, oldList = 0;
        }
        
        if(m_resMon && !m_settings.options().testFlag(Settings::NoResourceUsage)) {
            //CPU/MEM Begin
            m_resMon->BeginMeasureCPULoad();
            m_resMon->BeginMeasureMemoryLoad();
        }
        
        SimpleList *list = new SimpleList();
        m_dummyGen.Reset();
        fillSimpleList(m_dummyGen, m_settings.listItemCount(), list);
#if (QT_VERSION >= 0x040600)
        list->setListItemCaching(caching);
#endif
        m_currentListType = Simple;
        m_mainView->setTestWidget(list);
        
        if(m_resMon && !m_settings.options().testFlag(Settings::NoResourceUsage)) {
            //CPU/MEM End
            ResourceMonitorInterface::CpuUsage cpuUsage = m_resMon->EndMeasureCPULoad();
            ResourceMonitorInterface::MemoryAllocation memory = m_resMon->EndMeasureMemoryLoad();
            //For logging
            QString cpuLoad = QString("List Construction CPU usage: %1% application thread CPU use, %2% total CPU use ")
            .arg(cpuUsage.appTreadUsage)
            .arg(cpuUsage.systemUsage);
            qDebug()<<cpuLoad;
            QString memoryLoad = QString("List Construction Memory Usage: %1 allocated in cells in app heap,"
                    "%2 cells in use in app heap, %3 available in app heap, %4 RAM available in system, %5 RAM in System")
                    .arg(memory.allocatedInAppThread)
                    .arg(memory.numberOfAllocatedCellsInAppThread)
                    .arg(memory.availableMemoryInAppThreadHeap)
                    .arg(memory.availableMemoryInSystem)
                    .arg(memory.totalMemoryInSystem);
            qDebug()<<memoryLoad;
        }
    }

    if (m_createListMenuItem) {
        QString listText = "Create " + newListName(m_currentListType);
        m_createListMenuItem->setText(listText);
    }
}

QString Controller::newListName(ListType currentType)
{
    if(currentType==Simple)
        return QString("ItemRecyclingList");

    return QString("SimpleList");
}

void Controller::startAutoScroll()
{
    if(m_resMon && !m_settings.options().testFlag(Settings::NoResourceUsage)) {
        //CPU/MEM Begin
        m_resMon->BeginMeasureCPULoad();
        m_resMon->BeginMeasureMemoryLoad();
    }
    m_testDuration.start();
    emit doAutoScroll();
}

void Controller::autoScroll()
{
    ScrollBar *sb = 0;
    QGraphicsWidget *w = m_mainView->testWidget();

    ItemRecyclingList *rl = dynamic_cast<ItemRecyclingList *>(w);
    if (rl)
        sb = rl->verticalScrollBar();

    SimpleList *sl = dynamic_cast<SimpleList *>(w);
    if (sl)
        sb = sl->verticalScrollBar();

    if (sb) {
        qreal currentVal = sb->sliderPosition();

        if (currentVal+m_scrollDirection*ScrollStep > sb->sliderSize() ||
            currentVal+m_scrollDirection*ScrollStep < 0.0)
            m_scrollDirection = -1*m_scrollDirection;

        sb->setSliderPosition(currentVal+m_scrollDirection*ScrollStep);
    }

    if (m_testDuration.elapsed() < ScrollDuration)
        emit doAutoScroll();
    else{
        if(m_resMon && !m_settings.options().testFlag(Settings::NoResourceUsage)) {
            //CPU/MEM End
            ResourceMonitorInterface::CpuUsage cpuUsage = m_resMon->EndMeasureCPULoad();
            ResourceMonitorInterface::MemoryAllocation memory = m_resMon->EndMeasureMemoryLoad();
            //For logging
            QString cpuLoad = QString("List Scroll CPU usage: %1% application thread CPU use, %2% total CPU use ")
            .arg(cpuUsage.appTreadUsage)
            .arg(cpuUsage.systemUsage);
            qDebug()<<cpuLoad;
            QString memoryLoad = QString("List Scroll Memory Usage: %1 allocated in cells in app heap,"
                    "%2 cells in use in app heap, %3 available in app heap, %4 RAM available in system, %5 RAM in System")
                    .arg(memory.allocatedInAppThread)
                    .arg(memory.numberOfAllocatedCellsInAppThread)
                    .arg(memory.availableMemoryInAppThreadHeap)
                    .arg(memory.availableMemoryInSystem)
                    .arg(memory.totalMemoryInSystem);
            qDebug()<<memoryLoad;
        }
    }
}

void Controller::quit()
{
    QApplication::quit();
}

void Controller::changeTheme()
{
    if(m_resMon && !m_settings.options().testFlag(Settings::NoResourceUsage)) {
        //CPU/MEM Begin
        m_resMon->BeginMeasureCPULoad();
        m_resMon->BeginMeasureMemoryLoad();
    }

    Theme::p()->theme() == Theme::Blue ?  Theme::p()->setTheme(Theme::Lime) : Theme::p()->setTheme(Theme::Blue);
    
    if(m_resMon && !m_settings.options().testFlag(Settings::NoResourceUsage)) {
        //CPU/MEM End
        ResourceMonitorInterface::CpuUsage cpuUsage = m_resMon->EndMeasureCPULoad();
        ResourceMonitorInterface::MemoryAllocation memory = m_resMon->EndMeasureMemoryLoad();
        
        //For logging
        QString cpuLoad = QString("Theme Change CPU usage: %1% application thread CPU use, %2% total CPU use ")
        .arg(cpuUsage.appTreadUsage)
        .arg(cpuUsage.systemUsage);
        qDebug()<<cpuLoad;
        QString memoryLoad = QString("Theme Change Memory Usage: %1 allocated in cells in app heap,"
                "%2 cells in use in app heap, %3 available in app heap, %4 RAM available in system, %5 RAM in System")
                .arg(memory.allocatedInAppThread)
                .arg(memory.numberOfAllocatedCellsInAppThread)
                .arg(memory.availableMemoryInAppThreadHeap)
                .arg(memory.availableMemoryInSystem)
                .arg(memory.totalMemoryInSystem);
        qDebug()<<memoryLoad;
    }
}

void Controller::changeRotation()
{  
    if (m_mainView){
        if(m_resMon && !m_settings.options().testFlag(Settings::NoResourceUsage)) {
            //CPU/MEM Begin
            m_resMon->BeginMeasureCPULoad();
            m_resMon->BeginMeasureMemoryLoad();
        }
        
        m_mainView->rotateContent(90);
       
        if(m_resMon && !m_settings.options().testFlag(Settings::NoResourceUsage)) {
            //CPU/MEM End
            ResourceMonitorInterface::CpuUsage cpuUsage = m_resMon->EndMeasureCPULoad();
            ResourceMonitorInterface::MemoryAllocation memory = m_resMon->EndMeasureMemoryLoad();
            
            //For logging
            QString cpuLoad = QString("Rotate CPU usage: %1% application thread CPU use, %2% total CPU use ")
                .arg(cpuUsage.appTreadUsage)
                .arg(cpuUsage.systemUsage);
            qDebug()<<cpuLoad;
            QString memoryLoad = QString("Rotate Memory Usage: %1 allocated in cells in app heap,"
                "%2 cells in use in app heap, %3 available in app heap, %4 RAM available in system, %5 RAM in System")
                .arg(memory.allocatedInAppThread)
                .arg(memory.numberOfAllocatedCellsInAppThread)
                .arg(memory.availableMemoryInAppThreadHeap)
                .arg(memory.availableMemoryInSystem)
                .arg(memory.totalMemoryInSystem);
            qDebug()<<memoryLoad;
        }
    }
}

bool Controller::loadPlugin()
{
    QDir pluginsDir(qApp->applicationDirPath());
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            m_resMon = qobject_cast<ResourceMonitorInterface *>(plugin);
            if (m_resMon) 
                return true;
        }
    }
    return false;
}

void Controller::toggleColumns()
{
    if (!m_subTreeMenuItem || !m_mainView)
        return;

    if (m_currentListType == Simple) {
        SimpleList *list = static_cast<SimpleList *>(
                m_mainView->testWidget());
        list->setTwoColumns(!list->twoColumns());
    } else {
        ItemRecyclingList *list = static_cast<ItemRecyclingList *>(
                m_mainView->testWidget());
        list->setTwoColumns(!list->twoColumns());
    }
}
