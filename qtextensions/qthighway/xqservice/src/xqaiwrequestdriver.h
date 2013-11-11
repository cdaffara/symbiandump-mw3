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

#ifndef XQAIWREQUESTDRIVER_H
#define XQAIWREQUESTDRIVER_H

#include <f32file.h>
#include <xqaiwglobal.h>

#include <QObject>
#include <qglobal.h>
#include <QList>
#include <QVariant>
#include <QAction>
#include <xqaiwinterfacedescriptor.h>
#include <xqrequestinfo.h>

class XQAiwRequestDriver : public QObject
    {
    Q_OBJECT
    
public:

    XQAiwRequestDriver() :
            mEmbedded(false),
            mErrorCode(0),
            mSynchronous(false),
            mBackground(false)
    {};

    virtual ~XQAiwRequestDriver() {};
    
    virtual void setArguments(const QList<QVariant> &arguments) = 0;

    virtual bool send(QVariant& retValue) = 0;

    virtual QAction *createAction() = 0;

    virtual int lastError() const = 0;

    virtual const QString& lastErrorMessage() const = 0;

    virtual const XQAiwInterfaceDescriptor &descriptor() const = 0;

    virtual void setEmbedded(bool embedded) = 0;
    virtual bool isEmbedded() const = 0;

    virtual void setOperation(const QString &operation) = 0;
    virtual const QString &operation() const = 0;

    virtual void setSynchronous(bool synchronous)= 0;
    virtual bool isSynchronous() const = 0;
    
    virtual void setBackground(bool background )= 0;
    virtual bool isBackground() const = 0;

    virtual void setInfo(const XQRequestInfo &info) {mInfo = info;};
    virtual XQRequestInfo info() const {return mInfo;};
   
signals:

    void requestOk(const QVariant& result);
    void requestError(int errorCode, const QString& errorMessage);
  
protected:

    QString mOperation;
    bool mEmbedded;
    XQAiwInterfaceDescriptor mDescr;
    QString mErrorMsg;
    int mErrorCode;
    QList<QVariant> mArguments;
    bool mSynchronous;
    bool mBackground;
    XQRequestInfo mInfo;
    };

#endif /* XQAIWREQUESTDRIVER_H */ 
