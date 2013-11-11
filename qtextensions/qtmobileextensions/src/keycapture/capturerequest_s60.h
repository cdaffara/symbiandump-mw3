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

#ifndef CAPTUREREQUEST_S60_H
#define CAPTUREREQUEST_S60_H

#include <qnamespace.h>
#include <QMap>

#include "xqkeycapture.h"

class RWindowGroup;
class QKeyMapper;

#ifdef _XQKEYCAPTURE_UNITTEST_
    class MyTestWindowGroup;
#endif

class CaptureRequest {

public:
    enum CaptureRequestType {
    CaptureRequestTypeNormal,
    CaptureRequestTypeLong,
    CaptureRequestTypeUpAndDown
    };

    CaptureRequest(TUint aKey, Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier, CaptureRequestType aRequestType, XQKeyCapture::LongFlags aLongKeyFlags,
#ifndef _XQKEYCAPTURE_UNITTEST_
    RWindowGroup* aGroup
#else
    MyTestWindowGroup* aGroup
#endif
    );

    ~CaptureRequest();

    int request();

    int cancel();

    bool operator==(const CaptureRequest& other) const;

    bool matches(TUint aKey, Qt::KeyboardModifiers aModifiersMask, Qt::KeyboardModifiers aModifier,
        CaptureRequestType aRequestType, 
        XQKeyCapture::LongFlags captureLongFlag = XQKeyCapture::LongWaitNotApplicable) const;

private:
    TUint translatedModifierMask();
    TUint translatedModifier();
    TUint translatedModifier(Qt::KeyboardModifiers aModifier);
    
private:
    const TUint mKey;
    const Qt::KeyboardModifiers mModifiersMask;
    const Qt::KeyboardModifiers mModifier;
    const CaptureRequestType mRequestType;
    const XQKeyCapture::LongFlags longKeyFlags;
    
    long int mRequestHandle;
    long int mAdditionalRequestHandle;
    
#ifndef _XQKEYCAPTURE_UNITTEST_
    RWindowGroup* mGroup; //not owned
#else
    MyTestWindowGroup* mGroup;
#endif
    QKeyMapper* mMapper;
//    QMap<Qt::Key, TUint> *mQtToS60KeyMap;//not owned
//    QMap<Qt::Key, TUint> *mQtToS60StdScanCodeMap;    //not owned
};

#endif /* CAPTUREREQUEST_S60_H */

