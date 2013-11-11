/****************************************************************************
**
** Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Mobility Components.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in Technology Preview License Agreement accompanying
** this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** If you have questions regarding the use of this file, please
** contact Nokia at http://qt.nokia.com/contact.
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef XQSERVICEMETADATA_H
#define XQSERVICEMETADATA_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of the QLibrary class.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//
#include <QXmlStreamReader>
#include <QStringList>
#include <QList>
#include <QSet>
#include "xqserviceglobal.h"
#include "xqserviceerrdefs.h" // error codes
#include "xqaiwinterfacedescriptor.h"

class QIODevice;

QT_BEGIN_NAMESPACE

// FORWARD DECLARATIONS
class XQAiwInterfaceDescriptor;

class ServiceMetaDataResults 
{
public:
    enum ServiceMetadataVersion {
        VERSION_1 = 1,
        VERSION_2 = 2  // default
    };
    
    ServiceMetaDataResults() {};
    
    ServiceMetaDataResults(const ServiceMetaDataResults& other)
    {
        description = other.description;
        location = other.location;
        name = other.name;
        interfaces = other.interfaces;
        latestInterfaces = other.latestInterfaces;
        version = other.version;
    };
    
    QString location;
    QString name;
    QString description;
    QList<XQAiwInterfaceDescriptor> interfaces;
    QList<XQAiwInterfaceDescriptor> latestInterfaces;
    int version;  // ServiceMetadataVersion
};

#ifndef QT_NO_DATASTREAM
Q_SFW_EXPORT QDataStream &operator<<(QDataStream &, const ServiceMetaDataResults &);
Q_SFW_EXPORT QDataStream &operator>>(QDataStream &, ServiceMetaDataResults &);
#endif

class Q_SFW_EXPORT ServiceMetaData 
{
public:

    //! ServiceMetaData::ServiceMetadataErr
    /*!
     This enum describes the errors that may be returned by the Service metadata parser.
     */
    enum ServiceMetadataErr {
        SFW_ERROR_NO_SERVICE = QtService::METADATA_ERR_START_VALUE,    /* Can not find service root node in XML file*/
        SFW_ERROR_NO_SERVICE_NAME,                          /* Can not find service name in XML file */
        SFW_ERROR_NO_SERVICE_FILEPATH,                      /* Can not find service filepath in XML file */
        SFW_ERROR_NO_SERVICE_INTERFACE,                     /* No interface for the service in XML file*/
        SFW_ERROR_NO_INTERFACE_VERSION,                     /* Can not find interface version in XML file */
        SFW_ERROR_NO_INTERFACE_NAME,                        /* Can not find interface name in XML file*/
        SFW_ERROR_UNABLE_TO_OPEN_FILE,                      /* Error opening XML file*/
        SFW_ERROR_INVALID_XML_FILE,                         /* Not a valid XML file*/
        SFW_ERROR_PARSE_SERVICE,                            /* Error parsing service node */
        SFW_ERROR_PARSE_INTERFACE,                          /* Error parsing interface node */
        SFW_ERROR_DUPLICATED_INTERFACE,                     /* The same interface is defined twice */
        SFW_ERROR_INVALID_VERSION,
        SFW_ERROR_DUPLICATED_TAG,                           /* The tag appears twice */
        SFW_ERROR_INVALID_CUSTOM_TAG,                       /* The customproperty tag is not corectly formatted or otherwise incorrect*/
        SFW_ERROR_DUPLICATED_CUSTOM_KEY                     /* The customproperty appears twice*/
    };

public:

    ServiceMetaData(const QString &aXmlFilePath);

    ServiceMetaData(QIODevice *device);

    ~ServiceMetaData();

    void setDevice(QIODevice *device);
    
    QIODevice *device() const;

    bool extractMetadata();

    int getLatestError() const;

    ServiceMetaDataResults parseResults() const;

private:
    QList<XQAiwInterfaceDescriptor> latestInterfaces() const;
    XQAiwInterfaceDescriptor latestInterfaceVersion(const QString &interfaceName);
    bool processServiceElement(QXmlStreamReader &aXMLReader);
    bool processInterfaceElement(QXmlStreamReader &aXMLReader);

    // Support for old XML format
    bool getAttributeValue(const QXmlStreamReader &aDomElement,
                           const QString &aAttributeName, QString &aValue);
    bool processServiceElementPrevVersion(QXmlStreamReader &aXMLReader);
    bool processInterfaceElementPrevVersion(QXmlStreamReader &aXMLReader);

    void clearMetadata();

private:
    bool lessThan(const XQAiwInterfaceDescriptor &d1,
                    const XQAiwInterfaceDescriptor &d2) const;
    bool checkVersion(const QString &version) const;
    void transformVersion(const QString &version, int *major, int *minor) const;

    QIODevice *xmlDevice;
    bool ownsXmlDevice;
    QString serviceName;
    QString serviceLocation;
    QString serviceDescription;
    QList<XQAiwInterfaceDescriptor> serviceInterfaces;
    QSet<QString> duplicates;
    int latestError;
    QHash<QString, int> m_latestIndex;
    int version;  // data version 
};

QT_END_NAMESPACE

#endif // XQSERVICEMETADATA_H
