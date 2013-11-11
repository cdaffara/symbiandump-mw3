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

#include "xqservicelog.h"

#include <hbmainwindow.h>

#include <hbserviceprovidermainwindow.h>
#include <hbserviceproviderview.h>


HbServiceProviderMainWindow::HbServiceProviderMainWindow(QWidget *parent)
    : HbMainWindow(parent)
{
    XQSERVICE_DEBUG_PRINT("HbServiceProviderMainWindow::HbServiceProviderMainWindow");
    mMapService = new MapService();
    connect(mMapService,SIGNAL(showView(QString)), 
            this, SLOT(showView(QString)));

    mMapView = new HbMapView(mMapService);
    connect(mMapService,SIGNAL(setAddress(QString,QString,QString)), 
            mMapView, SLOT(setAddress(QString,QString,QString)));
    mDialerService = new DialerService();
    connect(mDialerService,SIGNAL(showView(QString)), 
            this, SLOT(showView(QString)));

    mDialerView = new HbDialerView(mDialerService);
    connect(mDialerService,SIGNAL(setNumber(QString)), 
            mDialerView, SLOT(setNumber(QString)));
}

HbServiceProviderMainWindow::~HbServiceProviderMainWindow()
{
    XQSERVICE_DEBUG_PRINT("HbServiceProviderMainWindow::~HbServiceProviderMainWindow");
    delete mMapService;
    delete mDialerService;
    delete mMapView;
    delete mDialerView;
}

void HbServiceProviderMainWindow::showView(const QString& name)
{
    XQSERVICE_DEBUG_PRINT("HbServiceProviderMainWindow::showView");
    if (name == "map") {    
        addView(mMapView);    
    }
    else if (name == "dialer") {
        addView(mDialerView);    
    }
}

