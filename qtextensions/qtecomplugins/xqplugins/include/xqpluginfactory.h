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

#ifndef XQPLUGINFACTORY_H
#define XQPLUGINFACTORY_H

#include <e32base.h>
#include <qglobal.h>

class QObject;
class QString;

#ifdef BUILD_XQPLUGINS_DLL
#define DLL_EXPORT Q_DECL_EXPORT
#else
#define DLL_EXPORT Q_DECL_IMPORT
#endif

class DLL_EXPORT CPluginFactory : public CBase
    {
public:
    static CPluginFactory* NewL(int aUid);
       
    virtual ~CPluginFactory();

    QObject* createQtPlugin();

private:
    virtual QObject* constructPlugin() = 0;

protected:
    TUid iDtor_ID_Key;    

    };

#endif /*XQPLUGINFACTORY_H*/
