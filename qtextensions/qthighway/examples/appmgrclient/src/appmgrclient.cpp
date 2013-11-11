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

#include "appmgrservices.h"
#include "appmgrclient.h"


//
// TODO: Better UI for test cases which allows
// more test cases.
//
AppMgrClient::AppMgrClient(QWidget *parent, Qt::WFlags f)
    : QWidget(parent, f),
      actionButton(0),
      req1(0),
      req2(0),
      req3(0),
      req4(0),
      req5(0),
      req6(0),
      req7(0),
      req8(0),
      req9(0),
      req10(0),
      anyReq(0),
      mImplIndex(0)      
{
    /* Adjust the palette */
#if defined(Q_WS_S60)
    QPalette p = qApp->palette();
    QColor color(192,192,192);
    QColor bg(0,128,192);
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

    
    QPushButton *testButton1 = 0;
    QPushButton *anyTestButton = 0;

    testButton1 = new QPushButton("Tests");
    mMenu = new QMenu(this);
    testButton1->setMenu(mMenu);
    anyTestButton = new QPushButton(tr("Any test"));
    connect(anyTestButton, SIGNAL(clicked()), this, SLOT(anyTest()));

    QAction *test1 = new QAction("1:Interface", this);
    connect(test1, SIGNAL(triggered()), this, SLOT(test1()));
    QAction *test2 = new QAction("2:Descriptor", this);
    connect(test2, SIGNAL(triggered()), this, SLOT(test2()));
    QAction *test3 = new QAction("3:Big data", this);
    connect(test3, SIGNAL(triggered()), this, SLOT(test3()));
    QAction *test4 = new QAction("4:QAction", this);
    connect(test4, SIGNAL(triggered()), this, SLOT(test4()));
    QAction *test5 = new QAction("5:appto:", this);
    connect(test5, SIGNAL(triggered()), this, SLOT(test5()));
    QAction *test6 = new QAction("6:testto:", this);
   connect(test6, SIGNAL(triggered()), this, SLOT(test6()));
    QAction *test7 = new QAction("7:MIME", this);
    connect(test7, SIGNAL(triggered()), this, SLOT(test7()));
    QAction *test8 = new QAction("8:URI", this);
    connect(test8, SIGNAL(triggered()), this, SLOT(test8()));
    QAction *test9 = new QAction("9:XQSharableFile", this);
    connect(test9, SIGNAL(triggered()), this, SLOT(test9()));
    QAction *test10 = new QAction("10:Select contact", this);
    connect(test10,  SIGNAL(triggered()), this, SLOT(test10()));
    QAction *test11 = new QAction("11:getDrmAttr", this);
    connect(test11, SIGNAL(triggered()), this, SLOT(test11()));

    mMenu = new QMenu(this);
    mMenu->addAction(test1);
    mMenu->addAction(test2);
    mMenu->addAction(test3);
    mMenu->addAction(test4);
    mMenu->addAction(test5);
    mMenu->addAction(test6);
    mMenu->addAction(test7);
    mMenu->addAction(test8);
    mMenu->addAction(test9);
    mMenu->addAction(test10);
    mMenu->addAction(test11);

    testButton1->setMenu(mMenu);
    
    mCheckEmbedded = new QCheckBox("Embedded");
    mSynchronous = new QCheckBox("Synchronous");
    mBackground = new QCheckBox("Background");
    mForeground = new QCheckBox("Foreground");
    mCheckDeleteRequest = new QCheckBox("Delete request");
    mGenericSend = new QCheckBox("Use generic send()");
    mCheckEmbedded->setCheckState(Qt::Checked);
    mSynchronous->setCheckState(Qt::Checked);
    mGenericSend->setCheckState(Qt::Checked);  // Apply one send() for both embedded/non-embedded


    QLabel *inputData = new QLabel(tr("Data size"));
    mDataSpinBox = new QSpinBox;
    mDataSpinBox->setMinimum(0);
    mDataSpinBox->setMaximum(1024*1024);
    mDataSpinBox->setValue(1024);
    mDataSpinBox->setSingleStep(1024*10);
       
    mReqArg = new QLineEdit("0");
    mTextRetValue = new QLineEdit("no ret value set");

    QFileInfo appinfo (qApp->applicationFilePath());
    mAppName = "XQTESTER " + appinfo.baseName();

    QLabel *label = new QLabel(mAppName);

    vl = new QVBoxLayout;
    vl->setMargin(0);
    vl->setSpacing(0);

    vl->addWidget(label);
    vl->addWidget(mCheckEmbedded);
    vl->addWidget(mSynchronous);
    vl->addWidget(mBackground);
    vl->addWidget(mForeground);
    vl->addWidget(mCheckDeleteRequest);
    vl->addWidget(mGenericSend);
    vl->addWidget(inputData);
    vl->addWidget(mDataSpinBox);
    vl->addWidget(mReqArg);
    vl->addWidget(mTextRetValue);
    vl->addWidget(new QLabel(" "));
    vl->addWidget(testButton1);
    vl->addWidget(anyTestButton);
    vl->addWidget(quitButton);

    setLayout(vl);

#if defined(Q_WS_X11) || defined(Q_WS_WIN)
    setFixedSize(QSize(360,640)); // nHD
#elif defined(Q_WS_S60)
    // showMaximized();
    showFullScreen();
#endif

}

AppMgrClient::~AppMgrClient()
{
    delete req1;
    delete req2;
    delete req3;
    delete req4;
    delete req5;
    delete req6;
    delete req7;
    delete req8;
    delete req9;
    delete req10;
    delete mMenu;
    
}

// Tester helper function
bool AppMgrClient::test(XQAiwRequest **req, const QString& interface, const QString& operation)
{
    bool embed = (mCheckEmbedded->checkState() == Qt::Checked);
    bool sync = (mSynchronous->checkState() == Qt::Checked);
    bool background = (mBackground->checkState() == Qt::Checked);
    
    qDebug() << mAppName << " test options: embed=" << embed << ",sync=" << sync << "background=" << background;
    
    if (!*req)
    {
        *req = appmgr.create(interface, operation);
        assert(mTestCase + ".req != NULL", *req != NULL);
        if (!*req)
        {
            qDebug() <<  mAppName << " AIW-ERROR::NULL request";
            qDebug() << mAppName << " Last error=" << appmgr.lastError();
            return false;
        }
        connectSignals(*req);        
    }
    // Test embedded funcions
    (*req)->setEmbedded(embed);
    (*req)->setSynchronous(sync);
    (*req)->setBackground(background);
    
    
    bool ret=test(req, mReqArg->text());

    return ret;
    
}


// Tester helper function
bool AppMgrClient::test(XQAiwRequest **req, XQAiwInterfaceDescriptor &impl, const QString& operation)
{
    if (!*req)
    {
        *req = appmgr.create(impl, operation);
        assert(mTestCase + ".req != NULL", *req != NULL);
        connectSignals(*req);        
    }
    bool ret = test(req, mReqArg->text());

    return ret;
}



// Tester helper function
bool AppMgrClient::test(XQAiwRequest **req, const QString &arg)
{

    if (!req || !*req)
    {
        qDebug() <<  mAppName << " AIW-ERROR::NULL request";
        return false;
    }

    
    bool embed = (mCheckEmbedded->checkState() == Qt::Checked);
    bool sync = (mSynchronous->checkState() == Qt::Checked);
    bool background = (mBackground->checkState() == Qt::Checked);
    bool foreground = (mForeground->checkState() == Qt::Checked);
    qDebug() << mAppName << " test options: embed=" << embed << ",sync=" << sync << "background=" << background;
    
    bool ret=true;
    
    // Set arguments for request
    QList<QVariant> args;
    args << arg;
    if ((*req)->operation() == OPERATION1)
    {
        args << QVariant(!sync);
    }
    (*req)->setArguments(args);

    bool genericSend = (mGenericSend->checkState() == Qt::Checked);
    (*req)->setEmbedded(embed);
    (*req)->setSynchronous(sync);
    (*req)->setBackground(background);
    
    // Apply additional options
    XQRequestInfo options;
    options.setForeground(foreground);
    // Save the test case to options as tester data. This data is also passed to service app
    // The test case is utilized in requestOk and requestError signal handlers
    options.setInfo(TESTCASE_INFO_KEY, mTestCase);
    (*req)->setInfo(options);
    
    // Make the request
    if (genericSend || !sync)
    {
        if (!(*req)->send())
        {
            qDebug() << mAppName << " AIW-ERROR:test: Send failed" << (*req)->lastError();;
            ret=false;
        }
    }
    else if (!genericSend && sync)
    {
        QVariant retValue;
        if (!(*req)->send(retValue))
        {
            qDebug() << mAppName <<  " AIW-ERROR: test: Send(retValue) failed" << (*req)->lastError();;
            ret=false;
        }
        else
        {
            if (retValue.canConvert<QString>())
            {
                qDebug("%s::retValue=%s,%s", qPrintable(mAppName),
                       retValue.typeName(),
                       qPrintable(retValue.value<QString>()));
                mTextRetValue->setText(retValue.value<QString>());
            }
            else
            {
                qDebug("%s:retValue=%s", qPrintable(mAppName),
                       retValue.typeName());
                mTextRetValue->setText("Not displayable");
            }

        }
    }

    assert(mTestCase + ".lastError==0", !(*req)->lastError());
    
    // Delete request if wanted
    bool deleteRequest = (mCheckDeleteRequest->checkState() == Qt::Checked);
    
    if ((*req)->lastError() == XQService::EMessageNotFound)
    {
        // Slot was not found
        deleteRequest = true;
    }
        
    if (deleteRequest)
    {
        qDebug() << mAppName <<  " AIW-NOTE: Request deleted";
        delete *req;
        *req = 0;
    }

    update();

    return ret;
    

}


// Tester helper function
void AppMgrClient::connectSignals(XQAiwRequest *req)
{
    if (req)
    {
            // Connect signals once
        connect(req, SIGNAL(requestOk(const QVariant&)), this, SLOT(handleOk(const QVariant&)));
        connect(req, SIGNAL(requestError(int,const QString&)), this, SLOT(handleError(int,const QString&)));
    }
}


// Tester helper function
void AppMgrClient::createTestFile(const QString &dir, const QString &fileName)
{

    QDir d(".");
    d.mkpath(dir);
    qDebug() << dir;

    QFile file(dir + "/" + fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << mAppName <<  " Creating file failed " << QString(dir + "/" + fileName);
        return;
    }
    QTextStream out(&file);
    out << "Text in file" << "\n";
    file.close();
    
}

// Tester helper function
void AppMgrClient::assert(const QString &testName, bool testResult)
{
    qDebug("%s,ASSERT,%s,%s", qPrintable(mAppName), qPrintable(testName),testResult?"OK" : "FAILED");
}


// Tester helper function
bool AppMgrClient::testRunning(const QString & service, const QString & interface)
{

    QList<XQAiwInterfaceDescriptor> impls = appmgr.list(service, interface, QString(""));
    if (impls.count() > 0)
    {
        bool b = appmgr.isRunning(impls[0]);
        return b;
    }
    else
    {
        return false;
    }
   
}


/*
* Test1:
* - Call the first IDIAL interface found.
* - Test XQAplicationManager::isRunning
* See appmgrservices.h
*/
void AppMgrClient::test1()
{

    mTestCase = "Test1";
    qDebug() << mAppName << " test1 START";
    assert("Test1",test(&req1, IDIAL, OPERATION1));

    bool embed = (mCheckEmbedded->checkState() == Qt::Checked);
    if (!embed)
    {
        // Works only for non-embedded cases (no point to check embedded case)
        assert("Test1.isRunning", testRunning("serviceapp", IDIAL));
    }
    qDebug() << mAppName << " test1 END";
  
}

/*
* Test2:
* - List all the IDIAL interfaces
* - List all the IDIAL interfaces of the old serviceapp service name
* - List all the IDIAL interfaces of the new serviceapp service name
* See appmgrservices.h
*/
void AppMgrClient::test2()
{
    mTestCase = "Test2";
    
    qDebug() << mAppName << " test2 START";
    
    QList<XQAiwInterfaceDescriptor> list=appmgr.list(IDIAL, "");
    qDebug() << mAppName << " list implementations: " << list.count();
    int i=0;
    assert("Test2.list.count() > 0", list.count() > 0);
    foreach (XQAiwInterfaceDescriptor d, list)
    {
        qDebug("%s::Service[%d]=%s",qPrintable(mAppName),i,qPrintable(d.serviceName()));
        qDebug("%s::Interface[%d]=%s",qPrintable(mAppName),i,qPrintable(d.interfaceName()));
        qDebug("%s::Implementation Id[%d]=%x",qPrintable(mAppName),i,d.property(XQAiwInterfaceDescriptor::ImplementationId).toInt());
        qDebug("%s::isRunning=%d", qPrintable(mAppName), testRunning(d.serviceName(), d.interfaceName()));
        qDebug("%s::status=%d", qPrintable(mAppName), appmgr.status(d));
        i++;
    }

    // Old, deprecated service name
    QList<XQAiwInterfaceDescriptor> list2=appmgr.list("com.nokia.services.serviceapp", IDIAL, "");
    qDebug() << mAppName << " list implementations 2: " << list2.count();
    i=0;
    assert("Test2.list2.count() > 0",list2.count() > 0);
    foreach (XQAiwInterfaceDescriptor d, list2)
    {
        qDebug("%s::Service[%d]=%s",qPrintable(mAppName),i,qPrintable(d.serviceName()));
        qDebug("%s::Interface[%d]=%s",qPrintable(mAppName),i,qPrintable(d.interfaceName()));
        qDebug("%s::Implementation Id[%d]=%x",qPrintable(mAppName),i,d.property(XQAiwInterfaceDescriptor::ImplementationId).toInt());
        i++;
    }

    // Service name
    QList<XQAiwInterfaceDescriptor> list3=appmgr.list("serviceapp", IDIAL, "");
    qDebug() << mAppName << " New: list implementations: " << list3.count();
    i=0;
    assert("Test2.list3.count() > 0", list3.count() > 0);
    foreach (XQAiwInterfaceDescriptor d, list3)
    {
        qDebug("%s::Service[%d]=%s",qPrintable(mAppName),i,qPrintable(d.serviceName()));
        qDebug("%s::Interface[%d]=%s",qPrintable(mAppName),i,qPrintable(d.interfaceName()));
        qDebug("%s::Implementation Id[%d]=%x",qPrintable(mAppName),i,d.property(XQAiwInterfaceDescriptor::ImplementationId).toInt());
        i++;
    }
    
    if (!list.isEmpty())
    {
        // Use the first found
        qDebug() << mAppName << " Using implementation nbr: " << mImplIndex;
        test(&req2,list[mImplIndex], OPERATION1);
    }

    qDebug() << mAppName << " test2 END";
    
}

/*
* Test3:
* - Using serviceapp and IDIAL interface test passing big amount of data.
    The datasize can be given in the mDataSpinBox field.
    At least 0.5 MB shall be passed OK
* See appmgrservices.h
*/
void AppMgrClient::test3()
{

    mTestCase = "Test3";
    qDebug() << mAppName << " test3 START";
    
    QByteArray data;
    qDebug() << mAppName << "Data size=" << mDataSpinBox->value();
    data.fill('X', mDataSpinBox->value());

    bool embed = (mCheckEmbedded->checkState() == Qt::Checked);
    bool sync = (mSynchronous->checkState() == Qt::Checked);
    bool background = (mBackground->checkState() == Qt::Checked);

    XQAiwRequest *req=0;
    req = appmgr.create(QLatin1String("serviceapp"), IDIAL, QLatin1String(""));
    assert("Test3.req != NULL", req != NULL);

    if (!req)
    {
        return;
    }

    // Apply this operation
    req->setOperation(QLatin1String("testVariant(QVariant)"));

    connectSignals(req);        

    // Set request attributes
    req->setEmbedded(embed);
    req->setSynchronous(sync);
    req->setBackground(background);

    XQRequestInfo info;
    // Save the test case to options as tester data. This data is also passed to service app
    // The test case is utilized in requestOk and requestError signal handlers
    info.setInfo(TESTCASE_INFO_KEY, mTestCase);
    req->setInfo(info);
    
    QList<QVariant> args;

    args.clear();
    args << qVariantFromValue(data);
    req->setArguments(args);
    
    assert("Test3.send", req->send());

    bool deleteRequest = (mCheckDeleteRequest->checkState() == Qt::Checked);
    if (deleteRequest)
    {
        delete req;
        req = 0;
    }
   
    qDebug() << mAppName << " test3 END";
    
}

/*
* Test4:
* - Test QAction creation using the service "com.nokia.services.hbserviceprovider" and IDIAL interface
    (The com.nokia.services.hbserviceprovider is implemented by the examples/hbserviceprovider
    which should be built and included in ROM)
    The successfully created QAction adds a Test button to UI.
    Pressing the button emits the signal "test4ActionTriggered" to get input args
    (the actual "send" happens after returning from the signal)
* See appmgrservices.h
*/
void AppMgrClient::test4()
{
    mTestCase = "Test4";
    qDebug() << mAppName << " test4 START";

    bool embed = (mCheckEmbedded->checkState() == Qt::Checked);
    bool sync = (mSynchronous->checkState() == Qt::Checked);
    bool foreground = (mForeground->checkState() == Qt::Checked);

    if (req4)
    {
        delete req4;  // Previous
        req4 = 0;
    }

    // Only hbserviceprovider support localization
    req4 = appmgr.create("com.nokia.services.hbserviceprovider", IDIAL, OPERATION1, true);
    assert("Test4.req4 != NULL", req4 != NULL);
    if (!req4)
    {
        return;
    }

    connectSignals(req4);
    req4->setSynchronous(sync);

    // In this test case, apply "options" for other options
    XQRequestInfo options;
    options.setEmbedded(embed);
    options.setForeground(foreground);
    // Save the test case to options as tester data. This data is also passed to service app
    // The test case is utilized in requestOk and requestError signal handlers
    options.setInfo(TESTCASE_INFO_KEY, mTestCase);
    req4->setInfo(options);

    if (actionButton)
    {
        // Previous
        vl->removeWidget(actionButton);
        delete actionButton;
        actionButton = 0;
        update();
    }
    
    QAction *action = req4->createAction();  // Also connects the triggered event to req !!!!
    assert("Test4.action != NULL", action != NULL);
    
    // Create UI
    if (action)
    {
        actionButton = new QPushButton(action->text());
        actionButton->addAction(action);
        vl->addWidget(actionButton);
        setLayout(vl);
        // For some reason triggered does not come from request
        // Workaround...
        connect(actionButton, SIGNAL(clicked()), action, SIGNAL(triggered()));
        connect(req4, SIGNAL(triggered()), this, SLOT(test4ActionTriggered()));
        qDebug() << mAppName << " Press Test button";
    }

    
}

// Test button pressed from UI
void AppMgrClient::test4ActionTriggered()
{
    XQAiwRequest *r = (XQAiwRequest *)sender();
    
    // Set arguments for request
    bool embed = (mCheckEmbedded->checkState() == Qt::Checked);
    QList<QVariant> args;
    args << QVariant(mReqArg->text());
    args << QVariant(!embed);
    r->setArguments(args);
}

/*
* Test5:
* - Test launching activity URI for the UID E0022E70 (fire-and-forget launch)
    (The E0022E70 is the examples/hbserviceclient which should be built and included in ROM)
* See appmgrservices.h
*/
void AppMgrClient::test5()
{
    mTestCase = "Test5";
    qDebug() << mAppName << " test5 START";

    QUrl uri(XQURI_SCHEME_ACTIVITY + "://E0022E70?" + XQURI_KEY_ACTIVITY_NAME + "=MainView&key1=data1&key2=data2"); 
    qDebug() << mAppName << " Uri=" << uri.toString();
    qDebug() << mAppName << " isValid=" << uri.isValid();
    qDebug() << mAppName << " Uri authority=" << uri.authority();
    QString old=mReqArg->text();
    if (!req5)
    {
        req5 = appmgr.create(uri);
        assert("Test5.req5 != NULL", req5 != NULL);
        connectSignals(req5); 
    }
    
    mReqArg->setText(uri.encodedQuery()); 
    assert("Test5", test(&req5, mReqArg->text()));
    mReqArg->setText(old);
    
    qDebug() << mAppName << " test5 END";
    
}

/*
* Test6:
* - Test launching URI scheme "testto" implemented by the examples/serviceapp
*/
void AppMgrClient::test6()
{

    mTestCase = "Test5";
    qDebug() << mAppName << " test6 START";

    // QUrl uri("testto://authority?param1=value1&param1=value2"); 
    QUrl uri("testto://authority?param1=value1&param1=value2"); 
    qDebug() << mAppName << " Uri=" << uri.toString();
    qDebug() << mAppName << " isValid=" << uri.isValid();
    qDebug() << mAppName << " Uri authority=" << uri.authority();

    QList<XQAiwInterfaceDescriptor> uriHandlers = appmgr.list(uri);
    assert("Test6.uriHandlers.count() > 0", uriHandlers.count() > 0);
    
    // Note : Only services supporting custom property are returned
    foreach (XQAiwInterfaceDescriptor d, uriHandlers)
    {
        qDebug() << mAppName << " Service=" << d.serviceName();
        qDebug() << mAppName << " Interface=" << d.interfaceName();
        qDebug("%s::Implementation Id=%x",qPrintable(mAppName),d.property(XQAiwInterfaceDescriptor::ImplementationId).toInt());
        qDebug("%s::isRunning=%d", qPrintable(mAppName), testRunning(d.serviceName(), d.interfaceName()));
    }

    if (!req6)
    {
        req6 = appmgr.create(uri);
        assert("Test5.req6 != NULL", req6 != NULL);
        connectSignals(req6); 
    }

    assert("Test6",test(&req6, uri.toString()));

    qDebug() << mAppName << " test6 END";
    
}

/*
* Test7:
* - Test launching com.nokia.symbian.IFileView for non-data-caged text file (MIME type text/plain)
*   At least the examples/serviceapp implements support for interface
*/
void AppMgrClient::test7()
{
    mTestCase = "Test7";
    qDebug() << mAppName << " test7 START";
    
    // Create test file  (MIME type text/plain)
    createTestFile("C:/data/Others", "test.txt");

    
    QFile file("C:/data/Others/test.txt");
    qDebug() << mAppName << " File=" << file.fileName();
    qDebug() << mAppName << " exists=" << file.exists();

    QList<XQAiwInterfaceDescriptor> fileHandlers = appmgr.list(file);
    assert("Test7.fileHandlers.count() > 0", fileHandlers.count() > 0);
    foreach (XQAiwInterfaceDescriptor d, fileHandlers)
    {
        qDebug() << mAppName << " Service=" << d.serviceName();
        qDebug() << mAppName << " Interface=" << d.interfaceName();
        qDebug("%s::Implementation Id=%x",qPrintable(mAppName),d.property(XQAiwInterfaceDescriptor::ImplementationId).toInt());
    }
    
    if (!req7)
    {
        req7 = appmgr.create(file);
        assert("Test5.req7 != NULL", req7 != NULL);
        connectSignals(req7); 
    }
    assert("Test7",test(&req7, file.fileName()));

    qDebug() << mAppName << " test7 END";
    
}


/*
* Test8:
* - Test launching http URI which should be implemented by the Browser
    Note ! http scheme is handled by the QDesktopServices::openUrl.
*/
void AppMgrClient::test8()
{
    mTestCase = "Test8";
    qDebug() << mAppName << " test8 START";

    // E0022E73 == ServiceApp
    QUrl uri("http://www.nokia.com"); 
    qDebug() << mAppName << " Uri=" << uri.toString();
    qDebug() << mAppName << " isValid=" << uri.isValid();
    qDebug() << mAppName << " Uri authority=" << uri.authority();

    if (!req8)
    {
        req8 = appmgr.create(uri);
        assert("Test8.req8 != NULL", req8 != NULL);
        connectSignals(req8); 
    }
    assert("Test8", test(&req8, uri.toString()));

    qDebug() << mAppName << " test8 END";
    
}

/*
* Test9:
* - Test launching com.nokia.symbian.IFileView for data-caged text file (MIME type text/plain)
*   At least the examples/serviceapp implements support for interface
*/
void AppMgrClient::test9()
{

    mTestCase = "Test9";
    qDebug() << mAppName << " test9 START";    

    bool embed = (mCheckEmbedded->checkState() == Qt::Checked);
    bool sync = (mSynchronous->checkState() == Qt::Checked);
    bool deleteRequest = (mCheckDeleteRequest->checkState() == Qt::Checked);


    // Access data-caged file 
    XQSharableFile sf;
    QString fileDir = "c:/private/e0022e74";
    
    if (mAppName == "appmgrclient2")
    {
        fileDir = "c:/private/e0022e76";
    }

    createTestFile(fileDir, "test.txt");
    if (!sf.open(fileDir + "\\test.txt"))
    {
        qDebug() << mAppName << " file open failed " << (fileDir + "/test.txt");
        return;
    }

    // Just test listing by sharable file
    QList<XQAiwInterfaceDescriptor> fileHandlers = appmgr.list(sf);
    assert("Test9.fileHandlers.count() > 0", fileHandlers.count() > 0);
    if (fileHandlers.count() > 0)
    {
        XQAiwInterfaceDescriptor d = fileHandlers.first();
        qDebug() << mAppName << " File Service=" << d.serviceName();
        qDebug() << mAppName << " File Interface=" << d.interfaceName();
        qDebug("%s::Implementation Id=%x",qPrintable(mAppName),d.property(XQAiwInterfaceDescriptor::ImplementationId).toInt());
        if (!req9)
        {
            // Create by descriptor
            req9 = appmgr.create(sf, d);
            assert("Test9.req9 != NULL", req9 != NULL);
        }
        if (!req9)
        {
            sf.close();
            return ;
        }
    }
    else
    {
        sf.close();
    }

    connectSignals(req9);        
    req9->setEmbedded(embed);
    req9->setSynchronous(sync);
    
    QList<QVariant> args;
    args << qVariantFromValue(sf);
    req9->setArguments(args);

    XQRequestInfo info;
    // Save the test case to options as tester data. This data is also passed to service app
    // The test case is utilized in requestOk and requestError signal handlers
    info.setInfo(TESTCASE_INFO_KEY, mTestCase);
    req9->setInfo(info);
    
    assert("Test9.send", req9->send());
    assert(mTestCase + ".lastError==0", !req9->lastError());
    
    // Remember to close the file !!!
    sf.close();

    if (deleteRequest)
    {
        delete req9;
        req9 = 0;
    }

    qDebug() << mAppName << " test9 END";
    
}

/*
* Test10:
* - Test fetching contacts via the "com.nokia.services.phonebookservices" interfaces
    Note ! This is deprecated interface name still used. The official name is "com.nokia.symbian.IContactFetch"
           once phonebook supports that.
*/
void AppMgrClient::test10()
{

    mTestCase = "Test10";
    qDebug() << mAppName << " test10 START";

    bool embed = (mCheckEmbedded->checkState() == Qt::Checked);
    bool sync = (mSynchronous->checkState() == Qt::Checked);
    bool background = (mBackground->checkState() == Qt::Checked);

    qDebug() << mAppName <<  " test10: embed=" << embed << ",sync=" << sync << "background=" << background;

    if (!req10)
    {
        req10 = appmgr.create(QLatin1String("com.nokia.services.phonebookservices"), QLatin1String("Fetch"), QLatin1String(""));
        assert("Test10.req10 != NULL", req10 != NULL);
        if (req10)
        {
            connect(req10, SIGNAL(requestOk(const QVariant&)), this, SLOT(handleOk(const QVariant&)));
            connect(req10, SIGNAL(requestError(int,const QString&)), this, SLOT(handleError(int,const QString&)));
        }
    }

    if (!req10)
    {
        return;
    }

    // Set request attributes
    req10->setOperation("fetch(QString,QString,QString)");
    req10->setEmbedded(embed);
    req10->setSynchronous(sync);
    req10->setBackground(background);

    // Set arguments
    QList<QVariant> args;
    args << "Contact"; 
    args << KCntActionAll;   
    args << KCntFilterDisplayAll;
    req10->setArguments(args);
    
    XQRequestInfo info;
    // Save the test case to options as tester data. This data is also passed to service app
    // The test case is utilized in requestOk and requestError signal handlers
    info.setInfo(TESTCASE_INFO_KEY, mTestCase);
    req10->setInfo(info);

    // Send the request
    assert("Test10.send", req10->send());
    assert(mTestCase + ".lastError==0", !req10->lastError());

    bool deleteRequest = (mCheckDeleteRequest->checkState() == Qt::Checked);
    if (deleteRequest)
    {
        delete req10;
        req10 = 0;
    }
    
    qDebug() << mAppName << " test10 END";


}


/*
* Test10:
* - Test getting DRM attributes of the files included in the examples\appmgrclient\DrmTestFiles.zip
    (You need to unzip/transfer these files in correct location)
*/
void AppMgrClient::test11()
{
    mTestCase = "Test11";
    qDebug() << mAppName << " test11 START";

    // Copy files from DrmTestFiles.zip into correct location
    QList<QString> drmFiles;
    drmFiles.append("C:/data/Others/RoAcqoffer-111-aac-i15m.ort");
    drmFiles.append("C:/data/Others/SD_Celebration_SP.dcf");
    drmFiles.append("C:/data/Others/111-test1.odf");
    drmFiles.append("C:/data/Others/SD_jpg_sun.dcf");
    drmFiles.append("C:/data/Others/STC1_128_44_16_2_CBR.wma");
    drmFiles.append("C:/data/Others/test.txt");
    drmFiles.append("C:/data/Others/some-nonexisting-file.txt");  // Error

    QList<int> attrNames;
    attrNames.append(XQApplicationManager::MimeType);
    attrNames.append(XQApplicationManager::IsProtected);
    attrNames.append(XQApplicationManager::IsForwardable);
    // See other attributes from epoc32/include/caf/caftypes.h

    // Test with file names
    foreach (QString f, drmFiles)
    {
        QFile file(f);
        QVariantList attrValues;
        bool ok = appmgr.getDrmAttributes(file, attrNames, attrValues);
        assert("Test11.getDrmAttributes.QFile/" + f,
               f != QString("C:/data/Others/some-nonexisting-file.txt") ? ok : !ok);
        int i=0;
        foreach (QVariant v, attrValues)
        {
            qDebug() << mAppName << " Attribute " << attrNames[i] << "=" << qPrintable(v.toString());
            i++;
        }
    }

    // Test with file handles
    foreach (QString f, drmFiles)
    {
        XQSharableFile file;
        file.open(f);  // Create handle manually
        QVariantList attrValues;
        bool ok = appmgr.getDrmAttributes(file, attrNames, attrValues);
        assert("Test11.getDrmAttributes.XQSharableFile/" + f,
               f != QString("C:/data/Others/some-nonexisting-file.txt") ? ok : !ok);
        int i=0;
        foreach (QVariant v, attrValues)
        {
            qDebug() << mAppName << " Attribute " << attrNames[i] << "=" << qPrintable(v.toString());
            i++;
        }
        file.close();
    }
    
    qDebug() << mAppName << " test11 END";

}


#define TXT_ID QLatin1String("txt_aiw_action_text")

//
// Miscellanous ad-hoc tests for trying bugs, etc.
//
void AppMgrClient::anyTest()
{

#if 0
    bool embed = (mCheckEmbedded->checkState() == Qt::Checked);
    bool sync = (mSynchronous->checkState() == Qt::Checked);
    bool background = (mBackground->checkState() == Qt::Checked);

    XQAiwRequest *req=0;
    req = appmgr.create(QLatin1String("serviceapp"), IDIAL, QLatin1String(""));

    if (!req)
    {
        qDebug() << mAppName << " AIW-ERROR NULL request";
        return;
    }

    // Apply this operation
    req->setOperation(QLatin1String("testVariant(QVariant)"));
    
    connectSignals(req);        

    // Set request attributes
    req->setEmbedded(embed);
    req->setSynchronous(sync);
    req->setBackground(background);

    QList<QVariant> args;

    /*
    // Just construct dummies
    MetaDummy1 dummy1;
    MetaDummy2 dummy2;
    */

    /*
    // THIS WORKS
    CntServicesContact cnt1;
    cnt1.mDisplayName = "Test1";
    cnt1.mPhoneNumber = "050-1111111";
    cnt1.mEmailAddress = "test1.test@nokia.com";
    
    CntServicesContact cnt2;
    cnt2.mDisplayName = "Test2";
    cnt2.mPhoneNumber = "050-2222222";
    cnt2.mEmailAddress = "test2.test@nokia.com";

    CntServicesContactList list;
    list.append(cnt1);
    list.append(cnt2);
    */

    QVariant v1;
    QVariant v2;
    QVariant v3;
    v1.setValue((int)-99);
    v2.setValue((bool)true);
    v3.setValue(QString("Variant3"));
    TestServiceData data1(1, v1);
    TestServiceData data2(2, v2);
    TestServiceData data3(3, v3);
    TestServiceDataList list;
    list.append(data1);
    list.append(data2);
    list.append(data3);
    
    args.clear();
    args << qVariantFromValue(list);
    req->setArguments(args);

    req->send();
    
    bool deleteRequest = (mCheckDeleteRequest->checkState() == Qt::Checked);
    if (deleteRequest)
    {
        delete req;
        req = 0;
    }

    qDebug() << mAppName << " test END";

    //  ---- OLD TESTS ------
    
    /*
    qDebug() << mAppName << "  name" << qApp->applicationName();
    qDebug() << mAppName << " dirpath" << qApp->applicationDirPath();
    qDebug() << mAppName << " filename" << qApp->applicationFilePath();
    qDebug() << mAppName << " pid" << qApp->applicationPid();

    QFileInfo appinfo (qApp->applicationFilePath());
    qDebug() << mAppName << " appinfo.applicationFilePath" << qApp->applicationFilePath();
    qDebug() << mAppName << " appinfo.absolutePath" << appinfo.absolutePath();
    qDebug() << mAppName << " appinfo.baseName" << appinfo.baseName();

    QString lang = QLocale::system().name();
    qDebug() << mAppName << " anyTest:" << lang;

    // QString textFile = "z:/resource/qt/translations/hbserviceprovider";
    QString textFile = "hbserviceprovider";
    QFileInfo info(textFile);
    qDebug() << mAppName << " base" << info.baseName();
    qDebug() << mAppName << " path" << info.filePath();
    if (info.baseName() == info.filePath())
    {
        textFile = qApp->applicationFilePath().left(2) + "/resource/qt/translations/" + textFile;
        qDebug() << mAppName << " path added" << textFile;
    }

    textFile += "_"; 
    textFile += lang;
    qDebug() << mAppName << " anyTest:" << textFile;

    QTranslator translator;
    bool res = translator.load(textFile);
    qDebug() << mAppName << " anyTest:" << res;
    if (res)
    {
        qApp->installTranslator(&translator);
    }

    QString textId = TXT_ID;
    QByteArray ba = textId.toLatin1();
    const char *textPtr = ba.data();

    QString text = qtTrId(textPtr);  // translate
    qDebug() << mAppName << " translated text:" << text;

    qApp->removeTranslator(&translator);

    Q_ASSERT(0==1);

    QString str("e0022e70");
    uint ui = str.toUInt();
    qDebug() << mAppName << " toUInt value=" << ui;
    bool b=false;
    ui=str.toUInt(&b,16);
    qDebug() << mAppName << " toUInt status=" << b <<  ",value=" << ui;

    
    */

#endif
}




// Aiw request responses
void AppMgrClient::handleOk(const QVariant& result)
{
    XQAiwRequest *r = static_cast<XQAiwRequest *>(sender());
    QString testCase = (r->info().info(TESTCASE_INFO_KEY)).toString();
    
    int impl=-1;
    impl = (r->descriptor().property(XQAiwInterfaceDescriptor::ImplementationId)).toInt();
    QString interface = r->descriptor().interfaceName();
    QString service = r->descriptor().serviceName();

    qDebug("%s,%s::requestOk,from [%s.%s,%x]",
           qPrintable(mAppName),
           qPrintable(testCase),
           qPrintable(service),
           qPrintable(interface),
           impl);
    qDebug("%s,%s::requestOk,result type=%s",
           qPrintable(mAppName),
           qPrintable(testCase),
           result.typeName());
    
    if (result.canConvert<CntServicesContactList>())
    {
        CntServicesContactList list;
        list = qVariantValue<CntServicesContactList>(result);
        for (int i = 0; i < list.count(); ++i)
        {
            qDebug() << mAppName << "," << testCase <<  ",result[" << i << "].mDisplayName=" << list[i].mDisplayName;
            qDebug() << mAppName << "," << testCase <<  " result[" << i << "].mPhoneNumber=" << list[i].mPhoneNumber;
            qDebug() << mAppName << "," << testCase <<  ",result[" << i << "].mEmailAddress=" << list[i].mEmailAddress;

        }
    }
    else if (result.canConvert<TestServiceDataList>())
    {
        TestServiceDataList list = qVariantValue<TestServiceDataList>(result);
        for (int i = 0; i < list.count(); ++i)
        {
            qDebug() << mAppName << "," << testCase << ",result[" << i << "].mType=" << list[i].mType;
            qDebug() << mAppName << "," << testCase << ",result[" << i << "].mData=" << list[i].mData.toString();
        }
        
    }
    else if (result.canConvert<QString>())
    {
        qDebug("%s,%s,result=%s",
               qPrintable(mAppName),
               qPrintable(testCase),
               qPrintable(result.value<QString>()));
        mTextRetValue->setText(result.value<QString>());
    }

    else
    {
        mTextRetValue->setText("Not displayable");
    }
}
  
void AppMgrClient::handleError(int errorCode, const QString& errorMessage)
{
    XQAiwRequest *r = static_cast<XQAiwRequest *>(sender());
    QString interface = r->descriptor().interfaceName();
    QString service = r->descriptor().serviceName();
    QString testCase = (r->info().info(TESTCASE_INFO_KEY)).toString();
    
    int impl=-1;
    impl = (r->descriptor().property(XQAiwInterfaceDescriptor::ImplementationId)).toInt();

    qDebug("%s,%s::requestError,from [%s.%s,%x]=(%d,%s)",
           qPrintable(mAppName),
           qPrintable(testCase),
           qPrintable(service),
           qPrintable(interface),
           impl, errorCode, qPrintable(errorMessage));
    
    mTextRetValue->setText(errorMessage);
}



Q_IMPLEMENT_USER_METATYPE(MetaDummy1)
Q_IMPLEMENT_USER_METATYPE(MetaDummy2)
Q_IMPLEMENT_USER_METATYPE(CntServicesContact)
Q_IMPLEMENT_USER_METATYPE_NO_OPERATORS(CntServicesContactList)
Q_IMPLEMENT_USER_METATYPE(TestServiceData)
Q_IMPLEMENT_USER_METATYPE_NO_OPERATORS(TestServiceDataList)
