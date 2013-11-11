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

#include <QtGlobal>
#include <QApplication>
#include <QKeyEvent>
#include <QLabel>
#include <QStackedWidget>
#include <QImageReader>
#include <QDebug>
#include <QTimer>
#include <QPushButton>
#include <QList>
#include <QLineEdit>
#include <QString>
#include <QCheckBox>
#include <QAction>
#include <QListView>
#include <QUrl>
#include <QFileInfo>
#include <math.h>
#include <QCompleter>
#include <xqserviceglobal.h>
#include <QDir>
#include <QTranslator>
#include <cntservicescontact.h>

#include <XQSharableFile.h>

// Include declarations to see if any compilation issues
#include <xqaiwdecl.h>
#include <xqaiwdeclplat.h>

#include "notreceiver.h"

#define CONNECT_ASSERT(a, b, c, d) if (!QObject::connect((a), (b), (c), (d))) { \
    qWarning("connection failed!"); \
    qApp->exit(0); \
    exit(0); \
    } 
    

QString NotificationsReceiver::POOLING_TEMPLATE("pooling: %1"); 
QString NotificationsReceiver::NOTIFY_TEMPLATE("notification: %1");

NotificationsReceiver::NotificationsReceiver(QWidget *parent, Qt::WFlags f)
    : QWidget(parent, f),
      poolingTimer(),
      pooledDescr(0)
{
    mgr = new XQApplicationManager();
    
    CONNECT_ASSERT(&poolingTimer, SIGNAL(timeout()), this, SLOT(poolForService()));
    
    bool ok;
    
    ok = QObject::connect(mgr, SIGNAL(serviceStarted(XQAiwInterfaceDescriptor)), this, SLOT(serviceStarted(XQAiwInterfaceDescriptor)));
    ok = QObject::connect(mgr, SIGNAL(serviceStopped(XQAiwInterfaceDescriptor)), this, SLOT(serviceStopped(XQAiwInterfaceDescriptor)));
    
    initUi();
    
    XQAiwInterfaceDescriptor descr();
    
    QString serviceName = QString("com.nokia.services.hbserviceprovider");
    QString interfaceName = QString("Dialer");
    QString opName = "dial(QString,bool)";
    
    QList<XQAiwInterfaceDescriptor> dscrList = mgr->list(serviceName, interfaceName, opName);
    
    if (dscrList.size() == 1) {
        XQAiwInterfaceDescriptor dscr = dscrList.at(0);
        mgr->notifyRunning(dscr);
        qDebug() << "Start notifications for: " << dscr.serviceName() << " " << dscr.interfaceName();
        if (mgr->isRunning(dscr)) {
            serviceStarted(dscr);
        } else {
            serviceStopped(dscr);
        }
        labelServiceName->setText(serviceName + "." + interfaceName + "." + opName);
        pooledDescr = new XQAiwInterfaceDescriptor(dscr);
    } else {
        qWarning("Whoops, requested service not found.");
        labelServiceName->setText("service not found");
    }
    
    poolingTimer.start(1000);
}

void NotificationsReceiver::initUi()
{
    /* Adjust the palette */
#if defined(Q_WS_S60)
    QPalette p = qApp->palette();
    QColor color(192,192,192);
    QColor bg(208, 242, 209);
    p.setColor(QPalette::Highlight, color.lighter(200));
    p.setColor(QPalette::Text, Qt::black);
    p.setColor(QPalette::Base, bg);
    p.setColor(QPalette::WindowText, Qt::black);
    p.setColor(QPalette::Window, bg);
    p.setColor(QPalette::ButtonText, Qt::black);
    p.setColor(QPalette::Button, color.lighter(150));
    p.setColor(QPalette::Link, QColor(240,40,40));
    qApp->setPalette(p);
#endif
    QPushButton *quitButton = new QPushButton(tr("quit"));
    connect(quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));
    
    labelServiceName = new QLabel("...service name...");
    labelPoolingResult = new QLabel(POOLING_TEMPLATE);
    labelNotifyResult = new QLabel(NOTIFY_TEMPLATE);
    
    vl = new QVBoxLayout;
    vl->setMargin(10);
    vl->setSpacing(5);

    vl->addWidget(labelServiceName);
    vl->addWidget(labelPoolingResult);
    vl->addWidget(labelNotifyResult);
    
    vl->insertStretch(-1);
    vl->addWidget(quitButton);

    setLayout(vl);

#if defined(Q_WS_X11) || defined(Q_WS_WIN)
    setFixedSize(QSize(360,640)); // nHD
#elif defined(Q_WS_S60)
    // showMaximized();
    showFullScreen();
#endif
}

NotificationsReceiver::~NotificationsReceiver()
{
    delete mgr;
    delete pooledDescr;
}

void NotificationsReceiver::serviceStarted(XQAiwInterfaceDescriptor dscr)
{
    qDebug("service started");
    labelNotifyResult->setText("[N] service started");
}

void NotificationsReceiver::serviceStopped(XQAiwInterfaceDescriptor dscr)
{
    qDebug("service started");    
    labelNotifyResult->setText("[N] service stopped");
}

void NotificationsReceiver::poolForService()
{
    qDebug(__PRETTY_FUNCTION__);
    XQAiwInterfaceDescriptor d(*pooledDescr);
    labelPoolingResult->setText(QString("[P] service %1").arg(mgr->isRunning(*pooledDescr) ? "running" : "stopped"));
}
    
