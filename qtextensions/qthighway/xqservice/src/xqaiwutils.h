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

#ifndef XQAIWUTILS_H
#define XQAIWUTILS_H

#include <xqaiwglobal.h>
#include <xqaiwinterfacedescriptor.h>

#include <QObject>
#include <QFile>
#include <QUrl>
#include <QVariant>
#include <QList>
#include <qglobal.h>
#include <xqsharablefile.h>

class XQAiwUtilsPrivate;
class XQAiwUtils : public QObject
    {
public:

    XQAiwUtils();
    virtual ~XQAiwUtils();
    int launchApplication(int applicationId, const QList<QVariant> &arguments);
    int launchFile(int applicationId, const QVariant &file);
    int mapError(int symbianError);
    int findApplication(const QFile &file, int &applicationId);
    int findApplication(const XQSharableFile &file, int &applicationId);
    int findApplication(const QUrl &uri, int &applicationId);
    QString createCmdlineArgs(const QList<QVariant> &args);
    static QString createErrorMessage(int errorCode, const QString context, const QString detail = "");
    
    bool getDrmAttributes(const QString &file, const QList<int> &attributes, QVariantList & result);
    bool getDrmAttributes(const XQSharableFile &file, const QList<int> & attributes, QVariantList & result);
    int toIntFromHex(const QString &str, bool *ok);
    
private:

private:
    XQAiwUtilsPrivate * d;
    
    };

#endif /* XQAIWUTILS_H */ 
