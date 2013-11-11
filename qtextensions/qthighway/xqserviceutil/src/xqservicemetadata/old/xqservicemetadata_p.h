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

#ifndef XQSERVICEMETADATA_H
#define XQSERVICEMETADATA_H

#include <QXmlStreamReader>
#include <QStringList>
#include <QList>
#include <QSet>
#include "qserviceglobal.h"
#include "xqaiwinterfacedescriptor.h"

class QIODevice;

QT_BEGIN_NAMESPACE

// FORWARD DECLARATIONS
class XQAiwInterfaceDescriptor;

class ServiceMetaDataResults 
{
public:
    ServiceMetaDataResults() {}

    ServiceMetaDataResults(const ServiceMetaDataResults& other)
    {
        description = other.description;
        location = other.location;
        name = other.name;
        interfaces = other.interfaces;
        latestInterfaces = other.latestInterfaces;
    }
    
    QString location;
    QString name;
    QString description;
    QList<XQAiwInterfaceDescriptor> interfaces;
    QList<XQAiwInterfaceDescriptor> latestInterfaces;
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
        SFW_ERROR_NO_SERVICE = 0,                           /* Can not find service root node in XML file*/
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
        SFW_ERROR_INVALID_VERSION,                          /* Invalid version number */
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

    /*TBR: void setServiceFilePath(const QString &aFilePath); */

    bool extractMetadata();

    int getLatestError();

    /*TBR:
    QString name();

    QString filePath();

    QString description();

    int interfaceCount();
    
    QList<SFWInterface> getInterfaces();
    
    bool checkVersion(const QString &version);
    */

    ServiceMetaDataResults parseResults() const;


private:
    bool getAttributeValue(const QXmlStreamReader &aDomElement,
            const QString &aAttributeName, QString &aValue);
    QList<XQAiwInterfaceDescriptor> latestInterfaces() const;
    XQAiwInterfaceDescriptor latestInterfaceVersion(const QString &interfaceName);
    bool processServiceElement(QXmlStreamReader &aXMLReader);
    
    bool processInterfaceElement(QXmlStreamReader &aXMLReader);

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
};

QT_END_NAMESPACE

#endif // XQSERVICEMETADATA_H
