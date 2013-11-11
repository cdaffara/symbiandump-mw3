/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description: CRemConCallHandlingTarget stub for testing keycapture
*
*/


#ifndef CREMCONCALLHANDLINGTARGET_H
#define CREMCONCALLHANDLINGTARGET_H

#include <e32base.h>
//#include <remcon/remconinterfacebase.h>

class CRemConInterfaceSelector;
class MRemConCallHandlingTargetObserver;

class CRemConCallHandlingTarget  : public CActive//, public CRemConInterfaceBase
    {

public:    
    static int getCount();
    static void setNewLLeave( bool value = true );
    static void NewLLeaveIfDesiredL();
    static int getSendResponseCount();
    
    void SendResponse( TRequestStatus& aStatus,
                       TInt aOperationId, 
                       TInt aError );
    
public:  // Constructors and destructor   
    static CRemConCallHandlingTarget* NewL(
        CRemConInterfaceSelector& aInterfaceSelector, 
        MRemConCallHandlingTargetObserver& aObserver );
    
    ~CRemConCallHandlingTarget();
    //TAny* GetInterfaceIf(TUid);
protected:

    // from CActive
    virtual void DoCancel();
    virtual void RunL();
    
private:    
    CRemConCallHandlingTarget();
    
private:
    TRequestStatus* iClientStatus;
    
};

#endif      // CREMCONCALLHANDLINGTARGET_H

// end of file

