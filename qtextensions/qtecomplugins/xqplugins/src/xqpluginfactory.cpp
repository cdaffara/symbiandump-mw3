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
* Description: This class implements plugins factory.
*
*/

#include <xqpluginfactory.h>

#include "xqpluginhandle.h"
#include <ecom/ecom.h>
#include <QString>
#include <QObject>

/*!
 Constructs plugin factory.
*/
CPluginFactory* CPluginFactory::NewL(int aUid)
{
    TUid requestedUid(KNullUid);
    requestedUid.iUid = aUid;
    return reinterpret_cast <CPluginFactory*>( 
                REComSession::CreateImplementationL(requestedUid, 
                                                    _FOFF(CPluginFactory, 
                                                            iDtor_ID_Key)));
}

/*!
  Destroys plugin factory.
*/
EXPORT_C CPluginFactory::~CPluginFactory()
{
    if (KNullUid != iDtor_ID_Key) {
        REComSession::DestroyedImplementation(iDtor_ID_Key);
    }
}


/*!
  Plugin factorizatoin method.
*/
QObject* CPluginFactory::createQtPlugin()
{
    QObject* retVal(NULL);
    if (KNullUid != iDtor_ID_Key) {
        retVal = constructPlugin();
        if (NULL != retVal){
            new XQPluginHandle(iDtor_ID_Key.iUid, retVal);
            iDtor_ID_Key = KNullUid;
        }
    }
    return retVal;
}

