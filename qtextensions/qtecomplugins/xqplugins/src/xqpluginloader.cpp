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
* Description:  This class implements an extended wrapper for the ECOM framework.
*
*/

#include "xqpluginloader.h"
#include "xqpluginloaderprivate.h"

#include <qfileinfo.h>
#include "qdebug.h"

/*!
  Constructs plugin loader with the given parent.
  @param parent - address of class instance parent 
*/
XQPluginLoader::XQPluginLoader(QObject* parent) 
: 
QObject(parent),
d(0),
did_load( false )
{
}

/*!
  Constructs plugin loader with the given parent and plugin implementation UID.
  @param uid - UID of plugin that should be loaded
  @param parent - address of class instance parent 
 */
XQPluginLoader::XQPluginLoader(int requestedUid, QObject* parent) 
: 
QObject(parent),
d(XQPluginLoaderPrivate::findOrCreate(requestedUid)), 
did_load( false )
{
}

/*!
  Destroys plugin loader. Unless unload() was called explicitly, plugins still stays in memory.
*/
XQPluginLoader::~XQPluginLoader()
{
    if(d) {
        d->release();
    }
}

/*!
  List available plugins which implement requested interface. Plugins are resolved using interface name.
  
  @param interfaceName - requested interface name
  @param impls - destination list where resolved plugins info will be stored
  @return true on success, false on any error
*/
bool XQPluginLoader::listImplementations(
        const QString &interfaceName, 
        QList< XQPluginInfo > &impls)
{
    RImplInfoPtrArray infoArr;
    TRAPD( errCode, XQPluginLoaderPrivate::listImplementationsL( infoArr, interfaceName, impls ) );
    infoArr.ResetAndDestroy();
    return ( KErrNone == errCode );
}

/*!
  Returns UID of requested plugin.
 */
int XQPluginLoader::uid()const
{
    return ( d ? d->uid : KNullUid.iUid );
}

/*!
  Return pointer to plugin root-component instance
  @return instance address on success, 0 otherwise  
 */
QObject* XQPluginLoader::instance()
{
    if (!load())
        return 0;
#ifndef Q_OS_SYMBIAN
    if (d->instance)
        return d->instance();
	else
		return 0;
#else
    return d->instance();
#endif
    
}

/*!
  Return information if plugin have been loaded
  @return true if plugin have been loaded, false otherwise 
 */
bool XQPluginLoader::isLoaded() const
{
    return d && d->pHnd
#ifndef Q_OS_SYMBIAN
        && d->instance;
#else
        ;
#endif    
}

/*!
  Load requested plugin.
  @return true on success, false otherwise
 */
bool XQPluginLoader::load()
{
    if (!d)
        return false;
    if (did_load)
        return d->pHnd;
    did_load = true;
    return d->loadPlugin();
}

/*!
  Unloads the plugin and returns true if plugin could be unloaded. All plugins are unloaded at aplication exit 
  so calling this method is not mandatory. 
  Actual unloading will succed only when all instances of given plugin loaders calls unload.
  @return true on success, false otherwise
 */
bool XQPluginLoader::unload()
{
    if (did_load) {
        did_load = false;
        return d->unload();
    }
    if (d)  // Ouch
        d->errorString = tr("The plugin was not loaded.");
    return false;
}

/*!
  Setter for UID of plugin. It is the same UID that may be specified in constructor.
*/
void XQPluginLoader::setUid ( int uid )
{
    if (d) {
        d->release();
        d = 0;
        did_load = false;
    }
    d = XQPluginLoaderPrivate::findOrCreate( uid );
}

/*!
  Return string.with description of last error that occured.
  @return error description
*/
QString XQPluginLoader::errorString () const
{
    return (!d || d->errorString.isEmpty()) ? tr("Unknown error") : d->errorString;
}
