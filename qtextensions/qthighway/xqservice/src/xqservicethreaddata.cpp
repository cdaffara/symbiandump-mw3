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

#include "xqservicethreaddata.h"
#include "xqservicechannel.h"
#include "xqserviceipcclient.h"

#include <QThreadStorage>
#include <qthread.h>

#ifndef QT_NO_THREAD

static QThreadStorage<XQServiceThreadData *> xqserviceThreadStorage;

XQServiceThreadData *XQService::serviceThreadData()
{
    XQSERVICE_DEBUG_PRINT("XQService::serviceThreadData");
    if (!xqserviceThreadStorage.hasLocalData()) {
        xqserviceThreadStorage.setLocalData(new XQServiceThreadData());
    }
    return xqserviceThreadStorage.localData() ;
}

#else

Q_GLOBAL_STATIC(XQServiceThreadData, serviceThreadData);

#endif


XQServiceThreadData *XQServiceThreadData::instance()
{
    XQSERVICE_DEBUG_PRINT("XQServiceThreadData::instance");
    return XQService::serviceThreadData();
}

XQServiceThreadData::~XQServiceThreadData()
{
    XQSERVICE_DEBUG_PRINT("XQServiceThreadData::~XQServiceThreadData");
    qDeleteAll(ipcConnMap);
    ipcConnMap.clear();
}

QString XQServiceThreadData::getIpcConnectioName(const QString& channel)
{
    XQSERVICE_DEBUG_PRINT("XQServiceThreadData::getIpcConnectioName");
    XQSERVICE_DEBUG_PRINT("channel: %s", qPrintable(channel));
/*
    // name of the connection is the name of application without interface
    int pos = channel.lastIndexOf('.');
    return pos != -1 ? channel.left(pos) : channel;
*/
    //TODO: this should avoid to have multiple connection in the same application if provide multiple interfaces
    return channel;
}

// Get the client connection object for this thread.
bool XQServiceThreadData::createClientConnection(const QString& channel, bool isServer, 
                                                 bool isSync, XQServiceRequestCompletedAsync* rc,
                                                const void *userData)
{
    XQSERVICE_DEBUG_PRINT("XQServiceThreadData::createClientConnection start");
    XQSERVICE_DEBUG_PRINT("channel: %s, isServer: %d, isSync: %d", qPrintable(channel), isServer, isSync);
    XQSERVICE_DEBUG_PRINT("userdata: %x", (int)userData);
    
    QString ipcConName = getIpcConnectioName(channel);
    XQSERVICE_DEBUG_PRINT("ipcConName: %s", qPrintable(ipcConName));
    XQServiceIpcClientMap::Iterator it = ipcConnMap.find(ipcConName);
    if (it == ipcConnMap.end()) {
        XQSERVICE_DEBUG_PRINT("Create new connection");
        // userData may be NULL !
        XQServiceIpcClient* conn = new XQServiceIpcClient(ipcConName,isServer,isSync,rc, userData);
        if (conn) {
            XQSERVICE_DEBUG_PRINT("conn created");
            bool ret=true;
            if (isServer) {
                XQSERVICE_DEBUG_PRINT("Listen server");
                ret=conn->listen();
            }
            else {
                XQSERVICE_DEBUG_PRINT("Connect to server");
                ret=conn->connectToServer();        
            }
            if (ret) {
                XQSERVICE_DEBUG_PRINT("Add connection to mapping");
                it = ipcConnMap.insert(ipcConName, conn);
            }
            else {
                XQSERVICE_DEBUG_PRINT("Couldn't connect");
                delete conn;        
            }
            XQSERVICE_DEBUG_PRINT("\t ret: %d", ret);
            XQSERVICE_DEBUG_PRINT("XQServiceThreadData::createClientConnection end (1)");
            return ret;
        }
    }
    
    XQSERVICE_DEBUG_PRINT("\t Connection already exists or creation failed");
    XQSERVICE_DEBUG_PRINT("XQServiceThreadData::createClientConnection end (2)");
    return false;
}

// Get the client connection object for this thread.
XQServiceIpcClient *XQServiceThreadData::clientConnection(const QString& channel)
{
    XQSERVICE_DEBUG_PRINT("XQServiceThreadData::clientConnection");
    XQSERVICE_DEBUG_PRINT("channel: %s", qPrintable(channel));
    QString ipcConName = getIpcConnectioName(channel);
    XQSERVICE_DEBUG_PRINT("ipcConName: %s", qPrintable(ipcConName));
    XQServiceIpcClientMap::Iterator it = ipcConnMap.find(ipcConName);
    if (it == ipcConnMap.end()) {
        XQSERVICE_DEBUG_PRINT("No client connection");
        return NULL;
    }
    return it.value();
}

void XQServiceThreadData::closeClientConnection(const QString& channel)
{
    XQSERVICE_DEBUG_PRINT("XQServiceThreadData::closeClientConnection");
    XQSERVICE_DEBUG_PRINT("channel: %s", qPrintable(channel));
    QString ipcConName = getIpcConnectioName(channel);
    XQSERVICE_DEBUG_PRINT("ipcConName: %s", qPrintable(ipcConName));
    XQServiceIpcClientMap::Iterator it = ipcConnMap.find(ipcConName);
    if (it != ipcConnMap.end()) {
        XQSERVICE_DEBUG_PRINT("disconnect");
        it.value()->disconnected();
        ipcConnMap.erase(it);
    }
}

// Determine if we have a client connection object for this thread.
bool XQServiceThreadData::hasClientConnection(const QString& channel)
{
    XQSERVICE_DEBUG_PRINT("XQServiceThreadData::hasClientConnection");
    XQSERVICE_DEBUG_PRINT("channel: %s", qPrintable(channel));
    QString ipcConName = getIpcConnectioName(channel);
    XQSERVICE_DEBUG_PRINT("ipcConName: %s", qPrintable(ipcConName));
    XQServiceIpcClientMap::Iterator it = ipcConnMap.find(ipcConName);
    XQSERVICE_DEBUG_PRINT("hasClientConnection: %d", it != ipcConnMap.end());
    return it != ipcConnMap.end();
}

int XQServiceThreadData::latestError()
{
    XQSERVICE_DEBUG_PRINT("XQServiceThreadData::latestError");
    XQSERVICE_DEBUG_PRINT("m_latestError: %d", m_latestError);
    return m_latestError;
}

void XQServiceThreadData::setLatestError(int latestError)
{
    XQSERVICE_DEBUG_PRINT("XQServiceThreadData::setLatestError %d", latestError);
    m_latestError = latestError;
}

QByteArray XQServiceThreadData::serializeRetData(const QVariant &value, int error)
{
    XQSERVICE_DEBUG_PRINT("XQServiceThreadData::serializeRetData");
    XQSERVICE_DEBUG_PRINT("value type: %d, null?=%d, valid?=%d, error: %d", value.type(), value.isNull(), value.isValid(), error);
    QByteArray array;
    // if (!value.isNull() && (value.type() != QVariant::Invalid)) {    maparnan
    if (value.isValid()) {    // 
        QDataStream stream(&array, QIODevice::WriteOnly | QIODevice::Append);
        
        stream << CmdRetData;
        stream << value;
    }
    else {
        if (error)
            {
            QVariant value(error);
            QDataStream stream(&array, 
                        QIODevice::WriteOnly | QIODevice::Append);
            stream << CmdErrData;
            stream << value;
            }
        }
    return array;
}

QVariant XQServiceThreadData::deserializeRetData(const QByteArray &retData)
{
    XQSERVICE_DEBUG_PRINT("XQServiceThreadData::deserializeRetData");
    XQSERVICE_DEBUG_PRINT("retData: %s", retData.constData());
    if (retData.length()) 
        {
        QDataStream stream(retData);
        int cmd ;
        stream >> cmd ;
        if (cmd == CmdRetData) 
            {
            QVariant retServiceData(stream);
            return retServiceData;
        }
        else
            {
            if (cmd == CmdErrData)
                {
                QVariant retServiceData(stream);
                int error = retServiceData.toInt();
                XQService::serviceThreadData()->setLatestError(error);
                }
            }
    }
    return QVariant();
}
