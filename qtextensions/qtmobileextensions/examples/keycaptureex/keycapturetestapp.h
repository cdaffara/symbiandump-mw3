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

#ifndef KeyCaptureTestApp_H
#define KEYCAPTUREMAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <XQKeyCapture>

#include "mapping.h"

class QPlainTextEdit;
class QAction;
class QWidget;
class QMenu;
class CaptureRequest;

class KeyCaptureTestApp : public QMainWindow
{
Q_OBJECT
public:
    KeyCaptureTestApp(QWidget *parent = 0);
	~KeyCaptureTestApp();
	void addTextLine(QString);
	
    bool event(QEvent *event);    
    bool eventFilter(QObject *, QEvent *);
public slots:
    void triggered(QAction* aAction);
	void cleanLog();
	
	void enableRemBasic(bool);
    void enableRemCallHandlingEx(bool);
    void enableRemoteSideKeys(bool);
    void enableRemoteExtEvents(bool);
	
    void remoteAll(bool enable);
    void remoteNone(bool enable);

private:
    void procesAction(CaptureRequest request);
    void processEvent(const QString &prefix, QEvent *event);
    
    QFlags<XQKeyCapture::CapturingFlag> getFlags();
private:	
    QPlainTextEdit *mTextArea;

    XQKeyCapture *mKeyCapture;
    
    QAction *toggleRemoteBasic;
    QAction *toggleRemoteCallHandlingEx;
    QAction *toggleRemoteSideKeys;
    QAction *toggleRemoteExtEvents;
    
    QAction *remoteAllOn;
    QAction *remoteAllOff;
    
    QMap<QString, Qt::Key> mKeysMap;
    QMenu* mKeysMenu;
    
    QMap<QString, XQKeyCapture::LongFlags> mLongFlagsMap;
    QMenu* mLongFlagsMenu;
  
    Mapping *mappingPtr;
};

#endif // KEYCAPTUREMAINWINDOW_H
