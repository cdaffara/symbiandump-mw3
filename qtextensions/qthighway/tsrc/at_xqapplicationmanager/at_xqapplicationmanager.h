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

#ifndef AT_XQAPPLICATIONMANAGER_H
#define AT_XQAPPLICATIONMANAGER_H

#include <QObject>
#include <QStringList>
#include <QList>
#include <xqappmgr.h>
#include <QVariant>

class Test_XQApplicationManager: public QObject
{
     Q_OBJECT

private slots:

    void init();
    
    void drm_file();
    void drm_sharablefile();
    void list_interface();
    void list_service_interface();
    void list_uri();
    void list_file();
    void list_sharablefile();
    void create_interface();
    void create_service_interface();
    void create_implementation();
    void create_uri();
    void create_uri_implementation();
    void create_file();
    void create_file_implementation();
    void create_sharablefile();
    void create_sharablefile_implementation();

    void cleanup();
    
    void handleOk(const QVariant& ret);
    void handleError(int err ,const QString& errString);

private:
    void testRequest(XQAiwRequest* request, const QString &operation, 
            const QString &arguments, bool embedded, bool synchronous, XQSharableFile *sharablefile = NULL);
    
private:
    const QList<int> listDrmAttr();
    QStringList mLog;
    XQApplicationManager *mAppManager;
    QVariant returnValue;
    bool mServiceAnswered;
    XQSharableFile* testSharableFile;
 };


#endif // AT_XQAPPLICATIONMANAGER_H
