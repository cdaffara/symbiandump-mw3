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

#include "targetwrapper.h"

#include <QMenuBar>
#include <QApplication>
#include <QVBoxLayout>
#include <QPlainTextEdit>
#include <QTimer>

#include <QDebug>
#include <QtGlobal>

#include "responsehandler.h"
#include "responsehandlerex.h"

#include "txlogger.h"

TargetWrapper::TargetWrapper()
:
selector(0),
target(0),
targetEx(0),
handler(0),
handlerEx(0)
{
    initMapping();
}

TargetWrapper::~TargetWrapper()
{
    cleanup();
}

void TargetWrapper::close(XQKeyCapture::CapturingFlags flags)
{
    captureFlags &= ~flags;
    reset();
}

void TargetWrapper::init(XQKeyCapture::CapturingFlags flags)
{
    captureFlags |= flags;
    reset();
}

void TargetWrapper::reset()
{
    TX_ENTRY
    try {
        cleanup();
    
        QT_TRAP_THROWING(selector = CRemConInterfaceSelector::NewL());
        
        if (captureFlags & XQKeyCapture::CaptureBasic) {
            QT_TRAP_THROWING(target = CRemConCoreApiTarget::NewL(*selector, *this));
            QT_TRAP_THROWING(handler = CResponseHandler::NewL(*target));
        }
         
        if (captureFlags & XQKeyCapture::CaptureCallHandlingExt) {
            QT_TRAP_THROWING(targetEx = CRemConCallHandlingTarget::NewL(*selector, *this));
            QT_TRAP_THROWING(handlerEx = CResponseHandlerEx::NewL(*targetEx));
        }
        
        QT_TRAP_THROWING(selector->OpenTargetL());
    
    } catch (const std::exception &e) {
        delete selector;
        selector = 0;
        target = 0;
        targetEx = 0;
        delete handler;
        handler = 0;
        delete handlerEx;
        handlerEx = 0;
        qDebug() << "TargetWrapper::init - exception: " << e.what();
        throw;
    }
    TX_EXIT
}

Qt::Key TargetWrapper::mapKey(TRemConCoreApiOperationId aOperationId)
{
    TX_ENTRY
    Qt::Key key = keyMapping.value(aOperationId);
    
    if (key == Qt::Key()) {
        key = Qt::Key_unknown;
    }
    TX_EXIT
    return key;
}

/*
 * Function for cleaning up selector and handlers, targets are handled via selector
 */
void TargetWrapper::cleanup()
{
    TX_ENTRY
    delete handler;
    handler = 0;
    delete handlerEx;
    handlerEx = 0;
    delete selector;
    selector = 0;
    TX_EXIT
}

void TargetWrapper::MrccatoCommand(TRemConCoreApiOperationId aOperationId, TRemConCoreApiButtonAction aButtonAct)
{
    TX_ENTRY
    if (target) {
        Qt::Key key = mapKey(aOperationId); 
        switch (aButtonAct) {
            case ERemConCoreApiButtonPress:
                TX_LOG_ARGS("ERemConCoreApiButtonPress");
                sendKey(QEvent::KeyPress, key, Qt::NoModifier, aOperationId);
                break;
            case ERemConCoreApiButtonRelease:
                TX_LOG_ARGS("ERemConCoreApiButtonRelease");
                sendKey(QEvent::KeyRelease, key, Qt::NoModifier, aOperationId);
                break;
            case ERemConCoreApiButtonClick:
                TX_LOG_ARGS("ERemConCoreApiButtonClick");
                sendKey(QEvent::KeyPress, key, Qt::NoModifier, aOperationId);
                sendKey(QEvent::KeyRelease, key, Qt::NoModifier, aOperationId);
                break;
            default:
                TX_EXIT
                return;
        }
    } else {
        qWarning() << "target in MrccatoCommand was not initialized";
    }
    
    if (handler) {
        handler->CompleteAnyKey(aOperationId);
    } else {
        qWarning() << "handler in MrccatoCommand was not initialized";
    }
    TX_EXIT
}

void TargetWrapper::AnswerCall()
{
    TX_ENTRY
    if (targetEx) {
        sendKey(QEvent::KeyPress, Qt::Key_Call, Qt::NoModifier);
        sendKey(QEvent::KeyRelease, Qt::Key_Call, Qt::NoModifier);
    } else {
        qWarning() << "targetEx in AnswerCall was not initialized";
    }
    if (handlerEx) { 
        handlerEx->CompleteAnyKey(0);
    } else {
        qWarning() << "handlerEx in AnswerCall was not initialized";
    }
    TX_EXIT
}

void TargetWrapper::EndCall()
{
    TX_ENTRY
    if (targetEx) {
        sendKey(QEvent::KeyPress, Qt::Key_Hangup, Qt::NoModifier);
        sendKey(QEvent::KeyRelease, Qt::Key_Hangup, Qt::NoModifier);
    } else {
        qWarning() << "targetEx in EndCall was not initialized";
    }
    if (handlerEx) {
        handlerEx->CompleteAnyKey(0);
    } else {
        qWarning() << "handlerEx in EndCall was not initialized";
    }
    TX_EXIT
}

void TargetWrapper::AnswerEndCall()
{
    TX_ENTRY
    if (targetEx) {
        sendKey(QEvent::KeyPress, Qt::Key_Hangup, Qt::NoModifier);  //TODO: Qt::Key_ToggleCallHangup
        sendKey(QEvent::KeyRelease, Qt::Key_Hangup, Qt::NoModifier); 
    } else {
        qWarning() << "targetEx in AnswerEndCall was not initialized";
    }    
    if (handlerEx) {
        handlerEx->CompleteAnyKey(0);
    } else {
        qWarning() << "handlerEx in AnswerEndCall was not initialized";
    }
    TX_EXIT    
}

void TargetWrapper::VoiceDial( const TBool aActivate )
{
    TX_ENTRY
    Q_UNUSED(aActivate)
    TX_EXIT
}

void TargetWrapper::LastNumberRedial()
{
    TX_ENTRY
    TX_EXIT
}

void TargetWrapper::DialCall( const TDesC8& aTelNumber )
{
    TX_ENTRY
    Q_UNUSED(aTelNumber)
    TX_EXIT
}

void TargetWrapper::MultipartyCalling( const TDesC8& aData )
{
    TX_ENTRY
    Q_UNUSED(aData)
    TX_EXIT
}

void TargetWrapper::GenerateDTMF( const TChar aChar )
{
    TX_ENTRY
    Q_UNUSED(aChar)
    TX_EXIT
}

void TargetWrapper::SpeedDial( const TInt aIndex )    
{
    TX_ENTRY
    Q_UNUSED(aIndex)
    TX_EXIT
}

void TargetWrapper::sendKey(QEvent::Type eventType, Qt::Key key, Qt::KeyboardModifiers modFlags, 
                    TRemConCoreApiOperationId aOperationId)
{
    TX_ENTRY
    QWidget *widget = getTargetWidget();
    if (widget) {
        QKeyEvent *event = NULL;
        if (captureFlags & XQKeyCapture::CaptureEnableRemoteExtEvents){
            if (eventType == QEvent::KeyPress){
                event = QKeyEvent::createExtendedKeyEvent(XQKeyCapture::remoteEventType_KeyPress(), 
                        key, modFlags, 0, aOperationId, 0);
            } else if (eventType == QEvent::KeyRelease){
                event = QKeyEvent::createExtendedKeyEvent(XQKeyCapture::remoteEventType_KeyRelease(), 
                        key, modFlags, 0, aOperationId, 0);
            }
        } else {
            event = new QKeyEvent(eventType, key, modFlags);
        }
        
        if (event) {
            TX_LOG_ARGS("Sending event: " << event->key() << ", " << event->modifiers() << "[" << event->nativeScanCode() << ", " << event->nativeVirtualKey() << ", " << event->nativeModifiers() << "]");
            QApplication::sendEvent(widget, event);
            TX_LOG_ARGS("Event sent: " << event->key());
            delete event;
        }
    }
    TX_EXIT
}

QWidget *TargetWrapper::getTargetWidget()
{
    TX_ENTRY
    QWidget *widget;
    widget = QWidget::keyboardGrabber();
    if (!widget) {
        widget = QApplication::focusWidget();
    }
    if (!widget) {
        if (QApplication::activePopupWidget()) {
            widget = QApplication::activePopupWidget()->focusWidget();
            if (!widget) {
                widget = QApplication::activePopupWidget();
            }
        }
    }
    TX_EXIT
    return widget;
}



void TargetWrapper::initMapping()
{
    TX_ENTRY
    keyMapping.insert(ERemConCoreApiSelect, Qt::Key_Select);
    keyMapping.insert(ERemConCoreApiUp, Qt::Key_Up);
    keyMapping.insert(ERemConCoreApiDown, Qt::Key_Down);
    keyMapping.insert(ERemConCoreApiLeft, Qt::Key_Left); // Qt::Key_Direction_L
    keyMapping.insert(ERemConCoreApiRight, Qt::Key_Right); // Qt::Key_Direction_R
    keyMapping.insert(ERemConCoreApiRightUp, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiRightDown, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiLeftUp, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiLeftDown, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiRootMenu, Qt::Key_TopMenu); // Qt::Key_Menu
    keyMapping.insert(ERemConCoreApiSetupMenu, Qt::Key_unknown); 
    keyMapping.insert(ERemConCoreApiContentsMenu, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiFavoriteMenu, Qt::Key_Favorites); 
    keyMapping.insert(ERemConCoreApiExit, Qt::Key_unknown); // Qt::Key_Escape, Qt::Key_Cancel, Qt::Key_No
    keyMapping.insert(ERemConCoreApi0, Qt::Key_0);
    keyMapping.insert(ERemConCoreApi1, Qt::Key_1);
    keyMapping.insert(ERemConCoreApi2, Qt::Key_2);
    keyMapping.insert(ERemConCoreApi3, Qt::Key_3);
    keyMapping.insert(ERemConCoreApi4, Qt::Key_4);
    keyMapping.insert(ERemConCoreApi5, Qt::Key_5);
    keyMapping.insert(ERemConCoreApi6, Qt::Key_6);
    keyMapping.insert(ERemConCoreApi7, Qt::Key_7);
    keyMapping.insert(ERemConCoreApi8, Qt::Key_8);
    keyMapping.insert(ERemConCoreApi9, Qt::Key_9);
    keyMapping.insert(ERemConCoreApiDot, Qt::Key_Period);
    keyMapping.insert(ERemConCoreApiEnter, Qt::Key_Enter);
    keyMapping.insert(ERemConCoreApiClear, Qt::Key_Clear);
    keyMapping.insert(ERemConCoreApiChannelUp, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiChannelDown, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiPreviousChannel, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiSoundSelect, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiInputSelect, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiDisplayInformation, Qt::Key_unknown);   // Qt::Key_Time ???
    keyMapping.insert(ERemConCoreApiHelp, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiPageUp, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiPageDown, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiPower, Qt::Key_unknown); // Qt::Key_PowerOff, Qt::Key_WakeUp, Qt::Key_PowerDown, Qt::Key_Suspend
    keyMapping.insert(ERemConCoreApiVolumeUp, Qt::Key_VolumeUp);
    keyMapping.insert(ERemConCoreApiVolumeDown, Qt::Key_VolumeDown);
    keyMapping.insert(ERemConCoreApiMute, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiPlay, Qt::Key_MediaPlay);
    keyMapping.insert(ERemConCoreApiStop, Qt::Key_MediaStop);
    keyMapping.insert(ERemConCoreApiPause, Qt::Key_unknown); // NEW: Qt::Key_MediaPause
    keyMapping.insert(ERemConCoreApiRecord, Qt::Key_MediaRecord);
    keyMapping.insert(ERemConCoreApiRewind, Qt::Key_AudioRewind);
    keyMapping.insert(ERemConCoreApiFastForward, Qt::Key_AudioForward);
    keyMapping.insert(ERemConCoreApiEject, Qt::Key_Eject);
    keyMapping.insert(ERemConCoreApiForward, Qt::Key_MediaNext);
    keyMapping.insert(ERemConCoreApiBackward, Qt::Key_MediaPrevious);
    keyMapping.insert(ERemConCoreApiAngle, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiSubpicture, Qt::Key_unknown);  // Qt::Key_SplitScreen ???
    keyMapping.insert(ERemConCoreApiPausePlayFunction, Qt::Key_MediaPlay); // NEW: Media_PausePlay
    keyMapping.insert(ERemConCoreApiRestoreVolumeFunction, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiTuneFunction, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiSelectDiskFunction, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiSelectAvInputFunction, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiSelectAudioInputFunction, Qt::Key_unknown);
    keyMapping.insert(ERemConCoreApiF1, Qt::Key_F1);
    keyMapping.insert(ERemConCoreApiF2, Qt::Key_F2);
    keyMapping.insert(ERemConCoreApiF3, Qt::Key_F3);
    keyMapping.insert(ERemConCoreApiF4, Qt::Key_F4);
    keyMapping.insert(ERemConCoreApiF5, Qt::Key_F5);
    keyMapping.insert(ENop, Qt::Key_unknown);
    TX_EXIT
}
