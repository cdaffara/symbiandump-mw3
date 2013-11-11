/*
* Copyright (c) 2000 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description: definition of dm constants/exported methods
* 	This is part of remotemgmt_plat.
*
*/


#ifndef __TERMINALCONTROL_H__
#define __TERMINALCONTROL_H__

/**
*  RTerminalControl class to establish TerminalControl server connection
*/

class RTerminalControl : public RSessionBase
{

	public:

	    /**
		* Connect Connects client side to policy engine server
	    * @return KErrNone Symbian error code
        */
		IMPORT_C TInt Connect();

	    /**
		* Close Close server connection
        */
		IMPORT_C void Close();

};

#endif //__TERMINALCONTROL_H__
