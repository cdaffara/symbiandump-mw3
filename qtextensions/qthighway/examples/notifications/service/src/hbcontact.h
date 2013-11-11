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

#ifndef HBSERVICEPROVIDERCONTACT_H
#define HBSERVICEPROVIDERCONTACT_H

#include <QList>
#include <QString>
#include <QUuid>
#include <xqserviceipcmarshal.h>


class HbContact
{
public:
    HbContact() {}; 
    virtual ~HbContact() {};

    QString mLabel;
    QString mNumber;
    QString mAddress;
    QString mCity;
    QString mCountry;
    QUuid mUid;
   
    template <typename Stream> void serialize(Stream &stream) const;
    template <typename Stream> void deserialize(Stream &stream);
};

/*
struct HbContact
{
public:
    QString mLabel;
    QString mNumber;
    QString mAddress;
    QString mCity;
    QString mCountry;
    QUuid mUid;
   
    template <typename Stream> void serialize(Stream &stream) const;
    template <typename Stream> void deserialize(Stream &stream);
};
*/

template <typename Stream> inline void HbContact::serialize(Stream &s) const
{
    s << mLabel;
    s << mNumber;
    s << mUid;
}

template <typename Stream> inline void HbContact::deserialize(Stream &s)
{
    s >> mLabel;
    s >> mNumber;
    s >> mUid;
}

typedef QList<HbContact> HbContactList;

Q_DECLARE_USER_METATYPE(HbContact)
Q_DECLARE_USER_METATYPE_NO_OPERATORS(HbContactList)

#endif // HBSERVICEPROVIDERCONTACT_H
