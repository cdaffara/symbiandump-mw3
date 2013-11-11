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

#ifndef HBSERVICECLIENTVIEW_H
#define HBSERVICECLIENTVIEW_H

#include <hbview.h>

class HbAction;
class HbLineEdit;
class XQServiceRequest;
//class HbCheckBox;

class HbServiceClientView : public HbView
{
    Q_OBJECT

public:
    HbServiceClientView(QGraphicsItem *parent = 0);
    ~HbServiceClientView();

signals:
    
protected slots:
    void callContact();
    void callContactEmbedded();
    void showAddresses();
    void requestCompleted(const QVariant& value);
    void launchContactSelecting();
    void addSelectedRecipients(const QVariant &value);
    
private:
    void doCallContact(bool isEmbedded);
    HbLineEdit* mNumberEdit;
    HbLineEdit* mServiceEdit;
    HbLineEdit* mRetValue;

    HbLineEdit* mAddressEdit;
    HbLineEdit* mCityEdit;
    HbLineEdit* mCountryEdit;
    HbLineEdit* mServiceMapEdit;
    HbLineEdit* mRetMapValue;
    XQServiceRequest* sndAsync;
    XQServiceRequest* mSnd;
};

#endif // HBSERVICECLIENTVIEW_H
