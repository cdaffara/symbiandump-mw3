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
#include <xqaiwinterfacedescriptor_p.h>
#include <xqaiwinterfacedescriptor.h>
#ifndef QT_NO_DATASTREAM
#include <qdatastream.h>
#endif

#include <QDebug>

//QT_BEGIN_NAMESPACE

/*!
    \class XQAiwInterfaceDescriptor
    \brief The XQAiwInterfaceDescriptor class identifies a service implementation.

    A service can implement multiple interfaces and each interface can have multiple implementations. 
    The XQAiwInterfaceDescriptor class enscapsulates this information, as illustrated
    by the diagram below.

    \image XQAiwInterfaceDescriptor.png Service-Interface-Implementation

    The major version tag indicates the interface version and the minor version tag identifies the implementation
    version. Subsequent versions of the same interface must be binary compatible to previous versions 
    of the same interface. 

    In the above example service A and B implement the interface \i com.nokia.qt.x.
    In fact Service A provides two different implementations for the very same interface. 
    This is indicated by the changed minor version number. Although Service B is 
    using the same interface it's implementation actually utilizes the second version of
    the interface \i com.nokia.qt.x. Binary compatibility guarantees that clients 
    who know version 1 can utilize version 2. If an existing interface has to be changed
    in a non-compatible way a new interface (name) is required.

    \section1 Namespaces

    A XQAiwInterfaceDescriptor (the quadruble of service name, 
    interface name, interface version and implementation version) uniquely 
    identifies a service implementation on a device. Interface names follow 
    the java namespace convention.
    
    The namespace \i com.nokia.qt.* is reserved for future Qt development.

    \sa QServiceFilter, QServiceManager
*/

/*!
    \enum XQAiwInterfaceDescriptor::PropertyKey

    This enum describes the possible property types which can be attached
    to a XQAiwInterfaceDescriptor.
*/

/*! \var XQAiwInterfaceDescriptor::PropertyKey  XQAiwInterfaceDescriptor::Capabilities
    
    The capabilities property is a QStringList and
    describes the capabilities that a service client
    would require to use the service if capability 
    checks are enforced.
*/

/*! \var XQAiwInterfaceDescriptor::PropertyKey  XQAiwInterfaceDescriptor::Location
    
    This property points to the location
    where the plug-in providing this service is stored.
    If the service is plug-in based the location is the
    name and/or path of the plugin.
*/

/*! \var XQAiwInterfaceDescriptor::PropertyKey  XQAiwInterfaceDescriptor::ServiceDescription
    
    This property provides a general description for
    the service.
*/

/*! \var XQAiwInterfaceDescriptor::PropertyKey  XQAiwInterfaceDescriptor::InterfaceDescription
    
    This property provides a description for the interface 
    implementation.
*/

/*! \var XQAiwInterfaceDescriptor::PropertyKey  XQAiwInterfaceDescriptor::ImplementationId

    Extension: settable property, contains implementation id
*/

/*!
    Creates a new XQAiwInterfaceDescriptor.
*/
XQAiwInterfaceDescriptor::XQAiwInterfaceDescriptor()
    :  d(0)
{
  XQSERVICE_DEBUG_PRINT("XQAiwInterfaceDescriptor");
  // Memory allocated in assigment operator !!!
}

/*!
    Destroys the XQAiwInterfaceDescriptor object.
*/
XQAiwInterfaceDescriptor::~XQAiwInterfaceDescriptor()
{
    XQSERVICE_DEBUG_PRINT("~XQAiwInterfaceDescriptor");
    if (d)
        delete d;
}

/*!
    Creates a copy of XQAiwInterfaceDescriptor contained in \a other.
    \param other Reference to the other XQAiwInterfaceDescriptor object, from
                 which new object will be created
*/
XQAiwInterfaceDescriptor::XQAiwInterfaceDescriptor(const XQAiwInterfaceDescriptor& other)
    : d(0)
{
    (*this) = other; //use assignment operator
}


/*!
    Copies the content of the XQAiwInterfaceDescriptor object contained 
    in \a other into this one.
    \param other Reference to XQAiwInterfaceDescriptor object, from which content will be copied
*/
XQAiwInterfaceDescriptor& XQAiwInterfaceDescriptor::operator=(const XQAiwInterfaceDescriptor& other)
{
    XQSERVICE_DEBUG_PRINT("XQAiwInterfaceDescriptor: operator=");
    if ( !other.isValid() ) {
        XQSERVICE_DEBUG_PRINT("other is invalid !!!");
        if (d) 
            delete d;
        d = 0;
        return *this;  // Create empty descriptor
    }
    if (!d)
        d = new XQAiwInterfaceDescriptorPrivate;
    (*d) = *(other.d);
    XQSERVICE_DEBUG_PRINT("XQAiwInterfaceDescriptor: operator= done");
    return *this;
}

/*!
    Compares a XQAiwInterfaceDescriptor to \a other.
    \param other Reference to XQAiwInterfaceDescriptor object, which will be compared
                 to this one.
    \return True if both instances are equal, false otherwise.
*/
bool XQAiwInterfaceDescriptor::operator==(const XQAiwInterfaceDescriptor& other) const
{
    XQSERVICE_DEBUG_PRINT("XQAiwInterfaceDescriptor: operator==");
    
    if (! (isValid() && other.isValid()) )
        return false;

    if (!d)
        return false;

    if ((*d) == *(other.d))
    {
        XQSERVICE_DEBUG_PRINT("== true");
        return true;
    }
    
    XQSERVICE_DEBUG_PRINT("== false");
    return false;
}

/*!
    \fn bool XQAiwInterfaceDescriptor::operator!=(const XQAiwInterfaceDescriptor& other) const

    Compares a XQAiwInterfaceDescriptor to \a other. Returns true
    if they are not equal and false otherwise.
    \param other Reference to XQAiwInterfaceDescriptor object, which will be compared
                 to this one.
    \return False if both instances are equal, true otherwise.
*/

/*!
    Checks if the descriptor is valid.
    \return True if this descriptor is valid, false otherwise
*/
bool XQAiwInterfaceDescriptor::isValid() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwInterfaceDescriptor:isValid");
    if (d)
    {
        XQSERVICE_DEBUG_PRINT("isValid=yes");
        return true;
    }
    XQSERVICE_DEBUG_PRINT("isValid=false");
    return false;
}

/*!
    Checks if this implementation is provided for all users on the system.
    \return True if this implementation is provided for all users on the system,
            false otherwise

    \sa QServiceManager::Scope
*/
bool XQAiwInterfaceDescriptor::inSystemScope() const
{
    return d ? d->systemScope : false;
}

/*!
    Gets the name of service that provides this implementation.
    \return Name of service
*/
QString XQAiwInterfaceDescriptor::serviceName() const
{
    return d ? d->serviceName : QString();
}

/*!
    Gets the name of the interface that is implemented.
    \return Name of the interface
*/
QString XQAiwInterfaceDescriptor::interfaceName() const
{
    return d ? d->interfaceName : QString();
}


/*!
    Gets the version of the interface. 
    Subsequent versions of an interface are binary compatible 
    to previous versions of the same interface. If an intcerface 
    is broken it must use a new interface name.
    \return Interface version as integer value
*/
int XQAiwInterfaceDescriptor::majorVersion() const
{
    return d ? d->major : -1;
}

/*!
    Gets the version of the implementation.
    \return Implementation version as integer value
*/
int XQAiwInterfaceDescriptor::minorVersion() const
{
    return d ? d->minor : -1;
}

/*!
    Gets the value for the property.
    \param key Key of the property
    \return Value of the property, invalid QVariant if does not exist
*/
QVariant XQAiwInterfaceDescriptor::property(XQAiwInterfaceDescriptor::PropertyKey key) const
{
    if (d)
    {
        return d->properties.value(key);
    }
    return QVariant();
}

/*!
    Extension:
    Sets given property. To be used by service management only.
*/
bool XQAiwInterfaceDescriptor::setProperty(XQAiwInterfaceDescriptor::PropertyKey key, const QVariant value )
{
    XQSERVICE_DEBUG_PRINT("XQAiwInterfaceDescriptor::setPropery %d", key);
    if (key < ImplementationId)
    {
        return false;  // Do not allow changing other properties
    }
    
    if (!d)
        d = new XQAiwInterfaceDescriptorPrivate;  // Ensure allocation
    
    if (d)
    {
        d->properties[key] = value;
        return true;
    }
    return false;
}


/*!
    Gets the value for the property.
    \param key Key of the custom property
    \return Value of the custom property, invalid null if does not exist
*/
QString XQAiwInterfaceDescriptor::customProperty(const QString& key) const
{
    XQSERVICE_DEBUG_PRINT("XQAiwInterfaceDescriptor::customProperty %s", qPrintable(key));
    QString val;
    if (d)
    {
        val = d->customProperties[key];
    }
    XQSERVICE_DEBUG_PRINT("XQAiwInterfaceDescriptor::customProperty value %s", qPrintable(val));
    
    return val;
}

/*!
    Gets list of custom property keys.
    \return list of custom property keys
*/
QStringList XQAiwInterfaceDescriptor::customPropertyKeys() const
{
    XQSERVICE_DEBUG_PRINT("XQAiwInterfaceDescriptor::customPropertyKeys");
    if (d)
    {
        return d->customProperties.keys();
    }

    return QStringList();
}

#ifndef QT_NO_DEBUG_STREAM
QDebug operator<<(QDebug dbg, const XQAiwInterfaceDescriptor &desc)
{
    if (desc.isValid()) {
        QString interface = QString("%1 %2.%3").arg(desc.interfaceName())
                .arg(desc.majorVersion() < 0 ? '?' : desc.majorVersion())
                .arg(desc.minorVersion() < 0 ? '?' : desc.minorVersion());
        dbg.nospace() << "XQAiwInterfaceDescriptor(";
        dbg.nospace() << "service=" << desc.serviceName() << ", ";
        dbg.nospace() << "interface=" << interface;
        dbg.nospace() << ")";
    } else {
        dbg.nospace() << "XQAiwInterfaceDescriptor(invalid)";
    }
    return dbg.space();
}
#endif

#ifndef QT_NO_DATASTREAM

QDataStream &operator<<(QDataStream &out, const XQAiwInterfaceDescriptor::PropertyKey &k)
{
    out << qint8(k);
    return out;
}

QDataStream &operator>>(QDataStream &in, XQAiwInterfaceDescriptor::PropertyKey &k)
{
    quint8 key;
    in >> key;
    k = (XQAiwInterfaceDescriptor::PropertyKey)key;
    return in;
}
/*! 
    \fn QDataStream &operator<<(QDataStream &out, const XQAiwInterfaceDescriptor &dc)
    \relates XQAiwInterfaceDescriptor

    Writes service interface descriptor \a dc to the stream \a out and returns a reference
    to the stream.
    \param out Stream to write to
    \param dc Interface descriptor written to stream
    \return Reference to the stream
*/

QDataStream &operator<<(QDataStream &out, const XQAiwInterfaceDescriptor &dc)
{
    const quint32 magicNumber = 0x77AFAFA;
    const quint16 majorVersion = 1;
    const quint16 minorVersion = 0;
    const qint8 valid = dc.isValid();
    out << magicNumber << majorVersion << minorVersion;
    out << valid;
    if (valid) {
       out << dc.d->serviceName; 
       out << dc.d->interfaceName;
       out << dc.d->major;
       out << dc.d->minor;
       out << dc.d->properties;
       out << dc.d->customProperties;
       out << dc.d->systemScope;
    }
    return out;
}

/*!
    \fn QDataStream &operator>>(QDataStream &in, XQAiwInterfaceDescriptor &dc)
    \relates XQAiwInterfaceDescriptor

    Reads a service interface descriptor into \a dc from the stream \a in and returns a
    reference to the stream.
    \param in Stream to read from
    \param dc Interface descriptor read from stream
    \return Reference to the stream
*/
QDataStream &operator>>(QDataStream &in, XQAiwInterfaceDescriptor &dc)
{
    const quint32 magicNumber = 0x77AFAFA;
    quint32 storedMagicNumber;
    in >> storedMagicNumber;
    if (storedMagicNumber != magicNumber) {
        qWarning() << "Datastream doesn't provide searialized XQAiwInterfaceDescriptor";
        return in;
    }
    
    const quint16 currentMajorVersion = 1;
    quint16 majorVersion = 0;
    quint16 minorVersion = 0;

    in >> majorVersion >> minorVersion;
    if (majorVersion != currentMajorVersion) {
        qWarning() << "Unknown serialization format for XQAiwInterfaceDescriptor.";
        return in;
    }
    //Allow all minor versions.

    qint8 valid;
    in >> valid;
    if (valid) {
        if (!dc.isValid())
            dc.d = new XQAiwInterfaceDescriptorPrivate;
        in >> dc.d->serviceName;
        in >> dc.d->interfaceName;
        in >> dc.d->major;
        in >> dc.d->minor;
        in >> dc.d->properties;
        in >> dc.d->customProperties;
        in >> dc.d->systemScope;
    } else { //input stream contains invalid descriptor
        //use assignment operator
        dc = XQAiwInterfaceDescriptor();
    }

    return in;
}
#endif //QT_NO_DATASTREAM



//QT_END_NAMESPACE

