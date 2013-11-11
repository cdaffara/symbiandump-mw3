/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Service metadata parser class implementation 
*
*/

#include "xqservicelog.h"

#include <QFile>
#include "xqservicemetadata_p.h"
#include "xqsfwinterface_p.h"
 

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


QT_BEGIN_NAMESPACE

static const char  PATH_SEPARATOR[] = "\\";

/*!
    \class ServiceMetaData

    Utility class (used by service database) that offers support for 
    parsing metadata service xml registry file during service registration. \n
    
    It uses QXMLStreamReader class for parsing. Supproted Operations are:
        - Parse the service and interfaces defined in XML file
        - name, version, capabilitiesList, description and filePath of service can be retrieved
        - each interface can be retrieved
*/

/*!
 *  Class constructor
 *
 * @param aXmlFilePath path to the xml file that describes the service. 
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
 *  Class constructor
 *
 * @param device QIODevice that contains the XML data that describes the service.
 */
ServiceMetaData::ServiceMetaData(QIODevice *device)
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::ServiceMetaData(2)");
    xmlDevice = device;
    ownsXmlDevice = false;
    latestError = 0;
}

/*!
 *  Class destructor
 * 
 */
ServiceMetaData::~ServiceMetaData()
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::~ServiceMetaData");
    if (ownsXmlDevice)
        delete xmlDevice;
}

/*!
    Sets the device containing the XML data that describes the service to \a device.
 */
void ServiceMetaData::setDevice(QIODevice *device)
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::setDevice");
    clearMetadata();
    xmlDevice = device;
    ownsXmlDevice = false;
}

/*!
    Returns the device containing the XML data that describes the service.
*/
QIODevice *ServiceMetaData::device() const
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::device");
    return xmlDevice;
}

/*!
 *  Gets the service name
 *
 * @return service name or default value (empty string) if it is not available
 */
QString ServiceMetaData::name()
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::name");
    XQSERVICE_DEBUG_PRINT("serviceName: %s", qPrintable(serviceName));
    return serviceName;
}
 
/*!
 *  Sets the path of service implementation file
 *
 * @param aFilePath path of service implementation file
 */
void ServiceMetaData::setServiceFilePath(const QString &aFilePath)
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::setServiceFilePath");
    XQSERVICE_DEBUG_PRINT("aFilePath: %s", qPrintable(aFilePath));
    serviceFilePath = aFilePath;
}
 
/*!
 *  Gets the path of the service implementation file
 *
 * @return service implementation filepath
 */
QString ServiceMetaData::filePath()
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::filePath");
    XQSERVICE_DEBUG_PRINT("serviceFilePath: %s", qPrintable(serviceFilePath));
    return serviceFilePath;
}
 
/*!
 *  Gets the service description
 *
 * @return service description or default value (empty string) if it is not available
 */
QString ServiceMetaData::description()
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::description");
    XQSERVICE_DEBUG_PRINT("serviceDescription: %s", qPrintable(serviceDescription));
    return serviceDescription;
}
 
/*!
   Returns the number of interfaces provided by the service description
 */
int ServiceMetaData::interfaceCount()
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::interfaceCount"); 
    XQSERVICE_DEBUG_PRINT("serviceInterfaces.count(): %d", serviceInterfaces.count());
    return serviceInterfaces.count();
}
 
/*!
   Returns the metadata of the interace at \a index; otherwise
   returns 0.
 */
QList<SFWInterface> ServiceMetaData::getInterfaces()
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::getInterfaces");
    return serviceInterfaces;
} 

/*!
    Parses the file and extracts the service metadata \n
    Custom error codes: \n
    SFW_ERROR_UNABLE_TO_OPEN_FILE in case can not open the XML file \n
    SFW_ERROR_INVALID_XML_FILE in case service registry is not a valid XML file \n
    SFW_ERROR_NO_SERVICE in case XML file has no service tag\n
    @return true if the metadata was read properly, false if there is an error
 */
bool ServiceMetaData::extractMetadata()
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::extractMetadata");
    latestError = 0;
    clearMetadata();                   
    QXmlStreamReader xmlReader;
    bool parseError = false;
    //Open xml file
    if (!xmlDevice->isOpen() && !xmlDevice->open(QIODevice::ReadOnly)) {
        XQSERVICE_DEBUG_PRINT("Couldn't open the file");
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
                if (!processServiceElement(xmlReader)) {
                    XQSERVICE_DEBUG_PRINT("Couldn't process service element");
                    parseError = true;
                }
            }
            else if (xmlReader.isStartElement() && xmlReader.name() != SERVICE_TAG) {
                XQSERVICE_DEBUG_PRINT("No service");
                latestError = ServiceMetaData::SFW_ERROR_NO_SERVICE;
                parseError = true;
            }
            else if (xmlReader.tokenType() == QXmlStreamReader::Invalid) {
                XQSERVICE_DEBUG_PRINT("Invalid XML");
                latestError = ServiceMetaData::SFW_ERROR_INVALID_XML_FILE;
                parseError = true;
            }
        }
        if (ownsXmlDevice)
            xmlDevice->close();
    }
    if (parseError) {
        clearMetadata();
    }
    XQSERVICE_DEBUG_PRINT("parseError: %d", parseError);
    return !parseError;
}
 
/*!
    Gets the latest parsing error \n
    @return parsing error(negative value) or 0 in case there is none
 */
int ServiceMetaData::getLatestError()
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::getLatestError");
    XQSERVICE_DEBUG_PRINT("latestError: %d", latestError);
    return latestError;
}
 
/*!
    Gets the value of the attribute from the XML node \n
    @param aDomElement xml node
    @param aAttributeName attribute name
    @param aValue [out] attribute value
    @return true if the value was read, false otherwise
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
  
/*!
    Parses and extracts the service metadata from the current xml <service> node \n
    Custom error codes: \n
    SFW_ERROR_NO_SERVICE_NAME in case no service name in XML file \n
    SFW_ERROR_NO_INTERFACE_VERSION in case no interface version in XML file \n
    SFW_ERROR_PARSE_SERVICE in case can not parse service section in XML file \n
    SFW_ERROR_NO_SERVICE_FILEPATH in case no service file path in XML file \n
    SFW_ERROR_INVALID_XML_FILE in case XML file is not valid \n
    SFW_ERROR_NO_SERVICE_INTERFACE in case no interface defined for service in XML file \n
    @param aXMLReader xml stream reader 
    @return true if the metadata was read properly, false if there is an error
 */
bool ServiceMetaData::processServiceElement(QXmlStreamReader &aXMLReader)
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::processServiceElement");
    Q_ASSERT(aXMLReader.isStartElement() && aXMLReader.name() == SERVICE_TAG);
    bool parseError = false;

    if (!getAttributeValue(aXMLReader, NAME_TAG, serviceName)) {
        XQSERVICE_DEBUG_PRINT("No service name");
        latestError = ServiceMetaData::SFW_ERROR_NO_SERVICE_NAME;
        parseError = true;
    }

    if (!parseError) {
        if (!getAttributeValue(aXMLReader, SERVICE_FILEPATH, serviceFilePath)) {
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
            if (!processInterfaceElement(aXMLReader)){
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
    Parses and extracts the interface metadata from the current xml <interface> node \n
    Custome error codes: \n
    SFW_ERROR_NO_INTERFACE_NAME in case no interface name in XML file \n
    SFW_ERROR_PARSE_INTERFACE in case error parsing interface section \n
    SFW_ERROR_INVALID_XML_FILE in case XML file is not valid \n
    @param aXMLReader xml stream reader 
    @return true if the metadata was read properly, false if there is an error
 */
bool ServiceMetaData::processInterfaceElement(QXmlStreamReader &aXMLReader)
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::processInterfaceElement");
    Q_ASSERT(aXMLReader.isStartElement() && aXMLReader.name() == INTERFACE_TAG);
    bool parseError = false;

    //Read interface parameter
    QString tmp;
    SFWInterface aInterface("");
    if (getAttributeValue(aXMLReader, NAME_TAG, tmp)) {
        XQSERVICE_DEBUG_PRINT("Name attribute value");
        aInterface = SFWInterface(tmp);
        tmp.clear();
        if (getAttributeValue(aXMLReader, INTERFACE_VERSION, tmp)) {
            XQSERVICE_DEBUG_PRINT("Interface version value");
            bool success = checkVersion(tmp);
            if ( success ) {
                aInterface.setVersion(tmp);
                tmp.clear();
                if (getAttributeValue(aXMLReader, INTERFACE_CAPABILITY, tmp)) {
                    XQSERVICE_DEBUG_PRINT("Interface capability value");
                    aInterface.setCapabilities(tmp.split(",", QString::SkipEmptyParts));
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
            aInterface.setDescription(aXMLReader.readElementText());
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

    if (!parseError) {
        const QString ident = aInterface.name()+aInterface.version();
        XQSERVICE_DEBUG_PRINT("ident: %s", qPrintable(ident));
        if (duplicates.contains(ident.toLower())) {
            XQSERVICE_DEBUG_PRINT("Duplicate interface");
            latestError = ServiceMetaData::SFW_ERROR_DUPLICATED_INTERFACE;
            parseError = true;
        } else {
            duplicates.insert(ident.toLower());
            serviceInterfaces.append(aInterface);
        }
    }
    XQSERVICE_DEBUG_PRINT("parseError: %d", parseError);
    return !parseError;
}

bool ServiceMetaData::checkVersion(const QString &version)
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

/*!
 *  Clears the service metadata
 *
 */
void ServiceMetaData::clearMetadata()
{
    XQSERVICE_DEBUG_PRINT("ServiceMetaData::clearMetadata");
    serviceName.clear();
    serviceFilePath.clear();
    serviceDescription.clear();
    serviceInterfaces.clear();
    duplicates.clear();
}

QT_END_NAMESPACE
