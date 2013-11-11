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
*/ 

#include <QtTest/QtTest>
#include <qdir.h>
#include <xqpluginloader.h>
#include <xqplugininfo.h>
#include "theplugin/plugininterface.h"

#include <e32cmn.h>
// Helper macros to let us know if some suffixes are valid
#define bundle_VALID    false
#define dylib_VALID     false
#define sl_VALID        false
#define a_VALID         false
#define so_VALID        false
#define dll_VALID       false

#if defined(Q_OS_DARWIN)
# undef bundle_VALID
# undef dylib_VALID
# undef so_VALID
# define bundle_VALID   true
# define dylib_VALID    true
# define so_VALID       true
# define SUFFIX         ".dylib"
# define PREFIX         "lib"

#elif defined(Q_OS_HPUX) && !defined(__ia64)
# undef sl_VALID
# define sl_VALID       true
# define SUFFIX         ".sl"
# define PREFIX         "lib"

#elif defined(Q_OS_AIX)
# undef a_VALID
# undef so_VALID
# define a_VALID        true
# define so_VALID       true
# define SUFFIX         ".so"
# define PREFIX         "lib"

#elif defined(Q_OS_WIN)
# undef dll_VALID
# define dll_VALID      true
# ifdef QT_NO_DEBUG
#  define SUFFIX         ".dll"
# else
#  define SUFFIX         "d.dll"
# endif
# define PREFIX         ""

#elif defined(Q_OS_SYMBIAN)
# undef dll_VALID
# define dll_VALID      true
# define SUFFIX         ".dll"
# define PREFIX         ""

#else  // all other Unix
# undef so_VALID
# define so_VALID       true
# define SUFFIX         ".so"
# define PREFIX         "lib"
#endif

static QString sys_qualifiedLibraryName(const QString &fileName)
{
    QString currDir = QDir::currentPath();
    return currDir + "/bin/" + PREFIX + fileName + SUFFIX;
}

//TESTED_CLASS=
//TESTED_FILES=

QT_FORWARD_DECLARE_CLASS(QPluginLoader)
class tst_XQPluginLoader : public QObject
{
    Q_OBJECT

public:
    tst_XQPluginLoader();
    virtual ~tst_XQPluginLoader();

private slots:
    void errorString();
    void deleteinstanceOnUnload();
    void listThePlugins();

};

tst_XQPluginLoader::tst_XQPluginLoader()

{
}

tst_XQPluginLoader::~tst_XQPluginLoader()
{
}

//#define SHOW_ERRORS 1

void tst_XQPluginLoader::errorString()
{
#if defined(Q_OS_WINCE)
    // On WinCE we need an QCoreApplication object for current dir
    int argc = 0;
    QCoreApplication app(argc,0);
#endif
    const QString unknown(QLatin1String("Unknown error"));

    {
    XQPluginLoader loader; // default constructed
    bool loaded = loader.load();
#ifdef SHOW_ERRORS
    qDebug() << loader.errorString();
#endif
    QCOMPARE(loaded, false);
    QCOMPARE(loader.errorString(), unknown);

    QObject *obj = loader.instance();
#ifdef SHOW_ERRORS
    qDebug() << loader.errorString();
#endif
    QCOMPARE(obj, static_cast<QObject*>(0));
    QCOMPARE(loader.errorString(), unknown);

    bool unloaded = loader.unload();
#ifdef SHOW_ERRORS
    qDebug() << loader.errorString();
#endif
    QCOMPARE(unloaded, false);
    QCOMPARE(loader.errorString(), unknown);
    }
    {
    XQPluginLoader loader( 0xE6E9EB83 );     //not a plugin sys_qualifiedLibraryName("mylib")
    bool loaded = loader.load();
#ifdef SHOW_ERRORS
    qDebug() << loader.errorString();
#endif
    QCOMPARE(loaded, false);
    QVERIFY(loader.errorString() != unknown);

    QObject *obj = loader.instance();
#ifdef SHOW_ERRORS
    qDebug() << loader.errorString();
#endif
    QCOMPARE(obj, static_cast<QObject*>(0));
    QVERIFY(loader.errorString() != unknown);

    bool unloaded = loader.unload();
#ifdef SHOW_ERRORS
    qDebug() << loader.errorString();
#endif
    QCOMPARE(unloaded, false);
    QVERIFY(loader.errorString() != unknown);
    }

    {
    XQPluginLoader loader( KNullUid.iUid );     //not a file sys_qualifiedLibraryName("nosuchfile")
    bool loaded = loader.load();
#ifdef SHOW_ERRORS
    qDebug() << loader.errorString();
#endif
    QCOMPARE(loaded, false);
    QVERIFY(loader.errorString() != unknown);

    QObject *obj = loader.instance();
#ifdef SHOW_ERRORS
    qDebug() << loader.errorString();
#endif
    QCOMPARE(obj, static_cast<QObject*>(0));
    QVERIFY(loader.errorString() != unknown);

    bool unloaded = loader.unload();
#ifdef SHOW_ERRORS
    qDebug() << loader.errorString();
#endif
    QCOMPARE(unloaded, false);
    QVERIFY(loader.errorString() != unknown);
    }

#if !defined Q_OS_WIN && !defined Q_OS_MAC && !defined Q_OS_HPUX && !defined Q_OS_SYMBIAN
    {
    XQPluginLoader loader(  );     //a plugin with unresolved symbols sys_qualifiedLibraryName("almostplugin")
    QCOMPARE(loader.load(), false);
#ifdef SHOW_ERRORS
    qDebug() << loader.errorString();
#endif
    QVERIFY(loader.errorString() != unknown);

    QCOMPARE(loader.instance(), static_cast<QObject*>(0));
#ifdef SHOW_ERRORS
    qDebug() << loader.errorString();
#endif
    QVERIFY(loader.errorString() != unknown);

    QCOMPARE(loader.unload(), false);
#ifdef SHOW_ERRORS
    qDebug() << loader.errorString();
#endif
    QVERIFY(loader.errorString() != unknown);
    }
#endif

    {
    XQPluginLoader loader( 0xE6E9EB84 );     //a plugin sys_qualifiedLibraryName("theplugin")
    QCOMPARE(loader.load(), true);
    QCOMPARE(loader.errorString(), unknown);

    QVERIFY(loader.instance() !=  static_cast<QObject*>(0));
    QCOMPARE(loader.errorString(), unknown);

    // Make sure that plugin really works
    PluginInterface* theplugin = qobject_cast<PluginInterface*>(loader.instance());
    QString pluginName = theplugin->pluginName();
    QCOMPARE(pluginName, QLatin1String("Plugin ok"));

    QCOMPARE(loader.unload(), true);
    QCOMPARE(loader.errorString(), unknown);
    }
}

void tst_XQPluginLoader::deleteinstanceOnUnload()
{
    for (int pass = 0; pass < 2; ++pass) {
        XQPluginLoader loader1;
        loader1.setUid( 0xE6E9EB84 ); // sys_qualifiedLibraryName("theplugin")
        if (pass == 0)
            loader1.load(); // not recommended, instance() should do the job.
        PluginInterface *instance1 = qobject_cast<PluginInterface*>(loader1.instance());
        QCOMPARE(instance1->pluginName(), QLatin1String("Plugin ok"));

        XQPluginLoader loader2;
        loader2.setUid( 0xE6E9EB84 ); // sys_qualifiedLibraryName("theplugin")
        if (pass == 0)
            loader2.load(); // not recommended, instance() should do the job.
        PluginInterface *instance2 = qobject_cast<PluginInterface*>(loader2.instance());
        QCOMPARE(instance2->pluginName(), QLatin1String("Plugin ok"));

        QSignalSpy spy1(loader1.instance(), SIGNAL(destroyed()));
        QSignalSpy spy2(loader2.instance(), SIGNAL(destroyed()));
        QCOMPARE(loader1.unload(), false);  // refcount not reached 0, not really unloaded
        QCOMPARE(spy1.count(), 0);
        QCOMPARE(spy2.count(), 0);
        QCOMPARE(instance1->pluginName(), QLatin1String("Plugin ok"));
        QCOMPARE(instance2->pluginName(), QLatin1String("Plugin ok"));
        QCOMPARE(loader2.unload(), true);   // refcount reached 0, did really unload
        QCOMPARE(spy1.count(), 1);
        QCOMPARE(spy2.count(), 1);
    }
}
void tst_XQPluginLoader::listThePlugins()
{
    QList<XQPluginInfo> plugins;
    XQPluginLoader loader;
    loader.listImplementations( tr( "xtheplugin.dll" ), plugins );
    QVERIFY( plugins.count() !=  0 );
    for( int i( 0 ); i < plugins.count(); ++i ) {
        
        loader.setUid( plugins[i ].uid() );
        QCOMPARE( loader.load(), true );
        PluginInterface *instance = qobject_cast<PluginInterface*>(loader.instance());
        QCOMPARE(instance->pluginName(), QLatin1String("Plugin ok"));
        QCOMPARE(loader.unload(), true);
        
    }
    plugins.clear();
}

QTEST_APPLESS_MAIN(tst_XQPluginLoader)
#include "tst_xqpluginloader.moc"

