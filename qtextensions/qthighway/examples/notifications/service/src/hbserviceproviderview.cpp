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
#include <hblabel.h>
#include <QTimer>
#include <QGraphicsLinearLayout>

#include <hbserviceproviderview.h>
#include <xqserviceprovider.h>
#include <xqservicerequest.h>
#include <xqserviceutil.h>

HbDialerView::HbDialerView(DialerService* service,QGraphicsItem *parent)
    : mService(service),
      HbView(parent),
      doomsdayTimer(),
      doomsdayCounter(5)
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
    timerLabel = new HbLabel("...");
    layout->addItem(mTextEdit);
    layout->addItem(mRetValue);
    layout->addItem(timerLabel);
    setLayout(layout);
    bool con = connect(&doomsdayTimer, SIGNAL(timeout()), this, SLOT(tick()));
    //doomsdayTimer.start(1000);
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

void HbDialerView::tick()
{
    if (doomsdayCounter>1) {
        doomsdayCounter--;
        timerLabel->setPlainText(QString("%1").arg(doomsdayCounter));
    } else if (doomsdayCounter == 1) {
        quit();
    } else {
        doomsdayTimer.stop();
        qApp->quit();
    }
}

void HbDialerView::embed()
{
    XQSERVICE_DEBUG_PRINT("HbDialerView::embed");
    if (sndEmbedded)
        delete sndEmbedded;
    
    //XQServiceRequest snd("com.nokia.services.hbserviceprovider.Dialer","dial(QString)",true);
    sndEmbedded = new XQServiceRequest("serviceapp.Dialer","dial(QString,bool)",true);
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

Q_IMPLEMENT_USER_METATYPE(HbContact)
Q_IMPLEMENT_USER_METATYPE_NO_OPERATORS(HbContactList)
 
