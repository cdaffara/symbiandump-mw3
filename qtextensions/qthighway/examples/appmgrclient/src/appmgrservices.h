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

#ifndef APPMGR_SERVICES_H
#define APPMGR_SERVICES_H

#include <xqaiwdecl.h>

// All the services should be declared in a component specific header file
// In order to avoid any mistakes in service naming.
// In QtHighway service interface is described in XML file, operation is not

// Service interfaces and related operations
#define SERVICE1 QLatin1String("serviceapp")
#define IDIAL QLatin1String("dialer")
#define OPERATION1 QLatin1String("dial(QString,bool)")

// Erroneus services
#define ERR_IDIAL QLatin1String("xxxx.yyy")
#define ERR_OPERATION1 QLatin1String("dial(QString,QString)")

#define TESTCASE_INFO_KEY QLatin1String("XQTestCase")

// Few dummy metatypes
class MetaDummy1
{
    public:
        MetaDummy1() {};
        virtual ~MetaDummy1() {};

        QString mTest;
        template <typename Stream> void serialize(Stream &stream) const;
        template <typename Stream> void deserialize(Stream &stream);
};

template <typename Stream> inline void MetaDummy1::serialize(Stream &s) const
{
    s << mTest;
}

template <typename Stream> inline void MetaDummy1::deserialize(Stream &s)
{
    s >> mTest;
}


class MetaDummy2
{
    public:
        MetaDummy2() {};
        virtual ~MetaDummy2() {};

        QString mTest;
        template <typename Stream> void serialize(Stream &stream) const;
        template <typename Stream> void deserialize(Stream &stream);
};

template <typename Stream> inline void MetaDummy2::serialize(Stream &s) const
{
    s << mTest;
}

template <typename Stream> inline void MetaDummy2::deserialize(Stream &s)
{
    s >> mTest;
}

// Testing QVariant
class TestServiceData
{
    public:

        TestServiceData() : mType(-1) {}
        TestServiceData(int aType, const QVariant &aData) :
                mType(aType),
                mData(aData)
                {}

        virtual ~TestServiceData() {}

       
        int        mType;
        QVariant   mData;

        template <typename Stream> void serialize(Stream &aStream) const;
        template <typename Stream> void deserialize(Stream &aStream);
};

template <typename Stream> inline void TestServiceData::serialize(Stream &aStream) const
{
    qDebug() << "TestServiceData::serialize 1";
    aStream << mType;
    qDebug() << "TestServiceData::serialize 2";
    aStream << mData;
    qDebug() << "TestServiceData::serialize 3";
}

template <typename Stream> inline void TestServiceData::deserialize(Stream &aStream)
{
    qDebug() << "TestServiceData::deserialize 1";
    aStream >> mType;
    qDebug() << "TestServiceData::deserialize 2";
    aStream >> mData;
    qDebug() << "TestServiceData::deserialize 3";
}

typedef QList<TestServiceData> TestServiceDataList;

Q_DECLARE_USER_METATYPE(TestServiceData)
Q_DECLARE_USER_METATYPE_NO_OPERATORS(TestServiceDataList)
Q_DECLARE_USER_METATYPE(MetaDummy1)
Q_DECLARE_USER_METATYPE(MetaDummy2)

#endif
