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

#include "xqconversions.h"
#include "utf.h"

/*!
    \class XQConversions
    \brief The XQConversions class offers functions for converting between Symbian S60 data types and Qt data types.
*/

/*!
    Converts a Symbian S60 descriptor (string) to a QString

    \param desc descriptor to be converted
    \return QString containing the converted string
*/
QString XQConversions::s60DescToQString(const TDesC& desc)
{
    return QString::fromUtf16(desc.Ptr(), desc.Length());
}

/*!
    Converts a QString to a Symbian S60 descriptor (string).

    Note: Ownership of the returned descriptor (string) is transferred to caller.

    \param string QString to be converted
    \return Pointer to a Symbian S60 descriptor on success;
            otherwise returns NULL pointer
*/
HBufC* XQConversions::qStringToS60Desc(const QString& string)
{
    TPtrC16 str(reinterpret_cast<const TUint16*>(string.utf16()));
    return str.Alloc();
}

/*!
    Converts a Symbian S60 8 bit descriptor (UTF8 string) to a QString

    \param desc 8 bit descriptor to be converted
    \return Converted QString on success; otherwise returns null QString
*/
QString XQConversions::s60Desc8ToQString(const TDesC8& desc)
{
    QString qtString;
    HBufC* s60str;
    TRAPD(error, s60str = CnvUtfConverter::ConvertToUnicodeFromUtf8L(desc));
    if (error == KErrNone)
    {
        qtString = QString::fromUtf16(s60str->Ptr(), s60str->Length());
        delete s60str;
    }
    return qtString;
}

/*!
    Converts a QString to a Symbian S60 8 bit descriptor (UTF8 string).

    Note: Ownership of the returned descriptor (string) is transferred to the caller

    \param string QString to be converted
    \return Pointer to a Symbian S60 descriptor containing the UTF8 string on success;
            otherwise returns NULL pointer
*/
HBufC8* XQConversions::qStringToS60Desc8(const QString& string)
{
    TPtrC16 str(reinterpret_cast<const TUint16*>(string.utf16()));
    HBufC8* s60str;
    TRAPD(error, s60str = CnvUtfConverter::ConvertFromUnicodeToUtf8L(str));
    if (error != KErrNone)
    {
        return NULL;
    }
    return s60str;
}

/*!
    Converts a Symbian S60 8 bit descriptor to a QByteArray

    \param desc 8 bit descriptor to be converted
    \return QByteArray on success; otherwise returns null QByteArray
*/
QByteArray XQConversions::s60Desc8ToQByteArray(const TDesC8& desc)
{
    return QByteArray((const char*)desc.Ptr(), desc.Length());
}

/*!
    Converts a QByteArray to a Symbian S60 8 bit descriptor.
    Note: Ownership of the returned descriptor (string) is transferred to the caller

    \param byteArray QByteArray to be converted
    \return Pointer to an 8 bit Symbian S60 descriptor string on success;
            otherwise returns NULL pointer
*/
HBufC8* XQConversions::qByteArrayToS60Desc8(const QByteArray& byteArray)
{
    TPtrC8 ptr8((TUint8*)(byteArray.constData()));
    return ptr8.Alloc();
}

// End of file
