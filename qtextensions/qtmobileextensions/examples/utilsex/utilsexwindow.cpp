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

 #include <QInputDialog>
#include "utilsexwindow.h"
#include "xqutils.h"

UtilsExWindow::UtilsExWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setupUi(this);
    
    m_utils = new XQUtils(this);
    m_timer = new QTimer(this);
    
    
    QString imagesPath = XQUtils::imagesPath();
    
    useResetInactivityTime();
    setuUiComponents();
}

UtilsExWindow::~UtilsExWindow()
{
}

void UtilsExWindow::useResetInactivityTime()
{
    m_timer->setInterval(1000);
    m_timer->start();
    connect(m_timer, SIGNAL(timeout()), m_utils, SLOT(resetInactivityTime()));
}

void UtilsExWindow::setuUiComponents()
{
    startLitButton->hide();
    connect(stopLitButton, SIGNAL(clicked()), m_timer, SLOT(stop()));
    connect(stopLitButton, SIGNAL(clicked()), stopLitButton, SLOT(hide()));
    connect(startLitButton, SIGNAL(clicked()), m_timer, SLOT(start()));
    connect(startLitButton, SIGNAL(clicked()), startLitButton, SLOT(hide()));
    connect(startLitButton, SIGNAL(clicked()), stopLitButton, SLOT(show()));
    connect(startLitButton, SIGNAL(clicked()), stopLitButton, SLOT(setFocus()));
    connect(stopLitButton, SIGNAL(clicked()), startLitButton, SLOT(show()));
    connect(stopLitButton, SIGNAL(clicked()), startLitButton, SLOT(setFocus()));
    
    m_exitAction = new QAction(tr("Exit"), this);
    menuBar()->addAction(m_exitAction);
    connect(m_exitAction, SIGNAL(triggered()), this, SLOT(close()));
}
