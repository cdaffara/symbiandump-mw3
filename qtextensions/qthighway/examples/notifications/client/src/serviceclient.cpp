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

#include <QApplication>
#include <QKeyEvent>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QImageReader>
#include <QDebug>
#include <QTimer>
#include <QPushButton>
#include <QLineEdit>
#include <QDebug>
#include <QString>
#include <QCheckBox>

#include <QListView>

#include <xqservicerequest.h>
#include <xqservicelog.h>

#include <xqappmgr.h>
#include "serviceclient.h"

ServiceClient::ServiceClient(QWidget *parent, Qt::WFlags f)
    : QWidget(parent, f)
{
    XQSERVICE_DEBUG_PRINT("ServiceClient::ServiceClient");
    snd = NULL;
    /* Adjust the palette */
#if defined(Q_WS_S60)
    QPalette p = qApp->palette();
    QColor color(80,20,20);
    QColor bg(256,20,20);
    p.setColor(QPalette::Highlight, color.lighter(200));
    p.setColor(QPalette::Text, Qt::white);
    p.setColor(QPalette::Base, bg);
    p.setColor(QPalette::WindowText, Qt::white);
    p.setColor(QPalette::Window, bg);
    p.setColor(QPalette::ButtonText, Qt::white);
    p.setColor(QPalette::Button, color.lighter(150));
    p.setColor(QPalette::Link, QColor(240,40,40));

    qApp->setPalette(p);
#endif

    QPushButton *quitButton = new QPushButton(tr("quit"));
    connect(quitButton, SIGNAL(clicked()), qApp, SLOT(quit()));

    QPushButton *callButton = new QPushButton(tr("call"));
    connect(callButton, SIGNAL(clicked()), this, SLOT(dial()));

    mCheckSync = new QCheckBox("Synchronous send");
    mEmb = new QCheckBox("Embedded");
    mCheckAsyncAnswer = new QCheckBox("Asynchronous Answer");
    mCheckDeleteRequest = new QCheckBox("Delete request");
    mCheckDeleteRequest->setCheckState(Qt::Checked);
    
    mTextEdit = new QLineEdit("+123456");
    
    mTextRetValue = new QLineEdit("no ret value set");

    QLabel *label = new QLabel("CLIENT TEST");

    QLabel *status = new QLabel("-- status --");

    QVBoxLayout *vl = new QVBoxLayout;
    vl->setMargin(0);
    vl->setSpacing(0);

    vl->addWidget(label);
    vl->addWidget(mCheckSync);
    vl->addWidget(mCheckAsyncAnswer);
    vl->addWidget(mEmb);
    vl->addWidget(mCheckDeleteRequest);
    vl->addWidget(mTextEdit);
    vl->addWidget(mTextRetValue);
    vl->addWidget(status);
    vl->addStretch(10);
    vl->addWidget(callButton);
    vl->addWidget(quitButton);

    setLayout(vl);

#if defined(Q_WS_X11) || defined(Q_WS_WIN)
    setFixedSize(QSize(360,640)); // nHD
#elif defined(Q_WS_S60)
    showMaximized();
    showFullScreen();
#endif
}

ServiceClient::~ServiceClient()
{
    XQSERVICE_DEBUG_PRINT("ServiceClient::~ServiceClient");
}

void ServiceClient::dial()
{
    XQSERVICE_DEBUG_PRINT("ServiceClient::dial");
    if (snd) {
        delete snd ;
    }
    mTextRetValue->setText("no ret value set");

    bool isSync = (mCheckSync->checkState() == Qt::Checked);
    bool asyncAnswer = (mCheckAsyncAnswer->checkState() == Qt::Checked);
    bool deleteRequest = (mCheckDeleteRequest->checkState() == Qt::Checked);
    
    snd = mgr.create("com.nokia.services.hbserviceprovider", "dialer", "dial(QString,bool)", isSync);
    
    QList<QVariant> args;
    
    args << QVariant(mTextEdit->text());
    args << QVariant(asyncAnswer);
    
    snd->setArguments(args);
    
    snd->setEmbedded(mEmb->checkState());

    if (isSync) {
        QVariant retValue;
        connect(snd, SIGNAL(requestError(int)), this, SLOT(requestError(int)));
        bool ret = snd->send(retValue);    
        mTextRetValue->setText(retValue.toString());
    } else {
        bool ret = snd->send();    
        connect(snd, SIGNAL(requestCompleted(QVariant)), this, SLOT(requestCompleted(QVariant)));
        connect(snd, SIGNAL(requestError(int)), this, SLOT(requestError(int)));
    }
    
    if (deleteRequest) {
        delete snd;
        snd = NULL;
    }
}

void ServiceClient::requestCompleted(const QVariant& value)
{
    XQSERVICE_DEBUG_PRINT("ServiceClient::requestCompleted");
    mTextRetValue->setText(value.toString());
}

void ServiceClient::requestError(int err)
{
    XQSERVICE_DEBUG_PRINT("ServiceClient::requestError");
    mTextRetValue->setText("error: " + QString::number(err));
}
