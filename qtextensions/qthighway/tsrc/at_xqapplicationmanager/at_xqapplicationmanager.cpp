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


#include <QtTest/QtTest>
#include <QObject>
#include <QUrl>
#include <QFile>
#include <QVariantList>

#include <xqaiwrequest.h>
#include <xqsharablefile.h>

#include "at_xqapplicationmanager.h"
#include "../inc/testdata.h"

namespace
{
    const QString endOfLine("\n");
    const QString testString("TestString");
    const QString operation("request(QString)");
    const QString operation_async("asyncRequest(QString)");
    const QString testFileStr(drive + ":\\data\\Others\\test.txt");

    const QUrl testUrl("testto://address");
    
    const QFile testFile(testFileStr);
}


void Test_XQApplicationManager::init()
{
    testSharableFile = new XQSharableFile();
    QVERIFY2(testSharableFile->open(testFileStr), mLog.join(endOfLine).toAscii());
    mAppManager = new XQApplicationManager();
    QVERIFY2(mAppManager != NULL, mLog.join(endOfLine).toAscii()); 
    QVERIFY2(mAppManager->lastError() == XQService::ENoError, mLog.join(endOfLine).toAscii()); 
}

void Test_XQApplicationManager::drm_file()
{
    mLog << "[QTH] [Test_XQApplicationManager] drm_file";

#ifdef __WINS__
    QSKIP("Checking drm attributes does not work correctly on emulator", SkipAll);
#endif
    
    QStringList logmem = mLog;
    
    foreach (QString fileStr, testData.files.keys()) {
        QFile file(fileStr);
        QVariantList drmValues;
        mAppManager->getDrmAttributes(file, listDrmAttr(), drmValues);
        
        mLog << "[QTH] [Test_XQApplicationManager] drm_file:" + fileStr;
        
        foreach (QVariant value, testData.files[fileStr].values()) {
            if (!value.isValid())
                mLog << "[QTH] [Test_XQApplicationManager] test attribute: not valid";
            else if (value.type() == QVariant::Int)
                mLog << "[QTH] [Test_XQApplicationManager] test attribute:" + QString::number(value.toInt());
            else if (value.type() == QVariant::String)
                mLog << "[QTH] [Test_XQApplicationManager] test attribute:" + value.toString();
        }
        
        foreach (QVariant value, drmValues) {
            if (!value.isValid())
                mLog << "[QTH] [Test_XQApplicationManager] getDrmAttributes: not valid";
            else if (value.type() == QVariant::Int)
                mLog << "[QTH] [Test_XQApplicationManager] getDrmAttributes:" + QString::number(value.toInt());
            else if (value.type() == QVariant::String)
                mLog << "[QTH] [Test_XQApplicationManager] getDrmAttributes:" + value.toString();
        }
        
        QVERIFY2(testData.files[fileStr].values() == drmValues, mLog.join(endOfLine).toAscii());
        mLog = logmem;
    }
            
    QVERIFY2(mAppManager->lastError() == XQService::ENoError, mLog.join(endOfLine).toAscii());
    
    mLog << "[QTH] [Test_XQApplicationManager] drm_file end";
}

void Test_XQApplicationManager::drm_sharablefile()
{
    mLog << "[QTH] [Test_XQApplicationManager] drm_sharablefile";
    
#ifdef __WINS__
    QSKIP("Checking drm attributes does not work correctly on emulator", SkipAll);
#endif
    
    QStringList logmem = mLog;

    foreach (QString fileStr, testData.files.keys()) {
        XQSharableFile sharableFile;
        QVariantList drmValues;
        
        sharableFile.open(fileStr);
        mAppManager->getDrmAttributes(sharableFile, listDrmAttr(), drmValues);
        sharableFile.close();
        
        mLog << "[QTH] [Test_XQApplicationManager] drm_sharablefile:" + fileStr;
        foreach (QVariant value, testData.files[fileStr].values()) {
            if (!value.isValid())
                mLog << "[QTH] [Test_XQApplicationManager] test attribute: not valid";
            else if (value.type() == QVariant::Int)
                mLog << "[QTH] [Test_XQApplicationManager] test attribute:" + QString::number(value.toInt());
            else if (value.type() == QVariant::String)
                mLog << "[QTH] [Test_XQApplicationManager] test attribute:" + value.toString();
        }
        
        foreach (QVariant value, drmValues) {
            if (!value.isValid())
                mLog << "[QTH] [Test_XQApplicationManager] getDrmAttributes: not valid";
            else if (value.type() == QVariant::Int)
                mLog << "[QTH] [Test_XQApplicationManager] getDrmAttributes:" + QString::number(value.toInt());
            else if (value.type() == QVariant::String)
                mLog << "[QTH] [Test_XQApplicationManager] getDrmAttributes:" + value.toString();
        }
        
        QVERIFY2(testData.files[fileStr].values() == drmValues, mLog.join(endOfLine).toAscii());
        mLog = logmem;
    }
            
    QVERIFY2(mAppManager->lastError() == XQService::ENoError, mLog.join(endOfLine).toAscii());
    
    mLog << "[QTH] [Test_XQApplicationManager] drm_sharablefile end";
}

void Test_XQApplicationManager::list_interface()
{
    mLog << "[QTH] [Test_XQApplicationManager] list_interface";
    
    foreach (QString interface, testData.interfaces.keys()) {
        if (interface == IUriView || interface == IFileView)
            continue;
    
        QList<XQAiwInterfaceDescriptor> list = mAppManager->list(interface, operation);
        
        mLog << "[QTH] [Test_XQApplicationManager] list_interface interfaces defined:" 
                 + QString::number(testData.interfaces.values(interface).count());
        mLog << "[QTH] [Test_XQApplicationManager] list_interface system interfaces:" 
                 + QString::number(list.count());
        QVERIFY2(list.count() == testData.interfaces.values(interface).count(), mLog.join(endOfLine).toAscii());
        
        foreach (XQAiwInterfaceDescriptor interfaceDesc, list) {
            bool equal = false;
            foreach (InterfaceData* interfaceData, testData.interfaces.values(interface)) {
                if (interfaceData->compare(interfaceDesc, mLog)) {
                    equal = true;
                    break;
                }
            }
            
            mLog << "[QTH] [Test_XQApplicationManager] list_interface service name:" + interfaceDesc.serviceName();
            mLog << "[QTH] [Test_XQApplicationManager] list_interface interface name:" + interfaceDesc.interfaceName();
            mLog << "[QTH] [Test_XQApplicationManager] list_interface interface OK?:" + QString::number((int)equal) << QString::number((int)interfaceDesc.isValid());
            
            QVERIFY2(equal, mLog.join(endOfLine).toAscii());
            QVERIFY2(interfaceDesc.isValid(), mLog.join(endOfLine).toAscii());
        }
    }
    QVERIFY2(mAppManager->lastError() == XQService::ENoError, mLog.join(endOfLine).toAscii());
    
    mLog << "[QTH] [Test_XQApplicationManager] list_interface end";
}

void Test_XQApplicationManager::list_service_interface()
{
    mLog << "[QTH] [Test_XQApplicationManager] list_service_interface";
    
    foreach (InterfaceData* interfaceData1, testData.interfaces.values()) {
        if (interfaceData1->mInterfaceName == IUriView || interfaceData1->mInterfaceName == IFileView)
            continue;
        
        QList<XQAiwInterfaceDescriptor> list = mAppManager->list(interfaceData1->mServiceName, interfaceData1->mInterfaceName, operation);
        
        int countInterfaces = 0;
        foreach (XQAiwInterfaceDescriptor interfaceDesc, list) {
            bool equal = false;
            foreach (InterfaceData* interfaceData2, testData.interfaces.values(interfaceData1->mInterfaceName))
            {
                if (interfaceData2->compare(interfaceDesc, mLog))
                    equal = true;
                if (interfaceData2->mServiceName == interfaceData1->mServiceName)
                    countInterfaces++;
            }
            
            mLog << "[QTH] [Test_XQApplicationManager] list_service_interface service name:" + interfaceDesc.serviceName();
            mLog << "[QTH] [Test_XQApplicationManager] list_service_interface interface name:" + interfaceDesc.interfaceName();
            mLog << "[QTH] [Test_XQApplicationManager] list_service_interface interface OK?:" + QString::number((int)equal)
                                                                                              + QString::number((int)interfaceDesc.isValid());
            
            QVERIFY2(equal, mLog.join(endOfLine).toAscii());
            QVERIFY2(interfaceDesc.isValid(), mLog.join(endOfLine).toAscii());
        }
        
        mLog << "[QTH] [Test_XQApplicationManager] list_service_interface interfaces defined:" 
                 + QString::number(countInterfaces);
        mLog << "[QTH] [Test_XQApplicationManager] list_service_interface system interfaces:" 
                 + QString::number(list.count());
        QVERIFY2(list.count() == countInterfaces, mLog.join(endOfLine).toAscii());
    }
    QVERIFY2(mAppManager->lastError() == XQService::ENoError, mLog.join(endOfLine).toAscii());
    
    mLog << "[QTH] [Test_XQApplicationManager] list_service_interface end";
}

void Test_XQApplicationManager::list_uri()
{
    mLog << "[QTH] [Test_XQApplicationManager] list_uri";
    
    QList<XQAiwInterfaceDescriptor> list = mAppManager->list(testUrl);
    
    mLog << "[QTH] [Test_XQApplicationManager] list_uri interfaces defined:" 
             + QString::number(testData.interfaces.values(IUriView).count());
    mLog << "[QTH] [Test_XQApplicationManager] list_uri system interfaces:" 
             + QString::number(list.count());    
    QVERIFY2(list.count() == testData.interfaces.values(IUriView).count(), mLog.join(endOfLine).toAscii());
    
    foreach (XQAiwInterfaceDescriptor interfaceDesc, list) {
        bool equal = false;
        foreach (InterfaceData* interfaceData, testData.interfaces.values(IUriView))
            if (interfaceData->compare(interfaceDesc, mLog))
            {
                equal = true;
                break;
            }
        
        mLog << "[QTH] [Test_XQApplicationManager] list_uri service name:" + interfaceDesc.serviceName();
        mLog << "[QTH] [Test_XQApplicationManager] list_uri interface name:" + interfaceDesc.interfaceName();
        mLog << "[QTH] [Test_XQApplicationManager] list_uri interface OK?:" + QString::number((int)equal) 
                                                                            + QString::number((int)interfaceDesc.isValid());
                    
        QVERIFY2(equal, mLog.join(endOfLine).toAscii());
        QVERIFY2(interfaceDesc.isValid(), mLog.join(endOfLine).toAscii());
    }
    
    QVERIFY2(mAppManager->lastError() == XQService::ENoError, mLog.join(endOfLine).toAscii());
    
    mLog << "[QTH] [Test_XQApplicationManager] list_uri end";
}

void Test_XQApplicationManager::list_file()
{
    mLog << "[QTH] [Test_XQApplicationManager] list_file";
    
    QSKIP("Currently function list(const QFile& file) returns maximum one interface", SkipAll);
    
    QList<XQAiwInterfaceDescriptor> list = mAppManager->list(testFile);
    
    foreach (InterfaceData* interfaceData, testData.interfaces.values(IFileView)) {
        bool equal = false;
        foreach (XQAiwInterfaceDescriptor interfaceDesc, list)
            if (interfaceData->compare(interfaceDesc, mLog)) {
                equal = true;
                break;
            }
        
        mLog << "[QTH] [Test_XQApplicationManager] list_file service name:" + interfaceData->mServiceName;
        mLog << "[QTH] [Test_XQApplicationManager] list_file interface name:" + interfaceData->mInterfaceName;
        mLog << "[QTH] [Test_XQApplicationManager] list_file interface OK?:" + QString::number((int)equal);
        
        QVERIFY2(equal, mLog.join(endOfLine).toAscii());
    }
    
    QVERIFY2(mAppManager->lastError() == XQService::ENoError, mLog.join(endOfLine).toAscii());
    
    mLog << "[QTH] [Test_XQApplicationManager] list_file end";
}

void Test_XQApplicationManager::list_sharablefile()
{
    mLog << "[QTH] [Test_XQApplicationManager] list_sharablefile";

    QSKIP("Currently function list(const XQSharableFile& file) returns maximum one interface", SkipAll);
    
    QList<XQAiwInterfaceDescriptor> list = mAppManager->list(*testSharableFile);
    
    foreach (InterfaceData* interfaceData, testData.interfaces.values(IFileView)) {
        bool equal = false;
        foreach (XQAiwInterfaceDescriptor interfaceDesc, list) {
            if (interfaceData->compare(interfaceDesc, mLog)) {
                equal = true;
                break;
            }
        }
        
        mLog << "[QTH] [Test_XQApplicationManager] list_sharablefile service name:" + interfaceData->mServiceName;
        mLog << "[QTH] [Test_XQApplicationManager] list_sharablefile interface name:" + interfaceData->mInterfaceName;
        mLog << "[QTH] [Test_XQApplicationManager] list_sharablefile interface OK?:" + QString::number((int)equal);
        
        QVERIFY2(equal, mLog.join(endOfLine).toAscii());
    }
    
    QVERIFY2(mAppManager->lastError() == XQService::ENoError, mLog.join(endOfLine).toAscii());
    
    mLog << "[QTH] [Test_XQApplicationManager] list_sharablefile end";
}

void Test_XQApplicationManager::create_interface()
{
    mLog << "[QTH] [Test_XQApplicationManager] create_interface";
    
    foreach (QString interface, testData.interfaces.keys()) {
        if (interface == IUriView || interface == IFileView)
            continue;
        
        mLog << "[QTH] [Test_XQApplicationManager] create_interface:" + interface;
        
        {
            XQAiwRequest* request = mAppManager->create(interface, operation, true);
            testRequest(request, operation, testString, true, true);
            delete request;
        }
        
        {
            XQAiwRequest* request = mAppManager->create(interface, operation_async, true);
            testRequest(request, operation_async, testString, true, false);
            delete request;
        }
        
        {
            XQAiwRequest* request = mAppManager->create(interface, operation, false);
            testRequest(request, operation, testString, false, true);
            delete request;
        }
        
        {
            XQAiwRequest* request = mAppManager->create(interface, operation_async, false);
            testRequest(request, operation_async, testString, false, false);
            delete request;
        }
    }
    QVERIFY2(mAppManager->lastError() == XQService::ENoError, mLog.join(endOfLine).toAscii());
    
    mLog << "[QTH] [Test_XQApplicationManager] create_interface end";
}

void Test_XQApplicationManager::create_service_interface()
{
    mLog << "[QTH] [Test_XQApplicationManager] create_service_interface";

    foreach (InterfaceData* interfaceData, testData.interfaces.values()) {
        if (interfaceData->mInterfaceName == IUriView || interfaceData->mInterfaceName == IFileView)
           continue;
    
        mLog << "[QTH] [Test_XQApplicationManager] create_service_interface service:" + interfaceData->mServiceName;
        mLog << "[QTH] [Test_XQApplicationManager] create_service_interface interface:" + interfaceData->mInterfaceName;
        
        {
            XQAiwRequest* request = mAppManager->create(interfaceData->mServiceName, interfaceData->mInterfaceName, operation, true);
            testRequest(request, operation, testString, true, true);
            delete request;
        }
            
        {
            XQAiwRequest* request = mAppManager->create(interfaceData->mServiceName, interfaceData->mInterfaceName, operation_async, true);
            testRequest(request, operation_async, testString, true, false);
            delete request;
        }
            
        {
            XQAiwRequest* request = mAppManager->create(interfaceData->mServiceName, interfaceData->mInterfaceName, operation, false);
            testRequest(request, operation, testString, false, true);
            delete request;
        }
            
        {
            XQAiwRequest* request = mAppManager->create(interfaceData->mServiceName, interfaceData->mInterfaceName, operation_async, false);
            testRequest(request, operation_async, testString, false, false);
            delete request;
        }
    }
    QVERIFY2(mAppManager->lastError() == XQService::ENoError, mLog.join(endOfLine).toAscii());
    
    mLog << "[QTH] [Test_XQApplicationManager] create_service_interface end";
}

void Test_XQApplicationManager::create_implementation()
{
    mLog << "[QTH] [Test_XQApplicationManager] create_implementation";

    foreach (InterfaceData* interfaceData, testData.interfaces.values()) {
        if (interfaceData->mInterfaceName == IUriView || interfaceData->mInterfaceName == IFileView)
            continue;
        foreach (XQAiwInterfaceDescriptor interfaceDesc, mAppManager->list(interfaceData->mServiceName, interfaceData->mInterfaceName, operation)) {
            mLog << "[QTH] [Test_XQApplicationManager] create_implementation service:" + interfaceDesc.interfaceName();
            mLog << "[QTH] [Test_XQApplicationManager] create_implementation interface:" + interfaceDesc.serviceName();
            mLog << "[QTH] [Test_XQApplicationManager] create_implementation version:" 
                     + QString::number(interfaceDesc.majorVersion()) + "." + QString::number(interfaceDesc.minorVersion());
            
            {
                XQAiwRequest* request = mAppManager->create(interfaceDesc, operation, true);
                testRequest(request, operation, testString, true, true);
                delete request;
            }
                
            {
                XQAiwRequest* request = mAppManager->create(interfaceDesc, operation_async, true);
                testRequest(request, operation_async, testString, true, false);
                delete request;
            }
                
            {
                XQAiwRequest* request = mAppManager->create(interfaceDesc, operation, false);
                testRequest(request, operation, testString, false, true);
                delete request;
            }
                
            {
                XQAiwRequest* request = mAppManager->create(interfaceDesc, operation_async, false);
                testRequest(request, operation_async, testString, false, false);
                delete request;
            }
        }
    }
    
    QVERIFY2(mAppManager->lastError() == XQService::ENoError, mLog.join(endOfLine).toAscii());
    
    mLog << "[QTH] [Test_XQApplicationManager] create_implementation end";
}

void Test_XQApplicationManager::create_uri()
{
    mLog << "[QTH] [Test_XQApplicationManager] create_uri";

    {
        XQAiwRequest* request = mAppManager->create(testUrl, true);
        testRequest(request, QString(), testUrl.toString(), true, true);
        delete request;
    }
        
    {
        XQAiwRequest* request = mAppManager->create(testUrl, false);
        testRequest(request, QString(), testUrl.toString(), false, true);
        delete request;
    }
            
    QVERIFY2(mAppManager->lastError() == XQService::ENoError, mLog.join(endOfLine).toAscii());
    
    mLog << "[QTH] [Test_XQApplicationManager] create_uri end";
}

void Test_XQApplicationManager::create_uri_implementation()
{
    mLog << "[QTH] [Test_XQApplicationManager] create_uri_implementation";

    QList<XQAiwInterfaceDescriptor> list = mAppManager->list(testUrl);
    
    foreach (XQAiwInterfaceDescriptor interfaceDesc, list)
        foreach (InterfaceData* interfaceData, testData.interfaces.values(IUriView))
        {
            if (interfaceData->compare(interfaceDesc, mLog)) {
                {
                    XQAiwRequest* request = mAppManager->create(testUrl, interfaceDesc, true);
                    testRequest(request, QString(), testUrl.toString(), true, true);
                    delete request;
                }
                    
                {
                    XQAiwRequest* request = mAppManager->create(testUrl, interfaceDesc, false);
                    testRequest(request, QString(), testUrl.toString(), false, true);
                    delete request;
                }
            }
        
        mLog << "[QTH] [Test_XQApplicationManager] create_uri_implementation service:" + interfaceDesc.interfaceName();
        mLog << "[QTH] [Test_XQApplicationManager] create_uri_implementation interface:" + interfaceDesc.serviceName();
        mLog << "[QTH] [Test_XQApplicationManager] create_uri_implementation version:" 
                 + QString::number(interfaceDesc.majorVersion()) + "." + QString::number(interfaceDesc.minorVersion());
        
        QVERIFY2(interfaceDesc.isValid(), mLog.join(endOfLine).toAscii());
    }
            
    QVERIFY2(mAppManager->lastError() == XQService::ENoError, mLog.join(endOfLine).toAscii());
    
    mLog << "[QTH] [Test_XQApplicationManager] create_uri_implementation end";
}

void Test_XQApplicationManager::create_file()
{
    mLog << "[QTH] [Test_XQApplicationManager] create_file";

    {
        XQAiwRequest* request = mAppManager->create(testFile, true);
        testRequest(request, QString(), testFileStr, true, true);
        delete request;
    }
        
    {
        XQAiwRequest* request = mAppManager->create(testFile, false);
        testRequest(request, QString(), testFileStr, false, true);
        delete request;
    }
            
    QVERIFY2(mAppManager->lastError() == XQService::ENoError, mLog.join(endOfLine).toAscii());
    
    mLog << "[QTH] [Test_XQApplicationManager] create_file end";
}

void Test_XQApplicationManager::create_file_implementation()
{
    mLog << "[QTH] [Test_XQApplicationManager] create_file_implementation";

    QList<XQAiwInterfaceDescriptor> list = mAppManager->list(testFile);
    QVERIFY2(list.count() > 0, mLog.join(endOfLine).toAscii());
    
    foreach (XQAiwInterfaceDescriptor interfaceDesc, list)
        foreach (InterfaceData* interfaceData, testData.interfaces.values(IFileView)) {
            if (interfaceData->compare(interfaceDesc, mLog)) {
                {
                    XQAiwRequest* request = mAppManager->create(testFile, interfaceDesc, true);
                    testRequest(request, QString(), testFileStr, true, true);
                    delete request;
                }
                    
                {
                    XQAiwRequest* request = mAppManager->create(testFile, interfaceDesc, false);
                    testRequest(request, QString(), testFileStr, false, true);
                    delete request;
                }
            }
        
        mLog << "[QTH] [Test_XQApplicationManager] create_file_implementation service:" + interfaceDesc.interfaceName();
        mLog << "[QTH] [Test_XQApplicationManager] create_file_implementation interface:" + interfaceDesc.serviceName();
        mLog << "[QTH] [Test_XQApplicationManager] create_file_implementation version:" 
                 + QString::number(interfaceDesc.majorVersion()) + "." + QString::number(interfaceDesc.minorVersion());
        
        QVERIFY2(interfaceDesc.isValid(), mLog.join(endOfLine).toAscii());
    }
            
    QVERIFY2(mAppManager->lastError() == XQService::ENoError, mLog.join(endOfLine).toAscii());
    
    mLog << "[QTH] [Test_XQApplicationManager] create_file_implementation end";
}

void Test_XQApplicationManager::create_sharablefile()
{
    mLog << "[QTH] [Test_XQApplicationManager] create_sharablefile";
    
    {
        XQAiwRequest* request = mAppManager->create(*testSharableFile, true);
        testRequest(request, QString(), QString(), true, true, testSharableFile);
        delete request;
    }
        
    {
        XQAiwRequest* request = mAppManager->create(*testSharableFile, false);
        testRequest(request, QString(), testFileStr, false, true, testSharableFile);
        delete request;
    }
            
    QVERIFY2(mAppManager->lastError() == XQService::ENoError, mLog.join(endOfLine).toAscii());
    
    mLog << "[QTH] [Test_XQApplicationManager] create_sharablefile end";
}

void Test_XQApplicationManager::create_sharablefile_implementation()
{
    mLog << "[QTH] [Test_XQApplicationManager] create_sharablefile_implementation";
    
    QList<XQAiwInterfaceDescriptor> list = mAppManager->list(*testSharableFile);
    QVERIFY2(list.count() > 0, mLog.join(endOfLine).toAscii());
    
    foreach (XQAiwInterfaceDescriptor interfaceDesc, list) {
        foreach (InterfaceData* interfaceData, testData.interfaces.values(IFileView)) {
            if (interfaceData->compare(interfaceDesc, mLog)) {
                {
                    XQAiwRequest* request = mAppManager->create(*testSharableFile, interfaceDesc, true);
                    testRequest(request, QString(), testFileStr, true, true, testSharableFile);
                    delete request;
                }
                    
                {
                    XQAiwRequest* request = mAppManager->create(*testSharableFile, interfaceDesc, false);
                    testRequest(request, QString(), testFileStr, false, true, testSharableFile);
                    delete request;
                }
            }
        }
        
        mLog << "[QTH] [Test_XQApplicationManager] create_sharablefile_implementation service:" + interfaceDesc.interfaceName();
        mLog << "[QTH] [Test_XQApplicationManager] create_sharablefile_implementation interface:" + interfaceDesc.serviceName();
        mLog << "[QTH] [Test_XQApplicationManager] create_sharablefile_implementation version:" 
                 + QString::number(interfaceDesc.majorVersion()) + "." + QString::number(interfaceDesc.minorVersion());
        
        QVERIFY2(interfaceDesc.isValid(), mLog.join(endOfLine).toAscii());
    }
            
    QVERIFY2(mAppManager->lastError() == XQService::ENoError, mLog.join(endOfLine).toAscii());
    
    mLog << "[QTH] [Test_XQApplicationManager] create_sharablefile_implementation end";
}

const QList<int> Test_XQApplicationManager::listDrmAttr()
{
    QList<int> attr;
    
    attr.append(XQApplicationManager::IsProtected);
    attr.append(XQApplicationManager::IsForwardable);
    attr.append(XQApplicationManager::Description);
    attr.append(XQApplicationManager::MimeType);
    
    return attr;
}


void Test_XQApplicationManager::cleanup()
{
    delete mAppManager;
    mAppManager = NULL;
    mLog.clear();
    testSharableFile->close();
    delete testSharableFile;
    testSharableFile = NULL;
}

void Test_XQApplicationManager::testRequest(XQAiwRequest* request, const QString &operation, 
        const QString &arguments, bool embedded, bool synchronous, XQSharableFile *sharablefile)
{
    QVERIFY2(request != NULL, mLog.join(endOfLine).toAscii());
    
    QVERIFY2(request->isEmbedded() == embedded, mLog.join(endOfLine).toAscii());
    
    request->setSynchronous(synchronous);
    QVERIFY2(request->isSynchronous() == synchronous, mLog.join(endOfLine).toAscii());
    
    if (!operation.isEmpty())
        QVERIFY2(request->operation() == operation, mLog.join(endOfLine).toAscii());
    
    QList<QVariant> args;
    if (sharablefile) {
        args << qVariantFromValue(*sharablefile);
    } else {
        args << arguments;
    }
    
    request->setArguments(args);
    
    connect(request, SIGNAL(requestOk(const QVariant&)), this, SLOT(handleOk(const QVariant&)));
    connect(request, SIGNAL(requestError(int,const QString&)), this, SLOT(handleError(int,const QString&)));
    
    mLog << "[QTH] [Test_XQApplicationManager] send " + operation;
    mServiceAnswered = false;
    QVERIFY2(request->send(), mLog.join(endOfLine).toAscii());
   
    if (!operation.isEmpty()) {
        while (!mServiceAnswered)
            QTest::qWait(20);
        
        QStringList retList = returnValue.toStringList();
        
        QVERIFY2(retList.value(0) == request->descriptor().serviceName(), mLog.join(endOfLine).toAscii());
        QVERIFY2(retList.value(1) == request->descriptor().interfaceName(), mLog.join(endOfLine).toAscii());
        QVERIFY2(retList.value(2) == testString, mLog.join(endOfLine).toAscii());
    }
    else
        QTest::qWait(100);
}

void Test_XQApplicationManager::handleOk(const QVariant& ret)
{
    mLog << "[QTH] [Test_XQApplicationManager] request results:" + ret.toStringList().join(", ");
    returnValue = ret;
    mServiceAnswered = true;
}

void Test_XQApplicationManager::handleError(int err ,const QString& errString)
{
    mLog << "[QTH] [Test_XQApplicationManager] handleError: " + errString; 
    mLog << "[QTH] [Test_XQApplicationManager] err: " + QString::number(err);
    mServiceAnswered = true;
}

int main (int argc, char* argv[]) 
{
    QApplication app(argc, argv);
    QTEST_DISABLE_KEYPAD_NAVIGATION
    Test_XQApplicationManager tc;
    int ret = 0;
    if (argc == 1) {
        int c = 5;
        char* v[] = {argv[0], "-o", "c:/at_am.txt", "-maxwarnings", "0"};
        ret = QTest::qExec(&tc, c, v);
    }
    else {
        ret = QTest::qExec(&tc, argc, argv);
    }
    return ret;
}

