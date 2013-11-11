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

#ifndef AT_SERVICESREGRESSION_H
#define AT_SERVICESREGRESSION_H

#include <QObject>
#include <xqappmgr.h>


class TestServicesRegression: public QObject
{
     Q_OBJECT

private slots:

    void initTestCase();
    void init();
    
    void platform_services_data();
    void platform_services();
    void platform_services_operations_data();
    void platform_services_operations();
    void platform_url_data();
    void platform_url();
    void platform_file_data();
    void platform_file();
    void platform_file_share();
    
    void cleanupTestCase();
    void cleanup();
    
private:
    void closeFocusedWG();
    
    bool closeApp;
    XQApplicationManager *mAppManager;
    XQAiwRequest* request;
 };


#endif // AT_SERVICES_REGRESSION_H
