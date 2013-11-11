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

#include "xqkeycapture.h"

#include "keycapture_p.h"
#include "keymapper.h"
#include <qnamespace.h>
#include <qcoreevent.h>

/*!
 Constructor.
*/
XQKeyCapture::XQKeyCapture() :
    d(new KeyCapturePrivate())
{

}

/*!
 Destructor.
*/
XQKeyCapture::~XQKeyCapture()
{
    delete d;
}

/*!
 Initializes remote key handler(s) given by parameter.    
 \param flags  Flag representing supported remote handler. Flags can be ORed for simulteanous initializations.  
               CaptureEnableRemoteExtEvents flag adds posibility to receive native events from remote device. 
               QKeyEvent::nativeVirtualKey() function returns native remote key code. 
               Important: Extended remote events have different custome type. Static functions 
               XQKeyCapture::remoteKeyPress() and XQKeyCapture::remoteKeyRelease() return custom
               event types.  
 */
bool XQKeyCapture::captureRemoteKeys(CapturingFlags flags)
{
    return d->initRemote(flags);
}
 
/*!
 Cancels remote key handler(s) given by parameter.   
 \param flags  Flag representing supported remote interfaces. Flags can be ORed for simulteanous cancellations.  
 */
bool XQKeyCapture::cancelCaptureRemoteKeys(CapturingFlags flags)
{
    return d->closeRemote(flags);
}

/*!
 Returns type of remote key press event. Event type is assigned dynamically.   
 \retval returns type of remote key press event.  
 */
QEvent::Type XQKeyCapture::remoteEventType_KeyPress()
{
    if (KeyCapturePrivate::mRemoteEventType_KeyPress == 0)
        KeyCapturePrivate::mRemoteEventType_KeyPress = QEvent::registerEventType();
    
    return (QEvent::Type)KeyCapturePrivate::mRemoteEventType_KeyPress;
}

/*!
 Returns type of remote key release event. Event type is assigned dynamically.    
 \retval returns type of remote key release event.  
 */
QEvent::Type XQKeyCapture::remoteEventType_KeyRelease()
{
    if (KeyCapturePrivate::mRemoteEventType_KeyRelease == 0)
        KeyCapturePrivate::mRemoteEventType_KeyRelease = QEvent::registerEventType();
    
    return (QEvent::Type)KeyCapturePrivate::mRemoteEventType_KeyRelease;
}

/*!
 Selects a given key for capturing key pressing. Requires a Qt key code.
 \param aKey A Qt key.
 \param aModifiersMask
 \param aModifier 
 \retval Returns true if aKey was succesfully added to the capturing system, otherwise returns false.
 */
bool XQKeyCapture::captureKey(Qt::Key aKey,
    Qt::KeyboardModifiers aModifiersMask, Qt::KeyboardModifiers aModifier)
{
    return d->doCapture(d->mMapper->mapQtToS60Key(aKey), aModifiersMask, aModifier,
        CaptureRequest::CaptureRequestTypeNormal);
}

/*!
 Selects a given key for capturing key pressing. Requires a S60 key code (TKeyCode).
 \param aKey A S60 key code (TKeyCode).
 \param aModifiersMask
 \param aModifier 
 \retval Returns true if aKey was succesfully added to the capturing system, otherwise returns false.
 */
bool XQKeyCapture::captureKey(TUint aKey,
    Qt::KeyboardModifiers aModifiersMask, Qt::KeyboardModifiers aModifier)
{
    return d->doCapture(aKey, aModifiersMask, aModifier,
        CaptureRequest::CaptureRequestTypeNormal);
}

/*!
 Selects a given key for capturing long pressing. Requires a Qt key code.
 \param aKey A Qt key.
 \param aModifiersMask
 \param aModifier 
 \retval Returns true if aKey was succesfully added to the capturing system, otherwise returns false.
 */
bool XQKeyCapture::captureLongKey(Qt::Key aKey,
    Qt::KeyboardModifiers aModifiersMask, Qt::KeyboardModifiers aModifier,
    XQKeyCapture::LongFlags aLongType)
{
    return d->doCapture(d->mMapper->mapQtToS60Key(aKey), aModifiersMask, aModifier,
        CaptureRequest::CaptureRequestTypeLong, aLongType);
}

/*!
 Selects a given key for capturing long pressing. Requires a S60 key code (TKeyCode).
 \param aKey A S60 key code (TKeyCode).
 \param aModifiersMap
 \param aModifier 
 \retval Returns true if aKey was succesfully added to the capturing system, otherwise returns false.
 */
bool XQKeyCapture::captureLongKey(TUint aKey,
    Qt::KeyboardModifiers aModifiersMap, Qt::KeyboardModifiers aModifier,
    XQKeyCapture::LongFlags aLongType)
{
    return d->doCapture(aKey, aModifiersMap, aModifier,
        CaptureRequest::CaptureRequestTypeLong, aLongType);
}

/*!
 Selects a given key for capturing pressing up and down. Requires a Qt key code.
 \param aKey A Qt key.
 \param aModifiersMask
 \param aModifier 
 \retval Returns true if aKey was succesfully added to the capturing system, otherwise returns false.
 */
bool XQKeyCapture::captureKeyUpAndDowns(Qt::Key aKey,
    Qt::KeyboardModifiers aModifiersMask, Qt::KeyboardModifiers aModifier)
{
    return d->doCapture(d->mMapper->mapQtToS60ScanCodes(aKey), aModifiersMask,
        aModifier, CaptureRequest::CaptureRequestTypeUpAndDown);
}

/*!
 Selects a given key for capturing pressing up and down. Requires a S60 key scan code (TStdScanCode).
 \param aKey A S60 key scan code (TStdScanCode).
 \param aModifiersMask
 \param aModifier 
 \retval Returns true if aKey was succesfully added to the capturing system, otherwise returns false.
 */
bool XQKeyCapture::captureKeyUpAndDowns(TUint aKey,
    Qt::KeyboardModifiers aModifiersMask, Qt::KeyboardModifiers aModifier)
{
    return d->doCapture(aKey, aModifiersMask,
        aModifier, CaptureRequest::CaptureRequestTypeUpAndDown);
}

/*!
 Deselects a given key from key capturing. Requires a Qt key code.
 \param aKey A Qt key.
 \param aModifiersMask
 \param aModifier 
 \retval Returns true if aKey was succesfully removed from the capturing system, otherwise returns false.
 */
bool XQKeyCapture::cancelCaptureKey(Qt::Key aKey,
    Qt::KeyboardModifiers aModifiersMask, Qt::KeyboardModifiers aModifier)
{
    return d->doCancelCapture(d->mMapper->mapQtToS60Key(aKey),
        aModifiersMask, aModifier,
        CaptureRequest::CaptureRequestTypeNormal);
}

/*!
 Deselects a given key from key capturing. Requires a S60 key code (TKeyCode).
 \param aKey A S60 key code (TKeyCode).
 \param aModifiersMask
 \param aModifier 
 \retval Returns true if aKey was succesfully removed from the capturing system, otherwise returns false.
 */
bool XQKeyCapture::cancelCaptureKey(TUint aKey,
    Qt::KeyboardModifiers aModifiersMask, Qt::KeyboardModifiers aModifier)
{
    return d->doCancelCapture(aKey, aModifiersMask, aModifier,
        CaptureRequest::CaptureRequestTypeNormal);
}

/*!
 Deselects a given key from capturing long pressing. Requires a Qt key code.
 \param aKey A Qt key.
 \param aModifiersMask
 \param aModifier 
 \retval Returns true if aKey was succesfully removed from the capturing system, otherwise returns false.
 */
bool XQKeyCapture::cancelCaptureLongKey(Qt::Key aKey,
    Qt::KeyboardModifiers aModifiersMask, Qt::KeyboardModifiers aModifier,
    XQKeyCapture::LongFlags aLongType)
{
    return d->doCancelCapture(d->mMapper->mapQtToS60Key(aKey), aModifiersMask,
        aModifier, CaptureRequest::CaptureRequestTypeLong, aLongType);
}

/*!
 Deselects a given key from capturing long pressing. Requires a S60 key code (TKeyCode).
 \param aKey A S60 key code (TKeyCode).
 \param aModifiersMask
 \param aModifier 
 \retval Returns true if aKey was succesfully removed from the capturing system, otherwise returns false.
 */
bool XQKeyCapture::cancelCaptureLongKey(TUint aKey,
    Qt::KeyboardModifiers aModifiersMask, Qt::KeyboardModifiers aModifier,
    XQKeyCapture::LongFlags aLongType)
{
    return d->doCancelCapture(aKey, aModifiersMask, aModifier,
        CaptureRequest::CaptureRequestTypeLong, aLongType);
}

/*!
 Deselects a given key from capturing pressing up and down. Requires a Qt key code.
 \param aKey A Qt key.
 \param aModifiersMask
 \param aModifier 
 \retval Returns true if aKey was succesfully removed from the capturing system, otherwise returns false.
 */
bool XQKeyCapture::cancelCaptureKeyUpAndDowns(Qt::Key aKey,
    Qt::KeyboardModifiers aModifiersMask, Qt::KeyboardModifiers aModifier)
{
    return d->doCancelCapture(d->mMapper->mapQtToS60ScanCodes(aKey),
        aModifiersMask, aModifier,
        CaptureRequest::CaptureRequestTypeUpAndDown);
}

/*!
 Deselects a given key from capturing pressing up and down. Requires a S60 key scan code (TStdScanCode).
 \param aKey A S60 key scan code (TStdScanCode).
 \param aModifiersMask
 \param aModifier 
 \retval Returns true if aKey was succesfully removed from the capturing system, otherwise returns false.
 */
bool XQKeyCapture::cancelCaptureKeyUpAndDowns(TUint aKey,
    Qt::KeyboardModifiers aModifiersMask, Qt::KeyboardModifiers aModifier)
{
    return d->doCancelCapture(aKey, aModifiersMask, aModifier,
        CaptureRequest::CaptureRequestTypeUpAndDown);
}

/*!
 Returns latest error string.
 \retval Latest error string.
 */
QString XQKeyCapture::errorString() const
{
    return d->errorString();
}

/*!
 Returns latest error id.
 \retval Latest error id.
 */
int XQKeyCapture::errorId() const
{
    return d->errorId();
}

/*!
 Selects a given keys for capturing key pressing. Requires a Qt key code.
 \param list A Qt keys list.
 \param aModifiersMask
 \param aModifier 
 \retval Returns true if aKey was succesfully added to the capturing system, otherwise returns false.
 */
bool XQKeyCapture::captureKey(XQKeyCaptureKeyList list, 
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier)
{
    Qt::Key key;
    bool result = true;
    foreach (key, list) {
        result = result & captureKey(key, aModifiersMask, aModifier);
    }
    return result;
}

/*!
 Selects a given keys for capturing key pressing. Requires a S60 key code (TKeyCode).
 \param list A S60 list of keys (TKeyCode codes).
 \param aModifiersMask
 \param aModifier 
 \retval Returns true if aKey was succesfully added to the capturing system, otherwise returns false.
 */
bool XQKeyCapture::captureKey(XQKeyCaptureNativeKeyList list, 
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier)
{
    TUint key;
    bool result = true;
    foreach (key, list) {
        result = result & captureKey(key, aModifiersMask, aModifier);
    }
    return result;
}

/*!
 Selects a given keys for capturing long pressing. Requires a Qt key code.
 \param aKey A Qt  list of keys.
 \param aModifiersMask
 \param aModifier 
 \retval Returns true if aKey was succesfully added to the capturing system, otherwise returns false.
 */
bool XQKeyCapture::captureLongKey(XQKeyCaptureKeyList list, 
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier,
        XQKeyCapture::LongFlags aLongType)
{
    Qt::Key key;
    bool result = true;
    foreach (key, list) {
        result = result & captureLongKey(key, aModifiersMask, aModifier, aLongType);
    }
    return result;
}

/*!
 Selects a given keys for capturing long pressing. Requires a S60 key code (TKeyCode).
 \param list A S60 list of keys (TKeyCode codes).
 \param aModifiersMap
 \param aModifier 
 \retval Returns true if aKey was succesfully added to the capturing system, otherwise returns false.
 */
 bool XQKeyCapture::captureLongKey(XQKeyCaptureNativeKeyList list, 
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier,
        XQKeyCapture::LongFlags aLongType)
{
    TUint key;
    bool result = true;
    foreach (key, list) {
        result = result & captureLongKey(key, aModifiersMask, aModifier, aLongType);
    }
    return result;
}

/*!
 Selects a given keys for capturing pressing up and down. Requires a Qt key code.
 \param list A Qt list of keys.
 \param aModifiersMask
 \param aModifier 
 \retval Returns true if aKey was succesfully added to the capturing system, otherwise returns false.
 */
bool XQKeyCapture::captureKeyUpAndDowns(XQKeyCaptureKeyList list, 
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier)
{
    Qt::Key key;
    bool result = true;
    foreach (key, list) {
        result = result & captureKeyUpAndDowns(key, aModifiersMask, aModifier);
    }
    return result;
}

/*!
 Selects a given keys for capturing pressing up and down. Requires a S60 key scan code (TStdScanCode).
 \param list A list of S60 key scan codes (TStdScanCode).
 \param aModifiersMask
 \param aModifier 
 \retval Returns true if aKey was succesfully added to the capturing system, otherwise returns false.
 */
bool XQKeyCapture::captureKeyUpAndDowns(XQKeyCaptureNativeKeyList list, 
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier)
{
    TUint key;
    bool result = true;
    foreach (key, list) {
        result = result & captureKeyUpAndDowns(key, aModifiersMask, aModifier);
    }
    return result;
}

/*!
 Deselects a given list of keys from key capturing. Requires a Qt key code.
 \param list  A Qt list of keys.
 \param aModifiersMask
 \param aModifier 
 \retval Returns true if aKey was succesfully removed from the capturing system, otherwise returns false.
 */
bool XQKeyCapture::cancelCaptureKey(XQKeyCaptureKeyList list, 
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier)
{
    Qt::Key key;
    bool result = true;
    foreach (key, list) {
        result = result & cancelCaptureKey(key, aModifiersMask, aModifier);
    }
    return result;
}

/*!
 Deselects a given list of keys from key capturing. Requires a S60 key code (TKeyCode).
 \param list  A S60 list of key codes (TKeyCode).
 \param aModifiersMask
 \param aModifier 
 \retval Returns true if aKey was succesfully removed from the capturing system, otherwise returns false.
 */
bool XQKeyCapture::cancelCaptureKey(XQKeyCaptureNativeKeyList list, 
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier)
{
    TUint key;
    bool result = true;
    foreach (key, list) {
        result = result & cancelCaptureKey(key, aModifiersMask, aModifier);
    }
    return result;
}

/*!
 Deselects a given list of keys from capturing long pressing. Requires a Qt key code.
 \param list A list of Qt keys.
 \param aModifiersMask
 \param aModifier 
 \retval Returns true if aKey was succesfully removed from the capturing system, otherwise returns false.
 */
bool XQKeyCapture::cancelCaptureLongKey(XQKeyCaptureKeyList list, 
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier,
        XQKeyCapture::LongFlags aLongType)
{
    Qt::Key key;
    bool result = true;
    foreach (key, list) {
        result = result & cancelCaptureLongKey(key, aModifiersMask, aModifier, aLongType);
    }
    return result;
}

/*!
 Deselects a given key from capturing long pressing. Requires a S60 key code (TKeyCode).
 \paramlist A list of S60 key codes (TKeyCode).
 \param aModifiersMask
 \param aModifier 
 \retval Returns true if aKey was succesfully removed from the capturing system, otherwise returns false.
 */
bool XQKeyCapture::cancelCaptureLongKey(XQKeyCaptureNativeKeyList list, 
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier,
        XQKeyCapture::LongFlags aLongType)
{
    TUint key;
    bool result = true;
    foreach (key, list) {
        result = result & cancelCaptureLongKey(key, aModifiersMask, aModifier, aLongType);
    }
    return result;
}

/*!
 Deselects a list of given keys from capturing pressing up and down. Requires a Qt key codes.
 \param list A list of Qt keys.
 \param aModifiersMask
 \param aModifier 
 \retval Returns true if aKey was succesfully removed from the capturing system, otherwise returns false.
 */
bool XQKeyCapture::cancelCaptureKeyUpAndDowns(XQKeyCaptureKeyList list, 
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier)
{
    Qt::Key key;
    bool result = true;
    foreach (key, list) {
        result = result & cancelCaptureKeyUpAndDowns(key, aModifiersMask, aModifier);
    }
    return result;
}

/*!
 Deselects a given list of keys from capturing pressing up and down. Requires a S60 key scan code (TStdScanCode).
 \param aKey A S60 key scan code (TStdScanCode).
 \param aModifiersMask
 \param aModifier 
 \retval Returns true if aKey was succesfully removed from the capturing system, otherwise returns false.
 */
 bool XQKeyCapture::cancelCaptureKeyUpAndDowns(XQKeyCaptureNativeKeyList list, 
        Qt::KeyboardModifiers aModifiersMask,
        Qt::KeyboardModifiers aModifier)
{
    TUint key;
    bool result = true;
    foreach (key, list) {
        result = result & cancelCaptureKeyUpAndDowns(key, aModifiersMask, aModifier);
    }
    return result;
}

// end of file
