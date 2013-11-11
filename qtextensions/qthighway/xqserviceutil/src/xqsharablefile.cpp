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
#include "xqsharablefile.h"
#include <QVariant>

/*!
    \class XQSharableFile
    \inpublicgroup QtBaseModule

    \ingroup ipc
    \brief Encapsulates needed functionality to pass one Symbian data-caged file handle to service provider.
*/

/*!
    Constructor.
*/
XQSharableFile::XQSharableFile() :
   mHandle(0)
{
    XQSERVICE_DEBUG_PRINT("XQSharableFile::XQSharableFile");
}

/*!
    Constructor.
    \param file Existing and valid file handle to be set to this XQSharableFile.
*/
XQSharableFile::XQSharableFile(RFile &file)
{
    XQSERVICE_DEBUG_PRINT("XQSharableFile::XQSharableFile(RFile)");
    setHandle(file);
}

/*!
    Destroys the descriptor.

    \b Note!
    The destructor does not close the file handle. 
    You have to close it via the close() method.
*/
XQSharableFile::~XQSharableFile()
{
    XQSERVICE_DEBUG_PRINT("XQSharableFile::~XQSharableFile");
    // One need to call close() explicitelly when all done
}

/*!
    Sets the existing and valid sharable file. Use this
    function if you obtained the handle from the other APIs.
    \return True if the handle is valid and can be set, 
            false otherwise.
*/
bool XQSharableFile::setHandle(RFile &file)
{
    XQSERVICE_DEBUG_PRINT("XQSharableFile::setHandle");
    
    TFileName name;
    TInt err = file.FullName(name);
    if (err != KErrNone)
    {
        XQSERVICE_DEBUG_PRINT("XQSharableFile::fullName err%d", err);
        mHandle = 0;
        return false;
    }
        
    mFileName = QString::fromUtf16(name.Ptr(), name.Length());

    mHandle = *((qlonglong *)&file);
    QString s = QString("File handle is %1").arg(mHandle);
    XQSERVICE_DEBUG_PRINT("%s", qPrintable(s));

    return true;
}

/*!
    Gets the set handle, if any.
    \param handle Reference to handle to be set with
                  this XQSharableFile's handle.
    \return True if the handle is valid, false otherwise.
*/
bool XQSharableFile::getHandle(RFile &handle) const
{
    XQSERVICE_DEBUG_PRINT("XQSharableFile::getHandle");
    QString s = QString("File handle is %1").arg(mHandle);
    if (mHandle == 0)
        return false;
    handle = *((RFile *)&mHandle);
    return true;
}

/*!
    Get file name associated with this XQSharableFile.
    \return Full file name associated with this XQSharableFile.
*/
QString XQSharableFile::fileName() const
{
    XQSERVICE_DEBUG_PRINT("XQSharableFile::fileName");
    return mFileName;
}

/*!
    Checks if this XQSharableFile is valid, that is if the associated
    file handle is valid.
    \return True if file handle of this XQSharableFile is valid,
            false otherwise.
*/
bool XQSharableFile::isValid() const
{
    XQSERVICE_DEBUG_PRINT("XQSharableFile::isValid=%d", mHandle != 0);
    return mHandle != 0;
}

/*!
    Creates and sets the sharable file handle of the given file.
    The file can be private, data-caged directory.
    Currently only supported access mode is R/O.
    \b Note! After opening the file, it should be closed by calling close().
    \param fileName File to be opened.
    \return True if the file name was ok and the file was opened successfuly,
            false otherwise.
*/
bool XQSharableFile::open(const QString &fileName)
{
    close();   // Close possibly existing old one

    QString symbianFileName = fileName;
    symbianFileName.replace("/", "\\");
    
    TInt err = mSharableFS.Connect();
    if (err != KErrNone)
    {
        return false;
    }
    
    mSharableFS.ShareProtected();
    TPtrC name( reinterpret_cast<const TUint16*>(symbianFileName.utf16()));
    RFile f;
    err = f.Open(mSharableFS, name, EFileShareReadersOnly);
    if (err != KErrNone)
    {
        // Error in opening, close the created session
        mSharableFS.Close();
        return false;
    }

    this->setHandle(f);
    return true;
}

/*!
    Close the sharable file handle.
    <b>It is very important to close the handle if it is no longer needed.</b>
*/
void XQSharableFile::close()
{
    XQSERVICE_DEBUG_PRINT("XQSharableFile::close");
    RFile f;
    if (getHandle(f))
    {
        f.Close();
    }

    if (mSharableFS.Handle() != NULL)
    {
        mSharableFS.Close();
    }
    
}

/*!
    Serializes XQSharableFile into a stream.
    \param s Stream to which this XQSharableFile will be serialized.
*/
template <typename Stream> void XQSharableFile::serialize(Stream &s) const
{
    XQSERVICE_DEBUG_PRINT("XQSharableFile::serialize");
    s << mFileName;
    s << mHandle;
    QString str = QString("\tFile handle is %1").arg(mHandle);
    XQSERVICE_DEBUG_PRINT("%s", qPrintable(str));
}

/*!
    Deserializes XQSharableFile from a stream.
    \param s Stream from which XQSharableFile will be deserialized.
*/
template <typename Stream> void XQSharableFile::deserialize(Stream &s)
{
    XQSERVICE_DEBUG_PRINT("XQSharableFile::deserialize");
    
    s >> mFileName;
    s >> mHandle;
    
    QString str = QString("\tFile handle is %1").arg(mHandle);
    XQSERVICE_DEBUG_PRINT("%s", qPrintable(str));
    
}

/*!
    Compares two XQSharableFile objects.
    \return True if both XQSharableFile objects are equal, false otherwise.
*/
bool XQSharableFile::operator==( const XQSharableFile &other )
{
    return (mFileName == other.mFileName) && (mHandle == other.mHandle);
}


Q_IMPLEMENT_USER_METATYPE(XQSharableFile)
