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

#ifndef XQAIWREQUEST_H_
#define XQAIWREQUEST_H_

#include <f32file.h>

#include <xqaiwglobal.h>

#include <QObject>
#include <qglobal.h>

#include <QList>
#include <QStringList>
#include <QVariant>
#include <QAction>
#include <QUrl>
#include <QFile>
#include <xqaiwinterfacedescriptor.h>
#include <xqrequestinfo.h>
#include <xqsharablefile.h>

class XQAiwRequestDriver;

class XQSERVICE_EXPORT XQAiwRequest : public QObject
    {
    Q_OBJECT

public:

    XQAiwRequest(const XQAiwInterfaceDescriptor &descriptor, const QString &operation, bool embedded = true);
    XQAiwRequest(const QUrl &uri, const XQAiwInterfaceDescriptor &descriptor, const QString &operation);
    XQAiwRequest(const QFile &file, const XQAiwInterfaceDescriptor &descriptor, const QString &operation);
    XQAiwRequest(const XQSharableFile &file, const XQAiwInterfaceDescriptor &descriptor, const QString &operation);
    virtual ~XQAiwRequest();

public slots:
    
    QAction *createAction();
    
    void setArguments(const QList<QVariant> &arguments);

    int lastError() const;

    const QString& lastErrorMessage() const;

    const XQAiwInterfaceDescriptor &descriptor() const;

    bool send();
    bool send(QVariant &returnValue);

    void setEmbedded(bool embedded);
    bool isEmbedded() const;

    void setOperation(const QString &operation);
    const QString &operation() const;

    void setSynchronous(bool synchronous);
    bool isSynchronous() const;
    
    void setBackground(bool background );
    bool isBackground() const;

    void setInfo(const XQRequestInfo &info);
    XQRequestInfo info() const;

    
signals:

/*!
    Emitted when the QAction attached to request has been triggered.
    Upon this signal is the latest time to call
    setArguments(const QList<QVariant> &arguments) if not done earlier.
*/
    void triggered();
    
/*!
    This signal is emitted when interworking request has been successfully
    executed (synchronous or asynchronous). That is, the service has returned
    data back according to the slot spec. The return value may be real data
    or indicate an service specific error.
    \param result Result of the executed request
*/
    void requestOk(const QVariant& result);
    
/*!
    This signal is emitted when an error has happened in interworking request
    handling. That is, e.g. the required slot could not be called or connection
    to server is lost. If the service itself return service specific errors,
    those should be returned as successfull return value of the slot
    (requestOk(const QVariant& result)).
    \param errorCode Error code as integer value
    \param errorMessage Contains needed R&D specific data that will be added to support error debugging
    \sa xqserviceglobal.h for error codes.
*/
    void requestError(int errorCode, const QString& errorMessage);

protected:
    
    virtual const QVariant& results() const;
    virtual bool sendExecute();
    
private slots:

   void handleAsyncResponse(const QVariant& value);
   void sendFromAction(bool checked);
   void handleAsyncError(int err);
    
private:

    XQAiwRequestDriver* currentRequest;
    QString errorMsg;
    int errorCode;
    QVariant result;  // Client can use results whenever wants
    QList<QAction*> actionList;
    bool completeSignalConnected;
    bool errorSignalConnected;
    
    };

#endif /* XQAIWREQUEST_H_ */ 
