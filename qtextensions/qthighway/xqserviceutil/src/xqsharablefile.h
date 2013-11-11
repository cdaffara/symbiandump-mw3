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

#ifndef XQSHARABLEFILE_H
#define XQSHARABLEFILE_H

#include <f32file.h>
#include <xqserviceglobal.h>
#include <xqserviceipcmarshal.h>

class XQSERVICEUTIL_EXPORT XQSharableFile
{

public:
    XQSharableFile() ;
    XQSharableFile(RFile &file) ;
    virtual ~XQSharableFile() ;

public:
    
    bool setHandle(RFile &file);
    bool getHandle(RFile &file) const;
    QString fileName() const;
    bool isValid() const;

    bool open(const QString &fileName);
    void close();
        
    template <typename Stream> void serialize(Stream &stream) const;
    template <typename Stream> void deserialize(Stream &stream);
    bool operator==( const XQSharableFile &other);
        
private:
    QString mFileName;
    qlonglong mHandle;
    RFs mSharableFS;
};  

Q_DECLARE_USER_METATYPE(XQSharableFile)

#endif
