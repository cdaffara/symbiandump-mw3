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

#ifndef XQSERVICECHANNEL_H
#define XQSERVICECHANNEL_H

#include <xqserviceglobal.h>
#include <QObject>
#include <QVariant>


class XQServiceChannelPrivate;
class XQSharableFile;

class XQServiceRequestCompletedAsync
{
public:
    virtual void requestCompletedAsync(const QVariant &retValue) = 0;
    virtual void requestErrorAsync(int err) = 0;
};

class XQSERVICE_EXPORT XQServiceChannel : public QObject
{
    Q_OBJECT
public:
    enum XQServiceCommand {
        ReturnValueDelivered = 1,
        ClientDisconnected
    };
    explicit XQServiceChannel(const QString& channel, bool isServer, QObject *parent=0);
    virtual ~XQServiceChannel();

    bool connectChannel();
    
    QString channel() const;

    //TODO: implement 2 separate apis sendSync sendAsync
    static bool send(const QString& channel, 
                     const QString& msg,
                     const QByteArray &data, QVariant &retValue, 
                     bool sync, XQServiceRequestCompletedAsync* rc = NULL);
    
    static bool send(const QString& channel, 
                     const QString& msg,
                     const QByteArray &data,
                     QVariant &retValue, 
                     bool sync,
                     XQServiceRequestCompletedAsync* rc,
                     const void *userData);  // Any caller given data

    static bool cancelPendingSend(const QString& channel);

    static QVariant sendLocally(const QString& ch, const QString& msg,
                                const QByteArray &data, const XQSharableFile &sf );
    
    static void sendCommand(const QString& ch,const XQServiceCommand cmd);
    static int latestError();
	
	
    virtual QVariant receive(const QString& msg, const QByteArray &data, const XQSharableFile &sf );
    virtual void commandReceive(const XQServiceCommand cmd);
    
Q_SIGNALS:
    void received(const QString& msg, const QByteArray &data, const XQSharableFile &sf);
    void commandReceived(const XQServiceCommand cmd);
    
protected:

private:
    XQServiceChannelPrivate* d;
};

#endif
