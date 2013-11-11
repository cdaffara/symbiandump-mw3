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

#ifndef XQAIWINTERFACEDESCRIPTOR_H
#define XQAIWINTERFACEDESCRIPTOR_H

#include <QString>
#include <QVariant>
#include <QStringList>
#include <xqserviceglobal.h>

class XQAiwInterfaceDescriptorPrivate;

// Custom keys
namespace XQAiw
{
// NOTE !!! Names need to be fixed when service naming policy has been decided !!!
//
static const char *CustomKeyScheme = "schemes";  // CSV of schemes
static const char *CustomKeyActionText = "txt_aiw_action_text";
static const char *InterfaceUri = "com.nokia.symbian.IUriView";  // Interface to handle URIs (http://, file://, etc.)
static const char *InterfaceFile = "com.nokia.symbian.IFileView"; // Interface to handle local files in file system
static const char *OperationView = "view(QString)";  // Default operation signature for viewing files
static const char *OperationViewSharable = "view(XQSharableFile)";  // Default signature for viewing sharable files
static const char *UriSchemeApp = "appto"; // Custom scheme: appto://uid3?query-part
static const char *UriSchemeFile = "file";  // Standard  file sceheme

}

class XQSERVICEUTIL_EXPORT XQAiwInterfaceDescriptor
{
public:
    enum PropertyKey {
        Capabilities = 0,
        Location, 
        ServiceDescription,
        InterfaceDescription,
        ImplementationId = 100  // Extension: Settable property, contains impl. id

    };
    
public:
    XQAiwInterfaceDescriptor();
    XQAiwInterfaceDescriptor(const XQAiwInterfaceDescriptor& other);
    ~XQAiwInterfaceDescriptor();
    XQAiwInterfaceDescriptor& operator=(const XQAiwInterfaceDescriptor& other);
    bool operator==(const XQAiwInterfaceDescriptor& other) const;
    inline bool operator!=(const XQAiwInterfaceDescriptor& other) const { return !operator==(other); };
    QString serviceName() const;
    QString interfaceName() const;
    int majorVersion() const;
    int minorVersion() const;
    bool isValid() const;
    bool inSystemScope() const;
    QVariant property(XQAiwInterfaceDescriptor::PropertyKey key) const;
    QString customProperty(const QString& key) const;
    QStringList customPropertyKeys() const;
public:
    // Extensions: For service management
    bool setProperty(XQAiwInterfaceDescriptor::PropertyKey key, const QVariant value );

private:
    XQAiwInterfaceDescriptorPrivate* d;
    friend class XQAiwInterfaceDescriptorPrivate;
    friend class ServiceMetaData;
    
#ifndef QT_NO_DATASTREAM
    friend XQSERVICEUTIL_EXPORT QDataStream &operator<<(QDataStream &, const XQAiwInterfaceDescriptor &);
    friend XQSERVICEUTIL_EXPORT QDataStream &operator>>(QDataStream &, XQAiwInterfaceDescriptor &);
#endif
};


#ifndef QT_NO_DATASTREAM
XQSERVICEUTIL_EXPORT QDataStream &operator<<(QDataStream &, const XQAiwInterfaceDescriptor &);
XQSERVICEUTIL_EXPORT QDataStream &operator>>(QDataStream &, XQAiwInterfaceDescriptor &);
#endif
#ifndef QT_NO_DEBUG_STREAM
XQSERVICEUTIL_EXPORT QDebug operator<<(QDebug, const XQAiwInterfaceDescriptor &);
#endif




#endif
