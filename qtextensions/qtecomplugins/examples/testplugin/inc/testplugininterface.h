/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef TESTPLUGININTERFACE_H_
#define TESTPLUGININTERFACE_H_

#include <QObject>

#ifdef BUILD_DLL
#define DLL_EXPORT Q_DECL_EXPORT
#else
#define DLL_EXPORT Q_DECL_IMPORT
#endif

class DLL_EXPORT  TestPluginInterface //: public QObject
    {
public:
	virtual QString test() = 0;
    };

Q_DECLARE_INTERFACE(TestPluginInterface, "org.nokia.mmdt.QViewPlugin/1.0" );


#endif /* TESTPLUGININTERFACE_H_ */
