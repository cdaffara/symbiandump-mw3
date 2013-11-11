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

#include "xqserviceglobal.h"
#include "xqserviceadaptor.h"
#include "xqservicechannel.h"
#include "xqservicethreaddata.h"
#include "xqserviceipcclient.h"

#include <xqserviceutil.h>
#include <xqrequestutil.h>
#include <qmetaobject.h>
#include <QMap>
#include <QDataStream>
#include <qatomic.h>
#include <QDebug>
#include <QVarLengthArray>
#include <e32err.h>
#include <xqsharablefile.h>

/*!
    \class XQServiceAdaptor
    \inpublicgroup QtBaseModule

    \ingroup ipc
    \brief The XQServiceAdaptor class provides an interface to messages on a XQService IPC channel which simplifies remote signal and slot invocations.

    Using this class, it is very easy to convert a signal emission into an IPC
    message on a channel, and to convert an IPC message on a channel into a
    slot invocation.  In the following example, when the signal \c{valueChanged(int)}
    is emitted from the object \c source, the IPC message \c{changeValue(int)} will
    be sent on the channel \c{QPE/Foo}:

    \code
    XQServiceAdaptor *adaptor = new XQServiceAdaptor("QPE/Foo");
    XQServiceAdaptor::connect
        (source, SIGNAL(valueChanged(int)), adaptor, MESSAGE(changeValue(int)));
    \endcode

    Note that we use XQServiceAdaptor::connect() to connect the signal to the
    IPC message, not QObject::connect().  A common error is to use \c{connect()}
    without qualifying it with \c{XQServiceAdaptor::} and picking up
    QObject::connect() by mistake.

    On the server side of an IPC protocol, the \c{changeValue(int)} message can
    be connected to the slot \c{setValue()} on \c dest:

    \code
    XQServiceAdaptor *adaptor = new XQServiceAdaptor("QPE/Foo");
    XQServiceAdaptor::connect
        (adaptor, MESSAGE(changeValue(int)), dest, SLOT(setValue(int)));
    \endcode

    Now, whenever the client emits the \c{valueChanged(int)} signal, the
    \c{setValue(int)} slot will be automatically invoked on the server side,
    with the \c int parameter passed as its argument.

    Only certain parameter types can be passed across an IPC boundary in this fashion.
    The type must be visible to QVariant as a meta-type.  Many simple built-in
    types are already visible; for user-defined types, use Q_DECLARE_METATYPE()
    and qRegisterMetaTypeStreamOperators().

    \sa XQServiceChannel
*/

class XQServiceAdaptorChannel : public XQServiceChannel
{
    Q_OBJECT
public:
    XQServiceAdaptorChannel(const QString& channel, XQServiceAdaptor *adapt)
        : XQServiceChannel(channel, true, adapt), adaptor(adapt) 
    {
        XQSERVICE_DEBUG_PRINT("XQServiceAdaptorChannel::XQServiceAdaptorChannel");
    }
    
    ~XQServiceAdaptorChannel() 
    {
        XQSERVICE_DEBUG_PRINT("XQServiceAdaptorChannel::~XQServiceAdaptorChannel");
    }

    QVariant receive(const QString& msg, const QByteArray &data, const XQSharableFile &sf);
    void commandReceive(const XQServiceCommand cmd);

private slots:
    
private:
    XQServiceAdaptor *adaptor;
};

QVariant XQServiceAdaptorChannel::receive(const QString& msg, const QByteArray &data, const XQSharableFile &sf)
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptorChannel::receive");
    return adaptor->received(msg, data, sf);
}

void XQServiceAdaptorChannel::commandReceive(const XQServiceCommand cmd)
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptorChannel::commandReceive %d", cmd);
    switch (cmd)
    {
        case ReturnValueDelivered:
            emit adaptor->returnValueDelivered() ;
            break;
            
        case ClientDisconnected:
            emit adaptor->clientDisconnected() ;
            break;
            
        default:
            XQSERVICE_DEBUG_PRINT("XQServiceAdaptorChannel::unhandled command %d", cmd);
        break;
    }
}

class XQServiceAdaptorSignalInfo
{
public:
    QObject *sender;
    int signalIndex;
    int destroyIndex;
    QString message;
    int *types;
    int numArgs;
};

class XQServiceAdaptorSlotInfo
{
public:
    ~XQServiceAdaptorSlotInfo()
    {
        XQSERVICE_DEBUG_PRINT("XQServiceAdaptorSlotInfo::~XQServiceAdaptorSlotInfo");
        qFree(types);
    }

    QObject *receiver;
    int memberIndex;
    bool destroyed;
    int returnType;
    int *types;
    int numArgs;
};

class XQServiceAdaptorPrivate : public QObject
{
    // Do not put Q_OBJECT here.
public:
    XQServiceAdaptorPrivate(XQServiceAdaptor *obj, const QString& chan);
    ~XQServiceAdaptorPrivate();

    QAtomicInt ref;
    XQServiceAdaptor *parent;
    QString channelName;
    bool connected;
    const QMetaObject *publishedTo;
    QMultiMap<QString, XQServiceAdaptorSlotInfo *> invokers;
    QList<XQServiceAdaptorSignalInfo *> signalList;
    int slotIndex;

    static const int QVariantId = -243;
    static int *connectionTypes(const QByteArray& member, int& nargs);
    static int typeFromName(const QByteArray& name);

protected:
    int qt_metacall(QMetaObject::Call c, int id, void **a);
};

XQServiceAdaptorPrivate::XQServiceAdaptorPrivate(XQServiceAdaptor *obj, const QString& chan)
    : ref(1), channelName(chan)
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptorPrivate::XQServiceAdaptorPrivate");
    parent = obj;
    connected = false;
    publishedTo = 0;

    // Fake slots start at this index in the QMetaObject.
    slotIndex = staticMetaObject.methodCount();
}

XQServiceAdaptorPrivate::~XQServiceAdaptorPrivate()
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptorPrivate::~XQServiceAdaptorPrivate");
    qDeleteAll(invokers);

    // Disconnect all of the signals associated with this adaptor.
    int index = slotIndex;
    XQSERVICE_DEBUG_PRINT("index: %d", index);
    foreach (XQServiceAdaptorSignalInfo *info, signalList) {
        XQSERVICE_DEBUG_PRINT("info->signalIndex: %d", info->signalIndex);
        if (info->signalIndex >= 0) {
            QMetaObject::disconnect(info->sender, info->signalIndex,
                                    this, index);
        }
        XQSERVICE_DEBUG_PRINT("info->destroyIndex: %d", info->destroyIndex);
        if (info->destroyIndex >= 0) {
            QMetaObject::disconnect(info->sender, info->destroyIndex,
                                    this, index + 1);
        }
        qFree(info->types);
        delete info;
        index += 2;
    }
}

// Get the QVariant type number for a type name.
int XQServiceAdaptorPrivate::typeFromName( const QByteArray& type )
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptorPrivate::typeFromName");
    XQSERVICE_DEBUG_PRINT("type: %s", type.constData());
    if (type.endsWith('*')){
        XQSERVICE_DEBUG_PRINT("Pointer");
        return QMetaType::VoidStar;
    }
    else if ( type.size() == 0 || type == "void" ){
        XQSERVICE_DEBUG_PRINT("Void");
        return QMetaType::Void;
    }
    else if ( type == "QVariant" ){
        XQSERVICE_DEBUG_PRINT("QVariant");
        return XQServiceAdaptorPrivate::QVariantId;
    }
    int id = QMetaType::type( type.constData() );
    XQSERVICE_DEBUG_PRINT("id: %d", id);
    if ( id != (int)QMetaType::Void )
        return id;
    return QVariant::nameToType(type);
}

// Returns the connection types associated with a signal or slot member.
int *XQServiceAdaptorPrivate::connectionTypes( const QByteArray& member, int& nargs )
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptorPrivate::connectionTypes");
    XQSERVICE_DEBUG_PRINT("member: %s", member.constData());
    // Based on Qt's internal queuedConnectionTypes function.
    nargs = 0;
    int *types = 0;
    const char *s = member.constData();
    while (*s != '\0' && *s != '(') { ++s; }
    if ( *s == '\0' )
        return 0;
    ++s;
    const char *e = s;
    while (*e != ')') {
        ++e;
        if (*e == ')' || *e == ',')
            ++nargs;
    }

    types = (int *) qMalloc((nargs+1)*sizeof(int));
    types[nargs] = 0;
    for (int n = 0; n < nargs; ++n) {
        e = s;
        while (*s != ',' && *s != ')')
            ++s;
        QByteArray type(e, s-e);
        ++s;

        types[n] = typeFromName(type);
        if (!types[n]) {
            XQSERVICE_WARNING_PRINT("XQServiceAdaptorPrivate::connectionTypes: Cannot marshal arguments of type '%s'", type.constData());
            qFree(types);
            return 0;
        }
    }
    XQSERVICE_DEBUG_PRINT("types: %d", types);
    return types;
}

int XQServiceAdaptorPrivate::qt_metacall(QMetaObject::Call c, int id, void **a)
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptorPrivate::qt_metacall");
    id = QObject::qt_metacall(c, id, a);
    XQSERVICE_DEBUG_PRINT("id: %d", id);
    if (id < 0)
        return id;
    if (c == QMetaObject::InvokeMetaMethod) {
        XQSERVICE_DEBUG_PRINT("InvokeMetaMethod");
        // Each signal that we have intercepted has two fake slots
        // associated with it.  The first is the activation slot.
        // The second is the destroyed() slot for the signal's object.
        if (id < signalList.size() * 2) {
            XQServiceAdaptorSignalInfo *info = signalList[id / 2];
            if ((id % 2) == 0) {
                // The signal we are interested in has been activated.
                if (info->types) {
                    XQSERVICE_DEBUG_PRINT("Signal has been activated");
                    QList<QVariant> args;
                    XQSERVICE_DEBUG_PRINT("info->numArgs: %d", info->numArgs);
                    for (int i = 0; i < info->numArgs; ++i) {
                        if (info->types[i] != XQServiceAdaptorPrivate::QVariantId) {
                            QVariant arg(info->types[i], a[i + 1]);
                            XQSERVICE_DEBUG_PRINT("arg: %s", qPrintable(arg.toString()));
                            args.append(arg);
                        } else {
                            args.append(*((const QVariant *)(a[i + 1])));
                        }
                    }
                    //TODO: CHECK HERE
                    QVariant retValue;
                    parent->sendMessage(info->message, args,retValue);
                }
            } else {
                XQSERVICE_DEBUG_PRINT("Sender has been destroyed");
                // The sender has been destroyed.  Clear the signal indices
                // so that we don't try to do a manual disconnect when our
                // own destructor is called.
                info->signalIndex = -1;
                info->destroyIndex = -1;
            }
        }
        id -= signalList.size() * 2;
    }
    XQSERVICE_DEBUG_PRINT("id: %d", id);
    return id;
}

/*!
    Construct a Qt Extended IPC message object for \a channel and attach it to \a parent.
    If \a channel is empty, then messages are taken from the application's
    \c{appMessage} channel.
*/
XQServiceAdaptor::XQServiceAdaptor(const QString& channel, QObject *parent)
    : QObject(parent)
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptor::XQServiceAdaptor");
    d = new XQServiceAdaptorPrivate(this, channel);
}

/*!
*/
XQServiceAdaptor::~XQServiceAdaptor()
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptor::~XQServiceAdaptor");
    if (!d->ref.deref())
        delete d;
    d = 0;
}

/*!
    Returns the name of the channel that this adaptor is associated with.
*/
QString XQServiceAdaptor::channel() const
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptor::channel");
    XQSERVICE_DEBUG_PRINT("d->channelName: %s", qPrintable(d->channelName));
    return d->channelName;
}

/*!
    Connects \a signal on \a sender to \a member on \a receiver.  Returns true
    if the receiveion was possible; otherwise returns false.

    If either \a sender or \a receiver are instances of
    XQServiceAdaptor, this function will arrange for the signal
    to be delivered over a Qt Extended IPC channel.  If both \a sender and
    \a receiver are local, this function is identical
    to QObject::connect().

    If the same signal is connected to same slot multiple times,
    then signal delivery will happen that many times.

    \sa XQServiceAdaptor::ChannelSelector
*/
bool XQServiceAdaptor::connect(QObject *sender, const QByteArray& signal,
                          QObject *receiver, const QByteArray& member)
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptor::connect");
    XQSERVICE_DEBUG_PRINT("signal: %s", signal.constData());
    XQSERVICE_DEBUG_PRINT("member: %s", member.constData());
    XQServiceAdaptor *senderProxy;
    XQServiceAdaptor *receiverProxy;

    // Bail out if the parameters are invalid.
    if (!sender || signal.isEmpty() || !receiver || member.isEmpty()){
        XQSERVICE_DEBUG_PRINT("Parameters are invalid");
        return false;
    }

    // Resolve the objects to find the remote proxies.
    senderProxy = qobject_cast<XQServiceAdaptor *>(sender);
    receiverProxy = qobject_cast<XQServiceAdaptor *>(receiver);

    // Remove proxies if the signal or member is not tagged with MESSAGE().
    if (!member.startsWith(QMESSAGE_CODE + '0')){
        XQSERVICE_DEBUG_PRINT("No received proxy");
        receiverProxy = 0;
    }
    if (!signal.startsWith(QMESSAGE_CODE + '0')){
        XQSERVICE_DEBUG_PRINT("No sender proxy");
        senderProxy = 0;
    }

    // If neither has a proxy, then use a local connect.
    if (!senderProxy && !receiverProxy){
        XQSERVICE_DEBUG_PRINT("Local connect");
        return QObject::connect(sender, signal, receiver, member);
    }

    // If both are still remote proxies, then fail the request.
    if (senderProxy && receiverProxy) {
        XQSERVICE_WARNING_PRINT("XQServiceAdaptor::connect: cannot connect MESSAGE() to MESSAGE()");
        return false;
    }

    // Determine which direction the connect needs to happen in.
    if (receiverProxy) {
        XQSERVICE_DEBUG_PRINT("Connecting a local signal to a remote slot");
        // Connecting a local signal to a remote slot.
        return receiverProxy->connectLocalToRemote(sender, signal, member);
    } else {
        XQSERVICE_DEBUG_PRINT("Connecting a remote signal to a local slot");
        // Connecting a remote signal to a local slot.
        return senderProxy->connectRemoteToLocal(signal, receiver, member);
    }
}

/*!
    Publishes the signal or slot called \a member on this object on
    the Qt Extended IPC channel represented by this XQServiceAdaptor.

    If \a member is a slot, then whenever an application sends a
    message to the channel with that name, the system will arrange
    for the slot to be invoked.

    If \a member is a signal, then whenever this object emits that
    signal, the system will arrange for a message with that name to
    be sent on the channel.

    Returns false if \a member does not refer to a valid signal or slot.

    \sa publishAll()
*/
bool XQServiceAdaptor::publish(const QByteArray& member)
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptor::publish");
    XQSERVICE_DEBUG_PRINT("member: %s", member.constData());
    // '1' is QSLOT_CODE in Qt 4.4 and below,
    // '5' is QSLOT_CODE in Qt 4.5 and higher.
    if (member.size() >= 1 && (member[0] == '1' || member[0] == '5')) {
        XQSERVICE_DEBUG_PRINT("Exporting a slot");
        // Exporting a slot.
        return connectRemoteToLocal("3" + member.mid(1), this, member);
    } else {
        XQSERVICE_DEBUG_PRINT("Exporting a sigal");
        // Exporting a signal.
        return connectLocalToRemote(this, member, member);
    }
}

/*!
    \enum XQServiceAdaptor::PublishType
    Type of members to publish via XQServiceAdaptor.

    \value Signals Publish only signals.
    \value Slots Publish only public slots.
    \value SignalsAndSlots Publish both signals and public slots.
*/

/*!
    Publishes all signals or public slots on this object within subclasses of
    XQServiceAdaptor.  This is typically called from a subclass constructor.
    The \a type indicates if all signals, all public slots, or both, should
    be published.  Private and protected slots will never be published.

    \sa publish()
*/
void XQServiceAdaptor::publishAll(XQServiceAdaptor::PublishType type)
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptor::publishAll(1)");
    publishAll(this,XQServiceAdaptor::staticMetaObject.methodCount(),type);
}

void XQServiceAdaptor::publishAll(QObject* object,int metCount, XQServiceAdaptor::PublishType type)
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptor::publishAll(2)");
    XQSERVICE_DEBUG_PRINT("metCount: %d", metCount);
    const QMetaObject *meta = object->metaObject();
    if (meta != d->publishedTo) {
        int count = meta->methodCount();
        XQSERVICE_DEBUG_PRINT("count: %d", count);
        int index;
        if (d->publishedTo)
            index = d->publishedTo->methodCount();
        else
            index = metCount;
        XQSERVICE_DEBUG_PRINT("index: %d", index);
        for (; index < count; ++index) {

            QMetaMethod method = meta->method(index);
            if (method.methodType() == QMetaMethod::Slot &&
                 method.access() == QMetaMethod::Public &&
                 (type == Slots || type == SignalsAndSlots)) {
                QByteArray name = method.signature();
                XQSERVICE_DEBUG_PRINT("name: %s", name.constData());
                XQSERVICE_DEBUG_PRINT("connectRemoteToLocal");
                connectRemoteToLocal("3" + name, object, "1" + name);
            } else if (method.methodType() == QMetaMethod::Signal &&
                        (type == Signals || type == SignalsAndSlots)) {
                QByteArray name = method.signature();
                XQSERVICE_DEBUG_PRINT("name: %s", name.constData());
                XQSERVICE_DEBUG_PRINT("connectLocalToRemote");
                connectLocalToRemote(object, "2" + name, "3" + name);
            }
        }
        d->publishedTo = meta;
    }
}

/*!
    Sends a message on the Qt Extended IPC channel which will cause the invocation
    of the single-argument \a member on receiving objects, with the
    argument \a arg1.
*/
bool XQServiceAdaptor::send(const QByteArray& member, const QVariant &arg1, QVariant &retData)
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptor::send(1)");
    XQSERVICE_DEBUG_PRINT("member: %s", member.constData());
    XQSERVICE_DEBUG_PRINT("arg1: %s", qPrintable(arg1.toString()));
    QList<QVariant> args;
    args.append(arg1);
    return sendMessage(memberToMessage(member), args, retData);
}

/*!
    Sends a message on the Qt Extended IPC channel which will cause the invocation
    of the double-argument \a member on receiving objects, with the
    arguments \a arg1 and \a arg2.
*/
bool XQServiceAdaptor::send(const QByteArray& member, const QVariant &arg1, const QVariant &arg2, QVariant &retData)
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptor::send(2)");
    XQSERVICE_DEBUG_PRINT("member: %s", member.constData());
    XQSERVICE_DEBUG_PRINT("arg1: %s, arg2: %s", qPrintable(arg1.toString()), qPrintable(arg2.toString()));
    QList<QVariant> args;
    args.append(arg1);
    args.append(arg2);
    return sendMessage(memberToMessage(member), args, retData);
}

/*!
    Sends a message on the Qt Extended IPC channel which will cause the invocation
    of the triple-argument \a member on receiving objects, with the
    arguments \a arg1, \a arg2, and \a arg3.
*/
bool XQServiceAdaptor::send(const QByteArray& member, const QVariant &arg1,
                       const QVariant &arg2, const QVariant &arg3, QVariant &retData)
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptor::send(3)");
    XQSERVICE_DEBUG_PRINT("member: %s", member.constData());
    XQSERVICE_DEBUG_PRINT("arg1: %s, arg2: %s, arg3: %s", qPrintable(arg1.toString()), qPrintable(arg2.toString()), qPrintable(arg3.toString()));
    QList<QVariant> args;
    args.append(arg1);
    args.append(arg2);
    args.append(arg3);
    return sendMessage(memberToMessage(member), args, retData);
}

/*!
    Sends a message on the Qt Extended IPC channel which will cause the invocation
    of the multi-argument \a member on receiving objects, with the
    argument list \a args.
*/
bool XQServiceAdaptor::send(const QByteArray& member, const QList<QVariant>& args, QVariant &retData)
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptor::send(4)");
    XQSERVICE_DEBUG_PRINT("member: %s", member.constData());
    for(int i=0;i<args.size();++i){
        XQSERVICE_DEBUG_PRINT("args[%d]: %s", i, qPrintable(args[i].toString()));
    }
    return sendMessage(memberToMessage(member), args, retData);
}

/*!
    Returns true if the message on the Qt Extended IPC channel corresponding to \a signal
    has been connected to a local slot; otherwise returns false.
*/
bool XQServiceAdaptor::isConnected(const QByteArray& signal)
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptor::isConnected");
    XQSERVICE_DEBUG_PRINT("signal: %s", signal.constData());
    return d->invokers.contains(memberToMessage(signal));
}

/*!
    Converts a signal or slot \a member name into a Qt Extended IPC message name.
    The default implementation strips the signal or slot prefix number
    from \a member and then normalizes the name to convert types
    such as \c{const QString&} into QString.

    \sa QMetaObject::normalizedSignature()
*/
QString XQServiceAdaptor::memberToMessage(const QByteArray& member)
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptor::memberToMessage");
    XQSERVICE_DEBUG_PRINT("member: %s", member.constData());
    if (member.size() >= 1 && member[0] >= '0' && member[0] <= '9') {
        return QString::fromLatin1
            (QMetaObject::normalizedSignature(member.constData() + 1));
    } else {
        return QString::fromLatin1(member.data(), member.size());
    }
}

QVariant XQServiceAdaptor::received(const QString& msg, const QByteArray& data, const XQSharableFile &sf)
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptor::received");
    XQSERVICE_DEBUG_PRINT("\tmsg: %s", qPrintable(msg));
    XQSERVICE_DEBUG_PRINT("\tdata: %s", data.constData());
    QVariant returnValue;
    bool found = false;
    // Increase the reference count on the private data just
    // in case the XQServiceAdaptor is deleted by one of the slots.
    XQServiceAdaptorPrivate *priv = d;
    priv->ref.ref();

    // Iterate through the slots for the message and invoke them.
    QMultiMap<QString, XQServiceAdaptorSlotInfo *>::ConstIterator iter;
    for (iter = priv->invokers.find(msg);
         iter != priv->invokers.end() && iter.key() == msg; ++iter) {
        XQServiceAdaptorSlotInfo *info = iter.value();
        if (info->destroyed){
            XQSERVICE_DEBUG_PRINT("\tDestroyed");
            continue;
        }

        // Convert "data" into a set of arguments suitable for qt_metacall.
        QDataStream stream(data);
        QList<QVariant> args;
        // a[0] is return value (numArgs+1)
        QVarLengthArray<void *, 32> a(info->numArgs + 1);
        
        XQSERVICE_DEBUG_PRINT("\tinfo->returnType: %d", info->returnType);
        
        if (info->returnType != (int)QVariant::Invalid) {
            returnValue = QVariant(info->returnType, (const void *)0);
            a[0] = returnValue.data();
        } else {
            a[0] = 0;
        }
        // Need to pick also the last XQRequestInfo form the stream
        // That's why iterating up to numArgs+1
        
        int reqInfoIndex = info->numArgs;

        
        for (int arg = 0; arg < info->numArgs+1; ++arg)             
            {
            XQSERVICE_DEBUG_PRINT("\tDesserialize argument: %d", arg);
            if (arg == reqInfoIndex)
            {
                // The last argument should be the request index
                XQSERVICE_DEBUG_PRINT("\tDesserialize XQRequestInfo");
                QVariant v;
                stream >> v;
                XQSERVICE_DEBUG_PRINT("\tXQRequestInfo:QVariant type=%s", v.typeName());
                if (QString(v.typeName()) == QString("XQRequestInfo"))
                {
                    XQRequestInfo info = v.value<XQRequestInfo>();
                    
                    //bring foreground or background based on RequestInfo from client side.
                    bool bg = info.isBackground();
                    bool fg = info.isForeground();
                    if (bg && !fg)
                    {
                        XQSERVICE_DEBUG_PRINT("\tApply background option");
                        XQServiceUtil::toBackground(true);
                    }
                    else if (fg && !bg)
                    {
                        XQSERVICE_DEBUG_PRINT("\tApply foreground option");
                        XQServiceUtil::toBackground(false);
                    }
                    // If both off or both on, do not do anything
                    
                    XQServiceIpcClient *cl = XQService::serviceThreadData()->clientConnection(d->channelName);
                    // Attach to current request before the metacall below !
                    cl->setRequestInfo(info);
                }
            }
            else if (info->types[arg] == XQServiceAdaptorPrivate::QVariantId)
            {
                // We need to handle QVariant specially because we actually
                // need the type header in this case.
                QVariant temp;
                stream >> temp;

                XQSERVICE_DEBUG_PRINT("\tQVariantId:QVariant type=%s", temp.typeName());

                if (QString(temp.typeName()) == QString("XQSharableFile"))
                {
                    //apply the patch
                    if ( sf.isValid())
                    {
                        temp = qVariantFromValue( sf );
                    }
                }

                args.append(temp);
                a[arg + 1] = (void *)&(args[arg]);
            }
            else {
                //
                // The default handling
                //
                QVariant temp;
                stream >> temp;
                
                XQSERVICE_DEBUG_PRINT("\tDefault:QVariant type=%s", temp.typeName());
                
                if (QString(temp.typeName()) == QString("XQSharableFile"))
                    {
                    //apply the patch
                    if ( sf.isValid())
                        {
                        temp = qVariantFromValue( sf );
                        }
                    }
                
                args.append(temp);
                a[arg + 1] = (void *)(args[arg].data());
            }
        }

        // Invoke the specified slot.
    #if !defined(QT_NO_EXCEPTIONS)
        try {
    #endif
            XQSERVICE_DEBUG_PRINT("Try to make metacall");
            info->receiver->qt_metacall
                (QMetaObject::InvokeMetaMethod, info->memberIndex, a.data());
            found = true;
    #if !defined(QT_NO_EXCEPTIONS)
        } catch (...) {
        }
    #endif
    }
    if (!found)
        {
        XQSERVICE_DEBUG_PRINT("Not found");
        XQService::serviceThreadData()->setLatestError(XQService::EMessageNotFound);
        }
    // Decrease the reference count and delete if necessary.
    if (!priv->ref.deref())
        delete priv;
    
    XQSERVICE_DEBUG_PRINT("returnValue: %s", qPrintable(returnValue.toString()));
    return returnValue;
}

void XQServiceAdaptor::receiverDestroyed()
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptor::receiverDestroyed");
    // Mark all slot information blocks that match the receiver
    // as destroyed so that we don't try to invoke them again.
    QObject *obj = sender();
    QMultiMap<QString, XQServiceAdaptorSlotInfo *>::Iterator it;
    for (it = d->invokers.begin(); it != d->invokers.end(); ++it) {
        if (it.value()->receiver == obj)
            it.value()->destroyed = true;
    }
}

bool XQServiceAdaptor::connectLocalToRemote
    (QObject *sender, const QByteArray& signal, const QByteArray& member)
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptor::connectLocalToRemote");
    XQSERVICE_DEBUG_PRINT("sender->objectName(): %s", qPrintable(sender->objectName()));
    XQSERVICE_DEBUG_PRINT("signal: %s", signal.constData());
    XQSERVICE_DEBUG_PRINT("member: %s", member.constData());
    XQServiceAdaptorSignalInfo *info = new XQServiceAdaptorSignalInfo();
    info->sender = sender;
    info->message = memberToMessage(member);

    // Resolve the signal name on the sender object.
    if (signal.size() > 0) {
        if (signal[0] != (QSIGNAL_CODE + '0')) {
            XQSERVICE_WARNING_PRINT("XQServiceAdaptor: `%s' is not a valid signal specification", signal.constData());
            delete info;
            return false;
        }
        QByteArray signalName =
            QMetaObject::normalizedSignature(signal.constData() + 1);
        info->signalIndex
            = sender->metaObject()->indexOfSignal(signalName.constData());
        XQSERVICE_DEBUG_PRINT("info->signalIndex: %d", info->signalIndex);
        if (info->signalIndex < 0) {
            XQSERVICE_WARNING_PRINT("XQServiceAdaptor: no such signal: %s::%s",
                                    sender->metaObject()->className(), signalName.constData());
            delete info;
            return false;
        }
        info->destroyIndex
            = sender->metaObject()->indexOfSignal("destroyed()");
        info->types = XQServiceAdaptorPrivate::connectionTypes
            (signalName, info->numArgs);
    } else {
        XQSERVICE_DEBUG_PRINT("No signal");
        delete info;
        return false;
    }

    // Connect up the signals.
    int index = d->slotIndex + d->signalList.size() * 2;
    XQSERVICE_DEBUG_PRINT("index: %d", index);
    QMetaObject::connect(sender, info->signalIndex, d, index,
                         Qt::DirectConnection, 0);
    if (info->destroyIndex >= 0) {
        QMetaObject::connect(sender, info->destroyIndex, d, index + 1,
                             Qt::DirectConnection, 0);
    }

    // Add the signal information to the active list.
    d->signalList.append(info);
    return true;
}

bool XQServiceAdaptor::connectRemoteToLocal
    (const QByteArray& signal, QObject *receiver, const QByteArray& member)
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptor::connectRemoteToLocal");
    XQSERVICE_DEBUG_PRINT("signal: %s", signal.constData());
    XQSERVICE_DEBUG_PRINT("receiver->objectName(): %s", qPrintable(receiver->objectName()));
    XQSERVICE_DEBUG_PRINT("member: %s", member.constData());
    // Make sure that we are actively monitoring the channel for messages.
    if (!d->connected) {
        XQSERVICE_DEBUG_PRINT("Try to connect to channel");
        QString chan = d->channelName;
        // Short-cut the signal emits in XQServiceChannel for greater
        // performance when dispatching incoming messages.
        XQServiceAdaptorChannel* adaptorChannel = new XQServiceAdaptorChannel(chan, this);
        bool ret = adaptorChannel ? adaptorChannel->connectChannel() : false;
        XQSERVICE_DEBUG_PRINT("ret: %d", ret);
        if (!ret) {
            return ret;
        }
        d->connected = true;
    }

    // Create a slot invoker to handle executing the member when necessary.
    XQServiceAdaptorSlotInfo *info = new XQServiceAdaptorSlotInfo();
    QByteArray name;
    if (member.size() > 0 && member[0] >= '0' && member[0] <= '9') {
        // Strip off the member type code.
        name = QMetaObject::normalizedSignature(member.constData() + 1);
    } else {
        name = QMetaObject::normalizedSignature(member.constData());
    }
    info->receiver = receiver;
    info->returnType = 0;
    info->types = 0;
    info->numArgs = 0;
    info->destroyed = false;
    if (receiver && name.size() > 0) {
        info->memberIndex
            = receiver->metaObject()->indexOfMethod(name.constData());
        XQSERVICE_DEBUG_PRINT("info->memberIndex: %d", info->memberIndex);
        if (info->memberIndex != -1) {
            connect(receiver, SIGNAL(destroyed()), this, SLOT(receiverDestroyed()));
            QMetaMethod method = receiver->metaObject()->method(info->memberIndex);
            info->returnType = XQServiceAdaptorPrivate::typeFromName(method.typeName());
            info->types = XQServiceAdaptorPrivate::connectionTypes(name, info->numArgs);
            if (!(info->types))
                info->destroyed = true;
        } else {
            XQSERVICE_WARNING_PRINT("XQServiceAdaptor: no such member: %s::%s",
                                     receiver->metaObject()->className(), name.constData());
        }
    } else {
        info->memberIndex = -1;
    }
    if (info->memberIndex == -1) {
        XQSERVICE_DEBUG_PRINT("No receiver");
        delete info;
        return false;
    }
    d->invokers.insert(memberToMessage(signal), info);
    return true;
}

bool XQServiceAdaptor::sendMessage(const QString& msg, const QList<QVariant>& args, QVariant &retData)
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptor::sendMessage");
    return send(d->channelName, msg, args, retData);
}


bool XQServiceAdaptor::send(const QString& channel,
                            const QString& msg, 
                            const QList<QVariant>& args,
                            QVariant& retValue,
                            bool sync,
                            XQServiceRequestCompletedAsync* rc)
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptor::send(1). No user data");
    return send(channel,msg,args,retValue,sync,rc,NULL);
}

bool XQServiceAdaptor::send(const QString& channel,
                            const QString& msg, 
                            const QList<QVariant>& args,
                            QVariant& retValue,
                            bool sync,
                            XQServiceRequestCompletedAsync* rc,
                            const void *userData)
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptor::send(2)");
    XQSERVICE_DEBUG_PRINT("channel: %s, sync: %d", qPrintable(channel), sync);
    XQSERVICE_DEBUG_PRINT("msg: %s", qPrintable(msg));
    XQSERVICE_DEBUG_PRINT("userdata: %x",(int)userData);
    for(int i=0;i<args.size();++i){
        XQSERVICE_DEBUG_PRINT("args[%d]:type=%s,value=%s", i, args[i].typeName(), qPrintable(args[i].toString()));
    }
    if (!sync && !rc) {
        // Something wrong as no callback given
        XQService::serviceThreadData()->setLatestError(XQService::EArgumentError);
        return false;
    }
    QByteArray array;
    QDataStream stream(&array, QIODevice::WriteOnly | QIODevice::Append);
    QList<QVariant>::ConstIterator iter;
    for (iter = args.begin(); iter != args.end(); ++iter) {
        stream << *iter;
    }
    // Stream is flushed and closed at this point.
    return XQServiceChannel::send(channel, msg, array, retValue, sync, rc, userData);
}

bool XQServiceAdaptor::cancelPendingSend(const QString& channel)
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptor::cancelPendingSend");
    return XQServiceChannel::cancelPendingSend(channel);
}

int XQServiceAdaptor::latestError()
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptor::latestError");
    return XQServiceChannel::latestError();
}



int XQServiceAdaptor::setCurrentRequestAsync()
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptor::setCurrentRequestAsync");
    XQServiceIpcClient *cl = XQService::serviceThreadData()->clientConnection(d->channelName);
    return cl->setCurrentRequestAsync();
}

bool XQServiceAdaptor::completeRequest(int index, const QVariant& retValue)
{
    XQSERVICE_DEBUG_PRINT("XQServiceAdaptor::completeRequest");
    XQServiceIpcClient *cl = XQService::serviceThreadData()->clientConnection(d->channelName);
    return cl->completeRequest(index, retValue);
}


XQRequestInfo XQServiceAdaptor::requestInfo() const
{
    XQServiceIpcClient *cl = XQService::serviceThreadData()->clientConnection(d->channelName);
    return cl->requestInfo();
}

#include "xqserviceadaptor.moc"
