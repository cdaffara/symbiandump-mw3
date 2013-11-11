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

#include "settingsmanagerexwindow.h"

#include <ProfileEngineSDKCRKeys.h>
#include <HWRMPowerStateSDKPSKeys.h>
#include <QMenuBar>
#include <QTimer>

#include "xqsettingsmanager.h"
#include "xqsettingskey.h"
#include "xqpublishandsubscribeutils.h"

const quint32 KExampleKey = 0x00000001;
const qint32 KExampleProperty = {0xE2F629E4}; //Same as SID

SettingsManagerExWindow::SettingsManagerExWindow(QWidget* parent) : QMainWindow(parent)
{
	setupUi(this);
	createMenus();

    m_settingsManager = new XQSettingsManager(this);

    XQSettingsKey profileKey (XQSettingsKey::TargetCentralRepository, KCRUidProfileEngine.iUid, KProEngActiveProfile);
    m_settingsManager->startMonitoring(profileKey);
    currentProfile(m_settingsManager->readItemValue(profileKey));
    
    XQSettingsKey chargerKey(XQSettingsKey::TargetPublishAndSubscribe, KPSUidHWRMPowerState.iUid, KHWRMChargingStatus);
    m_settingsManager->startMonitoring(chargerKey);
    currentChargerStatus(m_settingsManager->readItemValue(chargerKey));
    
    connect(m_settingsManager, SIGNAL(itemDeleted(XQSettingsKey)), this, SLOT(itemDeleted(XQSettingsKey)));
    connect(m_settingsManager, SIGNAL(valueChanged(XQSettingsKey, QVariant)), this, SLOT(handleChanges(XQSettingsKey, QVariant)));
}

void SettingsManagerExWindow::createMenus()
{
    m_createAction = new QAction(tr("Create item"), this);
    m_deleteAction = new QAction(tr("Delete item"), this);
    m_deleteAction->setDisabled(true);
    QAction* exitAct = new QAction(tr("Exit"), this);
    menuBar()->addAction(m_createAction);
    menuBar()->addAction(m_deleteAction);
    menuBar()->addAction(exitAct);
    
    connect(m_createAction, SIGNAL(triggered()), this, SLOT(createPropertyItem()));
    connect(m_deleteAction, SIGNAL(triggered()), this, SLOT(deletePropertyItem()));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));
}

void SettingsManagerExWindow::handleChanges(const XQSettingsKey& key, const QVariant& value)

{
    if (key.uid() == KCRUidProfileEngine.iUid && key.key() == KProEngActiveProfile) 
    {
        currentProfile(value);
    } 
    if (key.uid() == KPSUidHWRMPowerState.iUid && key.key() == KHWRMChargingStatus) 
    {
        currentChargerStatus(value);
    }
}

void SettingsManagerExWindow::currentProfile(QVariant value)
{ 
    switch (value.toInt()) 
    {
        case 0:
        {
            profileLabel->setText("Profile: General");
            break;
        }
        case 1:
        {
            profileLabel->setText("Profile: Silent");
            break;
        }
        case 2:
        {
            profileLabel->setText("Profile: Meeting");
            break;
        }
        case 3:
        {
            profileLabel->setText("Profile: Outdoor");
            break;
        }
        case 4 :
        {
            profileLabel->setText("Profile: Pager");
            break;
        }
        case 5:
        {
            profileLabel->setText("Profile: Off-line");
            break;
        }
        case 6:
        {
            profileLabel->setText("Profile:: Drive");
            break;
        }
        default:
        {
            profileLabel->setText("Profile: User defined");
        }
    }
}

void SettingsManagerExWindow::currentChargerStatus(QVariant value)
{
    switch (value.toInt()) 
    {
        case EChargingStatusNotConnected:
        {
            chargerLabel->setText("Charger: Is not connected");
            break;
        }
        case EChargingStatusCharging:
        {
            chargerLabel->setText("Charger: Charging");
            break;
        }
        case EChargingStatusNotCharging:
        {
            chargerLabel->setText("Charger: Not charging");
            break;
        }
        case EChargingStatusAlmostComplete:
        {
            chargerLabel->setText("Charger: Almost complete");
            break;
        }
        case EChargingStatusChargingComplete:
        {
            chargerLabel->setText("Charger: Battery full");
            break;    
        }
        case EChargingStatusChargingContinued :
        {
            chargerLabel->setText("Charger: Continued after brief interruption");
            break;
        }
        default:
        {
            chargerLabel->setText("Charger: Error");
        }
    }
}

void SettingsManagerExWindow::deletePropertyItem()
{
    XQPublishAndSubscribeUtils utils(*m_settingsManager);

    XQPublishAndSubscribeSettingsKey testKey(KExampleProperty, KExampleKey);
    utils.deleteProperty(testKey);
}

void SettingsManagerExWindow::itemDeleted(const XQSettingsKey& key)
{
    if (key.uid() == KExampleProperty && key.key() == KExampleKey) 
    {
        testLabel->setText("Property item deleted!");
    }
    m_deleteAction->setDisabled(true);
    m_createAction->setEnabled(true);
}

void SettingsManagerExWindow::createPropertyItem()
{
    XQPublishAndSubscribeUtils utils(*m_settingsManager);

    XQPublishAndSubscribeSettingsKey testKey(KExampleProperty, KExampleKey);
    utils.defineProperty(testKey, XQSettingsManager::TypeByteArray);

    m_settingsManager->writeItemValue(testKey, QByteArray("Property item created!"));
    
    m_settingsManager->startMonitoring(testKey);
    testLabel->setText(m_settingsManager->readItemValue(testKey).toString());
    m_createAction->setDisabled(true);
    m_deleteAction->setEnabled(true);
}
