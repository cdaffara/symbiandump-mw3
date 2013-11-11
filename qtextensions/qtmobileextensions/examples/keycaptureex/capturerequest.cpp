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

#include "capturerequest.h"

#include <QAction>

#include "mapping.h"

CaptureRequest::CaptureRequest(Mapping *mapping_)
{
    mRequestType = RequestTypeUndefined;
    mKey = Qt::Key_unknown;    
    mModifiersMap = Qt::NoModifier;
    mModifier = Qt::NoModifier;
    mLongFlags = XQKeyCapture::LongNormal;
    
    this->mapping = mapping_;
}

CaptureRequest::~CaptureRequest()
{
}

QString CaptureRequest::toString()
{
    QString res = "Request ";
    
    switch (mRequestType) {
        case CaptureRequest::RequestTypeKey :
            res +="Key(";
            break;
        case CaptureRequest::RequestTypeLongKey :
            res +="LongKey(";            
            break;
        case CaptureRequest::RequestTypeKeyUpAndDowns :
            res +="UpAndDowns(";  
            break;
        case CaptureRequest::RequestTypeCancelKey :
            res +="CancelKey(";
            break;
        case CaptureRequest::RequestTypeCancelLongKey :
            res +="CancelLongKey(";
            break;
        case CaptureRequest::RequestTypeCancelKeyUpAndDowns :
            res +="CancelUpAndDowns(";
            break;
        default:
            res +="Unknown";
            break;
    }
    
    if ( mRequestType == RequestTypeUndefined)
        return res;
    
//    res +=QString("%1").arg(mKey, 0, 16);
    
    res += mapping->name(mKey);
    
    res += QString("(0x%1)").arg(mKey, 0, 16);
    
    //TODO::put to res mModifiersMap    
    //TODO::put to res mModifier
    if ( mRequestType == RequestTypeLongKey || mRequestType == RequestTypeCancelLongKey)
        res +=QString(",%1)").arg(mLongFlags, 0, 16);
    else
        res +=")";
    
    return res;
}

bool CaptureRequest::setType(QAction* action)
{
    if (!action)
        return false;
    
    bool ok;
    
    int act = action->data().toInt(&ok);
    
    if (ok){
        switch (act){
            case 1 : mRequestType = RequestTypeKey; break;
            case 2 : mRequestType = RequestTypeLongKey; break;
            case 3 : mRequestType = RequestTypeKeyUpAndDowns; break;
            case 4 : mRequestType = RequestTypeCancelKey; break;
            case 5 : mRequestType = RequestTypeCancelLongKey; break;
            case 6 : mRequestType = RequestTypeCancelKeyUpAndDowns; break;
        }
        return mRequestType!=RequestTypeUndefined;
    }else{
        return false;
    }
}

bool CaptureRequest::setKey(QAction* action, QMap<QString, Qt::Key> *map)
{
    if (!action || !map || map->count()==0)
        return false;
    
    QString key = action->data().toString();
    
    if (!key.isNull() && map->contains(key)) {
        mKey = map->value(key);
        return true;
    }
    
    return false;
}

bool CaptureRequest::setLongFlags(QAction* action, QMap<QString, XQKeyCapture::LongFlags> *map)
{
    if (!action || !map || map->count()==0)
        return false;
    
    QString flag = action->data().toString();
    
    if ( !flag.isNull() && map->contains(flag)){
        mLongFlags = map->value(flag);
        return true;
    }
    
    return false;
}
