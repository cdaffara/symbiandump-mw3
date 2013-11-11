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

#include "keycapture_stub_p.h"

int KeyCapturePrivate::mRemoteEventType_KeyPress = 0;
int KeyCapturePrivate::mRemoteEventType_KeyRelease = 0;

KeyCapturePrivate::KeyCapturePrivate()
{

}

KeyCapturePrivate::~KeyCapturePrivate()
{
}

bool KeyCapturePrivate::initRemote(XQKeyCapture::CapturingFlags flags, XQKeyCapture::HandlingEvents events)
{
    Q_UNUSED(flags);
    Q_UNUSED(events);
    
    return false;
}

bool KeyCapturePrivate::closeRemote(XQKeyCapture::CapturingFlags flags, XQKeyCapture::HandlingEvents events)
{
    Q_UNUSED(flags);
    Q_UNUSED(events);
    
    return false;
}

bool KeyCapturePrivate::doCapture(TUint aKey,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier,
        CaptureRequest::CaptureRequestType aType,
        XQKeyCapture::LongFlags aLongType)
{
    Q_UNUSED(aKey);
    Q_UNUSED(aModifiersMask);
    Q_UNUSED(aModifier);
    Q_UNUSED(aType);
    Q_UNUSED(aLongType);
    return false;
}

bool KeyCapturePrivate::doCancelCapture(TUint aKey,
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier, 
        CaptureRequest::CaptureRequestType aType,
        XQKeyCapture::LongFlags aLongType)
{
    Q_UNUSED(aKey);
    Q_UNUSED(aModifiersMask);
    Q_UNUSED(aModifier);
    Q_UNUSED(aType);
    Q_UNUSED(aLongType);
    return false;
}

QString KeyCapturePrivate::errorString() const
{
    return QString();
}

int KeyCapturePrivate::errorId() const
{
    return 0;
}

