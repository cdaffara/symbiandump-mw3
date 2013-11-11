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

#include "xqservicelog.h"
#include "xqaiwutils.h"
#include "xqaiwfiledriver.h"

XQAiwFileDriver::XQAiwFileDriver(const QFile &file, const XQAiwInterfaceDescriptor& descriptor, const QString &operation)
    : XQAiwRequestDriver(),
      mUtilities(NULL)
{

    mErrorMsg = "";
    mDescr = descriptor; 
    mOperation = operation;
    mFile = file.fileName();
    
    XQSERVICE_DEBUG_PRINT("XQAiwFileDriver::XQAiwFileDriver: %s",
                          qPrintable(file.fileName()));
    
}

XQAiwFileDriver::XQAiwFileDriver(const XQSharableFile &file, const XQAiwInterfaceDescriptor& descriptor, const QString &operation)
    : XQAiwRequestDriver(),
      mUtilities(NULL)
{

    mErrorMsg = "";
    mDescr = descriptor; 
    mOperation = operation;
    mFile = qVariantFromValue(file);

    XQSERVICE_DEBUG_PRINT("XQAiwFileDriver::XQAiwFileDriver: sharable %s",
                          qPrintable(file.fileName()));

}


XQAiwFileDriver::~XQAiwFileDriver()
{
    XQSERVICE_DEBUG_PRINT("~XQAiwFileDriver::XQAiwFileDriver");
    delete mUtilities;
    
}

QAction *XQAiwFileDriver::createAction()
{
    return NULL;  //  Not supported
}

void XQAiwFileDriver::setArguments(const QList<QVariant> &arguments)
{
    XQSERVICE_DEBUG_PRINT("XQAiwFileDriver::setArguments");
    mArguments =  arguments;
}


bool XQAiwFileDriver::send(QVariant& retValue)
{
    XQSERVICE_DEBUG_PRINT("XQAiwFileDriver::send");

    if (mUtilities == NULL)
        mUtilities = new XQAiwUtils();
    if (mUtilities == NULL)
    {
        XQSERVICE_DEBUG_PRINT("XQAiwFileDriver::Can not create aiw utils");
        return false;
    }
    QVariant applicationId =  mDescr.property(XQAiwInterfaceDescriptor::ImplementationId);
    XQSERVICE_DEBUG_PRINT("Application id %x", applicationId.toInt());

    mErrorCode = mUtilities->launchFile(applicationId.toInt(), mFile);
    XQSERVICE_DEBUG_PRINT("XQAiwFileDriver::errorCode %d", mErrorCode);
    QVariant ret(!mErrorCode);
    retValue = ret;
    return (!mErrorCode);
}

const QString& XQAiwFileDriver::lastErrorMessage() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwFileDriver::lastErrorMessage: %s", qPrintable(mErrorMsg));
    return mErrorMsg;
}

int XQAiwFileDriver::lastError() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwFileDriver::lastError %d", mErrorCode);
    return mErrorCode;
}

const XQAiwInterfaceDescriptor &XQAiwFileDriver::descriptor() const 
{
    XQSERVICE_DEBUG_PRINT("XQAiwFileDriver::descriptor");
    return mDescr; 
}

void XQAiwFileDriver::setEmbedded(bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQAiwFileDriver::setEmbedded");
    mEmbedded = embedded;
}
bool XQAiwFileDriver::isEmbedded() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwFileDriver::isEmbedded");
    return mEmbedded;
}

void XQAiwFileDriver::setOperation(const QString &operation)
{
    XQSERVICE_DEBUG_PRINT("XQAiwFileDriver::setOperation");
    mOperation = operation;
}
const QString &XQAiwFileDriver::operation() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwFileDriver::operation");
    return mOperation;
}

void XQAiwFileDriver::setSynchronous(bool synchronous)
{
    XQSERVICE_DEBUG_PRINT("XQAiwFileDriver::setSynchronous");
    mSynchronous = synchronous;
}
bool XQAiwFileDriver::isSynchronous() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwFileDriver::isSynchronous");
    return mSynchronous;
}

void XQAiwFileDriver::handleAsyncResponse(const QVariant& value)
{
    XQSERVICE_DEBUG_PRINT("XQAiwFileDriver::handleAsyncResponse");
    emit requestOk(value);
}

void XQAiwFileDriver::handleAsyncError(int err)
{
    XQSERVICE_DEBUG_PRINT("XQAiwFileDriver::handleAsyncError");

   mErrorCode = err;
   mErrorMsg = XQAiwUtils::createErrorMessage(mErrorCode, "XQAiwFileDriver");
   emit requestError(mErrorCode, mErrorMsg);
}

void XQAiwFileDriver::setBackground(bool background )
{
    XQSERVICE_DEBUG_PRINT("XQAiwFileDriver::setBackground");
    mBackground = background;
}
bool XQAiwFileDriver::isBackground() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwFileDriver::isBackground");
    return mBackground;
}

