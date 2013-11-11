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

#ifndef ASYNCTIMER_H
#define ASYNCTIMER_H

#include <QObject>
#include <QStringList>
#include <qtimer.h>

class AsyncTimer : public QTimer
{
    Q_OBJECT

public:
    AsyncTimer(int requestId, QString input):mRequestID(requestId), mInput(input)
        {connect(this, SIGNAL(timeout()), this, SLOT(handle_timeout()));}
    void setRequestID(int requestId){mRequestID = requestId;}
    
public slots:
    void handle_timeout(){emit timeout(mRequestID, mInput); delete this;}
    
signals:
    void timeout(int requestId, QString input);

private:
    int mRequestID; 
    QString mInput;
};

#endif // ASYNCTIMER_H
