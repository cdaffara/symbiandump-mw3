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

#include <QGraphicsLinearLayout>
#include <hbtoolbar.h>
#include <hbaction.h>
#include <hblineedit.h>
#include <hblabel.h>
//#include <hbcheckbox.h>
#include <hbmessagebox.h>
#include <cntservicescontact.h>


#include <QMessageBox>
#include <QVariant>

#include "hbserviceclientview.h"

#include <xqservicerequest.h>

#include "../hbserviceprovider/src/hbcontact.h"


HbServiceClientView::HbServiceClientView(QGraphicsItem *parent)
    : HbView(parent)
{
    XQSERVICE_DEBUG_PRINT("HbServiceClientView::HbServiceClientView");
    setTitle(tr("HB Service client view"));

    QGraphicsLinearLayout* layout = new QGraphicsLinearLayout(Qt::Vertical,this);

    layout->addItem(new HbLabel("Call service",this));
    mNumberEdit = new HbLineEdit(this);
    mNumberEdit->setText("+35850123456789");
    layout->addItem(mNumberEdit);

    mServiceEdit = new HbLineEdit(this);
    mServiceEdit->setText("com.nokia.services.hbserviceprovider.Dialer");
    layout->addItem(mServiceEdit);

    /*
    mServiceEmbedded = HbCheckBox("Embedded",this)
    mServiceEmbedded->setTristate(true);
    layout->addItem(mServiceEmbedded);
    */
    
    mRetValue = new HbLineEdit(this);
    mRetValue->setText("*****");
    layout->addItem(mRetValue);

    layout->addItem(new HbLabel("Map service",this));
    mAddressEdit = new HbLineEdit(this);
    mAddressEdit->setText("Karakaari 13");
    layout->addItem(mAddressEdit);

    mCityEdit = new HbLineEdit(this);
    mCityEdit->setText("Espoo");
    layout->addItem(mCityEdit);

    mCountryEdit = new HbLineEdit(this);
    mCountryEdit->setText("Finland");
    layout->addItem(mCountryEdit);

    mServiceMapEdit = new HbLineEdit(this);
    mServiceMapEdit->setText("com.nokia.services.hbserviceprovider.Map");
    layout->addItem(mServiceMapEdit);

    mRetMapValue = new HbLineEdit(this);
    mRetMapValue->setText("*****");
    layout->addItem(mRetMapValue);
    
    setLayout(layout);

    HbToolBar* toolBar = this->toolBar();
    
    HbAction* callAction = new HbAction("Call"); 
    connect(callAction, SIGNAL(triggered()), this, SLOT(callContact()));

    HbAction* callEmbeddedAction = new HbAction("Call Embedded"); 
    connect(callEmbeddedAction, SIGNAL(triggered()), this, SLOT(callContactEmbedded()));

    HbAction* showAddressesAction = new HbAction("Select contacts"); 
    connect(showAddressesAction, SIGNAL(triggered()), this, SLOT(launchContactSelecting()));
    
    toolBar->addAction(callAction);
    toolBar->addAction(callEmbeddedAction);
    toolBar->addAction(showAddressesAction);
    sndAsync = NULL;
}

HbServiceClientView::~HbServiceClientView()
{
    XQSERVICE_DEBUG_PRINT("HbServiceClientView::~HbServiceClientView");
}

void HbServiceClientView::callContact()
{
    XQSERVICE_DEBUG_PRINT("HbServiceClientView::callContact");
    doCallContact(false);
}

void HbServiceClientView::callContactEmbedded()
{
    XQSERVICE_DEBUG_PRINT("HbServiceClientView::callContactEmbedded");
    doCallContact(true);
}

void HbServiceClientView::doCallContact(bool isEmbedded)
{
    XQSERVICE_DEBUG_PRINT("HbServiceClientView::doCallContact");
    if (isEmbedded) {
        XQRequestInfo info;  // matti
        info.setEmbedded(isEmbedded);
        
        XQServiceRequest snd(mServiceEdit->text(),
                             "dial(QString)",isEmbedded);
        snd << mNumberEdit->text();
        snd.setInfo(info);   // matti
        
        QVariant retValue;
        
        bool res=snd.send(retValue);
        if (!res) {
            int returnvalue = snd.latestError();
            mRetValue->setText(QString::number(returnvalue));
           // mRetValue->setText("send fail!");
        }
        else {
            if (retValue.isNull() || !retValue.isValid())
                mRetValue->setText("ret value null or not valid");
            else    
                mRetValue->setText(retValue.toString());
        }
    }
    else {
        if (sndAsync)
            delete sndAsync;
        sndAsync = new XQServiceRequest(mServiceEdit->text(),
                           "dial(QString)",isEmbedded);
        connect(sndAsync, SIGNAL(requestCompleted(QVariant)), this, SLOT(requestCompleted(QVariant)));
        *sndAsync << mNumberEdit->text();
        bool res=sndAsync->send();
        if (!res) {
            int returnvalue = sndAsync->latestError();
            mRetValue->setText(QString::number(returnvalue));
        }
        else {
            mRetValue->setText("call on going...");
        }
    }
}

void HbServiceClientView::requestCompleted(const QVariant& val)
{
    XQSERVICE_DEBUG_PRINT("HbServiceClientView::requestCompleted");
    mRetValue->setText(val.toString());
}

void HbServiceClientView::showAddresses()
{
    XQSERVICE_DEBUG_PRINT("HbServiceClientView::showAddresses");
    XQServiceRequest snd(mServiceMapEdit->text(),"showAddresses(QString,QString,QString)");
    snd << mAddressEdit->text();
    snd << mCityEdit->text();
    snd << mCountryEdit->text();
    HbContactList retValue;
    bool res=snd.send(retValue);
    if (!res) {
        mRetMapValue->setText("send fail!");
    }
    else {
        QString data;
        int count = 0;
        foreach (HbContact cnt,retValue) {
            //HbContact cnt = qVariantValue<HbContact>(v);
            QString tmp = QString::number(++count) + " : " + cnt.mLabel + " " + cnt.mNumber + "\n";
            data.append(tmp) ;
        }
        HbMessageBox::information(data);
        // QMessageBox msgBox;
        // msgBox.setWindowTitle("Return value");
        // msgBox.setText(data);
        // msgBox.exec();
        //mRetMapValue->setText(QString::number(retValue));
    }
}

void HbServiceClientView::launchContactSelecting()
{
    if (sndAsync)
        delete sndAsync;
    sndAsync = new XQServiceRequest("com.nokia.services.phonebookservices.Fetch",
                                    "fetch(QString,QString,QString)", false);

    connect(sndAsync, SIGNAL(requestCompleted(QVariant)),
            this, SLOT(addSelectedRecipients(QVariant)));
    *sndAsync << "Select contact"; 
    *sndAsync << KCntActionAll;   
    *sndAsync << KCntFilterDisplayAll;

    bool result = sndAsync->send();
    if (!result) {
    }  

}

void HbServiceClientView::addSelectedRecipients(const QVariant &value)
{
    CntServicesContactList list;
    if(value.canConvert<CntServicesContactList>()) {
        list = qVariantValue<CntServicesContactList>(value);
    }
    else {
        ;
    }    

    if (list.count() == 0) {
        HbMessageBox::information(tr("Nothing returned"));
        // note.setTimeout(10000);
        // "Nothing returned" will be replaced by a hbTrId when it is ready
        // note.setText(tr("Nothing returned"));
        // note.information();
    }
    else {
        QString data;
        foreach (CntServicesContact cnt, list)
        {
            QString recipientName = cnt.mDisplayName;
            data += recipientName + "\n";
        }
        HbMessageBox::information(data);
        //HbMessageBox msgBox;
        //msgBox.setWindowTitle("Returned value");
        //msgBox.setText(data);
        //msgBox.information();
    }
}
    
Q_IMPLEMENT_USER_METATYPE(HbContact)
Q_IMPLEMENT_USER_METATYPE_NO_OPERATORS(HbContactList)

Q_IMPLEMENT_USER_METATYPE(CntServicesContact)
Q_IMPLEMENT_USER_METATYPE_NO_OPERATORS(CntServicesContactList)
