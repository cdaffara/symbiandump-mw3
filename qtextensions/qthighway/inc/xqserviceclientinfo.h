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

#ifndef XQSERVICECLIENTINFO_H
#define XQSERVICECLIENTINFO_H

#include <QObject>
#include <QString>
#include <QBitArray>

namespace QtService {

/*!
    \class ClientInfo
    \brief Container class for client information in ipc requests.
*/
    // FORWARD DECLARATIONS
    class ClientInfo: public QObject
    {

    public:    
        /*!
            Sets the client's name.
            \param aName Client's name.
        */
        inline void setName(const QString &aName) { iName = aName; }

        /*!
            Gets the client's name.
            \return Client's name as QString.
        */
        inline QString name() { return iName; }

        /*!
            Sets the client's process ID.
            \param aProcessId Id of the process.
        */
        inline void setProcessId(const qint64 aProcessId) { iProcessId = aProcessId; }

        /*!
            Gets the client's process ID.
            \return Process ID.
        */
        inline qint64 processId() { return iProcessId; }

        /*!
            Sets the client's vendor ID.
            \param aVendorId Vendor ID.
        */
        inline void setVendorId(const qint64 aVendorId) { iVendorId = aVendorId; }

        /*!
            Gets the client's vendor ID.
            \return Vendor ID.
        */
        inline const qint64 vendorId() { return iVendorId; }

        /*!
            Sets the client's capabilities.
            \param capabilities Capabilities to be set.
        */
        inline void setCapabilities(const quint32 capabilities) { iCapabilities = capabilities; }

        /*!
            Gets the client's capabilities.
            \return Client capabilities.
        */
        inline const quint32 capabilities() { return iCapabilities; }

        
    private:
        QString iName;
        qint64 iProcessId;
        qint64 iVendorId;
        quint32 iCapabilities;
    };
}

#endif //XQSERVICECLIENTINFO_H
