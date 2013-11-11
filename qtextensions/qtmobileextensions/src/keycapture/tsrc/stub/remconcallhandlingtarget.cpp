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

#include <e32debug.h>

#include "stub/remconcallhandlingtarget.h"
#include "stub/remconinterfaceselector.h"

int gCRemConCallHandlingTargetCount = 0;
bool gCRemConCallHandlingTargetNewLLeave = false;
int gCRemConCallHandlingTargetSendResponseCount = 0;


//static functions start

int CRemConCallHandlingTarget::getCount()
{
    return gCRemConCallHandlingTargetCount;
}

void CRemConCallHandlingTarget::setNewLLeave( bool value )
{
    gCRemConCallHandlingTargetNewLLeave = value;
}

void CRemConCallHandlingTarget::NewLLeaveIfDesiredL()
{
    if (gCRemConCallHandlingTargetNewLLeave)
    {
        RDebug::Print(_L("CRemConCallHandlingTarget::NewLLeaveIfDesiredL Leave"));
        gCRemConCallHandlingTargetNewLLeave = false;
        User::Leave(KErrGeneral);
    }
}

CRemConCallHandlingTarget* CRemConCallHandlingTarget::NewL(CRemConInterfaceSelector& aInterfaceSelector, MRemConCallHandlingTargetObserver& /*aObserver*/)
{
    
    RDebug::Print(_L("stub CRemConCallHandlingTarget::NewL"));
    CRemConCallHandlingTarget::NewLLeaveIfDesiredL();
    CRemConCallHandlingTarget* self = new(ELeave) CRemConCallHandlingTarget();
    aInterfaceSelector.RegisterCallHandlingL(self);
    return self;
}


int CRemConCallHandlingTarget::getSendResponseCount()
{
    return gCRemConCallHandlingTargetSendResponseCount;
}

//static functions end


CRemConCallHandlingTarget::CRemConCallHandlingTarget()
    : CActive(EPriorityStandard), 
      iClientStatus(NULL)
{
    gCRemConCallHandlingTargetCount++;
}

CRemConCallHandlingTarget::~CRemConCallHandlingTarget()
{
    Cancel();
    gCRemConCallHandlingTargetCount--;
    gCRemConCallHandlingTargetSendResponseCount = 0;
}


void CRemConCallHandlingTarget::SendResponse(TRequestStatus& aStatus,
        TInt aOperationId, 
        TInt aError)
{
    RDebug::Print(_L(">>CRemConCoreApiTarget::SendResponse"));
    
    iClientStatus = &aStatus;
    gCRemConCallHandlingTargetSendResponseCount++;
    
    // stub should not be async
    /*TRequestStatus* status = &iStatus;
    User::RequestComplete(status, KErrNone);
    SetActive();*/

    RunL();
    
    RDebug::Print(_L("<<CRemConCoreApiTarget::SendResponse"));
}

void CRemConCallHandlingTarget::DoCancel()
{
}
/*
TAny* CRemConCallHandlingTarget::GetInterfaceIf(TUid aUid)
{
TAny* ret = NULL;
return ret;
}
*/
void CRemConCallHandlingTarget::RunL()
{
    RDebug::Print(_L(">>CRemConCallHandlingTarget::RunL"));
    User::RequestComplete(iClientStatus, KErrNone);
    RDebug::Print(_L("<<CRemConCallHandlingTarget::RunL"));
}
