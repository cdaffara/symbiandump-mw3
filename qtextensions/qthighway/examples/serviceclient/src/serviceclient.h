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

#ifndef SERVICECLIENT_H
#define SERVICECLIENT_H

#include <QWidget>
#include <QModelIndex>
#include <QMap>
#include <QStringList>

class QLineEdit;
class QCheckBox;
class QLabel;
class XQServiceRequest;
QT_BEGIN_HEADER
class ServiceClient : public QWidget
{
    Q_OBJECT
public:
    ServiceClient( QWidget *parent = 0, Qt::WFlags f = 0 );
    ~ServiceClient();

protected slots:
    void requestCompleted(const QVariant& value);
    void requestError(int err);
    
private slots:
    void dial();

private:
    
private:
    QCheckBox* mCheckSync;
    QCheckBox* mCheckAsyncAnswer;
    QCheckBox* mCheckDeleteRequest;
    QLineEdit *mTextEdit;
    QLineEdit *mTextRetValue;
    QLabel* mRetunSignal;
    XQServiceRequest* snd;
};


#endif
