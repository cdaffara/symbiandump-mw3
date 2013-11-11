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
* Description: remconinterfaceselector stub for testing keycapture
*
*/


#ifndef REMCONINTERFACESELECTOR_H
#define REMCONINTERFACESELECTOR_H


#include <e32base.h>

//forward declarations
class CRemConCallHandlingTarget;
class CRemConCoreApiTarget;

class CRemConInterfaceSelector : public CBase
    {

public:
    
    static int getCount();
    static void setNewLLeave();
    static void NewLLeaveIfDesiredL();
    static void setOpenTargetLLeave();
    static void OpenTargetLLeaveIfDesiredL();
    
public:

    static CRemConInterfaceSelector* NewL();
    virtual ~CRemConInterfaceSelector();
    void RegisterInterface();
    void OpenTargetL();
    void RegisterCallHandlingL( CRemConCallHandlingTarget* );
    void RegisterCoreApiL(CRemConCoreApiTarget* );
    
private:
    
    CRemConInterfaceSelector();
    RPointerArray<CRemConCallHandlingTarget> iCallHandlingArray;
    RPointerArray<CRemConCoreApiTarget> iCoreApiArray;
    };

#endif      // REMCONINTERFACESELECTOR_H

// End of File
