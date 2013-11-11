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
* Description: Private plugin's loader implementation.
*
*/

#ifndef QPLUGINLOADER2PRIVATE_H
#define QPLUGINLOADER2PRIVATE_H

#include <QObject>
#include <QString>
#include <QList>
#include <QtGlobal>
#include <QLibrary>
#include <QSettings>

#include <ecom/implementationinformation.h>
#include <ecom/ecom.h>

class XQPluginInfo;

class XQPluginLoaderPrivate
{
public:
    int pHnd;

    QString fileName;
    const int uid;
    bool loadPlugin(); 
    bool unload();
    void release();

    static XQPluginLoaderPrivate *findOrCreate(const QString &fileName, const QString &version = QString());
    static XQPluginLoaderPrivate *findOrCreate(int, const QString &version = QString());

    QString errorString;
    QLibrary::LoadHints loadHints;

    bool isPlugin(QSettings *settings = 0);

    QObject* instance();

    /**
     * Function retrive plugin description information from ECom framework
     * @param infoArr - plugins description array
     *                  ( definition from ECom framework )
     * @param interfaceName - reqursted plugin interface name
     * @param impls - list of plugin descrpions. plugins implement requested 
     *                interface 
     */ 
    static void listImplementationsL( 
            RImplInfoPtrArray& infoArr, 
            const QString &interfaceName, 
            QList<XQPluginInfo> &impls);
    
private:
    XQPluginLoaderPrivate( int uid, const QString &version= QString() );
    ~XQPluginLoaderPrivate();
    
    void loadL();
    static void findOrCreateL( XQPluginLoaderPrivate*& pluginLoader, const QString &fileName, const QString &version );
    void setFileName(const QString& fileName);
    QObject* pluginInstance;
    
    QAtomicInt loaderRefCount;
    QAtomicInt loaderUnloadCount;    
    
};

#endif // QPLUGINLOADER2PRIVATE_H
