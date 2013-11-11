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

#include "xqservicelog.h"

#include <hbapplication.h>
#include <hbtoolbar.h>
#include <hbaction.h>
#include <HbLineEdit.h>
#include <QTimer>
#include <QGraphicsLinearLayout>

#include <hbserviceproviderview.h>
#include <xqserviceprovider.h>
#include <xqservicerequest.h>
#include <xqserviceutil.h>

HbDialerView::HbDialerView(DialerService* service,QGraphicsItem *parent)
    : mService(service),HbView(parent)

{
    XQSERVICE_DEBUG_PRINT("HbDialerView::HbDialerView");
    sndEmbedded = NULL;
    if (XQServiceUtil::isService())
    {
        setTitle(tr("HB Dialer As Service"));
    }
    else
    {
        setTitle(tr("HB Dialer As Non-Service"));
    }

    HbToolBar* toolBar = this->toolBar();
    
    HbAction* endAction = new HbAction("End Call"); 
    connect(endAction, SIGNAL(triggered()), this, SLOT(quit()));
    
    toolBar->addAction(endAction);

    if (XQServiceUtil::isEmbedded()) {
        HbAction* embeddedAction = new HbAction("Embed"); 
        connect(embeddedAction, SIGNAL(triggered()), this, SLOT(embed()));
        toolBar->addAction(embeddedAction);
    }
    
    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Vertical,this);

    mTextEdit = new HbLineEdit(this);
    mRetValue = new HbLineEdit(this);
    layout->addItem(mTextEdit);
    layout->addItem(mRetValue);
    setLayout(layout);    
}

HbDialerView::~HbDialerView()
{
    XQSERVICE_DEBUG_PRINT("HbDialerView::~HbDialerView");
}

void HbDialerView::quit()
{
    XQSERVICE_DEBUG_PRINT("HbDialerView::quit");
    connect(mService, SIGNAL(returnValueDelivered()), qApp, SLOT(quit()));
    mService->complete();
}

void HbDialerView::embed()
{
    XQSERVICE_DEBUG_PRINT("HbDialerView::embed");
    if (sndEmbedded)
        delete sndEmbedded;
    
    //XQServiceRequest snd("com.nokia.services.hbserviceprovider.Dialer","dial(QString)",true);
    sndEmbedded = new XQServiceRequest("com.nokia.services.serviceapp.Dialer","dial(QString,bool)",true);
    connect(sndEmbedded, SIGNAL(requestCompleted(QVariant)), this, SLOT(requestCompleted(QVariant)));
    *sndEmbedded << mTextEdit->text();
    *sndEmbedded << true;
    QVariant retValue;
    bool res=sndEmbedded->send();
    if (!res) {
        mRetValue->setText("send fail!");
    }
}

void HbDialerView::requestCompleted(const QVariant& value)
{
    XQSERVICE_DEBUG_PRINT("HbDialerView::requestCompleted");
    mRetValue->setText(value.toString());
}

void HbDialerView::setNumber(const QString& number)
{
    XQSERVICE_DEBUG_PRINT("HbDialerView::setNumber");
    mTextEdit->setText("Call from " + number);
}

DialerService::DialerService(QObject* parent)
: XQServiceProvider("com.nokia.services.hbserviceprovider.Dialer",parent)
{
    XQSERVICE_DEBUG_PRINT("DialerService::DialerService");
    publishAll();
}

DialerService::~DialerService()
{
    XQSERVICE_DEBUG_PRINT("DialerService::~DialerService");
}

void DialerService::complete()
{
    XQSERVICE_DEBUG_PRINT("DialerService::complete");
    QString retvalue =  "retValue="+mNumber;
    completeRequest(1,retvalue);
}

QString DialerService::dial(const QString& number)
{
    XQSERVICE_DEBUG_PRINT("DialerService::dial");
    mNumber = number ;
    emit showView("dialer");
    emit setNumber(number);
    setCurrentRequestAsync();
    return "retValue="+number;
}

QString DialerService::dial(const QString& number, bool async)
{
    XQSERVICE_DEBUG_PRINT("DialerService::dial (2)");
    return dial(number);
}


HbMapView::HbMapView(MapService* service, QGraphicsItem *parent)
    : mMapService(service), HbView(parent) 

{
    XQSERVICE_DEBUG_PRINT("HbMapView::HbMapView");
    setTitle(tr("HB Map Service"));
    
    HbToolBar* toolBar = this->toolBar();
    
    HbAction* endAction = new HbAction("Done"); 
    connect(endAction, SIGNAL(triggered()), this, SLOT(quit()));

    HbAction* addAction = new HbAction("Add"); 
    connect(addAction, SIGNAL(triggered()), this, SLOT(addAddress()));

    HbAction* embedAction = new HbAction("Embed");
    connect(embedAction, SIGNAL(triggered()), this, SLOT(embed()));

    toolBar->addAction(endAction);
    toolBar->addAction(addAction);
    toolBar->addAction(embedAction);
    
    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Vertical,this);

    mTextAddress = new HbLineEdit(this);
    mTextCity = new HbLineEdit(this);
    mTextCountry = new HbLineEdit(this);
    mTextLabel = new HbLineEdit("stefano pironato", this);
    mTextNumber = new HbLineEdit("+358504876679", this);
    mRetValue = new HbLineEdit("******", this);

    layout->addItem(mTextLabel);
    layout->addItem(mTextNumber);
    layout->addItem(mTextAddress);
    layout->addItem(mTextCity);
    layout->addItem(mTextCountry);
    layout->addItem(mRetValue);
    setLayout(layout);
}

HbMapView::~HbMapView()
{
    XQSERVICE_DEBUG_PRINT("HbMapView::~HbMapView");
}

void HbMapView::quit()
{
    XQSERVICE_DEBUG_PRINT("HbMapView::quit");
    mMapService->complete();
    connect(mMapService, SIGNAL(returnValueDelivered()), qApp, SLOT(quit()));
//  NO quit now... we have to wait the signal that data has been delivered
//    qApp->quit();
}

void HbMapView::embed()
{
    XQSERVICE_DEBUG_PRINT("HbMapView::embed");
    //XQServiceRequest snd("com.nokia.services.hbserviceprovider.Dialer","dial(QString)",true);
    XQServiceRequest snd("com.nokia.services.serviceapp.Dialer","dial(QString,bool)",true);
    snd << mTextNumber->text();
    snd << true;
    QVariant retValue;
    bool res=snd.send(retValue);
    if (!res) {
        mRetValue->setText("send fail!");
    }
    else {
        if (retValue.isNull() || !retValue.isValid())
            mRetValue->setText("ret value null or not valid");
        else
            mRetValue->setText(retValue.toString());
    }
}

void HbMapView::setAddress(const QString& address,const QString& city,const QString& country)
{
    XQSERVICE_DEBUG_PRINT("HbMapView::setAddress");
    mTextAddress->setText("Address:" + address);
    mTextCity->setText("City:" + city);
    mTextCountry->setText("Country:" + country);
}

void HbMapView::addAddress()
{
    XQSERVICE_DEBUG_PRINT("HbMapView::addAddress");
    HbContact cnt;
    cnt.mLabel = mTextLabel->text();
    cnt.mNumber = mTextNumber->text();
    cnt.mAddress = mTextAddress->text();
    cnt.mCity = mTextCity->text();
    cnt.mCountry = mTextCountry->text();
    cnt.mUid = QUuid::createUuid();
    mMapService->addContact(cnt);
}

MapService::MapService(QObject* parent)
: XQServiceProvider(QLatin1String("com.nokia.services.hbserviceprovider.Map"),parent)
{
    XQSERVICE_DEBUG_PRINT("MapService::MapService");
    publishAll();
    mCurrentRequestIndex = 0;
}

MapService::~MapService()
{
    XQSERVICE_DEBUG_PRINT("MapService::~MapService");
}

void MapService::complete()
{
    XQSERVICE_DEBUG_PRINT("MapService::complete");
//    QVariant retValue;
//    retValue.setValue(contactList);
//    completeRequest(mCurrentRequestIndex, retValue);
    completeRequest(mCurrentRequestIndex, contactList);
    mCurrentRequestIndex = 0;
}

void MapService::addContact(const HbContact& cnt)
{
    XQSERVICE_DEBUG_PRINT("MapService::addContact");
    //QVariant v;
    //v.setValue(cnt);
    contactList.append(cnt);
}

HbContactList MapService::showAddresses(const QString& address,const QString& city,const QString& country)
{
    XQSERVICE_DEBUG_PRINT("MapService::showAddresses");
    emit showView("map");
    emit setAddress(address,city,country);
    mCurrentRequestIndex=setCurrentRequestAsync();
    HbContactList list;
    return list;
}


Q_IMPLEMENT_USER_METATYPE(HbContact)
Q_IMPLEMENT_USER_METATYPE_NO_OPERATORS(HbContactList)
 
