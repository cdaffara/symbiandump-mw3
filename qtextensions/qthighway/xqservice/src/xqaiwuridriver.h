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

#ifndef XQAIWURIDRIVER_H
#define XQAIWURIDRIVER_H

#include <xqaiwglobal.h>

#include <QObject>
#include <qglobal.h>

#include <QList>
#include <QVariant>
#include <QAction>
#include <QUrl>
#include <xqaiwinterfacedescriptor.h>
#include "xqaiwrequestdriver.h"
#include "xqaiwutils.h"

class XQServiceRequest;

class XQAiwUriDriver : public XQAiwRequestDriver
    {
    Q_OBJECT

public:

    XQAiwUriDriver(
         const QUrl &uri, const XQAiwInterfaceDescriptor& descriptor, const QString &operation);
    
    virtual ~XQAiwUriDriver();
    
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

    static bool hasCustomHandler(const QUrl &uri);

    void setBackground(bool background );
    bool isBackground() const;
    
private slots:

   void handleAsyncResponse(const QVariant& value);
   void handleAsyncError(int err);
   
private:   
   bool handleApptoUri(const QUrl &url);
   
private:
    QUrl mUri;
    XQAiwUtils * mUtilities;

    };

#endif /* XQAIWURIDRIVER_H */ 
