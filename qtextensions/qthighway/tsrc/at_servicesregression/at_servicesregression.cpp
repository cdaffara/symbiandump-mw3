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


#include "at_servicesregression.h"

#include <QtTest/QtTest>
#include <QUrl>
#include <QFile>

#include <xqaiwrequest.h>
#include <xqsharablefile.h>
#include <xqaiwdecl.h>
#include <xqaiwdeclplat.h>
#include <akndef.h>
#include <w32std.h>
#include <coemain.h>

#define LASTEROR QString(" - Last Error: " + QString::number(mAppManager->lastError()))

void TestServicesRegression::initTestCase()
{
    request = NULL;
    mAppManager = NULL;
}

void TestServicesRegression::cleanupTestCase()
{

}

void TestServicesRegression::init()
{
    mAppManager = new XQApplicationManager();
    QVERIFY(mAppManager != NULL); 
    QVERIFY(mAppManager->lastError() == XQService::ENoError); 
    closeApp = false;
}

void TestServicesRegression::cleanup()
{
    delete mAppManager;
    mAppManager = NULL;
    if (closeApp)
        closeFocusedWG();
}

void TestServicesRegression::platform_services_data()
{
    QStringList operations;
    
    QTest::addColumn<QString>("interface");
    QTest::addColumn<QStringList>("operations");
        
    operations << XQOP_IMAGE_FETCH.latin1();
    QTest::newRow("photos") << XQI_IMAGE_FETCH.latin1() << operations;
    operations.clear();
    
    operations << XQOP_MUSIC_FETCH.latin1();
    QTest::newRow("musicplayer") << XQI_MUSIC_FETCH.latin1() << operations;
    operations.clear();
    
    operations << XQOP_TONE_FETCH.latin1();
    QTest::newRow("tonefetcher") << XQI_TONE_FETCH.latin1() << operations;
    operations.clear();
    
    operations << XQOP_VIDEO_FETCH.latin1();
    QTest::newRow("videoplayer") << XQI_VIDEO_FETCH.latin1() << operations;
    operations.clear();
    
    operations << XQOP_VIDEO_PLAY.latin1() << XQOP_VIDEO_PLAY_PD.latin1() << XQOP_VIDEO_CLOSE.latin1();
    QTest::newRow("videoplayer") << XQI_VIDEO_PLAY.latin1() << operations;
    operations.clear();
    
    operations << XQOP_VIDEO_BROWSE.latin1();
    QTest::newRow("videoplayer") << XQI_VIDEO_BROWSE.latin1() << operations;
    operations.clear();
    
    operations << XQOP_CAMERA_CAPTURE.latin1();
    QTest::newRow("cxui") << XQI_CAMERA_CAPTURE.latin1() << operations;
    operations.clear();
    
    operations << XQOP_LOGS_SHOW.latin1();
    QTest::newRow("logs") << XQI_LOGS_VIEW.latin1() << operations;
    operations.clear();
    
    operations << XQOP_CONTACTS_EDIT_CREATE_NEW.latin1() << XQOP_CONTACTS_EDIT_CREATE_NEW_WITH_SUBTYPE.latin1() << XQOP_CONTACTS_EDIT_CREATE_NEW_VCARD.latin1() 
                << XQOP_CONTACTS_EDIT_EXISTING.latin1() << XQOP_CONTACTS_EDIT_UPDATE_EXISTING.latin1() << XQOP_CONTACTS_EDIT_UPDATE_EXISTING_WITH_SUBTYPE.latin1();
    QTest::newRow("phonebookservices") << XQI_CONTACTS_EDIT.latin1() << operations;
    operations.clear();
    
    operations << XQOP_CONTACTS_FETCH_MULTIPLE.latin1() << XQOP_CONTACTS_FETCH_SINGLE.latin1();
    QTest::newRow("phonebookservices") << XQI_CONTACTS_FETCH.latin1() << operations;
    operations.clear();
    
    operations << XQOP_CONTACTS_VIEW_CONTACT_CARD.latin1() << XQOP_CONTACTS_VIEW_TEMP_CONTACT_CARD_WITH_DETAIL.latin1() << XQOP_CONTACTS_VIEW_GROUP.latin1();
    QTest::newRow("phonebookservices") << XQI_CONTACTS_VIEW.latin1() << operations;
    operations.clear();
    
    operations << "send(const QString,const qint32, const QString)" << "send(const QString,const QString,const QString)"
                << "send(QVariantMap,const QString)";
    QTest::newRow("messaging") << XQI_MESSAGE_SEND.latin1() << operations;
    operations.clear();
    
    operations << "fetch(QVariantMap, QVariant)";
    QTest::newRow("filemgr") << XQI_FILE_FETCH.latin1() << operations;
    operations.clear();
    
    operations << XQOP_RADIO_CONTROL.latin1();
    QTest::newRow("qtfmradio") << XQI_RADIO_CONTROL.latin1() << operations;
    operations.clear();
    
    operations << XQOP_RADIO_MONITOR.latin1();
    QTest::newRow("qtfmradio") << XQI_RADIO_MONITOR.latin1() << operations;
    operations.clear();
    
    operations << XQOP_WLAN_SNIFFER.latin1();
    QTest::newRow("wlansniffer") << XQI_WLAN_SNIFFER.latin1() << operations;
    operations.clear();
    
    operations << XQOP1_WLAN_LOGIN.latin1() << XQOP2_WLAN_LOGIN.latin1();
    QTest::newRow("wlanlogin") << XQI_WLAN_LOGIN.latin1() << operations;
    operations.clear();
 
    operations << XQOP_EMAIL_INBOX_VIEW.latin1();
    QTest::newRow("nmailui") << XQI_EMAIL_INBOX_VIEW.latin1() << operations;
    operations.clear();
    
    operations << XQOP_EMAIL_MESSAGE_VIEW.latin1();
    QTest::newRow("nmailui") << XQI_EMAIL_MESSAGE_VIEW.latin1() << operations;
    operations.clear();
    
    operations << XQOP_EMAIL_MESSAGE_SEND.latin1();
    QTest::newRow("nmailui") << XQI_EMAIL_MESSAGE_SEND.latin1() << operations;
    operations.clear();
    
    operations << XQOP_EMAIL_REGISTER_ACCOUNT.latin1() << XQOP_EMAIL_UPDATE_MAILBOX.latin1() << XQOP_EMAIL_UNREGISTER_MAILBOX.latin1();
    QTest::newRow("nmailui") << XQI_EMAIL_REGISTER_ACCOUNT.latin1() << operations;
    operations.clear();
    
    operations << XQOP_EMAIL_MAILBOX_CONFIGURE.latin1();
    QTest::newRow("nmailui") << XQI_EMAIL_MAILBOX_CONFIGURE.latin1() << operations;
    operations.clear();
    
    operations << "dial(const QString&)" << "dial(const QString&, int)" << "dialVideo(const QString&)" << "dialVideo(const QString&, int)"
                << "dialVoip(const QString&)" << "dialVoip(const QString&, int)" << "dialVoipService(const QString&, int)"
                << "dialVoipService(const QString&, int, int)";
    QTest::newRow("phoneui") << XQI_CALL_DIAL.latin1() << operations;
    operations.clear();
    
    operations << "playDTMFTone(const QChar&)" << "stopDTMFPlay()";
    QTest::newRow("phoneui") << XQI_DTMF_PLAY.latin1() << operations;
    operations.clear();
}

void TestServicesRegression::platform_services()
{
    QFETCH(QString, interface);
    QFETCH(QStringList, operations);

    foreach (QString operation, operations) {
        XQAiwRequest* req = mAppManager->create(interface, operation);
        QVERIFY2(req != NULL, 
            QString("Cannot create request for interface:" + interface + ", operation:" + operation + LASTEROR).toAscii());
        delete req;
    }
        
}

void TestServicesRegression::platform_services_operations_data()
{
    platform_services_data();
}

void TestServicesRegression::platform_services_operations()
{
    QFETCH(QString, interface);
    QFETCH(QStringList, operations);
    
    foreach (QString operation, operations){
        if (closeApp)
            closeFocusedWG();
        
        request = mAppManager->create(interface, operation, false);
        if (request) {
            request->setSynchronous(false);
            closeApp = true;
                
            QVERIFY2(request->send(), QString("Send request failed, interface:" + interface + ", operation:" + operation + LASTEROR).toAscii());
        }
        else 
            closeApp = false;
    }
}

void TestServicesRegression::platform_url_data()
{
    QTest::addColumn<QString>("url");
        
    QTest::newRow("http") << "http:";
    QTest::newRow("https") << "https:";
    QTest::newRow("mailto") << "mailto:";
    QTest::newRow("cti") << "cti:";
    QTest::newRow("mmsto") << "mmsto:";
    QTest::newRow("sms") << "sms:";
    QTest::newRow("javaapp") << "javaapp:";
    QTest::newRow("tel") << "tel:";
}

void TestServicesRegression::platform_url()
{
    QFETCH(QString, url);
    
    url += "temp";
    
    XQAiwRequest* request = mAppManager->create(QUrl(url), false);
    QVERIFY2(request != NULL, QString("Cannot create request for url:" + url + LASTEROR).toAscii());
    request->setSynchronous(false);
   
    QList<QVariant> args;
    args << url;
    request->setArguments(args);
    
    closeApp = true;
    
    QVERIFY2(request->send(), QString("Send request failed, url:" + url + LASTEROR).toAscii());
}

void TestServicesRegression::platform_file_data()
{
#ifdef __WINS__
    const QString drive = "c";
#else
    const QString drive = "z";
#endif
    
    QTest::addColumn<QString>("fileStr");        
    QTest::newRow("image") << drive + ":\\data\\Others\\one_v2.jpg";
    QTest::newRow("audio") << drive + ":\\data\\Others\\111-test1_v2.odf";
    QTest::newRow("video") << drive + ":\\data\\Others\\324kbps_MPEG4_352x288_AAC_15fps.3gp";
}

void TestServicesRegression::platform_file()
{
    QFETCH(QString, fileStr);
    
    QFile file(fileStr);
    
    XQAiwRequest* request = mAppManager->create(file, false);
    QVERIFY2(request != NULL, QString("Cannot create request for file:" + fileStr + LASTEROR).toAscii());
    request->setSynchronous(false);
   
    QList<QVariant> args;
    args << fileStr;
    request->setArguments(args);
    
    closeApp = true;
    
    QVERIFY2(request->send(), QString("Send request failed, file:" + fileStr + LASTEROR).toAscii());
}

void TestServicesRegression::platform_file_share()
{
    QList<XQAiwInterfaceDescriptor> listFileShare = mAppManager->list(XQI_FILE_SHARE.latin1(), XQOP_FILE_SHARE.latin1());
    
    QVERIFY2(listFileShare.count() > 2, "There must be minimum three implementations in platform");
     
    foreach (XQAiwInterfaceDescriptor interface, listFileShare) {
        XQAiwRequest* request = mAppManager->create(interface, XQOP_FILE_SHARE.latin1());
        QVERIFY2(request != NULL, QString("Cannot create request for file share:" + interface.serviceName() + LASTEROR).toAscii());
        delete request;
    }
}

void TestServicesRegression::closeFocusedWG()
{
    QTest::qWait(10000); // wait for service start
    
    TInt myWGId = CCoeEnv::Static()->RootWin().Identifier();
    TInt focusedWGId = CCoeEnv::Static()->WsSession().GetFocusWindowGroup();
    
    if (myWGId != focusedWGId) {
        TWsEvent  event;
        event.SetType(EEventKey);
        event.SetTimeNow();
        event.SetType(KAknShutOrHideApp);
        CCoeEnv::Static()->WsSession().SendEventToWindowGroup(focusedWGId, event);
    }
    
    if (request) {
        delete request;
        request = NULL;
    }
    
    QTest::qWait(4000); // wait for service close
}

int main (int argc, char* argv[]) 
{
    QApplication app(argc, argv);
    QTEST_DISABLE_KEYPAD_NAVIGATION
    TestServicesRegression tc;
    int ret = 0;
    if (argc == 1) {
        int c = 5;
        char* v[] = {argv[0], "-o", "c:/at_regression.txt", "-maxwarnings", "0"};
        ret = QTest::qExec(&tc, c, v);
    }
    else {
        ret = QTest::qExec(&tc, argc, argv);
    }
    return ret;
}

