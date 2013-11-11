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

#ifndef UTILSEXWINDOW_H
#define UTILSEXWINDOW_H

#include <QtGui>
#include "xqutils.h"

#include "ui_utilsexwindow.h"

class XQUtils;

class UtilsExWindow : public QMainWindow,
    public Ui::UtilsExWindowClass
{
    Q_OBJECT
public:
    UtilsExWindow(QWidget* parent = 0);
    ~UtilsExWindow();
    
public:
    void useResetInactivityTime();
    void setuUiComponents();
    
private:
    XQUtils* m_utils;
    QTimer* m_timer;
    QAction* m_exitAction;
};

#endif // UTILSEXWINDOW_H
