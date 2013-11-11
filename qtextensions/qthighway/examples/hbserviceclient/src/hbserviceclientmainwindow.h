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

#ifndef HBSERVICECLIENTMAINWINDOW_H
#define HBSERVICECLIENTMAINWINDOW_H

#include <hbmainwindow.h>

class HbServiceClientView;

class HbServiceClientMainWindow : public HbMainWindow
{
    Q_OBJECT

public:
    HbServiceClientMainWindow(QWidget *parent=0);
    ~HbServiceClientMainWindow();

public slots:
    
private slots:
    
private:
    
private:
    HbServiceClientView*    mView;

};

#endif // HBSERVICECLIENTMAINWINDOW_H
