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
* Description:  Class implement extended wrapper for ECom framework
*
*/

#ifndef XQPLUGINLOADER_H
#define XQPLUGINLOADER_H

#include <QtGlobal>
#include <QObject>
#include <QList>
#include <QString>

class XQPluginInfo;
class XQPluginLoaderPrivate;

#ifdef BUILD_XQPLUGINS_DLL
#define DLL_EXPORT Q_DECL_EXPORT
#else
#define DLL_EXPORT Q_DECL_IMPORT
#endif

class DLL_EXPORT XQPluginLoader : public QObject
{
public:

    XQPluginLoader(QObject* parent = 0);
    
    XQPluginLoader(int uid, QObject* parent = 0);
    
    virtual ~XQPluginLoader();
    
    static bool listImplementations(const QString &interfaceName, 
                             QList<XQPluginInfo > &impls);
    
    int uid()const;
    
    QObject* instance();
    
    bool isLoaded() const;
    
    bool load();
    
    bool unload();
    
    void setUid ( int uid );
    
    QString errorString () const;
    
private:
    /**
     * Private extended plugin loader implementation.
     * Own
     */
    XQPluginLoaderPrivate* d;
    
    /**
     * Flag that inform about loading state
     */
    bool did_load;
    
    Q_DISABLE_COPY(XQPluginLoader)
};

#endif
