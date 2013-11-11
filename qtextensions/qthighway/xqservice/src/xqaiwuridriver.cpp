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

#include <QObject>
#include <qglobal.h>
#include <QDesktopServices>
#include <QLatin1String>

#include <xqaiwdecl.h>
#include "xqserviceglobal.h"
#include "xqservicelog.h"
#include "xqaiwuridriver.h"
#include <qservicemanager.h>  // Integrate to activity manager

using namespace QtMobility;

XQAiwUriDriver::XQAiwUriDriver(const QUrl &uri, const XQAiwInterfaceDescriptor& descriptor, const QString &operation)
    : XQAiwRequestDriver(),
       mUtilities(NULL)
{

    mErrorMsg = "";
    mDescr = descriptor;
    mUri = uri;
    mOperation = operation;

    XQSERVICE_DEBUG_PRINT("XQAiwUriDriver::XQAiwUriDriver: %s",
                          qPrintable(mUri.toString()));
    
}

XQAiwUriDriver::~XQAiwUriDriver()
{
    XQSERVICE_DEBUG_PRINT("~XQAiwUriDriver::XQAiwUriDriver");
    delete mUtilities;
    
}

QAction *XQAiwUriDriver::createAction()
{
    return NULL;  //  Not supported
}

void XQAiwUriDriver::setArguments(const QList<QVariant> &arguments)
{
    XQSERVICE_DEBUG_PRINT("XQAiwUriDriver::setArguments");
    mArguments =  arguments;
}


bool XQAiwUriDriver::send(QVariant& retValue)
{
    Q_UNUSED(retValue);
    XQSERVICE_DEBUG_PRINT("XQAiwUriDriver::send %s", qPrintable(mUri.toString()));

    mErrorCode = XQService::ENoError;
    
    // Browser URI
    if (mUri.scheme().compare("http", Qt::CaseInsensitive) == 0 ||
        mUri.scheme().compare("https", Qt::CaseInsensitive) == 0)
    {
        XQSERVICE_DEBUG_PRINT("Apply QDesktopServices::openUrl");
        bool b = QDesktopServices::openUrl(mUri);
        if (!b)
        {
            mErrorCode = XQService::EConnectionError;
            mErrorMsg = XQAiwUtils::createErrorMessage(mErrorCode, "QDesktopServices::openUrl", mUri.toString());
        }
        return b;
    }

    // "appto" URI
    if (mUri.scheme().compare(XQURI_SCHEME_ACTIVITY,Qt::CaseInsensitive) == 0)
    {
        return handleApptoUri(mUri);
    }

    //
    // The rest
    //
    mErrorCode = XQService::EArgumentError;
    mErrorMsg = XQAiwUtils::createErrorMessage(mErrorCode, "XQAiwUriDriver:Not custom handler for", mUri.toString());
    
    return false;
    
}

const QString& XQAiwUriDriver::lastErrorMessage() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwUriDriver::lastErrorMessage: %s", qPrintable(mErrorMsg));
    return mErrorMsg;
}

int XQAiwUriDriver::lastError() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwUriDriver::lastError %d", mErrorCode);
    return mErrorCode;
}

const XQAiwInterfaceDescriptor &XQAiwUriDriver::descriptor() const 
{
    XQSERVICE_DEBUG_PRINT("XQAiwUriDriver::descriptor");
    return mDescr; 
}

void XQAiwUriDriver::setEmbedded(bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQAiwUriDriver::setEmbedded");
    mEmbedded = embedded;
}
bool XQAiwUriDriver::isEmbedded() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwUriDriver::isEmbedded");
    return mEmbedded;
}

void XQAiwUriDriver::setOperation(const QString &operation)
{
    XQSERVICE_DEBUG_PRINT("XQAiwUriDriver::setOperation");
    mOperation = operation;
}
const QString &XQAiwUriDriver::operation() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwUriDriver::operation");
    return mOperation;
}

void XQAiwUriDriver::setSynchronous(bool synchronous)
{
    XQSERVICE_DEBUG_PRINT("XQAiwUriDriver::setSynchronous");
    mSynchronous = synchronous;
}
bool XQAiwUriDriver::isSynchronous() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwUriDriver::isSynchronous");
    return mSynchronous;
}

//
// Check if we can handle the URI
//
bool XQAiwUriDriver::hasCustomHandler(const QUrl &uri)
{
    QString scheme = uri.scheme();
    return  (scheme.compare(XQURI_SCHEME_ACTIVITY,Qt::CaseInsensitive) == 0 ||
            scheme.compare(XQURI_SCHEME_HTTP,Qt::CaseInsensitive) == 0 ||
            scheme.compare(XQURI_SCHEME_HTTPS,Qt::CaseInsensitive) == 0);
}


void XQAiwUriDriver::handleAsyncResponse(const QVariant& value)
{
    XQSERVICE_DEBUG_PRINT("XQAiwUriDriver::handleAsyncResponse");
    emit requestOk(value);
}

void XQAiwUriDriver::handleAsyncError(int err)
{
    XQSERVICE_DEBUG_PRINT("XQAiwUriDriver::handleAsyncError");

   mErrorCode = err;
   mErrorMsg = XQAiwUtils::createErrorMessage(mErrorCode,"XQAiwUriDriver", "async send");
   emit requestError(mErrorCode, mErrorMsg);
}

void XQAiwUriDriver::setBackground(bool background )
{
    XQSERVICE_DEBUG_PRINT("XQAiwUriDriver::setBackground");
    mBackground = background;
}
bool XQAiwUriDriver::isBackground() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwUriDriver::isBackground");
    return mBackground;
}

bool XQAiwUriDriver::handleApptoUri(const QUrl &uri)
{
    //
    // Use Qt service framework errors as is (they are in different range than XQ errors)
    //
    QLatin1String xml("Z:/resource/activity/afservice.xml");
    mErrorCode = QServiceManager::NoError;
                
    QtMobility::QServiceManager serviceManager;
    XQSERVICE_DEBUG_PRINT("XQAiwUriDriver::handleApptoUri");

    bool b = serviceManager.addService(xml);
    mErrorCode = serviceManager.error();
    XQSERVICE_DEBUG_PRINT("\taddService:xml=%s,status=%d", qPrintable(QString(xml)),mErrorCode);
    switch (mErrorCode)
    {
        case QServiceManager::NoError:
        case QServiceManager::ServiceAlreadyExists:
        case QServiceManager::ImplementationAlreadyExists:
            // These are OK cases.
            mErrorCode = QServiceManager::NoError;
            break;
        default:
            return false;
    }
    
    QObject* activityManager = serviceManager.loadInterface("com.nokia.qt.activities.ActivityManager");
    mErrorCode = serviceManager.error();
    XQSERVICE_DEBUG_PRINT("\tloadInterface %s=%d", "com.nokia.qt.activities.ActivityManager", mErrorCode);
    if (!activityManager)
    {
       mErrorCode = serviceManager.error();
       return false;
    }
    
    QMetaObject::invokeMethod(activityManager, "launchActivity", Q_ARG(QUrl,uri));
    mErrorCode = serviceManager.error();
    
    XQSERVICE_DEBUG_PRINT("\tinvokeMethod=%d", mErrorCode);
    delete activityManager;

    XQSERVICE_DEBUG_PRINT("XQAiwUriDriver::handleApptoUri done");
    
    return mErrorCode == QServiceManager::NoError;
}
