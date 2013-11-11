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

#ifndef TESTDATA_H_
#define TESTDATA_H_

#include <xqaiwinterfacedescriptor.h>
#include <QString>
#include <QStringList>
#include <QHash>
#include <QMultiHash>
#include <xqappmgr.h>
#include <QDebug>

const QString IUriView = "com.nokia.symbian.IUriView"; 
const QString IFileView = "com.nokia.symbian.IFileView";

#ifdef __WINS__
    const QString drive = "c";
#else
    const QString drive = "z";
#endif

typedef QHash<int, QVariant> FileDrm; 

class InterfaceData
{
public:
    InterfaceData(QString serviceName, QString interfaceName,int major, int minor):
        mServiceName(serviceName), mInterfaceName(interfaceName), mMajor(major), mMinor(minor){}
    
    void addProperty(XQAiwInterfaceDescriptor::PropertyKey key, QVariant value)
    {
        mProperties[key] = value;
    }
    
    void addCustomProperty(QString key, QString value)
    {
        mCustomProperties[key] = value;
    }
    
    bool compare(const XQAiwInterfaceDescriptor& interface, QStringList& log)
    {
        if(mServiceName != interface.serviceName())
        {
            return false;
        }
        
        if(mInterfaceName != interface.interfaceName())
        {
            return false;
        }
               
        if(mMajor != interface.majorVersion())
        {
            return false;
        }
               
        if(mMinor != interface.minorVersion())
        {
            return false;
        }
               
        if(mProperties[XQAiwInterfaceDescriptor::Capabilities] != interface.property(XQAiwInterfaceDescriptor::Capabilities))
        {
            log << "[QTH] [Test_XQApplicationManager] different capabilities";
            return false;
        }
               
        if(mProperties[XQAiwInterfaceDescriptor::Location] != interface.property(XQAiwInterfaceDescriptor::Location))
        {
            log << "[QTH] [Test_XQApplicationManager] different location";
            return false;
        }
               
        if(mProperties[XQAiwInterfaceDescriptor::ServiceDescription] != interface.property(XQAiwInterfaceDescriptor::ServiceDescription))
        {
            log << "[QTH] [Test_XQApplicationManager] different service description";
            return false;
        }
               
        if(mProperties[XQAiwInterfaceDescriptor::InterfaceDescription] != interface.property(XQAiwInterfaceDescriptor::InterfaceDescription))
        {
            log << "[QTH] [Test_XQApplicationManager] different interface description";
            return false;
        }
               
        if(mProperties[XQAiwInterfaceDescriptor::ImplementationId] != interface.property(XQAiwInterfaceDescriptor::ImplementationId))
        {
            log << "[QTH] [Test_XQApplicationManager] different implementation ID";
            return false;
        }
        
        QStringList customPropKeys = interface.customPropertyKeys();
        foreach(QString key, customPropKeys)
        {
            if(mCustomProperties[key] != interface.customProperty(key))
            {
                log << "[QTH] [Test_XQApplicationManager] different custom property:" 
                         << key << " test value:" << mCustomProperties[key] << " system value:" << interface.customProperty(key);
                return false;
            }
        }
        
        return true;
    }
           
public:
    QString mServiceName;
    QString mInterfaceName;
    QHash<XQAiwInterfaceDescriptor::PropertyKey, QVariant> mProperties;
    QHash<QString, QString> mCustomProperties;
    int mMajor;
    int mMinor;
};

class TestData
{
public:
    TestData()
    {
        // interfaces
        {
            QString serviceName("com.nokia.services.qthighway.test.service_1");
            QString interfaceName("interface_1");
            InterfaceData* interface = new InterfaceData(serviceName, interfaceName, 1, 0);
            interface->addProperty(XQAiwInterfaceDescriptor::ServiceDescription, QVariant(QString("QtHighway automatic tests server 1")));
            interface->addProperty(XQAiwInterfaceDescriptor::InterfaceDescription, QVariant(QString("Interface 1")));
            interface->addProperty(XQAiwInterfaceDescriptor::Location, QVariant(QString("No path")));
            interface->addProperty(XQAiwInterfaceDescriptor::ImplementationId, QVariant(int(0xE42DF0E6)));
            services.insert(serviceName, interface);
            interfaces.insert(interfaceName, interface);
        }
           
        {
            QString serviceName("com.nokia.services.qthighway.test.service_1");
            QString interfaceName("interface_2");
            InterfaceData* interface = new InterfaceData(serviceName, interfaceName, 1, 0);
            interface->addProperty(XQAiwInterfaceDescriptor::ServiceDescription, QVariant(QString("QtHighway automatic tests server 1")));
            interface->addProperty(XQAiwInterfaceDescriptor::InterfaceDescription, QVariant(QString("Interface 2")));
            interface->addProperty(XQAiwInterfaceDescriptor::Location, QVariant(QString("No path")));
            interface->addProperty(XQAiwInterfaceDescriptor::ImplementationId, QVariant(int(0xE42DF0E6)));
            services.insert(serviceName, interface);
            interfaces.insert(interfaceName, interface);
        }
        
        {
            QString serviceName("com.nokia.services.qthighway.test.service_1");
            QString interfaceName("interface_3");
            InterfaceData* interface = new InterfaceData(serviceName, interfaceName, 1, 0);
            interface->addProperty(XQAiwInterfaceDescriptor::ServiceDescription, QVariant(QString("QtHighway automatic tests server 1")));
            interface->addProperty(XQAiwInterfaceDescriptor::InterfaceDescription, QVariant(QString("Interface 3")));
            interface->addProperty(XQAiwInterfaceDescriptor::Location, QVariant(QString("No path")));
            interface->addProperty(XQAiwInterfaceDescriptor::ImplementationId, QVariant(int(0xE42DF0E6)));
            services.insert(serviceName, interface);
            interfaces.insert(interfaceName, interface);
        }
        
        {
            QString serviceName("com.nokia.services.qthighway.test.service_1");
            QString interfaceName(IUriView);
            InterfaceData* interface = new InterfaceData(serviceName, interfaceName, 1, 0);
            interface->addProperty(XQAiwInterfaceDescriptor::InterfaceDescription, QVariant(QString("Interface for showing URIs")));
            interface->addProperty(XQAiwInterfaceDescriptor::ServiceDescription, QVariant(QString("QtHighway automatic tests server 1")));
            interface->addProperty(XQAiwInterfaceDescriptor::Location, QVariant(QString("No path")));
            interface->addProperty(XQAiwInterfaceDescriptor::ImplementationId, QVariant(int(0xE42DF0E6)));
            interface->addCustomProperty("schemes","qhattestto");
            services.insert(serviceName, interface);
            interfaces.insert(interfaceName, interface);
        }
        
        {
            QString serviceName("com.nokia.services.qthighway.test.service_1");
            QString interfaceName(IFileView);
            InterfaceData* interface = new InterfaceData(serviceName, interfaceName, 1, 0);
            interface->addProperty(XQAiwInterfaceDescriptor::InterfaceDescription, QVariant(QString("Interface for showing Files")));
            interface->addProperty(XQAiwInterfaceDescriptor::ServiceDescription, QVariant(QString("QtHighway automatic tests server 1")));
            interface->addProperty(XQAiwInterfaceDescriptor::Location, QVariant(QString("No path")));
            interface->addProperty(XQAiwInterfaceDescriptor::ImplementationId, QVariant(int(0xE42DF0E6)));
            services.insert(serviceName, interface);
            interfaces.insert(interfaceName, interface);
        }
    
        {
            QString serviceName("com.nokia.services.qthighway.test.service_2");
            QString interfaceName("interface_1");
            InterfaceData* interface = new InterfaceData(serviceName, interfaceName, 1, 0);
            interface->addProperty(XQAiwInterfaceDescriptor::InterfaceDescription, QVariant(QString("Interface 1")));
            interface->addProperty(XQAiwInterfaceDescriptor::ServiceDescription, QVariant(QString("QtHighway automatic tests server 2")));
            interface->addProperty(XQAiwInterfaceDescriptor::Location, QVariant(QString("No path")));
            interface->addProperty(XQAiwInterfaceDescriptor::ImplementationId, QVariant(int(0xED698FE0)));
            services.insert(serviceName, interface);
            interfaces.insert(interfaceName, interface);
        }
        
        {
            QString serviceName("com.nokia.services.qthighway.test.service_2");
            QString interfaceName("interface_2");
            InterfaceData* interface = new InterfaceData(serviceName, interfaceName, 1, 0);
            interface->addProperty(XQAiwInterfaceDescriptor::InterfaceDescription, QVariant(QString("Interface 2")));
            interface->addProperty(XQAiwInterfaceDescriptor::ServiceDescription, QVariant(QString("QtHighway automatic tests server 2")));
            interface->addProperty(XQAiwInterfaceDescriptor::Location, QVariant(QString("No path")));
            interface->addProperty(XQAiwInterfaceDescriptor::ImplementationId, QVariant(int(0xED698FE0)));
            services.insert(serviceName, interface);
            interfaces.insert(interfaceName, interface);
        }
        
        {
            QString serviceName("com.nokia.services.qthighway.test.service_2");
            QString interfaceName(IUriView);
            InterfaceData* interface = new InterfaceData(serviceName, interfaceName, 1, 0);
            interface->addProperty(XQAiwInterfaceDescriptor::InterfaceDescription, QVariant(QString("Interface for showing URIs")));
            interface->addProperty(XQAiwInterfaceDescriptor::ServiceDescription, QVariant(QString("QtHighway automatic tests server 2")));
            interface->addProperty(XQAiwInterfaceDescriptor::Location, QVariant(QString("No path")));
            interface->addProperty(XQAiwInterfaceDescriptor::ImplementationId, QVariant(int(0xED698FE0)));
            interface->addCustomProperty("schemes","qhattestto");
            services.insert(serviceName, interface);
            interfaces.insert(interfaceName, interface);
        }
        
        {
            QString serviceName("com.nokia.services.qthighway.test.service_2");
            QString interfaceName(IFileView);
            InterfaceData* interface = new InterfaceData(serviceName, interfaceName, 1, 0);
            interface->addProperty(XQAiwInterfaceDescriptor::InterfaceDescription, QVariant(QString("Interface for showing Files")));
            interface->addProperty(XQAiwInterfaceDescriptor::ServiceDescription, QVariant(QString("QtHighway automatic tests server 2")));
            interface->addProperty(XQAiwInterfaceDescriptor::Location, QVariant(QString("No path")));
            interface->addProperty(XQAiwInterfaceDescriptor::ImplementationId, QVariant(int(0xED698FE0)));
            services.insert(serviceName, interface);
            interfaces.insert(interfaceName, interface);
        }
        
        // DRM files
        
        {
            FileDrm drm;
            drm[XQApplicationManager::IsProtected] = QVariant(0);
            drm[XQApplicationManager::IsForwardable] = QVariant(1);
            drm[XQApplicationManager::Description] = QVariant();
            drm[XQApplicationManager::MimeType] = QVariant(QString("text/plain"));
            
            files[QString(drive + ":\\data\\Others\\test.txt")] = drm;
            
        }
        
        {
            FileDrm drm;
            drm[XQApplicationManager::IsProtected] = QVariant(1);
            drm[XQApplicationManager::IsForwardable] = QVariant(1);
            drm[XQApplicationManager::Description] = QVariant();
            drm[XQApplicationManager::MimeType] = QVariant(QString("audio/aac"));
            
            files[QString(drive + ":\\data\\Others\\111-test1.odf")] = drm;
        }
        
        {
            FileDrm drm;
            drm[XQApplicationManager::IsProtected] = QVariant(0);
            drm[XQApplicationManager::IsForwardable] = QVariant(1);
            drm[XQApplicationManager::Description] = QVariant();
            drm[XQApplicationManager::MimeType] = QVariant(QString("image/jpeg"));
            
            files[QString(drive + ":\\data\\Others\\one.jpg")] = drm;
        }
        
        {
            FileDrm drm;
            drm[XQApplicationManager::IsProtected] = QVariant(0);
            drm[XQApplicationManager::IsForwardable] = QVariant(1);
            drm[XQApplicationManager::Description] = QVariant();
            drm[XQApplicationManager::MimeType] = QVariant(QString("application/vnd.oma.drm.roap-trigger+xml"));
            
            files[QString(drive + ":\\data\\Others\\RoAcqoffer-111-aac-i15m.ort")] = drm;
        }
        
        {
            FileDrm drm;
            drm[XQApplicationManager::IsProtected] = QVariant(1);
            drm[XQApplicationManager::IsForwardable] = QVariant(1);
            drm[XQApplicationManager::Description] = QVariant();
            drm[XQApplicationManager::MimeType] = QVariant(QString("audio/sp-midi"));
            
            files[QString(drive + ":\\data\\Others\\SD_Celebration_SP.dcf")] = drm;
        }
        
        {
            FileDrm drm;
            drm[XQApplicationManager::IsProtected] = QVariant(1);
            drm[XQApplicationManager::IsForwardable] = QVariant(1);
            drm[XQApplicationManager::Description] = QVariant();
            drm[XQApplicationManager::MimeType] = QVariant(QString("image/jpeg"));
            
            files[QString(drive + ":\\data\\Others\\SD_jpg_sun.dcf")] = drm;
        }
        
        {
            FileDrm drm;
            drm[XQApplicationManager::IsProtected] = QVariant(1);
            drm[XQApplicationManager::IsForwardable] = QVariant(1);
            drm[XQApplicationManager::Description] = QVariant(QString(""));
            drm[XQApplicationManager::MimeType] = QVariant(QString("audio/x-ms-wma"));
            
            files[QString(drive + ":\\data\\Others\\STC1_128_44_16_2_CBR.wma")] = drm;
        }
            

    }
    
    ~TestData()
    {
        foreach(InterfaceData* interface , interfaces.values())
            delete interface;
    }
    
public:
    QMultiHash<QString, InterfaceData*> services;
    QMultiHash<QString, InterfaceData*> interfaces;
    QHash<QString, FileDrm> files;
};

TestData testData;

#endif /* TESTDATA_H_ */
