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

#ifndef TESTSERVICEDATA_H
#define TESTSERVICEDATA_H

#include <QVariant>

// Custom metatype
class TestServiceData
{
    public:

        TestServiceData() : mType(-1) {}
        TestServiceData(int aType, const QVariant &aData) :
                mType(aType),
                mData(aData)
                {}

        virtual ~TestServiceData() {}

    public:
        template <typename Stream> void serialize(Stream &aStream) const;
        template <typename Stream> void deserialize(Stream &aStream);
        
    public:
        int        mType;
        QVariant   mData;

};

template <typename Stream> inline void TestServiceData::serialize(Stream &aStream) const
{
    aStream << mType;
    aStream << mData;
}

template <typename Stream> inline void TestServiceData::deserialize(Stream &aStream)
{
    aStream >> mType;
    aStream >> mData;
}

typedef QList<TestServiceData> TestServiceDataList;

Q_DECLARE_USER_METATYPE(TestServiceData)
Q_DECLARE_USER_METATYPE_NO_OPERATORS(TestServiceDataList)

#endif
