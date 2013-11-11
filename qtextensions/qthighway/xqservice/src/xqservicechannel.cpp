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

#include "xqservicechannel.h"

#include "xqservicethreaddata.h"
#include "xqserviceipcclient.h"

#include <xqserviceutil.h>
#include <xqsharablefile.h>



/*!
    \class XQServiceChannel
    \inpublicgroup QtBaseModule
    \ingroup qws

    \brief The XQServiceChannel class provides communication capabilities
    between clients.

    XQSERVICE is a many-to-many communication protocol for transferring
    messages on various channels. A channel is identified by a name,
    and anyone who wants to can listen to it. The XQSERVICE protocol allows
    clients to communicate both within the same address space and
    between different processes, but it is currently only available
    for \l {Qt for Embedded Linux} (on X11 and Windows we are exploring the use
    of existing standards such as DCOP and COM).

    Typically, XQServiceChannel is either used to send messages to a
    channel using the provided static functions, or to listen to the
    traffic on a channel by deriving from the class to take advantage
    of the provided functionality for receiving messages.

    XQServiceChannel provides a couple of static functions which are usable
    without an object: The send() function, which sends the given
    message and data on the specified channel, and the isRegistered()
    function which queries the server for the existence of the given
    channel.

    In addition, the XQServiceChannel class provides the channel() function
    which returns the name of the object's channel, the virtual
    receive() function which allows subclasses to process data
    received from their channel, and the received() signal which is
    emitted with the given message and data when a XQServiceChannel
    subclass receives a message from its channel.

    \sa XQServiceServer, {Running Qt for Embedded Linux Applications}
*/

/*!
    Constructs a XQService channel with the given \a parent, and registers it
    with the server using the given \a channel name.
    \param channel Channel name.
    \param isServer 
    \param parent Parent of this object.
    \sa isRegistered(), channel()
*/

XQServiceChannel::XQServiceChannel(const QString& channel, bool isServer, QObject *parent)
    : QObject(parent)
{
    XQSERVICE_DEBUG_PRINT("XQServiceChannel::XQServiceChannel");
    XQSERVICE_DEBUG_PRINT("channel: %s, isServer: %d", qPrintable(channel), isServer );
    d = new XQServiceChannelPrivate(this, channel, isServer);
    d->ref.ref();

    XQServiceThreadData *td = XQService::serviceThreadData();

    // do we need a new channel list ?
    XQServiceClientMap::Iterator it = td->clientMap.find(channel);
    if (it != td->clientMap.end()) {
        XQSERVICE_DEBUG_PRINT("Channel exits");
        it.value().append(XQServiceChannelPrivatePointer(d));
        return;
    }
    XQSERVICE_DEBUG_PRINT("New channel");
    it = td->clientMap.insert(channel, QList<XQServiceChannelPrivatePointer>());
    it.value().append(XQServiceChannelPrivatePointer(d));
}

/*!
    Destroys the client's end of the channel and notifies the server
    that the client has closed its connection. The server will keep
    the channel open until the last registered client detaches.

    \sa XQServiceChannel()
*/

XQServiceChannel::~XQServiceChannel()
{
    XQSERVICE_DEBUG_PRINT("XQServiceChannel::~XQServiceChannel");
    XQServiceThreadData *td = XQService::serviceThreadData();

    XQServiceClientMap::Iterator it = td->clientMap.find(d->channel);
    Q_ASSERT(it != td->clientMap.end());
    it.value().removeAll(XQServiceChannelPrivatePointer(d));
    // still any clients connected locally ?
    if (it.value().isEmpty()) {
        if (td->hasClientConnection(d->channel))
            td->closeClientConnection(d->channel);
        td->clientMap.remove(d->channel);
    }

    // Dereference the private data structure.  It may stay around
    // for a little while longer if it is in use by sendLocally().
    d->object = 0;
    if (!d->ref.deref())
        delete d;
}

bool XQServiceChannel::connectChannel()
{
    XQSERVICE_DEBUG_PRINT("XQServiceChannel::connectChannel");
    bool ret = true;

    XQServiceThreadData *td = XQService::serviceThreadData();
    
    if (!td->hasClientConnection(d->channel)) {
        XQSERVICE_DEBUG_PRINT("Create new client connection (1)");
        ret = td->createClientConnection(d->channel,d->server);
    }
    return ret;
}

/*!
    Returns the name of the channel.

    \sa XQServiceChannel()
*/

QString XQServiceChannel::channel() const
{
    XQSERVICE_DEBUG_PRINT("XQServiceChannel::channel");
    XQSERVICE_DEBUG_PRINT("channel: %s", qPrintable(d->channel));
    return d->channel;
}

/*!
    \fn void XQServiceChannel::receive(const QString& message, const QByteArray &data)

    This virtual function allows subclasses of XQServiceChannel to process
    the given \a message and \a data received from their channel. The default
    implementation emits the received() signal.

    Note that the format of the given \a data has to be well defined
    in order to extract the information it contains. In addition, it
    is recommended to use the DCOP convention. This is not a
    requirement, but you must ensure that the sender and receiver
    agree on the argument types.

    Example:

    \code
        void MyClass::receive(const QString &message, const QByteArray &data)
        {
            QDataStream in(data);
            if (message == "execute(QString,QString)") {
                QString cmd;
                QString arg;
                in >> cmd >> arg;
                ...
            } else if (message == "delete(QString)") {
                QString fileName;
                in >> fileName;
                ...
            } else {
                ...
            }
        }
    \endcode

    This example assumes that the \c message is a DCOP-style function
    signature and the \c data contains the function's arguments.

    \sa send()
 */
QVariant XQServiceChannel::receive(const QString& msg, const QByteArray &data, const XQSharableFile &sf )
{
    XQSERVICE_DEBUG_PRINT("XQServiceChannel::receive");
    XQSERVICE_DEBUG_PRINT("msg: %s, data: %s", qPrintable(msg), data.constData());
    emit received(msg, data,sf);
    return QVariant();
}

void XQServiceChannel::commandReceive(const XQServiceCommand cmd)
{
    XQSERVICE_DEBUG_PRINT("XQServiceChannel::commandReceive %d", cmd);
    emit commandReceived(cmd);
}

/*!
    \fn void XQServiceChannel::received(const QString& message, const QByteArray &data)

    This signal is emitted with the given \a message and \a data whenever the
    receive() function gets incoming data.

    \sa receive()
*/

/*!
    \fn bool XQServiceChannel::send(const QString& channel, const QString& message,
                       const QByteArray &data)

    Sends the given \a message on the specified \a channel with the
    given \a data.  The message will be distributed to all clients
    subscribed to the channel. Returns true if the message is sent
    successfully; otherwise returns false.

    It is recommended to use the DCOP convention. This is not a
    requirement, but you must ensure that the sender and receiver
    agree on the argument types.

    Note that QDataStream provides a convenient way to fill the byte
    array with auxiliary data.

    Example:

    \code
        QByteArray data;
        QDataStream out(&data, QIODevice::WriteOnly);
        out << QString("cat") << QString("file.txt");
        XQServiceChannel::send("System/Shell", "execute(QString,QString)", data);
    \endcode

    Here the channel is \c "System/Shell". The \c message is an
    arbitrary string, but in the example we've used the DCOP
    convention of passing a function signature. Such a signature is
    formatted as \c "functionname(types)" where \c types is a list of
    zero or more comma-separated type names, with no whitespace, no
    consts and no pointer or reference marks, i.e. no "*" or "&".

    \sa receive()
*/

bool XQServiceChannel::send(const QString& channel, const QString& msg,
                            const QByteArray &data, QVariant &retValue,
                            bool sync, XQServiceRequestCompletedAsync* rc)
{
    XQSERVICE_DEBUG_PRINT("XQServiceChannel::send(1). No user data");
    // Delegate to actual send.
    // No user data argument present in this version
    return send(channel,msg,data,retValue,sync,rc,NULL);
}



bool XQServiceChannel::send(const QString& channel, const QString& msg,
                       const QByteArray &data, QVariant &retValue,
                       bool sync, XQServiceRequestCompletedAsync* rc,
                       const void* userData)
{
    XQSERVICE_DEBUG_PRINT("XQServiceChannel::send(2) start");
    XQSERVICE_DEBUG_PRINT("\t channel: %s, msg: %s", qPrintable(channel), qPrintable(msg));
    XQSERVICE_DEBUG_PRINT("\t data: %s, sync: %d", data.constData(), sync);
    bool ret=true;
    
    if (!XQService::serviceThreadData()->hasClientConnection(channel)) {
        XQSERVICE_DEBUG_PRINT("\t Create new client connection (2)");
        ret = XQService::serviceThreadData()->createClientConnection(channel,false,sync,rc, userData);
        XQSERVICE_DEBUG_PRINT("\t creation succeeded: %d", ret);
    }
    if (ret) {
        XQSERVICE_DEBUG_PRINT("\t ret = true");
        XQServiceIpcClient *cl = XQService::serviceThreadData()->clientConnection(channel);
        QByteArray retData ;
        ret = cl ? cl->send(channel, msg, data, retData) : false;
        if (sync) {
            retValue = XQServiceThreadData::deserializeRetData(retData);
        }
    }
    XQSERVICE_DEBUG_PRINT("\t ret: %d", ret);
	XQSERVICE_DEBUG_PRINT("XQServiceChannel::send(2) end");
    return ret;
}


bool XQServiceChannel::cancelPendingSend(const QString& channel)
{
    XQSERVICE_DEBUG_PRINT("XQServiceChannel::cancelPendingSend start");
    XQSERVICE_DEBUG_PRINT("\t channel: %s", qPrintable(channel));
    bool ret=true;
    
    if (ret) {
        XQServiceIpcClient *cl = XQService::serviceThreadData()->clientConnection(channel);
        XQSERVICE_DEBUG_PRINT("\t XQService::serviceThreadData()->clientConnection(channel): %d", cl);
        XQSERVICE_DEBUG_PRINT("\t cl->cancelPendingSend(%s)", qPrintable(channel));
        ret = cl ? cl->cancelPendingSend(channel) : false;
    }
    
    XQSERVICE_DEBUG_PRINT("\t ret: %d", ret);
	XQSERVICE_DEBUG_PRINT("XQServiceChannel::cancelPendingSend end");
    return ret;
}

/*!
    \internal
    Client side: distribute received event to the XQService instance managing the
    channel.
*/
QVariant XQServiceChannel::sendLocally(const QString& ch, const QString& msg,
                                const QByteArray &data, const XQSharableFile &sf )
{
    XQSERVICE_DEBUG_PRINT("XQServiceChannel::sendLocally");
    XQSERVICE_DEBUG_PRINT("channel: %s, msg: %s", qPrintable(ch), qPrintable(msg));
    XQSERVICE_DEBUG_PRINT("data: %s", data.constData());
    QVariant ret;
    
    // feed local clients with received data
    XQServiceThreadData *td = XQService::serviceThreadData();
    QList<XQServiceChannelPrivatePointer> clients = td->clientMap[ch];
    for (int i = 0; i < clients.size(); ++i) {
        XQServiceChannelPrivate *channel = clients.at(i).data();
        if (channel->object)
            ret = channel->object->receive(msg, data,sf );
    }

#ifdef XQSERVICE_DEBUG
    QString s = ret.toString();
    int len=s.length();
    XQSERVICE_DEBUG_PRINT("sendLocally ret: type=%s,len=%d,value(max.1024)=%s",
                          ret.typeName(),len,qPrintable(s.left(1024)));
#endif
    return ret ;
}

int XQServiceChannel::latestError()
{
    XQSERVICE_DEBUG_PRINT("XQServiceChannel::latestError");
    return XQService::serviceThreadData()->latestError();
}

void XQServiceChannel::sendCommand(const QString& ch,const XQServiceCommand cmd)
{
    XQSERVICE_DEBUG_PRINT("XQServiceChannel::sendCommand");
    XQSERVICE_DEBUG_PRINT("channel: %s, cmd: %d", qPrintable(ch), cmd);
    // feed local clients with received data
    XQServiceThreadData *td = XQService::serviceThreadData();
    QList<XQServiceChannelPrivatePointer> clients = td->clientMap[ch];
    for (int i = 0; i < clients.size(); ++i) {
        XQServiceChannelPrivate *channel = clients.at(i).data();
        if (channel->object)
            channel->object->commandReceive(cmd);
    }
}

