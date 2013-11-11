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

#ifndef XQAIWSERVICEDRIVER_H
#define XQAIWSERVICEDRIVER_H

#include <xqaiwglobal.h>

#include <QObject>
#include <qglobal.h>
#include <qtranslator.h>

#include <QList>
#include <QVariant>
#include <QAction>
#include <xqaiwinterfacedescriptor.h>
#include "xqaiwrequestdriver.h"

class XQServiceRequest;

class XQAiwServiceDriver : public XQAiwRequestDriver
    {
    Q_OBJECT

public:

    XQAiwServiceDriver(const XQAiwInterfaceDescriptor& descriptor, const QString &operation);
    
    virtual ~XQAiwServiceDriver();
    
    void setArguments(const QList<QVariant> &arguments);

    bool send(QVariant& retValue);

    QAction *createAction();

    int lastError() const;

    const QString& lastErrorMessage() const;

    const XQAiwInterfaceDescriptor &descriptor() const;

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
    
private slots:

   void handleAsyncResponse(const QVariant& value);
   void handleAsyncError(int err);

private:
   bool installTranslator();
   void removeTranslator();
   QString makeFileName(const QString &textFile) const;
   
private:

    XQServiceRequest * currentRequest;
    bool completeSignalConnected;
    bool errorSignalConnected;
    QTranslator translator;
    QString lastLang;
    };

#endif /* XQAIWSERVICEDRIVER_H */ 
