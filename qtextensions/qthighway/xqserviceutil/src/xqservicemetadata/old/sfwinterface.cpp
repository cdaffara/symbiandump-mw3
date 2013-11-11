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

#include "xqservicelog.h"

#include "xqsfwinterface_p.h"

QT_BEGIN_NAMESPACE
/*!
    \class SFWInterface

    SFWInterface class is used by service metadata parser to keep interface related information (interface name and description) 
    from service XML registry file\n
*/

    
/*! 
 *  Class constructor 
 *
 * @param anInterfaceName interface name
 */
SFWInterface::SFWInterface(const QString& anInterfaceName)
{
    XQSERVICE_DEBUG_PRINT("SFWInterface::SFWInterface(1)");
    XQSERVICE_DEBUG_PRINT("anInterfaceName: %s", qPrintable(anInterfaceName));
    interfaceName = anInterfaceName;
}

/*!
    Creates a copy of \a other.
*/
SFWInterface::SFWInterface(const SFWInterface& other)
{
    XQSERVICE_DEBUG_PRINT("SFWInterface::SFWInterface(2)");
    (*this) = other; //use assignment operator

}

/*
    Copies the content of the SFWInterface object \a other 
    into this one.
*/
SFWInterface& SFWInterface::operator=(const SFWInterface&other)
{
    XQSERVICE_DEBUG_PRINT("SFWInterface::operator=");
    interfaceName = other.interfaceName;
    serviceName = other.serviceName;
    interfaceDescription = other.interfaceDescription;
    interfaceCapabilities = other.interfaceCapabilities;
    interfaceVersion = other.interfaceVersion;

    return *this;
}


/*!
 *  Class destructor 
 */
SFWInterface::~SFWInterface()
{
    XQSERVICE_DEBUG_PRINT("SFWInterface::~SFWInterface");
}

/*!
 *  Gets the interface name
 *
 * @return interface name or default value (empty string) if it is not available
 */
QString SFWInterface::name() const
{
    XQSERVICE_DEBUG_PRINT("SFWInterface::name");
    XQSERVICE_DEBUG_PRINT("interfaceName: %s", qPrintable(interfaceName));
    return interfaceName;
}

/*!
 *  Gets the interface description
 *
 * @return interface description or default value (empty string) if it is not available
 */
QString  SFWInterface::description() const
{
    XQSERVICE_DEBUG_PRINT("SFWInterface::description");
    XQSERVICE_DEBUG_PRINT("interfaceDescription: %s", qPrintable(interfaceDescription));
    return interfaceDescription;
}


/*! 
 *  Sets the interface description
 *
 * @param aDescription 
 */
void SFWInterface::setDescription(const QString& aDescription)
{
    XQSERVICE_DEBUG_PRINT("SFWInterface::setDescription");
    XQSERVICE_DEBUG_PRINT("aDescription: %s", qPrintable(aDescription));
    interfaceDescription = aDescription;
}

/*!
    Returns the list of capabilities required by this interface.
*/
QStringList SFWInterface::capabilities() const
{
    XQSERVICE_DEBUG_PRINT("SFWInterface::capabilities");
    for (int i = 0; i < interfaceCapabilities.size(); ++i){
        XQSERVICE_DEBUG_PRINT("capability: %s", qPrintable(interfaceCapabilities.at(i)));
    }
    return interfaceCapabilities;
}

/*!
    Sets the interface \a apabilites.
*/
void SFWInterface::setCapabilities(const QList<QString>& capabilities)
{
    XQSERVICE_DEBUG_PRINT("SFWInterface::setCapabilities");
    for (int i = 0; i < capabilities.size(); ++i){
        XQSERVICE_DEBUG_PRINT("capability: %s", qPrintable(capabilities.at(i)));
    }
    interfaceCapabilities = capabilities;
}

/*!
    Returns the version tag of the interface.
*/
QString SFWInterface::version() const
{
    XQSERVICE_DEBUG_PRINT("SFWInterface::version");
    XQSERVICE_DEBUG_PRINT("interfaceVersion: %s", qPrintable(interfaceVersion));
    return interfaceVersion;
}

/*!
    Sets the version tag of this interface to \a version.
*/
void SFWInterface::setVersion(const QString& version)
{
    XQSERVICE_DEBUG_PRINT("SFWInterface::setVersion(1)");
    XQSERVICE_DEBUG_PRINT("version: %s", qPrintable(version));
    interfaceVersion = version;
}

/*!
    Sets the version tag of this interface based on \a maj and \a min.
*/
void SFWInterface::setVersion(int maj, int min)
{
    XQSERVICE_DEBUG_PRINT("SFWInterface::setVersion(2)");
    XQSERVICE_DEBUG_PRINT("maj: %d, min: %d", maj, min);
    interfaceVersion = QString::number(maj) + "." + QString::number(min);
}

/*!
    Returns the service associated with this interface.
*/
QString SFWInterface::service() const
{
    XQSERVICE_DEBUG_PRINT("SFWInterface::service");
    XQSERVICE_DEBUG_PRINT("serviceName: %s", qPrintable(serviceName));
    return serviceName;
}

/*!
    Sets the service name associated with this interface
*/
void SFWInterface::setService(const QString &service)
{
    XQSERVICE_DEBUG_PRINT("SFWInterface::setService");
    XQSERVICE_DEBUG_PRINT("service: %s", qPrintable(service));
    serviceName = service;
}
QT_END_NAMESPACE
