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

#include "xqpublishandsubscribesecuritypolicy.h"

/*!
    \class XQPublishAndSubscribeSecurityPolicy

    \brief The XQPublishAndSubscribeSecurityPolicy defines security policies
    used in XQPublishAndSubscribeUtils class. This class is just a Qt-like wrapper
    to Symbian OS' TSecurityPolicy class.
*/

/*!
    \enum XQPublishAndSubscribeSecurityPolicy::SecPolicyType

    This enum defines the possible policy types.
*/
/*! \var XQPublishAndSubscribeSecurityPolicy::SecPolicyAlwaysFail XQPublishAndSubscribeSecurityPolicy::SecPolicyAlwaysFail
    The checks made against this policy always fail.
*/
/*! \var XQPublishAndSubscribeSecurityPolicy::SecPolicyAlwaysPass XQPublishAndSubscribeSecurityPolicy::SecPolicyAlwaysPass
    The checks made against this policy always pass.
*/
/*! \var XQPublishAndSubscribeSecurityPolicy::SecPolicyUndefined XQPublishAndSubscribeSecurityPolicy::SecPolicyUndefined
    Policy type is not defined.
*/

/*!
    \enum XQPublishAndSubscribeSecurityPolicy::Capability

    This enum defines the possible capabilities.
*/
/*! \var XQPublishAndSubscribeSecurityPolicy::CapabilityTCB XQPublishAndSubscribeSecurityPolicy::CapabilityTCB
    Grants TCB level access.
*/
/*! \var XQPublishAndSubscribeSecurityPolicy::CapabilityCommDD XQPublishAndSubscribeSecurityPolicy::CapabilityCommDD
    Grants CapabilityCommDD level access.
*/
/*! \var XQPublishAndSubscribeSecurityPolicy::CapabilityPowerMgmt XQPublishAndSubscribeSecurityPolicy::CapabilityPowerMgmt
    Grants CapabilityPowerMgmt level access.
*/
/*! \var XQPublishAndSubscribeSecurityPolicy::CapabilityMultimediaDD XQPublishAndSubscribeSecurityPolicy::CapabilityMultimediaDD
    Grants CapabilityMultimediaDD level access.
*/
/*! \var XQPublishAndSubscribeSecurityPolicy::CapabilityReadDeviceData XQPublishAndSubscribeSecurityPolicy::CapabilityReadDeviceData
    Grants CapabilityReadDeviceData level access.
*/
/*! \var XQPublishAndSubscribeSecurityPolicy::CapabilityWriteDeviceData XQPublishAndSubscribeSecurityPolicy::CapabilityWriteDeviceData
    Grants CapabilityWriteDeviceData level access.
*/
/*! \var XQPublishAndSubscribeSecurityPolicy::CapabilityDRM XQPublishAndSubscribeSecurityPolicy::CapabilityDRM
    Grants CapabilityDRM level access.
*/
/*! \var XQPublishAndSubscribeSecurityPolicy::CapabilityTrustedUI XQPublishAndSubscribeSecurityPolicy::CapabilityTrustedUI
    Grants CapabilityTrustedUI level access.
*/
/*! \var XQPublishAndSubscribeSecurityPolicy::CapabilityProtServ XQPublishAndSubscribeSecurityPolicy::CapabilityProtServ
    Grants CapabilityProtServ level access.
*/
/*! \var XQPublishAndSubscribeSecurityPolicy::CapabilityDiskAdmin XQPublishAndSubscribeSecurityPolicy::CapabilityDiskAdmin
    Grants CapabilityDiskAdmin level access.
*/
/*! \var XQPublishAndSubscribeSecurityPolicy::CapabilityNetworkControl XQPublishAndSubscribeSecurityPolicy::CapabilityNetworkControl
    Grants CapabilityNetworkControl level access.
*/
/*! \var XQPublishAndSubscribeSecurityPolicy::CapabilityAllFiles XQPublishAndSubscribeSecurityPolicy::CapabilityAllFiles
    Grants CapabilityAllFiles level access.
*/
/*! \var XQPublishAndSubscribeSecurityPolicy::CapabilitySwEvent XQPublishAndSubscribeSecurityPolicy::CapabilitySwEvent
    Grants CapabilitySwEvent level access.
*/
/*! \var XQPublishAndSubscribeSecurityPolicy::CapabilityNetworkServices XQPublishAndSubscribeSecurityPolicy::CapabilityNetworkServices
    Grants CapabilityNetworkServices level access.
*/
/*! \var XQPublishAndSubscribeSecurityPolicy::CapabilityLocalServices XQPublishAndSubscribeSecurityPolicy::CapabilityLocalServices
    Grants CapabilityLocalServices level access.
*/
/*! \var XQPublishAndSubscribeSecurityPolicy::CapabilityReadUserData XQPublishAndSubscribeSecurityPolicy::CapabilityReadUserData
    Grants CapabilityReadUserData level access.
*/
/*! \var XQPublishAndSubscribeSecurityPolicy::CapabilityWriteUserData XQPublishAndSubscribeSecurityPolicy::CapabilityWriteUserData
    Grants CapabilityWriteUserData level access.
*/
/*! \var XQPublishAndSubscribeSecurityPolicy::CapabilityLocation XQPublishAndSubscribeSecurityPolicy::CapabilityLocation
    Grants CapabilityLocation level access.
*/
/*! \var XQPublishAndSubscribeSecurityPolicy::CapabilitySurroundingsDD XQPublishAndSubscribeSecurityPolicy::CapabilitySurroundingsDD
    Grants CapabilitySurroundingsDD level access.
*/
/*! \var XQPublishAndSubscribeSecurityPolicy::CapabilityUserEnvironment XQPublishAndSubscribeSecurityPolicy::CapabilityUserEnvironment
    Grants CapabilityUserEnvironment level access.
*/

/*!
    \return Secure ID
*/
XQPublishAndSubscribeSecurityPolicy::SecureId::SecureId(long int uid) : m_uid(uid)
{
}

/*!
    \return Vendor ID
*/
XQPublishAndSubscribeSecurityPolicy::VendorId::VendorId(long int uid) : m_uid(uid)
{
}


/*!
    Constructs an XQPublishAndSubscribeSecurityPolicy object.
    \param secPolicyType Policy type
*/
XQPublishAndSubscribeSecurityPolicy::XQPublishAndSubscribeSecurityPolicy(SecPolicyType secPolicyType)
    : m_secPolicyType(secPolicyType), m_secureId(-1), m_vendorId(-1)
{
}

/*!
    Constructs an XQPublishAndSubscribeSecurityPolicy object.
    \param capabilities List of needed capabilities. Max seven capabilities can be given. Extra capabilities
    in the list will not be used.
*/
XQPublishAndSubscribeSecurityPolicy::XQPublishAndSubscribeSecurityPolicy(QList<Capability> capabilities)
    : m_secPolicyType(XQPublishAndSubscribeSecurityPolicy::SecPolicyUndefined),
    m_secureId(-1), m_vendorId(-1), m_capabilities(capabilities)
{
}

/*!
    Constructs an XQPublishAndSubscribeSecurityPolicy object.
    \param secureId Secure ID
    \param capabilities List of needed capabilities. Max three capabilities can be given. Extra capabilities
    in the list will not be used.
*/
XQPublishAndSubscribeSecurityPolicy::XQPublishAndSubscribeSecurityPolicy(SecureId secureId, QList<Capability> capabilities)
    : m_secPolicyType(XQPublishAndSubscribeSecurityPolicy::SecPolicyUndefined),
    m_secureId(secureId), m_vendorId(-1), m_capabilities(capabilities)
{
}

/*!
    Constructs an XQPublishAndSubscribeSecurityPolicy object.
    \param vendorId Vendor ID
    \param capabilities List of needed capabilities. Max three capabilities can be given. Extra capabilities
    in the list will not be used.
*/
XQPublishAndSubscribeSecurityPolicy::XQPublishAndSubscribeSecurityPolicy(VendorId vendorId, QList<Capability> capabilities)
    : m_secPolicyType(XQPublishAndSubscribeSecurityPolicy::SecPolicyUndefined),
    m_secureId(-1), m_vendorId(vendorId), m_capabilities(capabilities)
{
}

/*!
    Destructs an XQPublishAndSubscribeSecurityPolicy object.
*/
XQPublishAndSubscribeSecurityPolicy::~XQPublishAndSubscribeSecurityPolicy()
{
}

/*!
    \return Policy type.
*/
XQPublishAndSubscribeSecurityPolicy::SecPolicyType XQPublishAndSubscribeSecurityPolicy::secPolicyType() const
{
    return m_secPolicyType;
}

/*!
    \return Secure ID or -1 if not defined.
*/
XQPublishAndSubscribeSecurityPolicy::SecureId XQPublishAndSubscribeSecurityPolicy::secureId() const
{
    return m_secureId;
}

/*!
    \return Vendor ID or -1 if not defined.
*/
XQPublishAndSubscribeSecurityPolicy::VendorId XQPublishAndSubscribeSecurityPolicy::vendorId() const
{
    return m_vendorId;
}

/*!
    \return List of capabilities
*/
QList<XQPublishAndSubscribeSecurityPolicy::Capability> XQPublishAndSubscribeSecurityPolicy::capabilities() const
{
    return m_capabilities;
}
