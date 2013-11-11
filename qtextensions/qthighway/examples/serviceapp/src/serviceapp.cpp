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
#include <QListView>
#include <QMessageBox>
#include <xqsharablefile.h>

#include "testservicedata.h"
#include "serviceapp.h"
#include <xqserviceutil.h>

ServiceApp::ServiceApp(QWidget *parent, Qt::WFlags f)
    : QWidget(parent, f),
      mDialService(NULL),
      mUriService(NULL),
      mFileService(NULL),
      mNewDialService(NULL),
      mNewFileService(NULL),
      mNewUriService(NULL)
{
    XQSERVICE_DEBUG_PRINT("ServiceApp::ServiceApp");
    if (XQServiceUtil::isService())
    {
        mDialService = new DialerService(this);
        mUriService = new UriService(this);
        mFileService = new FileService(this);
        mNewDialService = new NewDialerService(this);
        mNewUriService = new NewUriService(this);
        mNewFileService = new NewFileService(this);
    }
    /* Adjust the palette */
#if defined(Q_WS_S60)
    QPalette p = qApp->palette();
    QColor color(192,192,192);
    QColor bg(201,250,250);
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

    QPushButton *quitButton = new QPushButton(tr("Quit"));
    QPushButton *answerButtonDial = new QPushButton(tr("Dial answer"));
    QPushButton *answerButtonUri = new QPushButton(tr("Uri answer"));
    QPushButton *answerButtonFile = new QPushButton(tr("File answer"));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(quit()));
    connect(answerButtonDial, SIGNAL(clicked()), this, SLOT(answerDial()));
    connect(answerButtonUri, SIGNAL(clicked()), this, SLOT(answerUri()));
    connect(answerButtonFile, SIGNAL(clicked()), this, SLOT(answerFile()));

    /*
    mEndCallButton = new QPushButton(tr("End Call"));
    mEndCallButton->setEnabled(false);
    connect(mEndCallButton, SIGNAL(clicked()), this, SLOT(endCall()));
    */
    bool isService = XQServiceUtil::isService();
    QString interface = XQServiceUtil::interfaceName();
    QString operation = XQServiceUtil::operationName();
    
    QString t = "SERVICEAPP:\n";
    t = t + (isService ?  "    Service launch\n" : "    Normal launch\n");
    t = t + (XQServiceUtil::isEmbedded() ? "    Embedded\n" : "    Not embedded\n");
    t = t + ("    Interface=" + interface + "\n");
    t = t + ("    Operation=" + operation + "\n");

    QLabel *title = new QLabel(t);

    mLabel = new QLabel("");
    mNumber = new QLineEdit("");
    
    QVBoxLayout *vl = new QVBoxLayout;
    vl->setMargin(0);
    vl->setSpacing(0);

    vl->addWidget(quitButton);
    vl->addWidget(answerButtonDial);
    vl->addWidget(answerButtonUri);
    vl->addWidget(answerButtonFile);
    vl->addWidget(title);
    vl->addWidget(mLabel);
    vl->addWidget(mNumber);

    setLayout(vl);
   
#if defined(Q_WS_X11) || defined(Q_WS_WIN)
    setFixedSize(QSize(360,640)); // nHD
#elif defined(Q_WS_S60)
    // showMaximized();
    showFullScreen();
#endif
//    new DialerService(this);
}


ServiceApp::~ServiceApp()
{
    XQSERVICE_DEBUG_PRINT("ServiceApp::~ServiceApp");
    delete mDialService;
    delete mUriService;
    delete mFileService;
}

void ServiceApp::quit()
{
    XQSERVICE_DEBUG_PRINT("ServiceApp::quit");
    qApp->quit();
}

void ServiceApp::answerDial()
{
    XQSERVICE_DEBUG_PRINT("ServiceApp::answerDial");
    if (mDialService && mDialService->asyncAnswer())
    {
        mDialService->complete(mNumber->text());
    }
    if (mNewDialService && mNewDialService->asyncAnswer())
    {
        mNewDialService->complete(mNumber->text());
    }
}


void ServiceApp::answerUri()
{
    XQSERVICE_DEBUG_PRINT("ServiceApp::answerUri");
    if (mUriService && mUriService->asyncAnswer())
    {
        mUriService->complete(true);
    }
    if (mNewUriService && mNewUriService->asyncAnswer())
    {
        mNewUriService->complete(true);
    }
}

void ServiceApp::answerFile()
{
    XQSERVICE_DEBUG_PRINT("ServiceApp::answerFile");
    if (mFileService && mFileService->asyncAnswer())
    {
        mFileService->complete(true);
    }
    if (mNewFileService && mNewFileService->asyncAnswer())
    {
        mNewFileService->complete(true);
    }
}



void ServiceApp::endCall()
{
    XQSERVICE_DEBUG_PRINT("ServiceApp::endCall");
    //QVBoxLayout *vl = qobject_cast<QVBoxLayout *>(layout()) ;
    //vl->removeWidget(mEndCallButton);

    //XQServiceUtil::toBackground(true);
}

void ServiceApp::setLabelNumber(QString label,QString number)
{
    XQSERVICE_DEBUG_PRINT("ServiceApp::setLabelNumber");
    mLabel->setText(label);
    mNumber->setText(number);
}

// ----------DialerService---------------

DialerService::DialerService(ServiceApp* parent)
: XQServiceProvider(QLatin1String("com.nokia.services.serviceapp.Dialer"),parent),
  mServiceApp(parent)
{
    XQSERVICE_DEBUG_PRINT("DialerService::DialerService");
    publishAll();
}

DialerService::~DialerService()
{
    XQSERVICE_DEBUG_PRINT("DialerService::~DialerService");
}

void DialerService::complete(QString number)
{
    XQSERVICE_DEBUG_PRINT("DialerService::complete");

    // Complete all IDs
    foreach (quint32 reqId, mAsyncReqIds)
    {
        XQSERVICE_DEBUG_PRINT("DialerService::complete %d", reqId);
        completeRequest(reqId, number.toInt());
    }
}

int DialerService::dial(const QString& number, bool asyncAnswer)
{
    Q_UNUSED(asyncAnswer);
    
    XQRequestInfo info = requestInfo();
    QString testCase = (info.info(TESTCASE_INFO_KEY)).toString();
    XQSERVICE_DEBUG_PRINT("XQTESTER serviceapp,%s,DialerService::dial", qPrintable(testCase));
    
    XQSERVICE_DEBUG_PRINT("\tRequest info: id=%d,sid=%X,vid=%X", info.id(), info.clientSecureId(), info.clientVendorId());
    QSet<int> caps = info.clientCapabilities();
    QSetIterator<int> i(caps);
    while (i.hasNext())
        qDebug() << "Has capability " << i.next();    
    XQSERVICE_DEBUG_PRINT("\tRequest info: embed=%d,sync=%d", info.isEmbedded(), info.isSynchronous());

    bool isAsync = !info.isSynchronous();
    QString label = "Dialer::dial:\n";
    label += QString("number=%1\n").arg(number);
    label += QString("async=%1\n").arg(isAsync);
    
    connect(this, SIGNAL(returnValueDelivered()), this, SLOT(handleAnswerDelivered()));
    
    mNumber = number ;
    mServiceApp->setLabelNumber(label, number);
    int ret = 0;
    if (isAsync)
    {
        mAsyncReqIds.insertMulti(info.clientSecureId(), setCurrentRequestAsync());
        connect(this, SIGNAL(clientDisconnected()), this, SLOT(handleClientDisconnect()));
    }
    else
    {
        ret = number.toInt();
    }
    return ret;
}

CntServicesContactList DialerService::testContactList(CntServicesContactList list)
{
    XQSERVICE_DEBUG_PRINT("DialerService::testContactList");
    showRecipients(list);

    // Create output
    CntServicesContact cnt1;
    cnt1.mDisplayName = "Test1-Return";
    cnt1.mPhoneNumber = "060-1111111";
    cnt1.mEmailAddress = "test1.return@nokia.com";

    CntServicesContact cnt2;
    cnt2.mDisplayName = "Test1-Return";
    cnt2.mPhoneNumber = "060-2222222";
    cnt2.mEmailAddress = "test2.return@nokia.com";

    CntServicesContactList ret;
    ret.append(cnt1);
    ret.append(cnt2);

    return ret;
    
}

QVariant DialerService::testVariant(QVariant variant)
{
    XQRequestInfo info = requestInfo();
    QString testCase = (info.info(TESTCASE_INFO_KEY)).toString();
    XQSERVICE_DEBUG_PRINT("XQTESTER serviceapp,%s,DialerService::testVariant", qPrintable(testCase));

    XQSERVICE_DEBUG_PRINT("DialerService::testVariant::variant(%d,%d,%s)",
                          variant.type(), variant.userType(), variant.typeName());
    XQSERVICE_DEBUG_PRINT("DialerService::testVariant::variant value=%s", qPrintable(variant.toString()));

    if (variant.typeName() == QLatin1String("QStringList"))
    {
        qDebug() << "DialerService::testVariant QStringList";
        QStringList ret = variant.toStringList();
        return qVariantFromValue(ret);
    }
    else if (variant.typeName() == QLatin1String("XQShabarableFile"))
    {
        qDebug() << "DialerService::testVariant XQShabarableFile";
        XQSharableFile sf = variant.value<XQSharableFile>();
        
        RFile file;
        bool ok = sf.getHandle( file );
        if (ok)
        {
            HBufC8* data = HBufC8::NewL(100);
            TPtr8 ptr = data->Des();
            TInt err  = file.Read( ptr );
            QString text = QString::fromUtf8((const char *)(data->Ptr()), data->Length());
            XQSERVICE_DEBUG_PRINT("DialerService::testVariant ::file content,%d,%s", err, qPrintable(text));
            sf.close();
            delete data;
        }

        return QVariant(ok);
        
    }
    else if (variant.typeName() == QLatin1String("XQRequestInfo"))
    {
        qDebug() << "DialerService::testVariant XQRequestInfo";
        XQRequestInfo info = variant.value<XQRequestInfo>();
        QStringList keys = info.infoKeys();
        foreach (QString key, keys)
        {
            XQSERVICE_DEBUG_PRINT("DialerService::testVariant: info %s=%s",
                                  qPrintable(key),
                                  qPrintable(info.info(key).toString()));
        }

        return qVariantFromValue(info);
        
    }
    else if (variant.typeName() == QLatin1String("CntServicesContactList"))
    {
        qDebug() << "DialerService::testVariant CntServicesContactList";
        // Show input
        showRecipients(variant);

        // Create output
        CntServicesContact cnt1;
        cnt1.mDisplayName = "Test1-Return";
        cnt1.mPhoneNumber = "060-1111111";
        cnt1.mEmailAddress = "test1.return@nokia.com";

        CntServicesContact cnt2;
        cnt2.mDisplayName = "Test1-Return";
        cnt2.mPhoneNumber = "060-2222222";
        cnt2.mEmailAddress = "test2.return@nokia.com";

        CntServicesContactList list;
        list.append(cnt1);
        list.append(cnt2);

        // Return contact list back
        return qVariantFromValue(list);
    }
    else
    {
        return variant.toString();
    }
}

void DialerService::handleClientDisconnect()
{
    XQSERVICE_DEBUG_PRINT("DialerService::handleClientDisconnect");

    // Get the info of the cancelled request
    XQRequestInfo info = requestInfo();
    XQSERVICE_DEBUG_PRINT("\tDisconnected request info: id=%d,sid=%X,vid=%X", info.id(),info.clientSecureId(), info.clientVendorId());
    
    // Just quit service application if client ends
    mServiceApp->quit();
}

void DialerService::handleAnswerDelivered()
{
    XQSERVICE_DEBUG_PRINT("DialerService::handleAnswerDelivered");
    XQRequestInfo info = requestInfo();
    XQSERVICE_DEBUG_PRINT("\tRequest info: sid=%X,vid=%X", info.clientSecureId(), info.clientVendorId());
    // Done
    mAsyncReqIds.remove(info.clientSecureId());
}



void DialerService::showRecipients(QVariant &value)
{
    CntServicesContactList list;
    if(value.canConvert<CntServicesContactList>())
    {
        qDebug() << "DialerService::showRecipients: canConvert";
        list = qVariantValue<CntServicesContactList>(value);
    }
    else
    {
        qDebug() << "DialerService::showRecipients: canConvert NOK";
        return;
    }    

    showRecipients(list);
}

void DialerService::showRecipients(CntServicesContactList &list)
{
    if (list.count() == 0)
    {
        qDebug() << "DialerService::showRecipients(2): Count==0";
    }
    else
    {
        for (int i = 0; i < list.count(); ++i)
        {
            qDebug() << "DialerService::showRecipients(2)[" << i << "]=" << list[i].mDisplayName;
            qDebug() << "DialerService::showRecipients(2)[" << i << "]=" << list[i].mPhoneNumber;
            qDebug() << "DialerService::showRecipients(2)[" << i << "]=" << list[i].mEmailAddress;
        }
    }
}


// ----------New dialler service---------------

NewDialerService::NewDialerService(ServiceApp* parent)
: XQServiceProvider(QLatin1String("serviceapp.Dialer"),parent),
mServiceApp(parent)
{
    XQSERVICE_DEBUG_PRINT("NewDialerService::NewDialerService");
    publishAll();
}

NewDialerService::~NewDialerService()
{
    XQSERVICE_DEBUG_PRINT("NewDialerService::~NewDialerService");
}

void NewDialerService::complete(QString number)
{
    XQSERVICE_DEBUG_PRINT("NewDialerService::complete");

    // Complete all IDs
    foreach (quint32 reqId, mAsyncReqIds)
    {
        XQSERVICE_DEBUG_PRINT("NewDialerService::complete %d", reqId);
        completeRequest(reqId, number.toInt());
    }
}

int NewDialerService::dial(const QString& number, bool asyncAnswer)
{
    Q_UNUSED(asyncAnswer);

    XQRequestInfo info = requestInfo();
    QString testCase = (info.info(TESTCASE_INFO_KEY)).toString();
    XQSERVICE_DEBUG_PRINT("XQTESTER serviceapp,%s,NewDialerService::dial", qPrintable(testCase));
    
    XQSERVICE_DEBUG_PRINT("\tRequest info: id=%d,sid=%X,vid=%X", info.id(), info.clientSecureId(), info.clientVendorId());
    QSet<int> caps = info.clientCapabilities();
    QSetIterator<int> i(caps);
    while (i.hasNext())
        qDebug() << "Has capability " << i.next();    
    XQSERVICE_DEBUG_PRINT("\tRequest info: embed=%d,sync=%d", info.isEmbedded(), info.isSynchronous());

    bool isAsync = !info.isSynchronous();
    
    QString label = "NewDialer::dial:\n";
    label += QString("number=%1\n").arg(number);
    label += QString("async=%1\n").arg(isAsync);

    connect(this, SIGNAL(returnValueDelivered()), this, SLOT(handleAnswerDelivered()));

    mNumber = number ;
    mServiceApp->setLabelNumber(label, number);
    int ret = 0;
    if (isAsync)
    {
        mAsyncReqIds.insertMulti(info.clientSecureId(), setCurrentRequestAsync());
        connect(this, SIGNAL(clientDisconnected()), this, SLOT(handleClientDisconnect()));
    }
    else
    {
        ret = number.toInt();
    }
    return ret;
}

CntServicesContactList NewDialerService::testContactList(CntServicesContactList list)
{
    XQSERVICE_DEBUG_PRINT("NewDialerService::testContactList");
    showRecipients(list);

    // Create output
    CntServicesContact cnt1;
    cnt1.mDisplayName = "Test1-Return";
    cnt1.mPhoneNumber = "060-1111111";
    cnt1.mEmailAddress = "test1.return@nokia.com";

    CntServicesContact cnt2;
    cnt2.mDisplayName = "Test1-Return";
    cnt2.mPhoneNumber = "060-2222222";
    cnt2.mEmailAddress = "test2.return@nokia.com";

    CntServicesContactList ret;
    ret.append(cnt1);
    ret.append(cnt2);

    return ret;

}

QVariant NewDialerService::testVariant(QVariant variant)
{
    XQRequestInfo info = requestInfo();
    QString testCase = (info.info(TESTCASE_INFO_KEY)).toString();
    XQSERVICE_DEBUG_PRINT("XQTESTER serviceapp,%s,NewDialerService::testVariant", qPrintable(testCase));
    
    XQSERVICE_DEBUG_PRINT("NewDialerService::testVariant::variant(%d,%d,%s)",
                          variant.type(), variant.userType(), variant.typeName());
    XQSERVICE_DEBUG_PRINT("NewDialerService::testVariant::variant value=%s", qPrintable(variant.toString()));

    if (variant.typeName() == QLatin1String("QStringList"))
    {
        QStringList ret = variant.toStringList();
        return qVariantFromValue(ret);
    }
    else if (variant.typeName() == QLatin1String("TestServiceDataList"))
    {
        qDebug() << "DialerService::testVariant TestServiceDataList";
        TestServiceDataList list;
        if(variant.canConvert<TestServiceDataList>())
        {
            qDebug() << "DialerService::TestServiceDataList: canConvert OK";
            list = qVariantValue<TestServiceDataList>(variant);
            for (int i = 0; i < list.count(); ++i)
            {
                qDebug() << "DialerService::TestServiceDataList[" << i << "]=" << list[i].mType;
                qDebug() << "DialerService::TestServiceDataList[" << i << "]=" << list[i].mData.toString();
            }
        }
        else
        {
            qDebug() << "DialerService::TestServiceDataList: canConvert NOK";
        }    

        TestServiceDataList resultList;
        QVariant v1;
        QVariant v2;
        QVariant v3;
        v1.setValue((int)99);
        v2.setValue((bool)false);
        v3.setValue(QString("Variant3 return"));
        TestServiceData data1(1, v1);
        TestServiceData data2(2, v2);
        TestServiceData data3(3, v3);
        resultList.append(data1);
        resultList.append(data2);
        resultList.append(data3);
        
        // Return some data back
        return qVariantFromValue(resultList);
    }
    else if (variant.typeName() == QLatin1String("XQShabarableFile"))
    {
        XQSharableFile sf = variant.value<XQSharableFile>();

        RFile file;
        bool ok = sf.getHandle( file );
        if (ok)
        {
            HBufC8* data = HBufC8::NewL(100);
            TPtr8 ptr = data->Des();
            TInt err  = file.Read( ptr );
            QString text = QString::fromUtf8((const char *)(data->Ptr()), data->Length());
            XQSERVICE_DEBUG_PRINT("NewDialerService::testVariant ::file content,%d,%s", err, qPrintable(text));
            sf.close();
            delete data;
        }

        return QVariant(ok);

    }
    else if (variant.typeName() == QLatin1String("XQRequestInfo"))
    {
        XQRequestInfo info = variant.value<XQRequestInfo>();
        QStringList keys = info.infoKeys();
        foreach (QString key, keys)
        {
            XQSERVICE_DEBUG_PRINT("NewDialerService::testVariant: info %s=%s",
                                  qPrintable(key),
                                  qPrintable(info.info(key).toString()));
        }

        return qVariantFromValue(info);

    }
    else if (variant.typeName() == QLatin1String("CntServicesContactList"))
    {
        // Show input
        showRecipients(variant);

        // Create output
        CntServicesContact cnt1;
        cnt1.mDisplayName = "Test1-Return";
        cnt1.mPhoneNumber = "060-1111111";
        cnt1.mEmailAddress = "test1.return@nokia.com";

        CntServicesContact cnt2;
        cnt2.mDisplayName = "Test1-Return";
        cnt2.mPhoneNumber = "060-2222222";
        cnt2.mEmailAddress = "test2.return@nokia.com";

        CntServicesContactList list;
        list.append(cnt1);
        list.append(cnt2);

        // Return contact list back
        return qVariantFromValue(list);
    }
    else if (variant.typeName() == QLatin1String("QByteArray"))
    {
        QByteArray val = variant.value<QByteArray>();
        qDebug() << "DialerService::QByteArray size=" << val.size();
        return qVariantFromValue(val);
    }
    else
    {
        return variant.toString();
    }
}

void NewDialerService::handleClientDisconnect()
{
    XQSERVICE_DEBUG_PRINT("NewDialerService::handleClientDisconnect");

    // Get the info of the cancelled request
    XQRequestInfo info = requestInfo();
    XQSERVICE_DEBUG_PRINT("\tDisconnected request info: id=%d,sid=%X,vid=%X", info.id(),info.clientSecureId(), info.clientVendorId());

    // Just quit service application if client ends
    mServiceApp->quit();
}

void NewDialerService::handleAnswerDelivered()
{
    XQSERVICE_DEBUG_PRINT("NewDialerService::handleAnswerDelivered");
    XQRequestInfo info = requestInfo();
    XQSERVICE_DEBUG_PRINT("\tRequest info: sid=%X,vid=%X", info.clientSecureId(), info.clientVendorId());
    // Done
    mAsyncReqIds.remove(info.clientSecureId());
}



void NewDialerService::showRecipients(QVariant &value)
{
    CntServicesContactList list;
    if(value.canConvert<CntServicesContactList>())
    {
        qDebug() << "NewDialerService::showRecipients: canConvert";
        list = qVariantValue<CntServicesContactList>(value);
    }
    else
    {
        qDebug() << "NewDialerService::showRecipients: canConvert NOK";
        return;
    }    

    showRecipients(list);
}

void NewDialerService::showRecipients(CntServicesContactList &list)
{
    if (list.count() == 0)
    {
        qDebug() << "NewDialerService::showRecipients(2): Count==0";
    }
    else
    {
        for (int i = 0; i < list.count(); ++i)
        {
            qDebug() << "NewDialerService::showRecipients(2)[" << i << "]=" << list[i].mDisplayName;
            qDebug() << "NewDialerService::showRecipients(2)[" << i << "]=" << list[i].mPhoneNumber;
            qDebug() << "NewDialerService::showRecipients(2)[" << i << "]=" << list[i].mEmailAddress;
        }
    }
}



Q_IMPLEMENT_USER_METATYPE(CntServicesContact)
Q_IMPLEMENT_USER_METATYPE_NO_OPERATORS(CntServicesContactList)

Q_IMPLEMENT_USER_METATYPE(TestServiceData)
Q_IMPLEMENT_USER_METATYPE_NO_OPERATORS(TestServiceDataList)

// ----------UriService---------------

UriService::UriService(ServiceApp* parent)
: XQServiceProvider(QLatin1String("com.nokia.services.serviceapp.com.nokia.symbian.IUriView"),parent),
    mServiceApp(parent)
    
{
    XQSERVICE_DEBUG_PRINT("UriService::UriService");
    publishAll();
}

UriService::~UriService()
{
    XQSERVICE_DEBUG_PRINT("UriService::~UriService");
}

void UriService::complete(bool ok)
{
    Q_UNUSED(ok)
    XQSERVICE_DEBUG_PRINT("UriService::complete");
    // Complete all IDs
    foreach (quint32 reqId, mAsyncReqIds)
    {
        XQSERVICE_DEBUG_PRINT("UriService::complete %d", reqId);
        completeRequest(reqId, QVariant(mRetValue));
    }
}

bool UriService::view(const QString& uri)
{
    XQSERVICE_DEBUG_PRINT("UriService::view(1)");
    return view(uri, true);
}

bool UriService::view(const QString& uri, bool retValue)
{
    XQRequestInfo info = requestInfo();
    QString testCase = (info.info(TESTCASE_INFO_KEY)).toString();
    XQSERVICE_DEBUG_PRINT("XQTESTER serviceapp,%s,UriService::view", qPrintable(testCase));
    
    QString label = "IUriView::view\n:";
    label += QString ("Uri=%1\n").arg(uri);
    label += QString ("retValue=%1\n").arg(retValue);
    QString param = QString ("retValue=%1\n").arg(retValue);

    bool isAsync = !info.isSynchronous();;
    connect(this, SIGNAL(returnValueDelivered()), this, SLOT(handleAnswerDelivered()));

    mRetValue = retValue;
    mServiceApp->setLabelNumber(label,param);
    if (isAsync)
    {
        mAsyncReqIds.insertMulti(info.clientSecureId(), setCurrentRequestAsync());
        connect(this, SIGNAL(clientDisconnected()), this, SLOT(handleClientDisconnect()));
    }
    
    return retValue;
}

void UriService::handleClientDisconnect()
{
    XQSERVICE_DEBUG_PRINT("UriService::handleClientDisconnect");
    XQRequestInfo info = requestInfo();
    XQSERVICE_DEBUG_PRINT("\tRequest info: id=%d,sid=%X,vid=%X", info.id(),info.clientSecureId(), info.clientVendorId());

    mAsyncReqIds.remove(info.clientSecureId());
    mServiceApp->quit();
}

void UriService::handleAnswerDelivered()
{
    XQSERVICE_DEBUG_PRINT("UriService::handleAnswerDelivered");
    XQRequestInfo info = requestInfo();
    XQSERVICE_DEBUG_PRINT("\tRequest info: sid=%X,vid=%X", info.clientSecureId(), info.clientVendorId());
    // Done
    mAsyncReqIds.remove(info.clientSecureId());
    
}

// ----------NewUriService---------------

NewUriService::NewUriService(ServiceApp* parent)
: XQServiceProvider(QLatin1String("serviceapp.com.nokia.symbian.IUriView"),parent),
mServiceApp(parent)

{
    XQSERVICE_DEBUG_PRINT("NewUriService::NewUriService");
    publishAll();
}

NewUriService::~NewUriService()
{
    XQSERVICE_DEBUG_PRINT("NewUriService::~NewUriService");
}

void NewUriService::complete(bool ok)
{
    Q_UNUSED(ok);
    XQSERVICE_DEBUG_PRINT("NewUriService::complete");
    // Complete all IDs
    foreach (quint32 reqId, mAsyncReqIds)
    {
        XQSERVICE_DEBUG_PRINT("NewUriService::complete %d", reqId);
        completeRequest(reqId, QVariant(mRetValue));
    }
}

bool NewUriService::view(const QString& uri)
{
    XQSERVICE_DEBUG_PRINT("NewUriService::view(1)");
    return view(uri, true);
}

bool NewUriService::view(const QString& uri, bool retValue)
{
    XQRequestInfo info = requestInfo();
    QString testCase = (info.info(TESTCASE_INFO_KEY)).toString();
    XQSERVICE_DEBUG_PRINT("XQTESTER serviceapp,%s,NewUriService::view", qPrintable(testCase));
    
    QString label = "New IUriView::view\n:";
    label += QString ("Uri=%1\n").arg(uri);
    label += QString ("retValue=%1\n").arg(retValue);
    QString param = QString ("retValue=%1\n").arg(retValue);

    bool isAsync = !info.isSynchronous();;
    connect(this, SIGNAL(returnValueDelivered()), this, SLOT(handleAnswerDelivered()));

    mRetValue = retValue;
    mServiceApp->setLabelNumber(label,param);
    if (isAsync)
    {
        mAsyncReqIds.insertMulti(info.clientSecureId(), setCurrentRequestAsync());
        connect(this, SIGNAL(clientDisconnected()), this, SLOT(handleClientDisconnect()));
    }

    return retValue;
}

void NewUriService::handleClientDisconnect()
{
    XQSERVICE_DEBUG_PRINT("NewUriService::handleClientDisconnect");
    XQRequestInfo info = requestInfo();
    XQSERVICE_DEBUG_PRINT("\tRequest info: id=%d,sid=%X,vid=%X", info.id(),info.clientSecureId(), info.clientVendorId());

    mAsyncReqIds.remove(info.clientSecureId());
    mServiceApp->quit();
}

void NewUriService::handleAnswerDelivered()
{
    XQSERVICE_DEBUG_PRINT("NewUriService::handleAnswerDelivered");
    XQRequestInfo info = requestInfo();
    XQSERVICE_DEBUG_PRINT("\tRequest info: sid=%X,vid=%X", info.clientSecureId(), info.clientVendorId());
    // Done
    mAsyncReqIds.remove(info.clientSecureId());

}


// ----------FileService---------------

FileService::FileService(ServiceApp* parent)
: XQServiceProvider(QLatin1String("com.nokia.services.serviceapp.com.nokia.symbian.IFileView"),parent),
    mServiceApp(parent)

{
    XQSERVICE_DEBUG_PRINT("FileService::FileService");
    publishAll();
}

FileService::~FileService()
{
    XQSERVICE_DEBUG_PRINT("FileService::~FileService");
}

void FileService::complete(bool ok)
{
    XQSERVICE_DEBUG_PRINT("FileService::complete");
    // Complete all
    foreach (quint32 reqId, mAsyncReqIds)
    {
        XQSERVICE_DEBUG_PRINT("FileService::complete %d", reqId);
        completeRequest(reqId, QVariant(ok));
    }
}

bool FileService::view(QString file)
{
    XQRequestInfo info = requestInfo();
    QString testCase = (info.info(TESTCASE_INFO_KEY)).toString();
    XQSERVICE_DEBUG_PRINT("XQTESTER serviceapp,%s,FileService::view", qPrintable(testCase));

    QString label = "IFileView::view\n:";
    QString param = QString ("File=%1\n").arg(file);

    bool isAsync = !info.isSynchronous();;
    connect(this, SIGNAL(returnValueDelivered()), this, SLOT(handleAnswerDelivered()));

    mServiceApp->setLabelNumber(label,param);
    if (isAsync)
    {
        mAsyncReqIds.insertMulti(info.clientSecureId(), setCurrentRequestAsync());
        connect(this, SIGNAL(clientDisconnected()), this, SLOT(handleClientDisconnect()));
    }
    
    return true;
}


bool FileService::view(XQSharableFile sf)
{
    XQSERVICE_DEBUG_PRINT("FileService::view(XQSharebleFile)");
    QString label = "IFile::view\n:";
    QString param = QString ("File=%1\n").arg(sf.fileName());

    RFile file;
    bool ok = sf.getHandle( file );
    if (ok)
    {
        HBufC8* data = HBufC8::NewL(100);
        TPtr8 ptr = data->Des();
        TInt err  = file.Read( ptr );
        QString text = QString::fromUtf8((const char *)(data->Ptr()), data->Length());
        XQSERVICE_DEBUG_PRINT("FileService::file read,%d,%s", err, qPrintable(text));
        sf.close();
        delete data;
    }

    XQRequestInfo info = requestInfo();
    bool isAsync = !info.isSynchronous();;
    
    mServiceApp->setLabelNumber(label,param);
    if (isAsync)
    {
        mAsyncReqIds.insertMulti(info.clientSecureId(), setCurrentRequestAsync());
        connect(this, SIGNAL(clientDisconnected()), this, SLOT(handleClientDisconnect()));
    }
    return true;
}


void FileService::handleClientDisconnect()
{
    XQSERVICE_DEBUG_PRINT("FileService::handleClientDisconnect");
    XQRequestInfo info = requestInfo();
    XQSERVICE_DEBUG_PRINT("\tRequest info: id=%d,sid=%X,vid=%X", info.id(),info.clientSecureId(), info.clientVendorId());

    mAsyncReqIds.remove(info.clientSecureId());
    mServiceApp->quit();
}


void FileService::handleAnswerDelivered()
{
    XQSERVICE_DEBUG_PRINT("FileService::handleAnswerDelivered");
    XQRequestInfo info = requestInfo();
    XQSERVICE_DEBUG_PRINT("\tRequest info: sid=%X,vid=%X", info.clientSecureId(), info.clientVendorId());
    // Done
    mAsyncReqIds.remove(info.clientSecureId());
    
}


// ----------NewFileService---------------

NewFileService::NewFileService(ServiceApp* parent)
: XQServiceProvider(QLatin1String("serviceapp.com.nokia.symbian.IFileView"),parent),
mServiceApp(parent)

{
    XQSERVICE_DEBUG_PRINT("NewFileService::NewFileService");
    publishAll();
}

NewFileService::~NewFileService()
{
    XQSERVICE_DEBUG_PRINT("NewFileService::~NewFileService");
}

void NewFileService::complete(bool ok)
{
    XQSERVICE_DEBUG_PRINT("NewFileService::complete");
    // Complete all
    foreach (quint32 reqId, mAsyncReqIds)
    {
        XQSERVICE_DEBUG_PRINT("NewFileService::complete %d", reqId);
        completeRequest(reqId, QVariant(ok));
    }
}

bool NewFileService::view(QString file)
{
    XQRequestInfo info = requestInfo();
    QString testCase = (info.info(TESTCASE_INFO_KEY)).toString();
    XQSERVICE_DEBUG_PRINT("XQTESTER serviceapp,%s,NewFileService::view", qPrintable(testCase));
    
    QString label = "New IFileView::view\n:";
    QString param = QString ("File=%1\n").arg(file);

    bool isAsync = !info.isSynchronous();;
    connect(this, SIGNAL(returnValueDelivered()), this, SLOT(handleAnswerDelivered()));

    mServiceApp->setLabelNumber(label,param);
    if (isAsync)
    {
        mAsyncReqIds.insertMulti(info.clientSecureId(), setCurrentRequestAsync());
        connect(this, SIGNAL(clientDisconnected()), this, SLOT(handleClientDisconnect()));
    }

    return true;
}


bool NewFileService::view(XQSharableFile sf)
{
    XQSERVICE_DEBUG_PRINT("NewFileService::view(XQSharebleFile)");
    QString label = "IFile::view\n:";
    QString param = QString ("File=%1\n").arg(sf.fileName());

    RFile file;
    bool ok = sf.getHandle( file );
    if (ok)
    {
        HBufC8* data = HBufC8::NewL(100);
        TPtr8 ptr = data->Des();
        TInt err  = file.Read( ptr );
        QString text = QString::fromUtf8((const char *)(data->Ptr()), data->Length());
        XQSERVICE_DEBUG_PRINT("NewFileService::file read,%d,%s", err, qPrintable(text));
        sf.close();
        delete data;
    }

    XQRequestInfo info = requestInfo();
    bool isAsync = !info.isSynchronous();;

    mServiceApp->setLabelNumber(label,param);
    if (isAsync)
    {
        mAsyncReqIds.insertMulti(info.clientSecureId(), setCurrentRequestAsync());
        connect(this, SIGNAL(clientDisconnected()), this, SLOT(handleClientDisconnect()));
    }
    return true;
}


void NewFileService::handleClientDisconnect()
{
    XQSERVICE_DEBUG_PRINT("NewFileService::handleClientDisconnect");
    XQRequestInfo info = requestInfo();
    XQSERVICE_DEBUG_PRINT("\tRequest info: id=%d,sid=%X,vid=%X", info.id(),info.clientSecureId(), info.clientVendorId());

    mAsyncReqIds.remove(info.clientSecureId());
    mServiceApp->quit();
}


void NewFileService::handleAnswerDelivered()
{
    XQSERVICE_DEBUG_PRINT("NewFileService::handleAnswerDelivered");
    XQRequestInfo info = requestInfo();
    XQSERVICE_DEBUG_PRINT("\tRequest info: sid=%X,vid=%X", info.clientSecureId(), info.clientVendorId());
    // Done
    mAsyncReqIds.remove(info.clientSecureId());

}

