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
 
#ifndef MYTESTWINDOWGROUP_H
#define MYTESTWINDOWGROUP_H

#include <qnamespace.h>
#include <QString>
#include <QList>
#include <qobject>
#include <W32STD.H>

    enum WindowGroupActionType {
        WGATCaptureKey,
        WGATCaptureLongKey,
        WGATCaptureKeyUpAndDowns,
        WGATCancelCaptureKey,
        WGATCancelCaptureKeyUpAndDowns,
        WGATCancelCaptureLongKey
        };

class MyTestWindowGroup : public QObject {
    Q_OBJECT

public:
    static MyTestWindowGroup* Instance();
    

    void setRequestNumber( long int req ) {
        requestNumber = req;
        };
    
    TInt32 CaptureKey(TUint aKeycode, TUint aModifierMask, TUint aModifier){
        QList<unsigned int> arglist;
        arglist << aKeycode << aModifierMask << aModifier;
        emit windowGroupAction( WGATCaptureKey, arglist );
        return requestNumber;
    };
    
    TInt32 CaptureLongKey(TUint aInputKeyCode,TUint aOutputKeyCode,TUint aModifierMask,TUint aModifier ,TInt aPriority,TUint aFlags){
        QList<unsigned int> arglist;
        arglist << aInputKeyCode << aOutputKeyCode << aModifierMask << aModifier << aPriority << aFlags;
        emit windowGroupAction( WGATCaptureLongKey, arglist );
        return requestNumber;
    };
    
    TInt32 CaptureKeyUpAndDowns(TUint aScanCode, TUint aModifierMask, TUint aModifier){
        QList<unsigned int> arglist;
        arglist << aScanCode << aModifierMask << aModifier;
        emit windowGroupAction( WGATCaptureKeyUpAndDowns, arglist );
        return requestNumber;
    };
    
    void CancelCaptureKey(TInt32 aCaptureKey){
        QList<long int> arglist;
        arglist << aCaptureKey ;
        emit windowGroupActionCancel( WGATCancelCaptureKey, arglist );
    };
    
    void CancelCaptureKeyUpAndDowns(TInt32 aCaptureKey){
        QList<long int> arglist;
        arglist << aCaptureKey ;
        emit windowGroupActionCancel( WGATCancelCaptureKeyUpAndDowns, arglist );
    };
    
    void CancelCaptureLongKey(TInt32 aCaptureKey){
        QList<long int> arglist;
        arglist << aCaptureKey ;
        emit windowGroupActionCancel( WGATCancelCaptureLongKey, arglist );
    };
    
signals:
    void windowGroupAction( WindowGroupActionType, QList<unsigned int> );
    void windowGroupActionCancel( WindowGroupActionType, QList<long int> );
    
protected:
    MyTestWindowGroup() : requestNumber(0) {};
    ~MyTestWindowGroup();
    
private:
    //static MyTestWindowGroup* instance;
    long int requestNumber;
};



#endif /* MYTESTWINDOWGROUP_H */
