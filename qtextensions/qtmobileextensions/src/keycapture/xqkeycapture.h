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

#ifndef XQKEYCAPTURE_H
#define XQKEYCAPTURE_H

#include <qnamespace.h>
#include <QString>
#include <QObject>
#include <QList>
#include <QFlag>
#include <QEvent>

#include <w32std.h>

#ifdef XQKEYCAPTURE_LIBRARY
#define XQKEYCAPTURE_EXPORT Q_DECL_EXPORT
#else
#define XQKEYCAPTURE_EXPORT Q_DECL_IMPORT
#endif

class KeyCapturePrivate;

typedef QList<Qt::Key> XQKeyCaptureKeyList; 
typedef QList<TUint> XQKeyCaptureNativeKeyList; 

class XQKEYCAPTURE_EXPORT XQKeyCapture {

public:
    enum LongFlags {
        LongShortEventImmediately = ELongCaptureShortEventImmediately,
        LongRepeatEvents = ELongCaptureRepeatEvents,
        LongNormal = ELongCaptureNormal,
        LongWaitShort = ELongCaptureWaitShort,
        LongWaitNotApplicable = 0
    };
        
    enum CapturingFlag {
        CaptureNone = 0x0,
        CaptureBasic = 0x1,
        CaptureCallHandlingExt = 0x2,
        CaptureSideKeys = 0x4, // for future use
        CaptureEnableRemoteExtEvents = 0x4000
    };
    
    Q_DECLARE_FLAGS(CapturingFlags, CapturingFlag)
    
public:
    XQKeyCapture();

    ~XQKeyCapture();

    bool captureKey(Qt::Key aKey,
        Qt::KeyboardModifiers aModifiersMask = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier);
    
    bool captureKey(TUint aKey,
        Qt::KeyboardModifiers aModifiersMask = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier);

    bool captureLongKey(Qt::Key aKey,
        Qt::KeyboardModifiers aModifiersMask = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier,
        XQKeyCapture::LongFlags aLongType = XQKeyCapture::LongNormal);

    bool captureLongKey(TUint aKey,
        Qt::KeyboardModifiers aModifiersMask = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier,
        XQKeyCapture::LongFlags aLongType = XQKeyCapture::LongNormal);

    bool captureKeyUpAndDowns(Qt::Key aKey,
        Qt::KeyboardModifiers aModifiersMask = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier);
    
    bool captureKeyUpAndDowns(TUint aKey,
        Qt::KeyboardModifiers aModifiersMask = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier);

    bool captureKey(XQKeyCaptureKeyList list, 
        Qt::KeyboardModifiers aModifiersMask = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier);
    
    bool captureKey(XQKeyCaptureNativeKeyList list, 
        Qt::KeyboardModifiers aModifiersMask = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier);
    
    bool captureLongKey(XQKeyCaptureKeyList list, 
        Qt::KeyboardModifiers aModifiersMask = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier,
        XQKeyCapture::LongFlags aLongType = XQKeyCapture::LongNormal);
    
    bool captureLongKey(XQKeyCaptureNativeKeyList list, 
        Qt::KeyboardModifiers aModifiersMask = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier,
        XQKeyCapture::LongFlags aLongType = XQKeyCapture::LongNormal);
    
    bool captureKeyUpAndDowns(XQKeyCaptureKeyList list, 
        Qt::KeyboardModifiers aModifiersMask = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier);
    
    bool captureKeyUpAndDowns(XQKeyCaptureNativeKeyList list, 
        Qt::KeyboardModifiers aModifiersMask = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier);
        
    bool cancelCaptureKey(Qt::Key aKey,
        Qt::KeyboardModifiers aModifiersMask = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier);

    bool cancelCaptureKey(TUint aKey,
        Qt::KeyboardModifiers aModifiersMask = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier);

    bool cancelCaptureLongKey(Qt::Key aKey,
        Qt::KeyboardModifiers aModifiersMask = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier,
        XQKeyCapture::LongFlags aLongType = XQKeyCapture::LongNormal);

    bool cancelCaptureLongKey(TUint aKey,
        Qt::KeyboardModifiers aModifiersMask = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier,
        XQKeyCapture::LongFlags aLongType = XQKeyCapture::LongNormal);

    bool cancelCaptureKeyUpAndDowns(Qt::Key aKey,
        Qt::KeyboardModifiers aModifiersMask = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier);

    bool cancelCaptureKeyUpAndDowns(TUint aKey,
        Qt::KeyboardModifiers aModifiersMask = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier);
    
    bool cancelCaptureKey(XQKeyCaptureKeyList list, 
        Qt::KeyboardModifiers aModifiersMask = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier);
    
    bool cancelCaptureKey(XQKeyCaptureNativeKeyList list, 
        Qt::KeyboardModifiers aModifiersMask = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier);
    
    bool cancelCaptureLongKey(XQKeyCaptureKeyList list, 
        Qt::KeyboardModifiers aModifiersMask = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier,
        XQKeyCapture::LongFlags aLongType = XQKeyCapture::LongNormal);
    
    bool cancelCaptureLongKey(XQKeyCaptureNativeKeyList list, 
        Qt::KeyboardModifiers aModifiersMask = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier,
        XQKeyCapture::LongFlags aLongType = XQKeyCapture::LongNormal);
    
    bool cancelCaptureKeyUpAndDowns(XQKeyCaptureKeyList list, 
        Qt::KeyboardModifiers aModifiersMask = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier);
    
    bool cancelCaptureKeyUpAndDowns(XQKeyCaptureNativeKeyList list, 
        Qt::KeyboardModifiers aModifiersMask = Qt::NoModifier,
        Qt::KeyboardModifiers aModifier = Qt::NoModifier);

    bool captureRemoteKeys(CapturingFlags flags = CaptureNone);
    
    bool cancelCaptureRemoteKeys(CapturingFlags flags = CaptureNone);
    
    static QEvent::Type remoteEventType_KeyPress();
    
    static QEvent::Type remoteEventType_KeyRelease();
        
    QString errorString() const;

    int errorId() const;

private:
    KeyCapturePrivate* d;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(XQKeyCapture::CapturingFlags)

#endif /* XQKEYCAPTURE_H */
