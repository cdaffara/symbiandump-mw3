/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation, version 2.1 of the License.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this program.  If not, 
* see "http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html/".
*
* Description:
*
*/

#include "sysinfoexwindow.h"
#include "xqsysinfo.h"
#include "xqlistwidget.h"
#ifndef KFeatureIdMmc   //Workaround for RnD SDKs
    #include <featureinfo.h>
#endif

#include <QSignalMapper>
#include <QMenu>
#include <QMenuBar>

SysInfoExWindow::SysInfoExWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setupUi(this);
    createMenus();
    
    // Initializing sysinfo
    XQSysInfo* sysInfo = new XQSysInfo(this);
    
    // System Info
    modelLabel->setText("Model: "+sysInfo->model());
    manufacturerLabel->setText("Manufacturer: " + sysInfo->manufacturer());
    versionLabel->setText("Version: "+sysInfo->softwareVersion());
    languageLabel->setText("Language: "+handleLanguage(sysInfo->currentLanguage()));
    imeiLabel->setText("Imei: "+sysInfo->imei());
    imsiLabel->setText("Imsi: "+sysInfo->imsi());
    
    // System Info 2
    signalStrengthLabel->setText("Signal strenght: " + QString::number(sysInfo->signalStrength()));
    batteryLevelLabel->setText("Battery level: " + QString::number(sysInfo->batteryLevel()));
    diskSpaceLabel->setText("Free space (c:): " + QString::number(sysInfo->diskSpace(XQSysInfo::DriveC)/1048576,'f',1) + "MB");
    
    if (sysInfo->isDiskSpaceCritical(XQSysInfo::DriveC))
    {
        criticalDiskSpaceLabel = new QLabel("Disk space (c:): Critical");
    }
    else
    {
        criticalDiskSpaceLabel = new QLabel("Disk space (c:): ok");   
    }
    
    // Feature Discover
    listWidget->addItem("MMC: "+boolToString(XQSysInfo::isSupported(KFeatureIdMmc)));
    listWidget->addItem("Camera: "+boolToString(XQSysInfo::isSupported(KFeatureIdCamera)));
    listWidget->addItem("Cover display: "+boolToString(XQSysInfo::isSupported(KFeatureIdCoverDisplay)));
    listWidget->addItem("Video recorder: "+boolToString(XQSysInfo::isSupported(KFeatureIdVideoRecorder)));
    listWidget->addItem("Flight mode: "+boolToString(XQSysInfo::isSupported(KFeatureIdFlightMode)));
    listWidget->addItem("Offline mode: "+boolToString(XQSysInfo::isSupported(KFeatureIdOfflineMode)));
    listWidget->addItem("OpenGL: "+boolToString(XQSysInfo::isSupported(KFeatureIdOpenGLES3DApi)));
    listWidget->addItem("IrDA: "+boolToString(XQSysInfo::isSupported(KFeatureIdIrda))); 
    listWidget->addItem("Bluetooth: "+boolToString(XQSysInfo::isSupported(KFeatureIdBt)));
    listWidget->addItem("Qwerty: "+boolToString(XQSysInfo::isSupported(KFeatureIdQwertyInput)));
    listWidget->addItem("USB: "+boolToString(XQSysInfo::isSupported(KFeatureIdUsb)));
    listWidget->addItem("BT Fax Profile: "+boolToString(XQSysInfo::isSupported(KFeatureIdBtFaxProfile)));
    listWidget->addItem("BT Imaging Profile: "+boolToString(XQSysInfo::isSupported(KFeatureIdBtImagingProfile)));
    listWidget->addItem("Instant Messaging: "+boolToString(XQSysInfo::isSupported(KFeatureIdIm)));
    listWidget->addItem("Precense feature: "+boolToString(XQSysInfo::isSupported(KFeatureIdPresence)));
    listWidget->addItem("SVGT: "+boolToString(XQSysInfo::isSupported(KFeatureIdSvgt)));
    listWidget->addItem("Audio effects: "+boolToString(XQSysInfo::isSupported(KFeatureIdAudioEffectsApi)));
    listWidget->addItem("Equalizer: "+boolToString(XQSysInfo::isSupported(KFeatureIdEqualizer)));
    listWidget->addItem("GSM cellular stack: "+boolToString(XQSysInfo::isSupported(KFeatureIdProtocolGsm)));
    listWidget->addItem("WCDMA cellular stack: "+boolToString(XQSysInfo::isSupported(KFeatureIdProtocolWcdma)));
    listWidget->addItem("CDMA cellular stack: "+boolToString(XQSysInfo::isSupported(KFeatureIdProtocolCdma)));
    listWidget->addItem("Wlan: "+boolToString(XQSysInfo::isSupported(KFeatureIdProtocolWlan)));
    listWidget->addItem("Location: "+boolToString(XQSysInfo::isSupported(KFeatureIdLocationFrameworkCore)));
    listWidget->addItem("Landmarks: "+boolToString(XQSysInfo::isSupported(KFeatureIdLandmarks)));
    
    stackedWidget->setCurrentIndex(0);
}

SysInfoExWindow::~SysInfoExWindow()
{
}

void SysInfoExWindow::createMenus()
{
    m_systemInfoAction = new QAction(tr("System Info"), this);
    menuBar()->addAction(m_systemInfoAction);
    connect(m_systemInfoAction, SIGNAL(triggered()), this, SLOT(handleSystemInfoAction()));
    
    m_systemInfo2Action = new QAction(tr("System Info 2"), this);
    menuBar()->addAction(m_systemInfo2Action);
    connect(m_systemInfo2Action, SIGNAL(triggered()), this, SLOT(handleSystemInfo2Action()));
    
    m_featureDiscoverAction = new QAction(tr("Feature discover"), this);
    menuBar()->addAction(m_featureDiscoverAction);
    connect(m_featureDiscoverAction, SIGNAL(triggered()), this, SLOT(handleFeatureDiscoverAction()));
    
    m_exitAction = new QAction(tr("Exit"), this);
    menuBar()->addAction(m_exitAction);
    connect(m_exitAction, SIGNAL(triggered()), this, SLOT(close()));
}

void SysInfoExWindow::handleSystemInfoAction()
{
    stackedWidget->setCurrentIndex(0);
}

void SysInfoExWindow::handleSystemInfo2Action()
{
    stackedWidget->setCurrentIndex(1);
}

void SysInfoExWindow::handleFeatureDiscoverAction()
{
    stackedWidget->setCurrentIndex(2);
}

QString SysInfoExWindow::boolToString(bool boolean)
{
    if (boolean) 
    {
        return "Supported";
    } 
    else 
    {
        return "Not supported";
    }
}

QString SysInfoExWindow::handleLanguage(XQSysInfo::Language language)
{
    switch (language) 
    {
        case XQSysInfo::LangEnglish:
            return "English";
        case XQSysInfo::LangFinnish:
            return "Finnish";
        case XQSysInfo::LangSwedish:
            return "Swedish";
        case XQSysInfo::LangNorwegian:
            return "Norwegian";
        case XQSysInfo::LangIcelandic:
            return "Icelandic";
        case XQSysInfo::LangFrench:
            return "French";
        case XQSysInfo::LangGerman:
            return "German";
        case XQSysInfo::LangSpanish:
            return "Spanish";
        case XQSysInfo::LangItalian:
            return "Italian";
        case XQSysInfo::LangPortuguese:
            return "Portuguese";
        default:
            return "Unknown";
    }
}
