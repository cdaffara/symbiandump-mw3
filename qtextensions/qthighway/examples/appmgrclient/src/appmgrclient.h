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

class QLineEdit;
class QCheckBox;
class QLabel;

class AppMgrClient : public QWidget
{
    Q_OBJECT
            
public:
        AppMgrClient( QWidget *parent = 0, Qt::WFlags f = 0 );
        ~AppMgrClient();

private slots:
        void test1();
        void test2();
        void test3();
        void test4();
        void test5();
        void test6();
        void test7();
        void test8();
        void test9();
        void test10();
        void test11();
        void anyTest();
        
       // Aiw request responses
        void handleOk(const QVariant &result);
        void handleError(int errorCode, const QString& errorMessage);
        void test4ActionTriggered();

              
private:
        bool test(XQAiwRequest **req,const QString& interface, const QString& operation);
        bool test(XQAiwRequest **req, XQAiwInterfaceDescriptor &impl, const QString& operation);
        bool test(XQAiwRequest **req, const QString &arg);
        void connectSignals(XQAiwRequest *req);
        void createTestFile(const QString &dir, const QString &file);
        bool testRunning(const QString & service, const QString & interface);
        void assert(const QString &testName, bool testResult);

private:
        QCheckBox* mCheckEmbedded;
        QCheckBox* mCheckDeleteRequest;
        QCheckBox* mBackground;
        QCheckBox* mForeground;
        QCheckBox* mGenericSend;
        QCheckBox* mSynchronous;
        QLineEdit *mReqArg;
        QLineEdit *mTextRetValue;
        QLabel* mRetunSignal;
        QVBoxLayout *vl;
        XQApplicationManager appmgr;
        QPushButton *actionButton;
        QMenu *mMenu;
        QSpinBox* mDataSpinBox;
        XQAiwRequest* req1;
        XQAiwRequest* req2;
        XQAiwRequest* req3;
        XQAiwRequest* req4;
        XQAiwRequest* req5;
        XQAiwRequest* req6;
        XQAiwRequest* req7;
        XQAiwRequest* req8;
        XQAiwRequest* req9;
        XQAiwRequest* req10;
        XQAiwRequest* anyReq;

        int mImplIndex;
        
        RFs fs;
        RFile file;

        QString mAppName;
        QString mTestCase;
};


#endif
