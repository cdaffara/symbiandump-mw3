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

#ifndef XQSERVICEADAPTOR_H
#define XQSERVICEADAPTOR_H

#include <xqserviceglobal.h>

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QVariant>
#include <QStringList>

#include <xqrequestinfo.h>

class XQServiceAdaptorPrivate;
class XQServiceRequestCompletedAsync;
class XQSharableFile;

class XQSERVICE_EXPORT XQServiceAdaptor : public QObject
{
    Q_OBJECT
    friend class XQServiceAdaptorPrivate;
    friend class XQServiceAdaptorChannel;
    friend class XQServiceProvider;
    
public:
    explicit XQServiceAdaptor(const QString& channel, QObject *parent = 0);
    virtual ~XQServiceAdaptor();

    QString channel() const;

    static bool connect(QObject *sender, const QByteArray& signal,
                        QObject *receiver, const QByteArray& member);

    bool send(const QByteArray& member, const QVariant &arg1, QVariant &retData);
    bool send(const QByteArray& member, const QVariant &arg1,
              const QVariant &arg2, QVariant &retData);
    bool send(const QByteArray& member, const QVariant &arg1,
              const QVariant &arg2, const QVariant &arg3, QVariant &retData);
    bool send(const QByteArray& member, const QList<QVariant>& args, QVariant &retData);

    bool isConnected(const QByteArray& signal);

    //todo: proposed to change in channel in 2 separate api sendSync sendAsync
    static bool send(const QString& channel,
                     const QString& msg, 
                     const QList<QVariant>& args, 
                     QVariant& retValue, 
                     bool sync = true,
                     XQServiceRequestCompletedAsync* rc = NULL);

    static bool send(const QString& channel,
                     const QString& msg, 
                     const QList<QVariant>& args, 
                     QVariant& retValue, 
                     bool sync,
                     XQServiceRequestCompletedAsync* rc,
                     const void *userData);
    
    static bool cancelPendingSend(const QString& channel);

    static int latestError();

    int setCurrentRequestAsync();

    bool completeRequest(int index, const QVariant& retValue);

    XQRequestInfo requestInfo() const;
    
 Q_SIGNALS:
    void returnValueDelivered() ;
    void clientDisconnected() ;
        
protected:
    enum PublishType
    {
        Signals,
        Slots,
        SignalsAndSlots
    };

    bool publish(const QByteArray& member);
    void publishAll(XQServiceAdaptor::PublishType type);
    virtual QString memberToMessage(const QByteArray& member);

private slots:
    void receiverDestroyed();

private:
    QVariant received(const QString& msg, const QByteArray& data,const XQSharableFile &sf);

    bool connectLocalToRemote(QObject *sender, const QByteArray& signal,
                              const QByteArray& member);
    bool connectRemoteToLocal(const QByteArray& signal, QObject *receiver,
                              const QByteArray& member);
    bool sendMessage(const QString& msg, const QList<QVariant>& args, QVariant &retData);
    
    void publishAll(QObject* object, int metCount, XQServiceAdaptor::PublishType type);

private:
    XQServiceAdaptorPrivate *d;
};

// Useful alias to make it clearer when connecting to messages on a channel.
#define MESSAGE(x)      "3"#x
#define QMESSAGE_CODE   3

#endif
