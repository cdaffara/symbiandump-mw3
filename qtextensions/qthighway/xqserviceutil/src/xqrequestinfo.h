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

#ifndef XQREQUESTINFO_H
#define XQREQUESTINFO_H

#include <QVariant>
#include <QStringList>
#include <QSet>
#include <xqserviceglobal.h>
#include <xqserviceipcmarshal.h>


class XQSERVICEUTIL_EXPORT XQRequestInfo
{
    public:
        XQRequestInfo() ;
        virtual ~XQRequestInfo() ;
        
    public:
        
        bool isValid() const;
        
        void setEmbedded(bool on);
        bool isEmbedded() const;
        void setBackground(bool on);
        bool isBackground() const;
        bool isSynchronous() const;
        void setForeground(bool on);
        bool isForeground() const;
        
        quint32 clientSecureId() const;
        quint32 clientVendorId() const;
        QSet<int> clientCapabilities() const;
        int id() const;

        void setInfo(const QString &key, const QVariant &value);
        QVariant info(const QString &key) const;
        QStringList infoKeys() const;
        
        template <typename Stream> void serialize(Stream &stream) const;
        template <typename Stream> void deserialize(Stream &stream);

    private:
        QHash<QString, QVariant> mInfo;
};

Q_DECLARE_USER_METATYPE(XQRequestInfo)

#endif
