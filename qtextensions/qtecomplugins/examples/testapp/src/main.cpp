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

#include <QtGui>

#include <QObject>
#include <QString>
#include <QList>

#include <xqplugin.h>
#include <xqpluginloader.h>
#include <xqplugininfo.h>

#include <stdio.h>

#include <testplugininterface.h>

void log(const QString& aString)
{
    FILE* poFile = fopen( "c:/trace.log", "a" );
    if( !poFile )
        return;
    fprintf( poFile, "%s\n", aString.toAscii().constData() );
    fclose( poFile );
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    log("--- Start ---");
    QList<XQPluginInfo> impls;
    
    XQPluginLoader::listImplementations("org.nokia.mmdt.TestPlugin/1.0", impls);

    log("list plugins:");
    
    for (int i=0; i<impls.size(); i++) {
        log("plugin found...");
        QString fmtStr("%1: %2");
        log(fmtStr.arg("dll: ").arg(impls[i].dllName()));
        log(fmtStr.arg("opaque data: ").arg(impls[i].opaqueData()));
        log(fmtStr.arg("uid: ").arg(impls[i].uid()));
        log("---");
    }

    XQPluginLoader pluginLoader;
    pluginLoader.setUid(0xE1253163);
    
    QObject *plugin = pluginLoader.instance();

    QString pluginFmt("Plugin instance: %1");
    log(pluginFmt.arg(reinterpret_cast<int>(plugin)));

    TestPluginInterface *pluginCast = qobject_cast<TestPluginInterface*>(plugin);
    log(pluginFmt.arg(reinterpret_cast<int>(pluginCast)));


    if (pluginCast!=0)
    	log(pluginFmt.arg(pluginCast->test()));
    
    log("--- Stop ---");
//    return app.exec();
}

