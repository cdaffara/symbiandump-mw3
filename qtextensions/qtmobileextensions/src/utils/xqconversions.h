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

#ifndef XQCONVERSIONS
#define XQCONVERSIONS

// INCLUDES
#include <e32cmn.h>
#include <e32des8.h>
#include <QObject>
#include "utils_global.h"

// CLASS DECLARATION
class XQUTILS_EXPORT XQConversions
{
public:
    static QString s60DescToQString(const TDesC& desc);
    static HBufC* qStringToS60Desc(const QString& string);
    static QString s60Desc8ToQString(const TDesC8& desc);
    static HBufC8* qStringToS60Desc8(const QString& string);
    static QByteArray s60Desc8ToQByteArray(const TDesC8& desc);
    static HBufC8* qByteArrayToS60Desc8(const QByteArray& string);
};

#endif // XQCONVERSIONS

// End of file
