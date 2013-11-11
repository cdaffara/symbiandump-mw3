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

#include <QDebug>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QApplication>
#include <QLabel>
#include <QThread>

#include "serviceview.h"

class SleeperThread : public QThread
{
public:
    static void msleep(unsigned long msecs)
    {
    QThread::msleep(msecs);
    }
};

TestServiceView::TestServiceView(QString service, QWidget *parent, Qt::WFlags f) 
    :QWidget( parent,f ), mService(service)
{
    qDebug() << "[QTH] [AutoTestServices] TestServiceView::TestServiceView START";

    QLabel *label = new QLabel(service);

    QVBoxLayout *vLayout = new QVBoxLayout;

    vLayout->addWidget(label);

    setLayout(vLayout);
    
    showFullScreen();
    
    qDebug() << "[QTH] [AutoTestServices] TestServiceView::TestServiceView END";
}

TestServiceView::~TestServiceView()
{
    qDebug() << "[QTH] [AutoTestServices] TestServiceView::~TestServiceView START";
    
    foreach(TestInterface* interface, mInterfaces)
        delete interface;
    
    qDebug() << "[QTH] [AutoTestServices] TestServiceView::~TestServiceView END";
}

void TestServiceView::addInterface(QString interface)
{
    TestInterface *testInterface = new TestInterface(mService, interface);
    connect(testInterface, SIGNAL(clientDisconnected()), this, SLOT(requestCompleted()));
    connect(testInterface, SIGNAL(returnValueDelivered()), this, SLOT(requestCompleted()));
    connect(this, SIGNAL(quit()), qApp, SLOT(quit()));
    mInterfaces.append(testInterface);
    qDebug() << "[QTH] [AutoTestServices] TestServiceView::addInterface:" << interface;
}

void TestServiceView::requestCompleted()
{
//    SleeperThread::msleep(3000);
    emit quit();
}
