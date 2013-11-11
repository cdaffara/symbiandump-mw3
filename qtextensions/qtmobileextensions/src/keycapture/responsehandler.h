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

#ifndef RESPONSEHANDLER_H_
#define RESPONSEHANDLER_H_

#include <remconcoreapitarget.h>

#include <remconcoreapi.h>
#include <remconinterfaceselector.h>
#include <remconcoreapitarget.h>
#include <remconcoreapitargetobserver.h>    

NONSHARABLE_CLASS(CResponseHandler) : public CActive
{
public:

    /**
     * Two-phased constructor.
     *
     * @since 3.0
     * @param aRemConCoreApiTarget RemCon core target object.
     * @return Pointer to newly created object.
     */
    static CResponseHandler* NewL( CRemConCoreApiTarget& aRemConCoreApiTarget );

    /**
     * Destructor.
     */
    virtual ~CResponseHandler();

    /**
     * Send the any key response back to Remcon server
     *
     * @since 3.0
     * @param aOperationId RemCon operation Id.
     */
    void CompleteAnyKey( TRemConCoreApiOperationId aOperationId );

private:

    /**
     * C++ default constructor.
     */
    CResponseHandler( CRemConCoreApiTarget& aRemConCoreApiTarget );

// from base class CActive

    /**
     * From CActive
     * Handles an active object's request completion event.
     */
    void RunL();

    /**
     * From CActive
     * Implements cancellation of an outstanding request.
     */
    void DoCancel();

private:    // Data

    // Response array.
    RArray<TRemConCoreApiOperationId> iResponseArray;   // Own
    CRemConCoreApiTarget& iRemConCoreApiTarget;         // Not own
};

#endif /* RESPONSEHANDLER_H_ */
