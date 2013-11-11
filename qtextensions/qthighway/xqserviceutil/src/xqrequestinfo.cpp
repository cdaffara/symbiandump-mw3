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
#include "xqrequestinfo.h"
#include "xqrequestutil.h"

/*!
    \class XQRequestInfo
    \inpublicgroup QtBaseModule

    \ingroup ipc
    \brief The XQRequestInfo class encapsulates additional information of a service request
    
    The XQRequestInfo class encapsulates additional information of a service request:
    - For service provider it contains additional information of the service request, some set by the client, some set by the QtHighway framework.
    - For client it gives possibility to add UI oriented options of the request
    The XQRequestInfo is exported by the xqserviceutil library
*/

/*!
    Constructor.
*/
XQRequestInfo::XQRequestInfo()
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::XQRequestInfo");
}

/*!
    Destructor.
*/
XQRequestInfo::~XQRequestInfo()
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::~XQRequestInfo");
}

/*!
    Requests service application to be launched as embedded mode.
    \param on Set to true to turn embedded mode on.
*/
void XQRequestInfo::setEmbedded(bool on)
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::setEmbedded %d", on);
    setInfo(XQServiceUtils::OptEmbedded, on);
}

/*!
    Get embedded option value.
    \return True if embedded mode is turned on, false otherwise.
*/
bool XQRequestInfo::isEmbedded() const
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::isEmbedded");
    return info(XQServiceUtils::OptEmbedded).toBool();
}

/*!
    Requests service application to be set to background before a slot call to service provider. 
    If this option is set to false or not set, the QtHighway does not alter the Z-order.
    \param on Set to true if service application should be launched to background.
*/
void XQRequestInfo::setBackground(bool on)
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::setBackground %d", on);
    setInfo(XQServiceUtils::OptBackground, on);
}

/*!
    Get the value of the background option.
    \return True if option has been set on, false otherwise.
*/
bool XQRequestInfo::isBackground() const
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::isBackground");
    return info(XQServiceUtils::OptBackground).toBool();
}

/*!
    Requests service application to be brought to foreground before a slot call to service provider.
    If this option is false or not set, the QtHighway does not alter the Z-order.
    \param on Set to true if service application should be launched to foreground.
*/
void XQRequestInfo::setForeground(bool on)
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::setForeground %d", on);
    setInfo(XQServiceUtils::OptForeground, on);
}

/*!
    Get the value of the foreground option.
    \return True if option has been set on, false otherwise.
*/
bool XQRequestInfo::isForeground() const
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::isForeground");
    return info(XQServiceUtils::OptForeground).toBool();
}

/*!
    Gets the vaule of the synchronous option.
    \return True if request is synchronous , false if asynchronous.
*/
bool XQRequestInfo::isSynchronous() const
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::isSynchronous");
    return info(XQServiceUtils::OptSynchronous).toBool();
}

/*!
    Gets the client's secure id option. Valid only for service provider.
    \return Calling client's secure ID as defined by the Symbian OS platform security model.
*/
quint32 XQRequestInfo::clientSecureId() const
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::clientSecureId");
    bool b=false;
    quint32 id = info(XQServiceUtils::InfoSID).toUInt(&b);
    XQSERVICE_DEBUG_PRINT("\tSID status=%d", b);
    return id;
}

/*!
    Gets the clients's vendor id option. Valid only for service provider.
    \return Calling client's vendor ID as defined by the Symbian OS platform security model.
*/
quint32 XQRequestInfo::clientVendorId() const
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::clientVendorId");
    bool b=false;
    quint32 id = info(XQServiceUtils::InfoVID).toUInt(&b);
    XQSERVICE_DEBUG_PRINT("\tVID status=%d", b);
    return id;
}

/*!
    Gets the client's capabilities. Valid only for service provider.
    \return Calling client's capabilities as defined by the Symbian OS platform security model. 
            The values in the QSet are compatible with the TCapability values in e32capability.h.
*/
QSet<int> XQRequestInfo::clientCapabilities() const
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::clientCapabilities");
    bool b=false;
    quint32 caps = info(XQServiceUtils::InfoCap).toUInt(&b);
    XQSERVICE_DEBUG_PRINT("\tCaps status", b);
    QSet<int> ret;
    if (!b)
        return ret;  // Empty set
    
    for(int i = 0; i < 32; i++)
    {
        quint32 ix = 0;
        ix |= 1 << i;
        if (caps & ix)
        {
            ret.insert(i);
        }
    }
    
    return ret;
}

/*!
    Sets a \a value of the named info \a key. Key names starting with "XQ" are reserved for
    the QtHighway internal usage. If the service operation (message) does not accept
    these parameters already, the recommened way is to pass these in the XQRequestInfo
    and use the following pre-defined keys:
    - XQINFO_KEY_WINDOW_TITLE for passing the title string be shown in the service application
      window (QtHighway does not pick the Orbit window title automatically).
    - ViewName (QString) for passing the view to be activated in the service application.
    \param key Info key for which \a value will be set.
    \param value Value to be set to a \a key
*/
void XQRequestInfo::setInfo(const QString &key, const QVariant &value)
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::setInfo %s:%s,%s",
        qPrintable(key), qPrintable(value.toString()), value.typeName());
    mInfo.insert(key, value);
}

/*!
    Gets the set value of the \a key. The returned value may be invalid if not set.
    \param key Key name for which value is retrieved.
    \return Value set to the key, or invalid QVariant if not set.
*/
QVariant XQRequestInfo::info(const QString &key) const
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::info %s", qPrintable(key));
    QVariant v =  mInfo.value(key);
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::info type=%s,valid=%d,str=%s", v.typeName(), v.isValid(), qPrintable(v.toString()));
    return v;
}

/*!
    Gets the list of key names set for the object.
    \return List of key names set for the object.
*/
QStringList XQRequestInfo::infoKeys() const
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::infoKeys");
    return mInfo.keys();
}

/*!
    Gets the internal id of the current request. As there can be several
    requests ongoing to the same interface, this separates multiple requests.
    Once the request has been completed the ID becomes invalid.
    This is the same value as returned by the XQServiceProvider::setCurrentRequestAsync()
    to set response asynchronous.
    \return Id of the current request as integer value.
*/
int XQRequestInfo::id() const
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::id");
    bool b=false;
    int id = info(XQServiceUtils::InfoId).toInt(&b);
    XQSERVICE_DEBUG_PRINT("\tId status=%d", b);
    if (!b)
    {
        return -1;
    }
    return id;
}

/*!
    Checks if object is valid.
    \return True if object is valid, false otherwise.
*/
bool XQRequestInfo::isValid() const
{
    return !mInfo.isEmpty();
}

/*!
    Serializes this XQRequestInfo data into the given stream.
    \param s Stream the data is serialized into.
*/
template <typename Stream> void XQRequestInfo::serialize(Stream &s) const
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::serialize");
    s << mInfo;
}

/*!
    Deserializes XQRequestInfo data from the given stream.
    \param s Stream the data is deserialized from.
*/
template <typename Stream> void XQRequestInfo::deserialize(Stream &s)
{
    XQSERVICE_DEBUG_PRINT("XQRequestInfo::de-serialize");
    s >> mInfo;
}

Q_IMPLEMENT_USER_METATYPE(XQRequestInfo)
