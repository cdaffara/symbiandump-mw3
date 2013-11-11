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

#ifndef SYSINFOEXWINDOW_H
#define SYSINFOEXWINDOW_H

#include <QtGui>
#include "xqsysinfo.h"

#include "ui_sysinfoexwindow.h"

class SysInfoExWindow : public QMainWindow,
    public Ui::SysInfoExWindowClass
{
    Q_OBJECT
public:
    SysInfoExWindow(QWidget* parent = 0);
    ~SysInfoExWindow();
    
public Q_SLOTS:
    QString handleLanguage(XQSysInfo::Language language);
    void handleSystemInfoAction();
    void handleSystemInfo2Action();
    void handleFeatureDiscoverAction();
    
public:
    QString boolToString(bool boolean);
    void createMenus();
    
private:
    QAction* m_systemInfoAction;
    QAction* m_systemInfo2Action;
    QAction* m_featureDiscoverAction;
    QAction* m_exitAction;
};

#endif // SYSINFOEXWINDOW_H
