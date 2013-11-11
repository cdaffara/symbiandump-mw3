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

#ifndef XQSERVICETHREADDATA_H
#define XQSERVICETHREADDATA_H

#include "xqservicelog.h"

#include <QtCore/qobject.h>

#include <QtCore/qshareddata.h>
#include <QtCore/qregexp.h>
#include <QMap>
#include <QVariant>

const int CmdRetData = 0xF001 ;
const int CmdErrData = 0xF010 ;

class XQServiceChannel;
class XQServiceIpcClient;
class XQServiceRequestCompletedAsync;

// Special variant class that can perform QDataStream operations
// without the QVariant header information.
class XQServiceVariant : public QVariant
{
public:
    XQServiceVariant() : QVariant() {}
    explicit XQServiceVariant(const QVariant& value)
        : QVariant(value) 
    {
        XQSERVICE_DEBUG_PRINT("XQServiceVariant::XQServiceVariant");
        XQSERVICE_DEBUG_PRINT("value: %s", qPrintable(value.toString()));
    }

    void load(QDataStream& stream, int typeOrMetaType)
    {
        XQSERVICE_DEBUG_PRINT("XQServiceVariant::load");
        clear();
        create(typeOrMetaType, 0);
        d.is_null = false;
        QMetaType::load(stream, d.type, const_cast<void *>(constData()));
    }

    void save(QDataStream& stream) const
    {
        XQSERVICE_DEBUG_PRINT("XQServiceVariant::save");
        QMetaType::save(stream, d.type, constData());
    }
};

/*!
    \class XQServiceChannelPrivate
    \brief Private implementation of XQServiceChannelPrivate
*/
class XQServiceChannelPrivate : public QSharedData
{
public:
    XQServiceChannelPrivate(XQServiceChannel *obj, const QString& chan, bool isServer)
        : object(obj), channel(chan), server(isServer)
    {
        XQSERVICE_DEBUG_PRINT("XQServiceChannelPrivate::XQServiceChannelPrivate");        
    }

    XQServiceChannel *object;
    QString channel;
    bool server;
};

typedef QExplicitlySharedDataPointer<XQServiceChannelPrivate> XQServiceChannelPrivatePointer;
typedef QMap<QString, QList<XQServiceChannelPrivatePointer> > XQServiceClientMap;
typedef QMap<QString, XQServiceIpcClient*> XQServiceIpcClientMap;

// Thread-specific data for XQService client and server implementations.
class XQServiceThreadData
{
public:
    XQServiceThreadData()
    {
        XQSERVICE_DEBUG_PRINT("XQServiceThreadData::XQServiceThreadData");
        m_latestError = 0; 
    }

    virtual ~XQServiceThreadData();


    static XQServiceThreadData *instance();
    
    bool createClientConnection(const QString& channel,bool isServer=false, 
                                bool isSync=true, XQServiceRequestCompletedAsync* rc=NULL,
                               const void *userData=NULL);
    void closeClientConnection(const QString& channel);

    XQServiceIpcClient *clientConnection(const QString& channel);
    
    // Determine if we have a client connection object for this thread.
    bool hasClientConnection(const QString& ipcConName);
    QString getIpcConnectioName(const QString& channel);

    static QVariant deserializeRetData(const QByteArray &retData);
    static QByteArray serializeRetData(const QVariant &value, int error);
    
	int latestError();
    
    void setLatestError(int latestError);
    // Map client-side channel names to lists of XQServiceChannel objects.
    XQServiceClientMap clientMap;

    XQServiceIpcClientMap ipcConnMap;
	
	int m_latestError;
};

namespace XQService
{
    XQServiceThreadData *serviceThreadData();
}

#endif
