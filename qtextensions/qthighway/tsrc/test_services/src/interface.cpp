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


#include "interface.h"
#include <asynctimer.h>

#include <QDebug>


TestInterface::TestInterface(QString service, QString interface) 
 : XQServiceProvider(QLatin1String((service + "." + interface).toLatin1())), 
   mService(service), 
   mInterface(interface)
{
    publishAll();
}

TestInterface::~TestInterface()
{

}

QStringList TestInterface::request(QString input)
{
    qDebug() << "[QTH] [TestInterface] request";
     
    qDebug() << "[QTH] [TestInterface] service:" << mService; 
    qDebug() << "[QTH] [TestInterface] interface:" << mInterface; 
    qDebug() << "[QTH] [TestInterface] input:" << input; 
    
    QStringList ret;
    
    ret.append(mService);
    ret.append(mInterface);
    ret.append(input);
    
    qDebug() << "[QTH] [TestInterface] request end";
    
    return ret;
}

void TestInterface::asyncRequest(QString input)
{
    qDebug() << "[QTH] [TestInterface] async request";
     
    AsyncTimer* timer = new AsyncTimer(setCurrentRequestAsync(), input); 
    connect(timer, SIGNAL(timeout(int, QString)), this, SLOT(complete(int, QString)));
    timer->start(10);
    
    qDebug() << "[QTH] [TestInterface] async request end";
}

void TestInterface::complete(int requestID, QString input)
{
    qDebug() << "[QTH] [TestInterface] complete request";
     
    qDebug() << "[QTH] [TestInterface] service:" << mService; 
    qDebug() << "[QTH] [TestInterface] interface:" << mInterface; 
    qDebug() << "[QTH] [TestInterface] input:" << input; 
    
    QStringList ret;
    
    ret.append(mService);
    ret.append(mInterface);
    ret.append(input);
    
    completeRequest(requestID,  QVariant(ret)); 
    
    qDebug() << "[QTH] [TestInterface] complete request end";
}

void TestInterface::view(QString pathOrUri)
{
    qDebug() << "[QTH] [TestInterface] view";
     
    if(mInterface == "com.nokia.symbian.IUriView")
    {
        qDebug() << "[QTH] [TestInterface] uri:" << pathOrUri;
    }
    else if(mInterface == "com.nokia.symbian.IFileView")
    {
        qDebug() << "[QTH] [TestInterface] path:" << pathOrUri;
    }
    
    qDebug() << "[QTH] [TestInterface] view end";
}

void TestInterface::view(XQSharableFile file)
{
    qDebug() << "[QTH] [TestInterface] view sharable file";
}


