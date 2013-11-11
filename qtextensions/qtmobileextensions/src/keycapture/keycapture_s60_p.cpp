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

#include "keycapture_s60_p.h"
#include "capturerequest_s60.h"

#include "keymapper.h"

#include <w32std.h>
#include <coemain.h> 
#include <eikenv.h> 

#include "targetwrapper.h"
#include "txlogger.h"

#ifdef _XQKEYCAPTURE_UNITTEST_
    #include "tsrc\mytestwindowgroup.h"
#endif

int KeyCapturePrivate::mRemoteEventType_KeyPress = 0;
int KeyCapturePrivate::mRemoteEventType_KeyRelease = 0;

KeyCapturePrivate::KeyCapturePrivate() :
    mLastError(KErrNone), mLastErrorString(QString("OK")),
#ifndef _XQKEYCAPTURE_UNITTEST_
        mWindowGroup( CEikonEnv::Static()->RootWin()), 
#else
        mWindowGroup( *MyTestWindowGroup::Instance()),
#endif
        mRequestsList(new QList<CaptureRequest*> ()),
        mMapper(new QKeyMapper()),
        tgWrapper(new TargetWrapper())
{

}

KeyCapturePrivate::~KeyCapturePrivate()
{
    if (mRequestsList){
        qDeleteAll(mRequestsList->begin(), mRequestsList->end());
        mRequestsList->clear();
    }
    delete mRequestsList;
    delete mMapper;
    delete tgWrapper;
}

bool KeyCapturePrivate::doCapture(TUint aKey,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier,
        CaptureRequest::CaptureRequestType aType,
        XQKeyCapture::LongFlags aLongType)
{
    int err = mLastError;
    CaptureRequest *req = new CaptureRequest(aKey, aModifiersMask, aModifier,
            aType, aLongType, &mWindowGroup);
    mLastError = req->request();
    mRequestsList->append(req);
    if (err != mLastError)
        regenerateError();

    return errorId() == KErrNone;
}

bool KeyCapturePrivate::doCancelCapture(TUint aKey,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier, 
        CaptureRequest::CaptureRequestType aType,
        XQKeyCapture::LongFlags aLongType)
{
    int err = mLastError;

    for (int i(0), size(mRequestsList->count()); i < size; i++) {
        CaptureRequest *request = mRequestsList->at(i);
        if (request && request->matches(aKey, aModifiersMask, aModifier, aType,
                aLongType)) {
            mLastError = request->cancel();
            mRequestsList->removeAt(i);
            delete request;
            break;
        }
    }

    if (err != mLastError)
        regenerateError();

    return errorId() == KErrNone;
}

QString KeyCapturePrivate::errorString() const
{
    return mLastErrorString;
}

int KeyCapturePrivate::errorId() const
{
    return mLastError;
}

void KeyCapturePrivate::regenerateError()
{
    if (mLastError != KErrNone) {
        mLastErrorString = QString("ERROR: %1").arg(mLastError);
    } else {
        mLastErrorString = QString("OK");
    }
}

bool KeyCapturePrivate::initRemote(XQKeyCapture::CapturingFlags flags)
{
    int err;
    QT_TRYCATCH_ERROR(err, tgWrapper->init(flags));
    mLastError = err;
    if (err != mLastError)
        regenerateError();

    return errorId() == KErrNone;
}

bool KeyCapturePrivate::closeRemote(XQKeyCapture::CapturingFlags flags)
{
    int err;
    QT_TRYCATCH_ERROR(err, tgWrapper->close(flags));
    mLastError = err;
    if (err != mLastError)
        regenerateError();

    return errorId() == KErrNone;
}
