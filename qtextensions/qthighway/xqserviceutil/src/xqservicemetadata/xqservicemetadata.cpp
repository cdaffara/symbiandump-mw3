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

#include <QFile>
#include "xqservicelog.h"
#include "xqservicemetadata_p.h"
#include <xqaiwinterfacedescriptor_p.h>

//XML tags and attributes
//General
#define NAME_TAG  "name"
#define DESCRIPTION_TAG "description"

//Service related
#define SERVICE_TAG "service" 
#define SERVICE_FILEPATH "filepath"

//Interface related
#define INTERFACE_TAG "interface"
#define INTERFACE_VERSION "version" 
#define INTERFACE_CAPABILITY "capabilities"
#define INTERFACE_CUSTOM_PROPERTY "customproperty"

QT_BEGIN_NAMESPACE

#ifndef QT_NO_DATASTREAM
QDataStream &operator<<(QDataStream &out, const ServiceMetaDataResults &r)
{
    out << r.name << r.location;
    out << r.description << r.interfaces << r.latestInterfaces;

    return out;
}

QDataStream &operator>>(QDataStream &in, ServiceMetaDataResults &r)
{
    in >> r.name >> r.location;
    in >> r.description >> r.interfaces >> r.latestInterfaces;

    return in;
}
#endif

/*!
    \class ServiceMetaData
    \brief Utility class offering support for parsing metadata
           service xml registry files.

    Utility class (used by service database) that offers support for 
    parsing metadata service xml registry file during service registration. \n
    
    It uses QXMLStreamReader class for parsing. Supproted Operations are:
        - Parse the service and interfaces defined in XML file
        - name, version, capabilitiesList, description and filePath of service can be retrieved
        - each interface can be retrieved
*/

/*!
    Constructor
    \param aXmlFilePath Path to the xml file that describes the service. 
*/
ServiceMetaData::ServiceMetaData(const QString &aXmlFilePath)
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::ServiceMetaData(1)");
    XQSERVICE_DEBUG_PRINT("aXmlFilePath: %s", qPrintable(aXmlFilePath));
    xmlDevice = new QFile(aXmlFilePath);
    ownsXmlDevice = true;
    latestError = 0;
}

/*!
    Constructor
    \param device QIODevice that contains the XML data that describes the service.
*/
ServiceMetaData::ServiceMetaData(QIODevice *device)
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::ServiceMetaData(2)");
    xmlDevice = device;
    ownsXmlDevice = false;
    latestError = 0;
}

/*!
    Destructor
*/
ServiceMetaData::~ServiceMetaData()
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::~ServiceMetaData");
    if (ownsXmlDevice)
        delete xmlDevice;
}

/*!
    Sets the device containing the XML data that describes the service to \a device.
    \param device Device containing XML data.
*/
void ServiceMetaData::setDevice(QIODevice *device)
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::setDevice");
    clearMetadata();
    xmlDevice = device;
    ownsXmlDevice = false;
}

/*!
    Gets the device containing the XML data that describes the service.
    \return Device containing the XML data.
*/
QIODevice *ServiceMetaData::device() const
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::device");
    return xmlDevice;
}

/*!
    Gets the service name.
    \return Service name or default value (empty string) if it is not available.
*/
 
/*QString ServiceMetaData::name() const
{
    return serviceName;
}*/
 
/*!
    Gets the path of the service implementation file.
    \return Service implementation filepath.
*/
/*QString ServiceMetaData::location() const
{
    return serviceLocation;
}*/
 
/*!
    Gets the service description.
    \return Service description or default value (empty string) if it is not available.
*/
/*QString ServiceMetaData::description() const
{
    return serviceDescription;
}*/
 
/*!
    Gets the list of interfaces.
    \return List interfaces.
*/
/*QList<XQServiceInterfaceDescriptor> ServiceMetaData::getInterfaces() const
{
    return serviceInterfaces;
} */

/*!
    \internal
    Returns a streamable object containing the results of the parsing.
*/
ServiceMetaDataResults ServiceMetaData::parseResults() const
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::parseResults");
    
    ServiceMetaDataResults results;
    results.location = serviceLocation;
    results.name = serviceName;
    results.description = serviceDescription;
    results.interfaces = serviceInterfaces;
    results.latestInterfaces = latestInterfaces();
    results.version = version;

    return results;
}

/*!
    Parses the file and extracts the service metadata \n
    Custom error codes: \n
    SFW_ERROR_UNABLE_TO_OPEN_FILE in case can not open the XML file \n
    SFW_ERROR_INVALID_XML_FILE in case service registry is not a valid XML file \n
    SFW_ERROR_NO_SERVICE in case XML file has no service tag\n
    \return true if the metadata was read properly, false if there is an error.
 */
bool ServiceMetaData::extractMetadata()
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::extractMetadata");
    latestError = 0;
    clearMetadata();
    version = ServiceMetaDataResults::VERSION_2;  // default
    
    QXmlStreamReader xmlReader;
    bool parseError = false;
    //Open xml file
    if (!xmlDevice->isOpen() && !xmlDevice->open(QIODevice::ReadOnly)) {
        XQSERVICE_DEBUG_PRINT("XML error:Couldn't open the file");
        latestError = ServiceMetaData::SFW_ERROR_UNABLE_TO_OPEN_FILE;
        parseError = true;
    } else {
        //Load xml content
        xmlReader.setDevice(xmlDevice);
        // Read XML doc 
        while (!xmlReader.atEnd() && !parseError) {
            xmlReader.readNext();
            //Found a <service> node, read service related metadata
            if (xmlReader.isStartElement() && xmlReader.name() == SERVICE_TAG) {

                // Support for previous XML version. Check if service element has name attribute
                // If so, assume the old element
                if (getAttributeValue(xmlReader, NAME_TAG, serviceName)) {
                    if (!processServiceElementPrevVersion(xmlReader)) {
                        XQSERVICE_DEBUG_PRINT("XML error: Couldn't process service element");
                        parseError = true;
                    }
                }
                else if (!processServiceElement(xmlReader)) {
                    XQSERVICE_DEBUG_PRINT("XML error: Couldn't process service element");
                    parseError = true;
                }
            }
            else if (xmlReader.isStartElement() && xmlReader.name() != SERVICE_TAG) {
                XQSERVICE_DEBUG_PRINT("XML error: No service");
                latestError = ServiceMetaData::SFW_ERROR_NO_SERVICE;
                parseError = true;
            }
            else if (xmlReader.tokenType() == QXmlStreamReader::Invalid) {
                XQSERVICE_DEBUG_PRINT("XML error: Invalid XML");
                latestError = ServiceMetaData::SFW_ERROR_INVALID_XML_FILE;
                parseError = true;
            }
        }
        if (ownsXmlDevice)
            xmlDevice->close();
    }
    if (parseError) {
        {
        XQSERVICE_DEBUG_PRINT("XML Parse error, line=%d,column=%d", xmlReader.lineNumber(), xmlReader.columnNumber());
        clearMetadata();
        }
    }
    XQSERVICE_DEBUG_PRINT("XML parseError: %d", parseError);
    return !parseError;
}
 
/*!
    Gets the latest parsing error.
    \return Parsing error(negative value) or 0 in case there is none.
*/
int ServiceMetaData::getLatestError() const
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::getLatestError");
    XQSERVICE_DEBUG_PRINT("latestError: %d", latestError);
    return latestError;
}
 
/*!
    Parses and extracts the service from the current xml <service> node
    using the new format (Version 2).
    Schema:    
     <!ELEMENT service ( name, filepath, description?, interface+ ) >
        <!ELEMENT description ( #CDATA ) >
        <!ELEMENT filepath ( #PCDATA ) >
        <!ELEMENT interface ( name, version, description?, capabilities?, customproperty* ) >
        <!ELEMENT capabilities ( #PCDATA ) >
        <!ELEMENT name ( #PCDATA ) >
        <!ELEMENT version ( #PCDATA ) >
        <!ELEMENT customproperty ( #CDATA ) >
        <!ATTLIST customproperty key NMTOKEN #REQUIRED >    
 */
bool ServiceMetaData::processServiceElement(QXmlStreamReader &aXMLReader)
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::processServiceElement");
    Q_ASSERT(aXMLReader.isStartElement() && aXMLReader.name() == SERVICE_TAG);
    bool parseError = false;

    int dupSTags[3] = {0 //->tag name
        ,0 //-> service description
        ,0 //-> filepath
    };
    while(!parseError && !aXMLReader.atEnd()) {
        aXMLReader.readNext();
        if (aXMLReader.isStartElement() && aXMLReader.name() == DESCRIPTION_TAG) {
            //Found <description> tag
            serviceDescription = aXMLReader.readElementText();
            dupSTags[1]++;
        } else if (aXMLReader.isStartElement() && aXMLReader.name() == NAME_TAG) {
            serviceName = aXMLReader.readElementText();
            dupSTags[0]++;
        } else if (aXMLReader.isStartElement() && aXMLReader.name() == INTERFACE_TAG) {
            //Found a <interface> node, read module related metadata  
            if (!processInterfaceElement(aXMLReader)) 
                parseError = true;
        } else if (aXMLReader.isStartElement() && aXMLReader.name() == SERVICE_FILEPATH ) {
            //Found <filepath> tag
            dupSTags[2]++;
            serviceLocation = aXMLReader.readElementText();
        } else if (aXMLReader.isStartElement() && aXMLReader.name() == "version") {
            //FOUND <version> tag on service level. We ignore this for now
            aXMLReader.readElementText();
        } else if (aXMLReader.isEndElement() && aXMLReader.name() == SERVICE_TAG) {
            //Found </service>, leave the loop
            break;
        } else if (aXMLReader.isEndElement() || aXMLReader.isStartElement()) {
            latestError = ServiceMetaData::SFW_ERROR_PARSE_SERVICE;
            parseError = true;            
        } else if (aXMLReader.tokenType() == QXmlStreamReader::Invalid) {
            latestError = ServiceMetaData::SFW_ERROR_INVALID_XML_FILE;
            parseError = true;
        }
    }
    if ( !parseError ) {
        if (serviceName.isEmpty()) {
            latestError = ServiceMetaData::SFW_ERROR_NO_SERVICE_NAME;
            parseError = true;
        } else if (serviceLocation.isEmpty()) {
            latestError = ServiceMetaData::SFW_ERROR_NO_SERVICE_FILEPATH;
            parseError = true;
        }
    }

    for(int i=0;!parseError && i<3;i++) {
        if (dupSTags[i] > 1) {
            parseError = true;
            latestError = SFW_ERROR_DUPLICATED_TAG;
            break;
        }
    }
        
    //update all interfaces with service data
    const int icount = serviceInterfaces.count();
    if (icount == 0 && latestError == 0) {
        latestError = ServiceMetaData::SFW_ERROR_NO_SERVICE_INTERFACE;
        parseError = true;
    }
    for (int i = 0; i<icount; i++) {
        serviceInterfaces.at(i).d->serviceName = serviceName;
        serviceInterfaces.at(i).d->properties[XQAiwInterfaceDescriptor::Location] = serviceLocation;
        serviceInterfaces.at(i).d->properties[XQAiwInterfaceDescriptor::ServiceDescription] = serviceDescription;
    }

    if (parseError) {
        clearMetadata();
    }
    XQSERVICE_DEBUG_PRINT("processServiceElement parseError: %d", parseError);
    return !parseError;
}

/*!
    Parses and extracts the service from the current xml <service> node
    using the new format (Version 1) \n
    
    <!ELEMENT service ( description?, interface+ ) >
    <!ATTLIST service name #CDATA  #REQUIRED >
    <!ATTLIST service filepath #CDATA  #REQUIRED >
    <!ELEMENT description ( #CDATA ) >
    <!ELEMENT interface ( description? ) >
    <!ATTLIST interface name #CDATA  #REQUIRED >
    <!ATTLIST interface version #CDATA  #REQUIRED >
    <!ATTLIST interface capabilities #CDATA  >
    
    Custom error codes: \n
    SFW_ERROR_NO_SERVICE_NAME in case no service name in XML file \n
    SFW_ERROR_NO_INTERFACE_VERSION in case no interface version in XML file \n
    SFW_ERROR_PARSE_SERVICE in case can not parse service section in XML file \n
    SFW_ERROR_NO_SERVICE_FILEPATH in case no service file path in XML file \n
    SFW_ERROR_INVALID_XML_FILE in case XML file is not valid \n
    SFW_ERROR_NO_SERVICE_INTERFACE in case no interface defined for service in XML file.
    \param aXMLReader xml stream reader .
    \return true if the metadata was read properly, false if there is an error.

    
 */
bool ServiceMetaData::processServiceElementPrevVersion(QXmlStreamReader &aXMLReader)
{
    version = ServiceMetaDataResults::VERSION_1;  // Previous version
    
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::processServiceElementPrevVersion");
    Q_ASSERT(aXMLReader.isStartElement() && aXMLReader.name() == SERVICE_TAG);
    bool parseError = false;

    QString tmp;
    if (!getAttributeValue(aXMLReader, NAME_TAG, tmp)) {
        XQSERVICE_DEBUG_PRINT("No service name");
        latestError = ServiceMetaData::SFW_ERROR_NO_SERVICE_NAME;
        parseError = true;
    }

    if (!parseError) {
        if (!getAttributeValue(aXMLReader, SERVICE_FILEPATH, serviceLocation)) {
            XQSERVICE_DEBUG_PRINT("No service filepath");
            latestError = ServiceMetaData::SFW_ERROR_NO_SERVICE_FILEPATH;
            parseError = true;
        }
    }

    while (!parseError && !aXMLReader.atEnd()) {
        aXMLReader.readNext();  
        if (aXMLReader.name() == DESCRIPTION_TAG) {
            serviceDescription = aXMLReader.readElementText();
            XQSERVICE_DEBUG_PRINT("serviceDescription: %s", qPrintable(serviceDescription));
        //Found a <interface> node, read module related metadata  
        } else if (aXMLReader.isStartElement() && aXMLReader.name() == INTERFACE_TAG) {
            if (!processInterfaceElementPrevVersion(aXMLReader)){
                XQSERVICE_DEBUG_PRINT("Couldn't process interface element");
                parseError = true;
            }
        //Found </service>, leave the loop
        } else if (aXMLReader.isEndElement() && aXMLReader.name() == SERVICE_TAG) {
            XQSERVICE_DEBUG_PRINT("Service element handled");
            break;
        } else if (aXMLReader.isEndElement() || aXMLReader.isStartElement()) {
            XQSERVICE_DEBUG_PRINT("Service parse error");
            latestError = ServiceMetaData::SFW_ERROR_PARSE_SERVICE;
            parseError = true;            
        } else if (aXMLReader.tokenType() == QXmlStreamReader::Invalid) {
            XQSERVICE_DEBUG_PRINT("Invalid XML");
            latestError = ServiceMetaData::SFW_ERROR_INVALID_XML_FILE;
            parseError = true;
        }
    }

    if (serviceInterfaces.count() == 0 && latestError == 0) {
        XQSERVICE_DEBUG_PRINT("No service interface");
        latestError = ServiceMetaData::SFW_ERROR_NO_SERVICE_INTERFACE;
        parseError = true;
    }
    if (parseError) {
        clearMetadata();
    }
    XQSERVICE_DEBUG_PRINT("parseError: %d", parseError);
    return !parseError;
}


/*!
    Parses and extracts the interface metadata from the current xml <interface> node.
    \param aXMLReader xml stream reader .
    \return true if the metadata was read properly, false if there is an error.
*/
bool ServiceMetaData::processInterfaceElement(QXmlStreamReader &aXMLReader)
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::processInterfaceElement");
    Q_ASSERT(aXMLReader.isStartElement() && aXMLReader.name() == INTERFACE_TAG);
    bool parseError = false;

    //Read interface parameter
    QString tmp;
    XQAiwInterfaceDescriptor aInterface;
    int dupITags[4] = {
        0,  //->iface name tag
        0,  //->version
        0,  //->capabilities
        0   //->description
    };
    aInterface.d = new XQAiwInterfaceDescriptorPrivate;
    while (!parseError && !aXMLReader.atEnd()) {
        aXMLReader.readNext();
        //Read interface description
        if (aXMLReader.isStartElement() && aXMLReader.name() == NAME_TAG) {
            aInterface.d->interfaceName = aXMLReader.readElementText();
            dupITags[0]++;
            //Found <name> tag for interface
        } else if (aXMLReader.isStartElement() && aXMLReader.name() == DESCRIPTION_TAG) {
            //Found <description> tag
            aInterface.d->properties[XQAiwInterfaceDescriptor::InterfaceDescription] = aXMLReader.readElementText();
            dupITags[3]++;
        //Found </interface>, leave the loop
        } else if (aXMLReader.isStartElement() && aXMLReader.name() == INTERFACE_VERSION) {
            tmp.clear();
            tmp = aXMLReader.readElementText();
            if (tmp.isEmpty())
                continue;  //creates NO_INTERFACE_VERSION error further below
            bool success = checkVersion(tmp);
            if ( success ) {
                int majorVer = -1;
                int minorVer = -1;
                transformVersion(tmp, &majorVer, &minorVer);
                aInterface.d->major = majorVer;
                aInterface.d->minor = minorVer;
                dupITags[1]++;
            } else {
                latestError = ServiceMetaData::SFW_ERROR_INVALID_VERSION;
                parseError = true;
            }
        } else if (aXMLReader.isStartElement() && aXMLReader.name() == INTERFACE_CAPABILITY) {
            tmp.clear();
            tmp= aXMLReader.readElementText();
            aInterface.d->properties[XQAiwInterfaceDescriptor::Capabilities] = tmp.split(",", QString::SkipEmptyParts);
            dupITags[2]++;
        } else if (aXMLReader.isStartElement() && aXMLReader.name() == INTERFACE_CUSTOM_PROPERTY) {
            parseError = true;
            if (aXMLReader.attributes().hasAttribute("key")) {
                const QString ref = aXMLReader.attributes().value("key").toString();
                XQSERVICE_DEBUG_PRINT("Custom property key: %s", qPrintable(ref));
                if (!ref.isEmpty()) {
                    if (aInterface.d->customProperties.contains(ref)) {
                        latestError = SFW_ERROR_DUPLICATED_CUSTOM_KEY;
                        continue;
                    } else {
                        QString value = aXMLReader.readElementText();
                        if (value.isEmpty() || value.isNull())
                            value = QString("");
                        XQSERVICE_DEBUG_PRINT("Custom property value: %s", qPrintable(value));
                        aInterface.d->customProperties[ref] = value;
                        parseError = false;
                    }
                }
            }
            if (parseError)
                latestError = SFW_ERROR_INVALID_CUSTOM_TAG;
        } else if (aXMLReader.isEndElement() && aXMLReader.name() == INTERFACE_TAG) {
            break;
        } else if (aXMLReader.isStartElement() || aXMLReader.isEndElement()) {
            latestError = ServiceMetaData::SFW_ERROR_PARSE_INTERFACE;
            parseError = true;
        } else if (aXMLReader.tokenType() == QXmlStreamReader::Invalid) {
            latestError = ServiceMetaData::SFW_ERROR_INVALID_XML_FILE;
            parseError = true;
        }
    }

    if (!parseError) {
        if (dupITags[1] == 0) { //no version tag found
            latestError = ServiceMetaData::SFW_ERROR_NO_INTERFACE_VERSION;
            parseError = true;
        } else if (aInterface.d->interfaceName.isEmpty()) {
            latestError = ServiceMetaData::SFW_ERROR_NO_INTERFACE_NAME;
            parseError = true;
        }
    }
    
    for(int i=0;!parseError && i<4;i++) {
        if (dupITags[i] > 1) {
            parseError = true;
            latestError = SFW_ERROR_DUPLICATED_TAG;
            break;
        }
    }

    if (!parseError) {
        const QString ident = aInterface.d->interfaceName
                                + QString::number(aInterface.majorVersion())
                                + "."
                                + QString::number(aInterface.minorVersion());
        if (duplicates.contains(ident.toLower())) {
            latestError = ServiceMetaData::SFW_ERROR_DUPLICATED_INTERFACE;
            parseError = true;
        } else {
            duplicates.insert(ident.toLower());
            serviceInterfaces.append(aInterface);
            if (!m_latestIndex.contains(aInterface.d->interfaceName.toLower())
                    || lessThan(latestInterfaceVersion(aInterface.d->interfaceName), aInterface))

            {
                    m_latestIndex[aInterface.d->interfaceName.toLower()] = serviceInterfaces.count() - 1;
            }
        }
    }

    if (parseError)
    {
        // Delete garbage
        delete aInterface.d;
        aInterface.d = 0;
    }
   XQSERVICE_DEBUG_PRINT("processInterfaceElement parseError: %d", parseError);
    return !parseError;
}

/*!
    Parses and extracts the interface metadata from the current xml <interface> node. \n
    Custome error codes: \n
    SFW_ERROR_NO_INTERFACE_NAME in case no interface name in XML file \n
    SFW_ERROR_PARSE_INTERFACE in case error parsing interface section \n
    SFW_ERROR_INVALID_XML_FILE in case XML file is not valid \n
    \param aXMLReader xml stream reader. 
    \return true if the metadata was read properly, false if there is an error.
*/
bool ServiceMetaData::processInterfaceElementPrevVersion(QXmlStreamReader &aXMLReader)
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::processInterfaceElementPrevVersion");
    Q_ASSERT(aXMLReader.isStartElement() && aXMLReader.name() == INTERFACE_TAG);
    bool parseError = false;

    //Read interface parameter
    QString tmp;
    XQAiwInterfaceDescriptor aInterface;
    aInterface.d = new XQAiwInterfaceDescriptorPrivate;
    aInterface.d->serviceName = serviceName;  // picked earlier !!!
    XQSERVICE_DEBUG_PRINT("Service name  %s", qPrintable(serviceName));
    
    int dupITags[4] = {
        0,  //->iface name tag
        0,  //->version
        0,  //->capabilities
        0   //->description
    };
    
    if (getAttributeValue(aXMLReader, NAME_TAG, tmp)) {
        XQSERVICE_DEBUG_PRINT("Name attribute value");
        aInterface.d->interfaceName = tmp;
        tmp.clear();
        if (getAttributeValue(aXMLReader, INTERFACE_VERSION, tmp)) {
            XQSERVICE_DEBUG_PRINT("Interface version value");
            bool success = checkVersion(tmp);
            if ( success ) {
                int majorVer = -1;
                int minorVer = -1;
                transformVersion(tmp, &majorVer, &minorVer);
                aInterface.d->major = majorVer;
                aInterface.d->minor = minorVer;
                dupITags[1]++;

                tmp.clear();  // Rememember to clear
                if (getAttributeValue(aXMLReader, INTERFACE_CAPABILITY, tmp)) {
                    XQSERVICE_DEBUG_PRINT("Interface capability value");
                    aInterface.d->properties[XQAiwInterfaceDescriptor::Capabilities] = tmp.split(",", QString::SkipEmptyParts);
                }
            } else {
                XQSERVICE_DEBUG_PRINT("Invalid interface version");
                latestError = ServiceMetaData::SFW_ERROR_INVALID_VERSION;
                parseError = true;
            }
        }
        else{
            XQSERVICE_DEBUG_PRINT("No interface version");
            latestError = ServiceMetaData::SFW_ERROR_NO_INTERFACE_VERSION;
            parseError = true;
        }
    } else {
        XQSERVICE_DEBUG_PRINT("No interface name");
        latestError = ServiceMetaData::SFW_ERROR_NO_INTERFACE_NAME;
        parseError = true;
    }

    while (!parseError && !aXMLReader.atEnd()) {
        aXMLReader.readNext();
        //Read interface description
        if (aXMLReader.isStartElement() && aXMLReader.name() == DESCRIPTION_TAG) {
            XQSERVICE_DEBUG_PRINT("Interface description");
            aInterface.d->properties[XQAiwInterfaceDescriptor::InterfaceDescription] = aXMLReader.readElementText();
        //Found </interface>, leave the loop
        } else if (aXMLReader.isEndElement() && aXMLReader.name() == INTERFACE_TAG) {
            XQSERVICE_DEBUG_PRINT("Interface handled");
            break;  
        } else if (aXMLReader.isStartElement() || aXMLReader.isEndElement()) {
            XQSERVICE_DEBUG_PRINT("Interface parse error");
            latestError = ServiceMetaData::SFW_ERROR_PARSE_INTERFACE;
            parseError = true;
        } else if (aXMLReader.tokenType() == QXmlStreamReader::Invalid) {
            XQSERVICE_DEBUG_PRINT("Invalid XML");
            latestError = ServiceMetaData::SFW_ERROR_INVALID_XML_FILE;
            parseError = true;
        }
    }

    // Consistency check
    if (!parseError) {
        if (dupITags[1] == 0) { //no version tag found
            latestError = ServiceMetaData::SFW_ERROR_NO_INTERFACE_VERSION;
            parseError = true;
        } else if (aInterface.d->interfaceName.isEmpty()) {
            latestError = ServiceMetaData::SFW_ERROR_NO_INTERFACE_NAME;
            parseError = true;
        }
    }

    if (!parseError) {
        const QString ident = aInterface.d->interfaceName
                              + QString::number(aInterface.majorVersion())
                              + "."
                              + QString::number(aInterface.minorVersion());
        XQSERVICE_DEBUG_PRINT("ident: %s", qPrintable(ident));
        if (duplicates.contains(ident.toLower())) {
            XQSERVICE_DEBUG_PRINT("Duplicate interface");
            latestError = ServiceMetaData::SFW_ERROR_DUPLICATED_INTERFACE;
            parseError = true;
        } else {
            duplicates.insert(ident.toLower());
            serviceInterfaces.append(aInterface);
            if (!m_latestIndex.contains(aInterface.d->interfaceName.toLower())
                || lessThan(latestInterfaceVersion(aInterface.d->interfaceName), aInterface))

            {
                m_latestIndex[aInterface.d->interfaceName.toLower()] = serviceInterfaces.count() - 1;
            }
        }
    }

    if (parseError)
    {
        // Delete garbage
        delete aInterface.d;
        aInterface.d = 0;
    }
    
    XQSERVICE_DEBUG_PRINT("processInterfaceElementPrevVersion parseError: %d", parseError);
    return !parseError;
}


/*!
    Gets the value of the attribute from the XML node.
    \param aDomElement Xml node.
    \param aAttributeName Attribute name.
    \param aValue [out] attribute value.
    \return true if the value was read, false otherwise.
*/
bool ServiceMetaData::getAttributeValue(const QXmlStreamReader &aXMLReader, const QString &aAttributeName, QString &aValue)
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::getAttributeValue");
    XQSERVICE_DEBUG_PRINT("aAttributeName: %s", qPrintable(aAttributeName));
    bool result = false;
    for (int i = 0; i < aXMLReader.attributes().count(); i++){
        QXmlStreamAttribute att = aXMLReader.attributes()[i];
        if (att.name() == aAttributeName) {
            if (att.value().isNull() || att.value().isEmpty()) {
                result = false;
            } else {
                result = true;
                aValue = att.value().toString();
                XQSERVICE_DEBUG_PRINT("aValue: %s", qPrintable(aValue));
            }
        }
    }
    // Capability attribute is allowed to be empty
    if (aAttributeName == INTERFACE_CAPABILITY) {
        result = true;
    }
    XQSERVICE_DEBUG_PRINT("result: %d", result);
    return result;
}


XQAiwInterfaceDescriptor ServiceMetaData::latestInterfaceVersion(const QString &interfaceName)
{
    XQAiwInterfaceDescriptor ret;
    if (m_latestIndex.contains(interfaceName.toLower()))
        return serviceInterfaces[m_latestIndex[interfaceName.toLower()]];
    else
        return ret;
}

QList<XQAiwInterfaceDescriptor> ServiceMetaData::latestInterfaces() const
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::latestInterfaces");
    QList<XQAiwInterfaceDescriptor> interfaces;
    QHash<QString,int>::const_iterator i = m_latestIndex.constBegin();
    while(i != m_latestIndex.constEnd())
    {
        interfaces.append(serviceInterfaces[i.value()]);
        ++i;
    }
    return interfaces;
}

bool ServiceMetaData::lessThan(const XQAiwInterfaceDescriptor &d1,
                                const XQAiwInterfaceDescriptor &d2) const
{
    return (d1.majorVersion() < d2.majorVersion())
            || ( d1.majorVersion() == d2.majorVersion()
                    && d1.minorVersion() < d2.minorVersion());

}

bool ServiceMetaData::checkVersion(const QString &version) const
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::checkVersion");
    //match x.y as version format
    QRegExp rx("^([1-9][0-9]*)\\.(0+|[1-9][0-9]*)$");
    int pos = rx.indexIn(version);
    QStringList list = rx.capturedTexts();
    bool success = false;
    if (pos == 0 && list.count() == 3
            && rx.matchedLength() == version.length() )
    {
        list[1].toInt(&success);
        if ( success ) {
            list[2].toInt(&success);
        }
    }
    XQSERVICE_DEBUG_PRINT("success: %d", success);
    return success;
}

void ServiceMetaData::transformVersion(const QString &version, int *major, int *minor) const
{
    Q_ASSERT(major != NULL);
    Q_ASSERT(minor != NULL);
    if(!checkVersion(version)) {
        *major = -1;
        *minor = -1;
    } else {
        QRegExp rx("^([1-9][0-9]*)\\.(0+|[1-9][0-9]*)$");
        rx.indexIn(version);
        QStringList list = rx.capturedTexts();
        Q_ASSERT(list.count() == 3);
        *major = list[1].toInt();
        *minor = list[2].toInt();
    }
}

/*!
    Clears the service metadata.
*/
void ServiceMetaData::clearMetadata()
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::clearMetadata");
    serviceName.clear();
    serviceLocation.clear();
    serviceDescription.clear();
    serviceInterfaces.clear();
    duplicates.clear();
    m_latestIndex.clear();
    version = 0;
}



QT_END_NAMESPACE
