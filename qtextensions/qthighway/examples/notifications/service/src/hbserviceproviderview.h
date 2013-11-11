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

#ifndef HBSERVICEPROVIDERVIEW_H
#define HBSERVICEPROVIDERVIEW_H

#include <hbview.h>
#include <xqserviceprovider.h>

#include <QTimer>

#include "hbcontact.h"

class HbLineEdit;
class HbLabel;
class DialerService;
class XQServiceRequest;

class HbDialerView : public HbView
{
    Q_OBJECT

public:
    HbDialerView(DialerService* service,QGraphicsItem *parent = 0);
    ~HbDialerView();

signals:

public slots:
    void setNumber(const QString& number);
    void embed();
    void requestCompleted(const QVariant& value);
    
protected slots:
    void quit();
    
    void tick();
    
private:
    HbLineEdit* mTextEdit;
    HbLineEdit* mRetValue;
    HbLabel* timerLabel;
    DialerService* mService;
    XQServiceRequest* sndEmbedded;
    QTimer doomsdayTimer;
    int doomsdayCounter;
};

class DialerService : public XQServiceProvider
{
    Q_OBJECT
public:
    DialerService( QObject *parent = 0 );
    ~DialerService();

    void complete();

Q_SIGNALS:
    void setNumber(const QString& number);
    void showView(const QString& name);
    
public slots:
    QString dial(const QString& number);
    QString dial(const QString& number, bool async);
    
private:
    QString mNumber;
};

#endif // HBSERVICEPROVIDERVIEW_H
