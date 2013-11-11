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
* Description: This is the private implementation of the XQPluginLoader class.
*
*/

#include "xqpluginloaderprivate.h"
#include "xqplugininfo.h"

#include <qglobal.h>
#include <QObject>
#include <QLibrary>
#include <QSettings>

#include <xqplugin.h>

#include "xqpluginfactory.h"

#include <qstring.h>
#include <qmutex.h>
#include <qfileinfo.h>

//TODO warning: use of internal include ??
#include <qglobal.h>

#include <ecom/ecom.h>
#include <e32base.h>
#include <e32std.h>
#include <utf.h>

#include "xqtecom.hrh"

//TODO warning: use of Qt internal macro
Q_GLOBAL_STATIC(QMutex, qt_pluginloader_mutex)

typedef QMap<int, XQPluginLoaderPrivate*> PluginLoadersMap;
//TODO warning: use of Qt internal macro
Q_GLOBAL_STATIC(PluginLoadersMap, pluginLoadersMap)


XQPluginLoaderPrivate::XQPluginLoaderPrivate( int requestedUid, const QString &version)
:
pHnd(0),
fileName(),
uid( requestedUid ),
pluginInstance(0),
loaderRefCount(1), 
loaderUnloadCount(0)
{ 
    pluginLoadersMap()->insert(uid, this);
    Q_UNUSED(version);
}

XQPluginLoaderPrivate::~XQPluginLoaderPrivate()
{
    PluginLoadersMap * const map = pluginLoadersMap();
    if (map) {
        XQPluginLoaderPrivate *that = map->take( uid );
        Q_ASSERT(this == that);
        Q_UNUSED(that);
    }
}

void XQPluginLoaderPrivate::release()
{
    QMutexLocker locker(qt_pluginloader_mutex());
    if (!loaderRefCount.deref()) {
        delete this;
    }
}

XQPluginLoaderPrivate *XQPluginLoaderPrivate::findOrCreate(const QString &requestedFileName, const QString &version)
{
    QString fileName(requestedFileName);
    if (fileName.endsWith(".qtplugin", Qt::CaseInsensitive)) {
        fileName.chop(8);
        fileName.append("dll");
    }
    
    XQPluginLoaderPrivate* retVal( NULL );
    TRAPD( err, findOrCreateL( retVal, fileName, version ) );
    if( KErrNone != err ) {
        retVal = findOrCreate( KNullUid.iUid, version );
        retVal->errorString = QString( "Unknown error %1" ).arg( err );
    }
    return retVal;
}

XQPluginLoaderPrivate *XQPluginLoaderPrivate::findOrCreate(int id, const QString &version)
{
    QMutexLocker locker(qt_pluginloader_mutex());
    XQPluginLoaderPrivate *pluginLoader = pluginLoadersMap()->value( id );
    if ( pluginLoader ) {
        pluginLoader->loaderRefCount.ref();
    } else {
        pluginLoader = new XQPluginLoaderPrivate(id, version);
    }
    return pluginLoader;
}

void XQPluginLoaderPrivate::findOrCreateL( XQPluginLoaderPrivate*& pluginLoader, const QString &requestedFileName , const QString &version )
{
    if( 0 >= requestedFileName.length() ) {
        User::Leave( KErrBadName );
    }
    TPtrC symbianFileName( requestedFileName.utf16(), requestedFileName.length() ); //set string content into descriptor
    TEComResolverParams resolverParams;
    RImplInfoPtrArray infoArr;
    TUid customResolverUid( KNullUid ), pluginInterfaceUid( KNullUid );
    customResolverUid.iUid = KEComDisplayNameResolverImplementationUid;
    pluginInterfaceUid.iUid  = KQtEcomPluginInterfaceUID;
    
    HBufC8* symbianFileNamePtr( CnvUtfConverter::ConvertFromUnicodeToUtf8L( symbianFileName ) );//convert file name to UTF8
    CleanupStack::PushL( symbianFileNamePtr );
    resolverParams.SetDataType( *symbianFileNamePtr );//initialize resolver params struct
    REComSession::ListImplementationsL( pluginInterfaceUid, resolverParams, customResolverUid, infoArr );//list matching plugins using custom resolver
    CleanupStack::PopAndDestroy( symbianFileNamePtr );
    
    if( 0 >= infoArr.Count() ) {
        User::Leave( KErrNotFound );
    }
    //analyse available plugins versions
    int optimalPluginOffset( 0 );
    for( int i( 0 ); i < infoArr.Count(); ++i ) {
        if( infoArr[ optimalPluginOffset ]->Version() < infoArr[ i ]->Version() ) {
            optimalPluginOffset = i;
        }
    }
    const int resolvedUid( infoArr[ optimalPluginOffset ]->ImplementationUid().iUid );
    infoArr.ResetAndDestroy();
    
    pluginLoader = findOrCreate( resolvedUid, version );
    User::LeaveIfNull( pluginLoader ); 
    pluginLoader->setFileName( requestedFileName );
}

bool XQPluginLoaderPrivate::isPlugin(QSettings *settings)
{
    Q_UNUSED(settings)
    return true;
}

bool XQPluginLoaderPrivate::loadPlugin()
{
    loaderUnloadCount.ref();
    if (pluginInstance) {
        return true;
    }
    pHnd = 0;
    errorString = "Can't load plugin.";
    if( KNullUid.iUid != uid ) {
        TRAPD(loadError, loadL());
        if( KErrNone == loadError ) {
            pHnd = 1;
            errorString.clear();
        }
    }
    return pHnd; 
}

void XQPluginLoaderPrivate::loadL()
{
    CPluginFactory* pluginFactory = CPluginFactory::NewL( uid );
    pluginInstance = pluginFactory->createQtPlugin();
}

QObject* XQPluginLoaderPrivate::instance()
{
    return pluginInstance;
}

bool XQPluginLoaderPrivate::unload()
{
    if (!pHnd)
        return false;
    
    if (!loaderUnloadCount.deref()) { // only unload if ALL loader instances wanted to
        if (pluginInstance)
            delete pluginInstance;
            pHnd = 0;
    }
    
    return (pHnd == 0);
}

void XQPluginLoaderPrivate::setFileName( const QString& value )
{
    fileName = value;
}

void XQPluginLoaderPrivate::listImplementationsL( 
        RImplInfoPtrArray& infoArr, 
        const QString &interfaceName, 
        QList<XQPluginInfo> &impls)
{
    const TUid uid = { 0x20022FCE };
    TEComResolverParams resolverParams;

    HBufC8* ifName = HBufC8::NewLC(interfaceName.length());
    ifName->Des().Copy(TPtrC(interfaceName.utf16()));
    resolverParams.SetDataType(*ifName);
    REComSession::ListImplementationsL(uid, resolverParams, infoArr);
    CleanupStack::PopAndDestroy( ifName );
    for (int i( 0 ); i < infoArr.Count(); ++i) {
        const TDesC &name = infoArr[i]->DisplayName();
        QString dllName = QString::fromUtf16( 
                reinterpret_cast<const ushort*>(name.Ptr()), 
                    name.Length());
        
        const TDesC8 &opaque_data = infoArr[i]->OpaqueData();
        QString opaqueData = QString::fromAscii( 
                        reinterpret_cast<const char*>( opaque_data.Ptr() ), 
                            opaque_data.Length());
        
        impls.append(XQPluginInfo(infoArr[i]->ImplementationUid().iUid, 
                                 infoArr[i]->Version(),
                                 dllName, 
                                 opaqueData));
    }
}
