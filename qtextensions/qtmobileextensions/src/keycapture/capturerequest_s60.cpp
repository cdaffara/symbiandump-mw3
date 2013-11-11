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

#include "capturerequest_s60.h"
#include <w32std.h>
#include "txlogger.h"

#ifdef _XQKEYCAPTURE_UNITTEST_
    #include "tsrc\mytestwindowgroup.h"
#endif

CaptureRequest::CaptureRequest(TUint aKey, Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier, CaptureRequestType aRequestType, 
        XQKeyCapture::LongFlags aLongKeyFlags, 
#ifndef _XQKEYCAPTURE_UNITTEST_
    RWindowGroup* aGroup
#else
    MyTestWindowGroup* aGroup
#endif
    ):
    mKey(aKey), 
    mModifiersMask(aModifiersMask), 
    mModifier(aModifier), 
    mRequestType(aRequestType),
    longKeyFlags(aLongKeyFlags),
    mRequestHandle(-1), 
    mAdditionalRequestHandle(-1),
    mGroup(aGroup)  
{
}

CaptureRequest::~CaptureRequest()
{
}

int CaptureRequest::request()
{
    TX_ENTRY
    int res(KErrNone);
    if (!mGroup) {
        res = KErrNotReady;
        TX_LOG_ARGS( QString("!mGroup"));
    } else {
        switch (mRequestType) {
        case CaptureRequestTypeNormal:
            mRequestHandle = mGroup->CaptureKey(mKey, translatedModifierMask(),
                translatedModifier());
            TX_LOG_ARGS( QString("CaptureKey done"))
            break;
        case CaptureRequestTypeLong:
            mRequestHandle = mGroup->CaptureLongKey(mKey, mKey,
                translatedModifierMask(), translatedModifier(), 0, longKeyFlags);
            TX_LOG_ARGS( QString("CaptureLongKey done"))
            break;
        case CaptureRequestTypeUpAndDown:
            mRequestHandle = mGroup->CaptureKeyUpAndDowns(mKey,
                translatedModifierMask(), translatedModifier());
            TX_LOG_ARGS( QString("CaptureKeyUpAndDowns done"))
            break;
        default:
            res = KErrNotSupported;
            TX_LOG_ARGS( QString("request not done"))
            break;
        }
        
        if (mRequestHandle < 0){
            res = mRequestHandle;
        } else { // following probably is not needed, it could be used to capture Modifires keys as normal keys
            //In Qt you might request capture CTRL key, in symbian you have to capture Left and Right CTRL keys
            bool additionalAction = false;
            TUint additionalKey = 0;
            switch(mKey){
                case EKeyLeftShift:
                    additionalAction = true;
                    additionalKey = (mRequestType == CaptureRequestTypeUpAndDown) ? EStdKeyRightShift : EKeyRightShift;
                    break;
                case EKeyLeftCtrl:
                    additionalAction = true;
                    additionalKey = (mRequestType == CaptureRequestTypeUpAndDown) ? EStdKeyRightCtrl : EKeyRightCtrl;
                    break;
            }

            if (additionalAction && additionalKey != 0) {
                TX_LOG_ARGS( QString("additionalAction needed"));
                switch (mRequestType) {
                case CaptureRequestTypeNormal:
                    mAdditionalRequestHandle = mGroup->CaptureKey(additionalKey,
                        translatedModifierMask(), translatedModifier());
                    break;
                case CaptureRequestTypeLong:
                    mAdditionalRequestHandle = mGroup->CaptureLongKey(additionalKey, additionalKey,
                        translatedModifierMask(), translatedModifier(), 0, longKeyFlags);
                    break;
                case CaptureRequestTypeUpAndDown:
                    mAdditionalRequestHandle = mGroup->CaptureKeyUpAndDowns(additionalKey,
                        translatedModifierMask(), translatedModifier());
                    break;
                default:
                    res = KErrNotSupported;
                    break;
                }
                if (mAdditionalRequestHandle < 0)
                    res = mAdditionalRequestHandle;
            }
        }
    }
    TX_EXIT
    return res;
}

int CaptureRequest::cancel()
{
    TX_ENTRY
    int res(KErrNone);
    if (mRequestHandle < 0 || !mGroup) {
    res = KErrNotReady;
    } else {
    switch (mRequestType) {
        case CaptureRequestTypeNormal:
        mGroup->CancelCaptureKey(mRequestHandle);
        break;
        case CaptureRequestTypeLong:
        mGroup->CancelCaptureLongKey(mRequestHandle);
        break;
        case CaptureRequestTypeUpAndDown:
        mGroup->CancelCaptureKeyUpAndDowns(mRequestHandle);
        break;
        default:
        res = KErrNotSupported;
        break;
    }
    if (mAdditionalRequestHandle > -1) {
        switch (mRequestType) {
        case CaptureRequestTypeNormal:
            mGroup->CancelCaptureKey(mAdditionalRequestHandle);
            break;
        case CaptureRequestTypeLong:
            mGroup->CancelCaptureLongKey(mAdditionalRequestHandle);
            break;
        case CaptureRequestTypeUpAndDown:
            mGroup->CancelCaptureKeyUpAndDowns(mAdditionalRequestHandle);
            break;
        default:
            res = KErrNotSupported;
            break;
        }
    }
    }TX_EXIT
    return res;
}

bool CaptureRequest::operator==(const CaptureRequest& other) const
{
    bool res = mKey == other.mKey && mModifiersMask == other.mModifiersMask && mModifier
        == other.mModifier && mRequestType == other.mRequestType;
    return res;
}

bool CaptureRequest::matches(TUint aKey, Qt::KeyboardModifiers aModifiersMask,
    Qt::KeyboardModifiers aModifier, CaptureRequestType aRequestType,
    XQKeyCapture::LongFlags captureLongFlag) const
{
    bool res = mKey == aKey && mModifiersMask == aModifiersMask && mModifier == aModifier
        && mRequestType == aRequestType && 
            longKeyFlags == captureLongFlag;
    return res;
}

TUint CaptureRequest::translatedModifierMask()
{
    TUint res = translatedModifier(mModifiersMask);
    return res;
}

TUint CaptureRequest::translatedModifier()
{
    TUint res = translatedModifier(mModifier);
    return res;
}

TUint CaptureRequest::translatedModifier(Qt::KeyboardModifiers aModifier)
{
    TUint res = 0;
    if (aModifier & Qt::ShiftModifier) {
        res |= EModifierShift;
    }

    if (aModifier & Qt::ControlModifier) {
        res |= EModifierCtrl;
    }

    if (aModifier & Qt::AltModifier) {
        res |= EModifierAlt;
    }

    if (aModifier & Qt::KeypadModifier) {
        res |= EModifierKeypad;
    }
    return res;
}

