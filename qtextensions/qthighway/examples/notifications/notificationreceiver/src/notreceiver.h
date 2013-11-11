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

#ifndef APPMGRCLIENT_H
#define APPMGRCLIENT_H

#include <QVBoxLayout>
#include <QWidget>
#include <QModelIndex>
#include <QMap>
#include <QStringList>
#include <QPushButton>
#include <QMenu>
#include <QSpinBox>
#include <xqappmgr.h>
#include <XQSharableFile.h>
#include <QTimer>

class QLineEdit;
class QCheckBox;
class QLabel;

class NotificationsReceiver : public QWidget
{
    Q_OBJECT
            
public:
        NotificationsReceiver( QWidget *parent = 0, Qt::WFlags f = 0 );
        ~NotificationsReceiver();

public slots:
    void serviceStarted(XQAiwInterfaceDescriptor dscr);
    void serviceStopped(XQAiwInterfaceDescriptor dscr);
    
    void poolForService();
        
private:
        void initUi();
        
private:
        static QString POOLING_TEMPLATE;
        static QString NOTIFY_TEMPLATE;
        
        QTimer poolingTimer;
        
        XQApplicationManager *mgr;
        XQAiwInterfaceDescriptor *pooledDescr;
        
        QVBoxLayout *vl;
        QLabel *labelServiceName;
        QLabel *labelPoolingResult;
        QLabel *labelNotifyResult;
};

#endif
