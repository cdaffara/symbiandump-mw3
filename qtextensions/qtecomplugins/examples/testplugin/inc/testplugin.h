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

#ifndef TESTPLUGIN_H_
#define TESTPLUGIN_H_

#include <QObject>

#include <testplugininterface.h>

#ifdef BUILD_DLL
#define DLL_EXPORT Q_DECL_EXPORT
#else
#define DLL_EXPORT Q_DECL_IMPORT
#endif

class TestPlugin : public QObject, public TestPluginInterface
    {
    Q_OBJECT
    Q_INTERFACES(TestPluginInterface)

public:
	TestPlugin();
	
	virtual ~TestPlugin();

	virtual QString test();
    };

#endif /* TESTPLUGIN_H_ */
