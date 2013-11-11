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

#ifndef XQSERVICEREQUEST_H
#define XQSERVICEREQUEST_H

#include <xqserviceglobal.h>
#include <qdatastream.h>
#include <qbuffer.h>
#include <QVariant>
#include <xqserviceipcmarshal.h>
#include <xqaiwinterfacedescriptor.h>
#include <xqrequestinfo.h>

class XQServiceRequest_Private;

class XQSERVICE_EXPORT XQServiceRequest : public QObject
{
    friend bool operator==( const XQServiceRequest &m1, const XQServiceRequest &m2 );
    friend class XQServiceRequest_Private;
    
    Q_OBJECT

public:
    
    XQServiceRequest();
    XQServiceRequest(const QString& fullServiceName, const QString& message, const bool& synchronous = true);
    XQServiceRequest(const XQServiceRequest& orig);
    XQServiceRequest(const XQAiwInterfaceDescriptor &descriptor, const QString& message, const bool& synchronous = true);
    
    ~XQServiceRequest();

    bool send();
    bool send(QVariant& retValue) ;
    
    template <typename T> 
    inline bool send(T& retValue)
    {
        QVariant retData;     
        bool ret=send(retData);        
        retValue = qVariantValue<T>(retData);
        return ret;
    }
    
    bool isNull() const;

    void setSynchronous(const bool &synchronous);
    bool isSynchronous() const;

    void setService(const QString &fullServiceName);
    QString service() const;
    void setMessage(const QString& message);
    QString message() const;

    const QList<QVariant> &arguments() const;
    void setArguments(const QList<QVariant> &arguments);
    
    int latestError();

    void setInfo(const XQRequestInfo &info);
    XQRequestInfo info() const;
    
    XQServiceRequest& operator=(const XQServiceRequest& orig);
    
    template<typename T>
    inline XQServiceRequest &operator<< (const T &var)
    {
        QVariant v = qVariantFromValue(var);
        addArg(v);
        return *this;
    }

    inline XQServiceRequest &operator<< (const char *var)
    {
        QVariant v = QVariant(QString(var));
        addArg(v);
        return *this;
    }

    inline void addVariantArg(const QVariant& var)
    {
        addArg(var);
    }

    static QByteArray serializeArguments(const XQServiceRequest &action);
    static void deserializeArguments(XQServiceRequest &action, const QByteArray &data);

    template <typename Stream> void serialize(Stream &stream) const;
    template <typename Stream> void deserialize(Stream &stream);

signals:
    void requestCompleted(const QVariant& value) ;
    void requestError(int err);

private:
    void addArg(const QVariant& v);
    bool handleSharableFileArgs();
    
private:
    XQServiceRequest_Private *mData;
};

inline bool operator==( const XQServiceRequest &m1, const XQServiceRequest &m2 )
{
    return (m1.service() == m2.service()) && (m1.message() == m2.message())
            && (m1.arguments() == m2.arguments());
}

Q_DECLARE_USER_METATYPE(XQServiceRequest)

#endif
