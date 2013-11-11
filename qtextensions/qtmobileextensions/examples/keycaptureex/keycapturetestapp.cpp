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

#include <QDebug>
#include <QApplication>
#include <QKeyEvent>
#include <QLabel>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QImageReader>
#include <QDebug>
#include <QTimer>
#include <QPushButton>
#include <QLineEdit>
#include <QDebug>
#include <QString>
#include <QCheckBox>
#include <QPlainTextEdit>
#include <QString>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QEvent>


#include <QFont>

#include "keycapturetestapp.h"
#include "txlogger.h"

#include "mapping.h"
#include "capturerequest.h"
#include "mybutton.h"

KeyCaptureTestApp::KeyCaptureTestApp( QWidget *parent) : QMainWindow(parent)
{   
    TX_ENTRY
    
    qApp->installEventFilter(this);
    QCoreApplication::instance()->installEventFilter(this);
    
    
	setWindowTitle(tr("KeyCaptureTestApp"));
    
    mKeyCapture = new XQKeyCapture();
    
    mKeysMap.insert("Up", Qt::Key_Up);
    mKeysMap.insert("Down", Qt::Key_Down);
    mKeysMap.insert("Menu", Qt::Key_Menu);
    
    
    mKeysMenu = new QMenu(this);
    foreach (QString value, mKeysMap.keys())
        mKeysMenu->addAction(value)->setData( QVariant(value) );
    
    mLongFlagsMap.insert("LongNormal", XQKeyCapture::LongNormal);
    mLongFlagsMap.insert("LongRepeatEvents", XQKeyCapture::LongRepeatEvents);
    mLongFlagsMap.insert("LongShortEventImmediately", XQKeyCapture::LongShortEventImmediately);
    mLongFlagsMap.insert("LongWaitNotApplicable", XQKeyCapture::LongWaitNotApplicable);
    mLongFlagsMap.insert("LongWaitShort", XQKeyCapture::LongWaitShort);
    
    mLongFlagsMenu = new QMenu(this);
    foreach (QString value, mLongFlagsMap.keys())
        mLongFlagsMenu->addAction(value)->setData( QVariant(value) );
    
	QMenu *captureMenu = menuBar()->addMenu(QString("Capture"));
    connect(captureMenu, SIGNAL(triggered(QAction*)), this, SLOT(triggered(QAction*)));
	captureMenu->addAction(QString("Key"))->setData( QVariant(1) );
	captureMenu->addAction(QString("Long Key"))->setData( QVariant(2) );
	captureMenu->addAction(QString("Up and Down Key"))->setData( QVariant(3) );

	QMenu *cancelCaptureMenu = menuBar()->addMenu(QString("Cancel Capture"));
    connect(cancelCaptureMenu, SIGNAL(triggered(QAction*)), this, SLOT(triggered(QAction*)));
	cancelCaptureMenu->addAction(QString("Cancel Key"))->setData( QVariant(4) );
	cancelCaptureMenu->addAction(QString("Cancel Long Key"))->setData( QVariant(5) );
	cancelCaptureMenu->addAction(QString("Cancel Up and Down Key"))->setData( QVariant(6) );

    QMenu *remoteMenu = menuBar()->addMenu(QString("Remote"));

    // *** remcon ***
    
    remoteAllOn = remoteMenu->addAction(QString("Turn on all"));
    remoteAllOff = remoteMenu->addAction(QString("Turn off all"));
    
    toggleRemoteBasic = remoteMenu->addAction(QString("Basic Remote"));
    toggleRemoteBasic->setCheckable(true);
    
    toggleRemoteCallHandlingEx = remoteMenu->addAction(QString("Call Handl. Ex Remote"));
    toggleRemoteCallHandlingEx->setCheckable(true);

    toggleRemoteSideKeys = remoteMenu->addAction(QString("Side Keys Events"));
    toggleRemoteSideKeys->setCheckable(true);
    toggleRemoteSideKeys->setEnabled(false); // not implemented yet

    toggleRemoteExtEvents = remoteMenu->addAction(QString("Extended Remote Events"));
    toggleRemoteExtEvents->setCheckable(true);

    connect(toggleRemoteBasic, SIGNAL(toggled(bool)), this, SLOT(enableRemBasic(bool)));
    connect(toggleRemoteCallHandlingEx, SIGNAL(toggled(bool)), this, SLOT(enableRemCallHandlingEx(bool)));
    connect(toggleRemoteSideKeys, SIGNAL(toggled(bool)), this, SLOT(enableRemoteSideKeys(bool)));
    connect(toggleRemoteExtEvents, SIGNAL(toggled(bool)), this, SLOT(enableRemoteExtEvents(bool)));

    connect(remoteAllOn, SIGNAL(triggered(bool)), this, SLOT(remoteAll(bool)));
    connect(remoteAllOff, SIGNAL(triggered(bool)), this, SLOT(remoteNone(bool)));
    
    // *** utilities ***

    connect(menuBar()->addAction(QString("Clear Log")), SIGNAL(triggered()), this, SLOT(cleanLog()));
    connect(menuBar()->addAction(QString("Exit")), SIGNAL(triggered()), qApp, SLOT(quit()));

    QWidget *window = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout;

	mTextArea = new QPlainTextEdit("");
	mTextArea->setTextInteractionFlags(Qt::NoTextInteraction);
	QFont font = QFont(mTextArea->font());
	font.setPixelSize(10);
	mTextArea->setFont(font);
	
	layout->addWidget(new MyButton(mTextArea));
	
	layout->addWidget(mTextArea);
	
    window->setLayout(layout);
    setCentralWidget(window);
    window->show();;

    mappingPtr = new Mapping();
    
    TX_EXIT
}
	
KeyCaptureTestApp::~KeyCaptureTestApp()
{
    delete mappingPtr;
}

void KeyCaptureTestApp::triggered(QAction* aAction)
{
    CaptureRequest request(mappingPtr);
    if (!request.setType(aAction)){
        return;
    }
    if (!request.setKey(mKeysMenu->exec(), &mKeysMap)){
        return;
    }
//TODO: add modifiersMap selection menu;
//TODO: add modifier selection menu;
    if ( request.mRequestType == CaptureRequest::RequestTypeLongKey || request.mRequestType == CaptureRequest::RequestTypeCancelLongKey){
        if (!request.setLongFlags(mLongFlagsMenu->exec(), &mLongFlagsMap)){
            return;
        }
    }
    procesAction(request);
}

void KeyCaptureTestApp::cleanLog()
{
    mTextArea->setPlainText("");
}

void KeyCaptureTestApp::procesAction(CaptureRequest request)
{
    TX_ENTRY
    switch (request.mRequestType) {
        case CaptureRequest::RequestTypeKey :
            mKeyCapture->captureKey(request.mKey, request.mModifiersMap, request.mModifier );
            addTextLine(QString("%1:%2\n").arg(request.toString()).arg(mKeyCapture->errorString()));
            break;
        case CaptureRequest::RequestTypeLongKey :
            mKeyCapture->captureLongKey(request.mKey, request.mModifiersMap, request.mModifier, request.mLongFlags);
            addTextLine(QString("%1:%2\n").arg(request.toString()).arg(mKeyCapture->errorString()));
            break;
        case CaptureRequest::RequestTypeKeyUpAndDowns :
            mKeyCapture->captureKeyUpAndDowns(request.mKey, request.mModifiersMap, request.mModifier );
            addTextLine(QString("%1:%2\n").arg(request.toString()).arg(mKeyCapture->errorString()));
            break;
        case CaptureRequest::RequestTypeCancelKey :
            mKeyCapture->cancelCaptureKey(request.mKey, request.mModifiersMap, request.mModifier );
            addTextLine(QString("%1:%2\n").arg(request.toString()).arg(mKeyCapture->errorString()));
            break;
        case CaptureRequest::RequestTypeCancelLongKey :
            mKeyCapture->cancelCaptureLongKey(request.mKey, request.mModifiersMap, request.mModifier, request.mLongFlags);
            addTextLine(QString("%1:%2\n").arg(request.toString()).arg(mKeyCapture->errorString()));
            break;
        case CaptureRequest::RequestTypeCancelKeyUpAndDowns :
            mKeyCapture->cancelCaptureKeyUpAndDowns(request.mKey, request.mModifiersMap, request.mModifier );
            addTextLine(QString("%1:%2\n").arg(request.toString()).arg(mKeyCapture->errorString()));
            break;
        default:
            break;
    }

    TX_EXIT    
}

void KeyCaptureTestApp::addTextLine(QString aText)
{
    TX_ENTRY
    TX_LOG_ARGS( QString("aText=%1").arg(aText));
	if ( !aText.endsWith("\n"))
		aText = aText + "\n";
	QString msg = mTextArea->toPlainText();
	msg = aText + msg;
	mTextArea->setPlainText(msg);
	TX_EXIT
}

bool KeyCaptureTestApp::event(QEvent *ev)
{
    TX_ENTRY
//    processEvent(QString("[E]"), ev);
    bool ret = QMainWindow::event(ev);
    TX_EXIT_ARGS("ret=" << ret);
    return ret; 
}

bool KeyCaptureTestApp::eventFilter(QObject *o, QEvent *ev)
{
    TX_ENTRY
//    processEvent(QString("[F]"), ev);
    bool ret=QMainWindow::eventFilter(o, ev); 
    TX_EXIT_ARGS("ret=" << ret);
    return ret; 
}

void KeyCaptureTestApp::processEvent(const QString &prefix, QEvent *ev)
{
    TX_ENTRY_ARGS(reinterpret_cast<int>(ev));
    if (ev){
        if (ev->type() == QEvent::KeyPress){
           QKeyEvent *keyEvent = static_cast<QKeyEvent *>(ev);
           QString keyName = mappingPtr->name(static_cast<Qt::Key>(keyEvent->key())); 
           
           addTextLine(prefix + QString("KeyPress:%1\n").arg(keyName));
        } else if (ev->type() == QEvent::KeyRelease){
           QKeyEvent *keyEvent = static_cast<QKeyEvent *>(ev);
           QString keyName = mappingPtr->name(static_cast<Qt::Key>(keyEvent->key()));
           
           addTextLine(prefix + QString("KeyRelease:%1\n").arg(keyName));
        } else if (ev->type() == XQKeyCapture::remoteEventType_KeyPress()){
           QKeyEvent *keyEvent = static_cast<QKeyEvent *>(ev);
           QString keyName = mappingPtr->name(static_cast<Qt::Key>(keyEvent->key()));

           addTextLine(prefix + QString("KeyPress:%1 (native:%2)\n").arg(keyName).arg(static_cast<int>(keyEvent->nativeVirtualKey())));
        } else if (ev->type() == XQKeyCapture::remoteEventType_KeyRelease()){
           QKeyEvent *keyEvent = static_cast<QKeyEvent *>(ev);
           QString keyName = mappingPtr->name(static_cast<Qt::Key>(keyEvent->key()));
                      
           addTextLine(prefix + QString("KeyRelease:%1 (native:%2)\n").arg(keyName).arg(static_cast<int>(keyEvent->nativeVirtualKey())));
        }
    }
    TX_EXIT_ARGS(reinterpret_cast<int>(ev));
}

void KeyCaptureTestApp::enableRemBasic(bool enable)
{
    if (enable) {
        addTextLine("Remote Basic enabled");
        QFlags<XQKeyCapture::CapturingFlag> flags = getFlags() | XQKeyCapture::CaptureBasic;
        mKeyCapture->captureRemoteKeys(flags);
    } else {
        addTextLine("Remote Basic disabled");
        mKeyCapture->cancelCaptureRemoteKeys(XQKeyCapture::CaptureBasic);
    }
}

void KeyCaptureTestApp::enableRemoteSideKeys(bool enable)
{
    if (enable) {
        addTextLine("Side Keys enabled");
        QFlags<XQKeyCapture::CapturingFlag> flags = getFlags() | XQKeyCapture::CaptureSideKeys;
        mKeyCapture->captureRemoteKeys(flags);
    } else {
        addTextLine("Side Keys disabled");
        mKeyCapture->cancelCaptureRemoteKeys(XQKeyCapture::CaptureSideKeys);
    }
}

void KeyCaptureTestApp::enableRemCallHandlingEx(bool enable)
{
    if (enable) {
        addTextLine("Remote Call Handling Ext. enabled");
        QFlags<XQKeyCapture::CapturingFlag> flags = getFlags() | XQKeyCapture::CaptureCallHandlingExt;
        mKeyCapture->captureRemoteKeys(flags);
    } else {
        addTextLine("Remote Call Handling Ext. disabled");
        mKeyCapture->cancelCaptureRemoteKeys(XQKeyCapture::CaptureCallHandlingExt);
    }
}

void KeyCaptureTestApp::enableRemoteExtEvents(bool enable)
{
    if (enable) {
        addTextLine("Remote Events Ext. enabled");
        QFlags<XQKeyCapture::CapturingFlag> flags = getFlags() | XQKeyCapture::CaptureEnableRemoteExtEvents;
        mKeyCapture->captureRemoteKeys(flags);
    } else {
        addTextLine("Remote Events Ext. disabled");
        mKeyCapture->cancelCaptureRemoteKeys(XQKeyCapture::CaptureEnableRemoteExtEvents);
    }
}

void KeyCaptureTestApp::remoteAll(bool enable)
{
    Q_UNUSED(enable);
    toggleRemoteBasic->setChecked(true);
    toggleRemoteCallHandlingEx->setChecked(true);
    toggleRemoteExtEvents->setChecked(true);
    addTextLine("Remote: enable all");
    mKeyCapture->captureRemoteKeys(XQKeyCapture::CaptureCallHandlingExt |  XQKeyCapture::CaptureBasic | 
            XQKeyCapture::CaptureSideKeys | XQKeyCapture::CaptureEnableRemoteExtEvents);
}

void KeyCaptureTestApp::remoteNone(bool enable)
{
    Q_UNUSED(enable);
    toggleRemoteBasic->setChecked(false);
    toggleRemoteCallHandlingEx->setChecked(false);
    toggleRemoteExtEvents->setChecked(false);
    addTextLine("Remote: disable all");
    mKeyCapture->cancelCaptureRemoteKeys(XQKeyCapture::CaptureCallHandlingExt | XQKeyCapture::CaptureBasic | 
            XQKeyCapture::CaptureSideKeys | XQKeyCapture::CaptureEnableRemoteExtEvents);
}

QFlags<XQKeyCapture::CapturingFlag> KeyCaptureTestApp::getFlags()
{
    QFlags<XQKeyCapture::CapturingFlag> flags = XQKeyCapture::CaptureNone
            // remote call handling extension
           | (toggleRemoteCallHandlingEx->isChecked() 
                   ? XQKeyCapture::CaptureCallHandlingExt : XQKeyCapture::CaptureNone)
            // basic remcon accesory events
           | (toggleRemoteBasic->isChecked() 
                   ? XQKeyCapture::CaptureBasic : XQKeyCapture::CaptureNone)
            // side keys api 
           | (toggleRemoteSideKeys->isChecked() 
                   ? XQKeyCapture::CaptureSideKeys : XQKeyCapture::CaptureNone)
            // generate extevents
           | (toggleRemoteExtEvents->isChecked() 
                   ? XQKeyCapture::CaptureEnableRemoteExtEvents : XQKeyCapture::CaptureNone); 
    return flags;
}
