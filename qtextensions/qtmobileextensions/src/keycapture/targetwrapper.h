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

#ifndef TARGETWRAPPER_H_
#define TARGETWRAPPER_H_

#include <remconcoreapi.h>
#include <remconinterfaceselector.h>
#include <remconcoreapitarget.h>
#include <remconcoreapitargetobserver.h>    
#include <remconcoreapitargetobserver.h>    
#include <remconcallhandlingtargetobserver.h>
#include <remconcallhandlingtarget.h>

#include <QMainWindow>
#include <QMap>
#include <QEvent>

#include <e32base.h>

#include <XQKeyCapture>

class QPlainTextEdit;
class QAction;
class QWidget;
class QMenu;

#ifdef XQKEYCAPTURE_LIBRARY
#define XQKEYCAPTURE_EXPORT Q_DECL_EXPORT
#else
#define XQKEYCAPTURE_EXPORT Q_DECL_IMPORT
#endif


class CResponseHandler;
class CResponseHandlerEx;

class TargetWrapper : public MRemConCoreApiTargetObserver, public MRemConCallHandlingTargetObserver
{
public:
    TargetWrapper();
    
    ~TargetWrapper();
    
    void init(XQKeyCapture::CapturingFlags flags);
    void close(XQKeyCapture::CapturingFlags flags);

public:
    // -- MRemConCoreApiTargetObserver overloaded methods --    
    void MrccatoCommand(TRemConCoreApiOperationId aOperationId, TRemConCoreApiButtonAction aButtonAct);
    
    // -- MRemConCallHandlingTargetObserver overloaded methods --    
    
     void AnswerCall();

     void EndCall();
    
     void AnswerEndCall();
    
     void VoiceDial( const TBool aActivate );
    
     void LastNumberRedial();
    
     void DialCall( const TDesC8& aTelNumber );
    
     void MultipartyCalling( const TDesC8& aData );
    
     void GenerateDTMF( const TChar aChar );
    
     void SpeedDial( const TInt aIndex );    
     
protected:    
     
    void reset();
    void initMapping();

    QWidget *getTargetWidget();
    
    void sendKey(QEvent::Type eventType, Qt::Key key, Qt::KeyboardModifiers modFlags, 
            TRemConCoreApiOperationId aOperationId = ENop);

    Qt::Key mapKey(TRemConCoreApiOperationId aOperationId);

private:
    void cleanup();
    
private:
    CRemConInterfaceSelector *selector;
    
    CRemConCoreApiTarget *target;
    CRemConCallHandlingTarget *targetEx;

    CResponseHandler *handler;
    CResponseHandlerEx *handlerEx;
    
    QFlags<XQKeyCapture::CapturingFlags> captureFlags;
    
    QMap<TRemConCoreApiOperationId, Qt::Key> keyMapping;
    
#ifdef _XQKEYCAPTURE_UNITTEST_ 
    friend class TestXQKeyCapture;
#endif // _XQKEYCAPTURE_UNITTEST_
};

#endif /* TARGETWRAPPER_H_ */
