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

#ifndef SETTINGSMANAGEREXWINDOW_H
#define SETTINGSMANAGEREXWINDOW_H

// INCLUDES
#include <QMainWindow>
#include "ui_settingsmanagerexwindow.h"

// FORWARD DECLARATIONS
class XQSettingsManager;
class XQSettingsKey;

// CLASS DECLARATION
class SettingsManagerExWindow : public QMainWindow,
    public Ui::SettingsManagerExClass
{
    Q_OBJECT

public:
	SettingsManagerExWindow(QWidget* parent = 0);
	
    void createMenus();
    void currentProfile(QVariant value);
    void currentChargerStatus(QVariant value);

public slots:
    void deletePropertyItem();
    void createPropertyItem();
    void itemDeleted(const XQSettingsKey& key);
    void handleChanges(const XQSettingsKey& key, const QVariant& value);

private:
    XQSettingsManager* m_settingsManager;
    QAction* m_createAction;
    QAction* m_deleteAction;
};

#endif // SETTINGSMANAGEREXWINDOW_H
