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

#include "hbcontact.h"

class HbLineEdit;
class MapService;
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
    
private:
    HbLineEdit* mTextEdit;
    HbLineEdit* mRetValue;
    DialerService* mService;
    XQServiceRequest* sndEmbedded;
};

class HbMapView : public HbView
{
    Q_OBJECT

public:
    HbMapView(MapService * service, QGraphicsItem *parent = 0);
    ~HbMapView();

signals:

public slots:
    void addAddress();
    void setAddress(const QString& address,const QString& city,const QString& country);
       
protected slots:
    void quit();
    void embed();

private:
    HbLineEdit* mTextAddress;
    HbLineEdit* mTextCity;
    HbLineEdit* mTextCountry;
    HbLineEdit* mTextLabel;
    HbLineEdit* mTextNumber;
    HbLineEdit* mRetValue;

    MapService* mMapService ;
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

class MapService : public XQServiceProvider
{
    Q_OBJECT
public:
    MapService( QObject *parent = 0 );
    ~MapService();

    void complete();    
    void addContact(const HbContact& cnt);

Q_SIGNALS:    
    void setAddress(const QString& address,const QString& city,const QString& country);
    void showView(const QString& name);
    
public slots:
    HbContactList showAddresses(const QString& address,const QString& city,const QString& country);

private:
    HbContactList contactList;    
    QString mNumber;
    int mCurrentRequestIndex;
};

#endif // HBSERVICEPROVIDERVIEW_H
