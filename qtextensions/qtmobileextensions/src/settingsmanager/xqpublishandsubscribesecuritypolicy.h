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

#ifndef XQPUBLISHANDSUBSCRIBESECURITYPOLICY_H
#define XQPUBLISHANDSUBSCRIBESECURITYPOLICY_H

#include "settingsmanager_global.h"

#include <QList>

class XQSETTINGSMANAGER_EXPORT XQPublishAndSubscribeSecurityPolicy
{
public:

    class SecureId
    {
    public:
        SecureId(long int uid);
        inline long int uid() const {return m_uid;};

    private:
        long int m_uid;
    };

    class VendorId
    {
    public:
        VendorId(long int uid);
        inline long int uid() const {return m_uid;};

    private:
        long int m_uid;
    };

    enum SecPolicyType
    {
        SecPolicyAlwaysFail,
        SecPolicyAlwaysPass,
        SecPolicyUndefined
    };
    
    enum Capability
    {
        CapabilityTCB,
        CapabilityCommDD,
        CapabilityPowerMgmt,
        CapabilityMultimediaDD,
        CapabilityReadDeviceData,
        CapabilityWriteDeviceData,
        CapabilityDRM,
        CapabilityTrustedUI,
        CapabilityProtServ,
        CapabilityDiskAdmin,
        CapabilityNetworkControl,
        CapabilityAllFiles,
        CapabilitySwEvent,
        CapabilityNetworkServices,
        CapabilityLocalServices,
        CapabilityReadUserData,
        CapabilityWriteUserData,
        CapabilityLocation,
        CapabilitySurroundingsDD,
        CapabilityUserEnvironment
    };

    XQPublishAndSubscribeSecurityPolicy(SecPolicyType secPolicyType);
    XQPublishAndSubscribeSecurityPolicy(QList<Capability> capabilities);
    XQPublishAndSubscribeSecurityPolicy(SecureId secureId, QList<Capability> capabilities);
    XQPublishAndSubscribeSecurityPolicy(VendorId vendorId, QList<Capability> capabilities);
    ~XQPublishAndSubscribeSecurityPolicy();

    SecPolicyType secPolicyType() const;
    SecureId secureId() const;
    VendorId vendorId() const;
    QList<Capability> capabilities() const;

private:
    SecPolicyType m_secPolicyType;
    SecureId m_secureId;
    VendorId m_vendorId;
    QList<Capability> m_capabilities;
};

#endif //XQPUBLISHANDSUBSCRIBESECURITYPOLICY_H
