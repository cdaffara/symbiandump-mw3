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
* Description: Platform wide application interworking declarations
*              All declations shall have XQ prefix
*
*/

#ifndef XQAIWDECLPLAT_H
#define XQAIWDECLPLAT_H

#include <QLatin1String>

//
// All declations shall have XQ prefix
//

/*!
 FM Radio controlling interface and related operations
 Operation: command( int commandId )
  commandId: defined in radioserviceconst.h
  
*/
#define XQI_RADIO_CONTROL QLatin1String("com.nokia.symbian.IRadioControl")
#define XQOP_RADIO_CONTROL QLatin1String("command(int)")

/*!
 FM Radio monitoring interface and related operations
*/
#define XQI_RADIO_MONITOR QLatin1String("com.nokia.symbian.IRadioMonitor")
#define XQOP_RADIO_MONITOR QLatin1String("requestNotifications(void)")

/*!
 WLAN sniffer interface and operations for
 the WLAN Sniffer list view. 
*/
#define XQI_WLAN_SNIFFER  QLatin1String("com.nokia.symbian.IWlanSniffer")
#define XQOP_WLAN_SNIFFER  QLatin1String("listView()")

/*!
 WLAN Login interface and operations for
 the WLAN Login start and stop. 
*/
#define XQI_WLAN_LOGIN  QLatin1String("com.nokia.symbian.IWlanLogin")
#define XQOP1_WLAN_LOGIN QLatin1String("start(int,int,QUrl)")
#define XQOP2_WLAN_LOGIN QLatin1String("stop()")

/*!
 Incomplete list of interfaces.
 To be completed with operation signatures and related declarations.
*/
#define XQI_EMAIL_INBOX_VIEW  QLatin1String("com.nokia.symbian.IEmailInboxView")
#define XQOP_EMAIL_INBOX_VIEW  QLatin1String("displayInboxByMailboxId(QVariant)")

#define XQI_EMAIL_MESSAGE_VIEW   QLatin1String("com.nokia.symbian.IEmailMessageView")
#define XQOP_EMAIL_MESSAGE_VIEW   QLatin1String("viewMessage(QVariant,QVariant)")

#define XQI_EMAIL_MESSAGE_SEND   QLatin1String("com.nokia.symbian.IEmailMessageSend")
#define XQOP_EMAIL_MESSAGE_SEND   QLatin1String("send(QVariant)")

#define XQI_EMAIL_REGISTER_ACCOUNT   QLatin1String("com.nokia.symbian.IEmailRegisterAccount")
#define XQOP_EMAIL_REGISTER_ACCOUNT   QLatin1String("registerNewMailbox(quint64,QString,QString)")
#define XQOP_EMAIL_UPDATE_MAILBOX   QLatin1String("updateMailboxName(quint64,QString)")
#define XQOP_EMAIL_UNREGISTER_MAILBOX   QLatin1String("unregisterMailbox(quint64)")

#define XQI_EMAIL_MAILBOX_CONFIGURE   QLatin1String("com.nokia.symbian.IEmailMailboxConfigure")
#define XQOP_EMAIL_MAILBOX_CONFIGURE   QLatin1String("launchWizard(QVariant)")

#define XQI_CALL_DIAL   QLatin1String("com.nokia.symbian.ICallDial")
#define XQI_DTMF_PLAY   QLatin1String("com.nokia.symbian.IDtmfPlay ")

// Platform service related constants

#endif
