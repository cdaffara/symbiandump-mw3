/*
*
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
#include <QApplication>
#include <QLatin1String>
#include <QLocale>
#include <QIcon>
#include <QFileInfo>

#include <xqaiwdecl.h>
#include <xqservicerequest.h>
#include "xqrequestutil.h"
#include "xqservicelog.h"
#include "xqaiwutils.h"
#include "xqaiwservicedriver.h"

// Constants

XQAiwServiceDriver::XQAiwServiceDriver(const XQAiwInterfaceDescriptor& descriptor, const QString &operation)
    : XQAiwRequestDriver(),
      currentRequest(NULL),
      completeSignalConnected(false),
      errorSignalConnected(false)
    {
    mErrorMsg = "";
    mDescr = descriptor; 
    mOperation = operation; 
    XQSERVICE_DEBUG_PRINT("XQAiwServiceDriver::XQAiwServiceDriver: %s %s,%x",
                          qPrintable(mDescr.interfaceName()),
                          qPrintable(mOperation),
                          mDescr.property(XQAiwInterfaceDescriptor::ImplementationId).toInt());
    
    // Allocate QtHighway request
    currentRequest = new XQServiceRequest(mDescr, mOperation);
    }

XQAiwServiceDriver::~XQAiwServiceDriver()
{
    XQSERVICE_DEBUG_PRINT("~XQAiwServiceDriver::XQAiwServiceDriver");

    // Disconnect signals
    if (completeSignalConnected)
    {
        disconnect(currentRequest, SIGNAL(requestCompleted(const QVariant&)), this, SLOT(handleAsyncResponse(const QVariant&)));
    }
    if (errorSignalConnected)
    {
        disconnect(currentRequest, SIGNAL(requestError(int)), this, SLOT(handleAsyncError(int)));
    }
    
    delete currentRequest; // Destructor cancels the async request

    XQSERVICE_DEBUG_PRINT("~XQAiwServiceDriver::XQAiwServiceDriver 2");
    removeTranslator();
    currentRequest = NULL;
}



QAction *XQAiwServiceDriver::createAction()
{
    XQSERVICE_DEBUG_PRINT("XQAiwServiceDriver::createAction");
    
    if (!qApp)
        return NULL;  // Not supported in non-GUI environments

    installTranslator();
    
    QString textId = mDescr.customProperty(XQCUSTOM_PROP_AIW_TEXT);
    if (textId.isEmpty())
    {
        // Applye the key name and make it visible to client as indication of
        // missing property ..
        textId = "#missing " + XQCUSTOM_PROP_AIW_TEXT;
    }
    
    QByteArray ba = textId.toLatin1();
    const char *textPtr = ba.data();
    QString text = qtTrId(textPtr);  // translate
    XQSERVICE_DEBUG_PRINT("Translated text %s", qPrintable(text));

    QAction *action=0;
    QIcon icon;
    QString iconFile = mDescr.customProperty(XQCUSTOM_PROP_AIW_ICON);
    if (!iconFile.isEmpty())
    {
        XQSERVICE_DEBUG_PRINT("QAction with icon and text");
        icon.addFile(iconFile);
        action = new QAction(icon, text, 0);
    }
    else
    {
        XQSERVICE_DEBUG_PRINT("QAction with text");
        action = new QAction(text,0);
    }

    return action;
}

void XQAiwServiceDriver::setArguments(const QList<QVariant> &arguments)
{
    XQSERVICE_DEBUG_PRINT("XQAiwServiceDriver::setArguments");
    currentRequest->setArguments(arguments);
}


bool XQAiwServiceDriver::send(QVariant& retValue)
{
    XQSERVICE_DEBUG_PRINT("XQAiwServiceDriver::send>>>");

    // Update info  (these ones  can be given via XQAiwRequest function count)
    XQRequestInfo opt = info();
    QVariant emb = opt.info(XQServiceUtils::OptEmbedded);
    QVariant bg = opt.info(XQServiceUtils::OptBackground);
    if (!emb.isValid())
    {
        // Not set via setInfo
        opt.setEmbedded(mEmbedded); 
    }
    if (!bg.isValid())
    {
        // Not set via setInfo
        opt.setBackground(mBackground);
    }
    currentRequest->setInfo(opt);
    
    QStringList list;
    bool res = true;
    if (!currentRequest->isSynchronous() && !completeSignalConnected)
    {
        // Async request. Connect signal only once
        XQSERVICE_DEBUG_PRINT("request::async send");
        connect(currentRequest, SIGNAL(requestCompleted(const QVariant&)), this, SLOT(handleAsyncResponse(const QVariant&)));
        completeSignalConnected = true;
    }
    if (!errorSignalConnected)
    {
        // Connect error signal only once
        connect(currentRequest, SIGNAL(requestError(int)), this, SLOT(handleAsyncError(int)));
        errorSignalConnected = true;
    }
    
    XQSERVICE_DEBUG_PRINT("request::send>>>");
    res = currentRequest->send(retValue);  // Result is valid for sync request only
    XQSERVICE_DEBUG_PRINT("request::send: %d<<<", res);
    
    mErrorCode = XQRequestUtil::mapError(currentRequest->latestError());
    if (res && !mErrorCode)
    {
        mErrorMsg = "";
    }
    else
    {
        // This is for debugging/trace purposes only,  no need to localize
        mErrorMsg = XQAiwUtils::createErrorMessage(mErrorCode, "XQAiwServiceDriver", "sync send");
    }
    
    XQSERVICE_DEBUG_PRINT("XQAiwServiceDriver::send: %d<<<", res);
    
    return res;

}

const QString& XQAiwServiceDriver::lastErrorMessage() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwServiceDriver::lastErrorMessage: %s", qPrintable(mErrorMsg));
    return mErrorMsg;
}

int XQAiwServiceDriver::lastError() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwServiceDriver::lastError %d", mErrorCode);
    return mErrorCode;
}

const XQAiwInterfaceDescriptor &XQAiwServiceDriver::descriptor() const 
{
    XQSERVICE_DEBUG_PRINT("XQAiwServiceDriver::descriptor");
    return mDescr; 
}

void XQAiwServiceDriver::setEmbedded(bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQAiwServiceDriver::setEmbedded");
    // Embedded is option
    mEmbedded = embedded;
}
bool XQAiwServiceDriver::isEmbedded() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwServiceDriver::isEmbedded");
    // Embedded is option
    return mEmbedded;
}


void XQAiwServiceDriver::setBackground(bool background )
{
    XQSERVICE_DEBUG_PRINT("XQAiwServiceDriver::setBackground");
    mBackground = background;
}
bool XQAiwServiceDriver::isBackground() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwServiceDriver::isBackground");
    return mBackground;
}


void XQAiwServiceDriver::setOperation(const QString &operation)
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::setOperation");
    mOperation = operation;
    currentRequest->setMessage(operation);
}
const QString &XQAiwServiceDriver::operation() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwRequest::operation");
    return mOperation;
}

void XQAiwServiceDriver::setSynchronous(bool synchronous)
{
    XQSERVICE_DEBUG_PRINT("XQAiwServiceDriver::setSynchronous");
    currentRequest->setSynchronous(synchronous);
}
bool XQAiwServiceDriver::isSynchronous() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwServiceDriver::isSynchronous");
    
    return currentRequest->isSynchronous();
}

void XQAiwServiceDriver::setInfo(const XQRequestInfo &info)
{
    XQSERVICE_DEBUG_PRINT("XQAiwServiceDriver::setInfo");
    currentRequest->setInfo(info);
}

XQRequestInfo XQAiwServiceDriver::info() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwServiceDriver::info");
    return currentRequest->info();
}

void XQAiwServiceDriver::handleAsyncResponse(const QVariant& value)
{
    XQSERVICE_DEBUG_PRINT("XQAiwServiceDriver::handleAsyncResponse");
    emit requestOk(value);
}

void XQAiwServiceDriver::handleAsyncError(int err)
{
    XQSERVICE_DEBUG_PRINT("XQAiwServiceDriver::handleAsyncError");

   mErrorCode = XQRequestUtil::mapError(err);
   mErrorMsg = XQAiwUtils::createErrorMessage(mErrorCode, "XQAiwServiceDriver", "async send");
   emit requestError(mErrorCode, mErrorMsg);

}


bool XQAiwServiceDriver::installTranslator()
{
    XQSERVICE_DEBUG_PRINT("XQAiwServiceDriver::installTranslator");

    if (!qApp)
    {
        XQSERVICE_DEBUG_PRINT("\tNo application context");
        return false;
    }

    // The the name of the resource file containing the text
    QString textFile = mDescr.customProperty(XQCUSTOM_PROP_AIW_TEXT_FILE);
    if (textFile.isEmpty())
    {
        XQSERVICE_DEBUG_PRINT("\tCustom property missing for text file");
        return false;
    }
    
    // Check if locale has changed since last request
    QString lang = QLocale::system().name();
    XQSERVICE_DEBUG_PRINT("\tLanguage now is%s", qPrintable(lang));
    if (!this->lastLang.isEmpty() && (this->lastLang != lang))
    { 
        // Language has changed since last time
        // Remove previous translator, if any
        removeTranslator();
    }

    textFile = makeFileName(textFile);
    
    bool res=false;
    if (this->translator.isEmpty())
    {
        // Construct the full name of the localized resource
        XQSERVICE_DEBUG_PRINT("textFile name is %s", qPrintable(textFile));
        res = this->translator.load(textFile);
        if (res)
        {
            qApp->installTranslator(&this->translator);
            this->lastLang = lang;  // Remember the current language
            XQSERVICE_DEBUG_PRINT("Translator installed %s", qPrintable(lang));
        }
    }
    
    return res;
    
}

void XQAiwServiceDriver::removeTranslator()
{
    if (qApp && !this->translator.isEmpty())
    {
        qApp->removeTranslator(&this->translator);
    }
}

QString XQAiwServiceDriver::makeFileName(const QString &textFile) const
{
    QFileInfo info(textFile);
    QString ret = textFile;
    
    XQSERVICE_DEBUG_PRINT("XQAiwServiceDriver::makeFileName %s", qPrintable(textFile));
    
    if (info.baseName() == info.filePath())
    {
        // No path, apply the default path
        // Drive is where app is loaded, like "C:" or "Z:"
        ret = qApp->applicationFilePath().left(2) + "/resource/qt/translations/" + textFile;
    }

    // Add current language
    QString lang = QLocale::system().name();
    ret += "_"; 
    ret += lang;
    
    XQSERVICE_DEBUG_PRINT("XQAiwServiceDriver::makeFileName ret=%s", qPrintable(ret));

    return ret;
    
}
