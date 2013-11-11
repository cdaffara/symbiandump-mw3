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

#ifndef XQSERVICEABSTRACTSERVICE_H
#define XQSERVICEABSTRACTSERVICE_H

#include <xqserviceglobal.h>

#include <qobject.h>
#include <qstring.h>
#include <QVariant>
#include <xqrequestinfo.h>

class XQServiceProvider_Private;
class XQSERVICE_EXPORT XQServiceProvider : public QObject
{
    Q_OBJECT
public:
    explicit XQServiceProvider( const QString& service, QObject *parent = 0 );
    ~XQServiceProvider();
    void SetPlugin(QObject* impl_plugin);

Q_SIGNALS:
    void returnValueDelivered() ;
    void clientDisconnected() ;
    
protected:    
    void publishAll();
    int setCurrentRequestAsync();
    bool completeRequest(int index, const QVariant& retValue);
    XQRequestInfo requestInfo() const;

    template <typename T> 
    inline bool completeRequest(int index, const T& retValue)
    {
        QVariant retValueVariant;        
        retValueVariant.setValue(retValue);
        return completeRequest(index, retValueVariant);
    }
    
private:
    XQServiceProvider_Private *m_data;
};

#endif
