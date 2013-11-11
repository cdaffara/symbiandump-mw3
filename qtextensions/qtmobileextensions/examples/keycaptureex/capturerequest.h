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

#ifndef CAPTUREREQUEST_H_
#define CAPTUREREQUEST_H_

#include <QMap>
#include <QString>
#include <QtGlobal>

#include <XQKeyCapture>

class Mapping;
class QAction;

class CaptureRequest
{
public:
    
    enum RequestType {
        RequestTypeUndefined =-1,
        RequestTypeKey,
        RequestTypeLongKey,
        RequestTypeKeyUpAndDowns,
        RequestTypeCancelKey,
        RequestTypeCancelLongKey,
        RequestTypeCancelKeyUpAndDowns        
    };
    
public:
    CaptureRequest(Mapping *);
    ~CaptureRequest();
    
    bool setType(QAction* action);
    bool setKey(QAction* action, QMap<QString, Qt::Key> *map);
    bool setLongFlags(QAction* action, QMap<QString, XQKeyCapture::LongFlags> *map);
    
    QString toString();

public:
    RequestType mRequestType;
    Qt::Key mKey;    
    Qt::KeyboardModifier mModifiersMap;
    Qt::KeyboardModifier mModifier;
    XQKeyCapture::LongFlags mLongFlags;

    Mapping *mapping;

};

#endif /* CAPTUREREQUEST_H_ */
