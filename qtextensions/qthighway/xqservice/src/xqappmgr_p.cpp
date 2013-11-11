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

#include <xqservicerequest.h>
#include <xqservicemanager.h>
#include <QList>
#include <xqsettingsmanager.h>
#include <xqsettingskey.h>
#include <e32std.h>

#include "xqservicelog.h"
#include "xqserviceutil.h"
#include "xqaiwutils.h"
#include "xqaiwuridriver.h"
#include "xqaiwdecl.h"
#include "xqappmgr_p.h"

#include <xqsettingsmanager.h>

/*!
    \class XQApplicationManagerPrivate
    \brief Private implementation of the XQApplicationManager
*/

XQApplicationManagerPrivate::XQApplicationManagerPrivate():
   QObject(),
   v_ptr(0),
   serviceMgr(0),
   aiwUtilities(0),
   settingsManagerInstance(0)
   
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate");
    serviceMgr = new XQServiceManager();
}

XQApplicationManagerPrivate::~XQApplicationManagerPrivate()
{
    XQSERVICE_DEBUG_PRINT("~XQApplicationManagerPrivate");
    delete serviceMgr;
    delete aiwUtilities;
    delete settingsManagerInstance;
}

XQAiwRequest* XQApplicationManagerPrivate::create( const QString &interface, const QString &operation, bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::create(interface)");
    return create("", interface, operation, embedded);
}

XQAiwRequest* XQApplicationManagerPrivate::create(
    const QString &service, const QString &interface, const QString &operation, bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::create(service+interface)");

    QList<XQAiwInterfaceDescriptor> impls;
    if (service.isEmpty())
        impls = serviceMgr->findFirstInterface(interface);
    else
        impls = serviceMgr->findFirstInterface(service, interface);

    // Pick up the first implementation
    if (impls.count())
    {
        return new XQAiwRequest(impls[0], operation, embedded);
    }
    return 0;
}


XQAiwRequest* XQApplicationManagerPrivate::create(
    const XQAiwInterfaceDescriptor &implementation, const QString &operation, bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::create (impl)");
    return new XQAiwRequest(implementation, operation, embedded);
}

XQAiwRequest* XQApplicationManagerPrivate::create(
    const QUrl &uri, const XQAiwInterfaceDescriptor *implementation, bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::create (uri)");
    XQSERVICE_DEBUG_PRINT("uri=%s", qPrintable(uri.toString()));
    
    if (!uri.isValid())
    {
        XQSERVICE_CRITICAL_PRINT("Invalid URI %s", qPrintable(uri.toString()));
        return 0;
    }

    if (hasCustomHandler(uri))
    {
        XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::custom handler applied");
        XQAiwInterfaceDescriptor nullDescr;
        return new XQAiwRequest(uri, nullDescr, XQOP_URI_VIEW);
    }
    
    const XQAiwInterfaceDescriptor *impl = implementation;
    QList<XQAiwInterfaceDescriptor> impls;
    if (impl == 0) 
    {
        // Implementation not given, find first one        
        impls = list(uri);
        if (impls.count())
        {
            impl = &impls[0];
            XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::create (impl) use first)");
        }
    }

    XQAiwRequest *req = 0;
    if (impl != 0)
    {
        // Create service request for viewing the URI
        XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::create (impl) creating request)");
        req = new XQAiwRequest(uri, *impl, XQOP_URI_VIEW);
        if (req)
        {
            req->setEmbedded(embedded);
        }
    }

    return req;

    
}

XQAiwRequest* XQApplicationManagerPrivate::create(
     const QFile &file, const XQAiwInterfaceDescriptor *implementation, bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::create(file)");
    XQSERVICE_DEBUG_PRINT("file=%s", qPrintable(file.fileName()));

    const XQAiwInterfaceDescriptor *impl = implementation;
    QList<XQAiwInterfaceDescriptor> impls;
    if (impl == 0) 
    {
        // Implementation not given, find services capable of handling the "file"
        impls = list(file);
        if (impls.count())
        {
            impl = &impls[0];
        }
    }

    XQAiwRequest * req = 0;
    if (impl != 0)
    {
        QVariant v = impl->property(XQAiwInterfaceDescriptor::ImplementationId);
        req = new XQAiwRequest(file, *impl, XQOP_FILE_VIEW);
        if (req)
        {
            req->setEmbedded(embedded);
            XQSERVICE_DEBUG_PRINT("File handled by %x", v.toInt());
        }
    }
    
    return req;
}

XQAiwRequest* XQApplicationManagerPrivate::create(
    const XQSharableFile &file, const XQAiwInterfaceDescriptor *implementation, bool embedded)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::create(XQSharableFile)");
    if (!file.isValid())
    {
        XQSERVICE_DEBUG_PRINT("\tInvalid XQSharableFile)");
        return 0;
    }
    
    const XQAiwInterfaceDescriptor *impl = implementation;
    QList<XQAiwInterfaceDescriptor> impls;
    if (impl == 0) 
    {
        // Implementation not given, find services capable of handling the "file"
        impls = list(file);
        if (impls.count())
        {
            impl = &impls[0];
        }
    }

    XQAiwRequest * req = 0;
    if (impl != 0)
    {
        QVariant v = impl->property(XQAiwInterfaceDescriptor::ImplementationId);
        req = new XQAiwRequest(file, *impl, XQOP_FILE_VIEW_SHARABLE);
        if (req)
        {
            req->setEmbedded(embedded);
            XQSERVICE_DEBUG_PRINT("File handled by %x", v.toInt());
        }
    }

    return req;
}



QList<XQAiwInterfaceDescriptor> XQApplicationManagerPrivate::list(
    const QString &interface, const QString& operation)
{

    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::list(interface)");
    Q_UNUSED(operation);

    return serviceMgr->findInterfaces(interface);
}

QList<XQAiwInterfaceDescriptor> XQApplicationManagerPrivate::list(
    const QString &service, const QString& interface, const QString& operation)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::list (service+interface)");
    Q_UNUSED(operation);

    return serviceMgr->findInterfaces(service, interface);
}

QList<XQAiwInterfaceDescriptor> XQApplicationManagerPrivate::list(
    const QUrl &uri)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::list (uri) %s", qPrintable(uri.toString()));

    QList<XQAiwInterfaceDescriptor> result;
    if (!uri.isValid())
    {
        XQSERVICE_CRITICAL_PRINT("Invalid URI %s", qPrintable(uri.toString()));
        return result; // Empty
    }

    QString scheme = uri.scheme();
    
    if (scheme.compare(XQURI_SCHEME_FILE,Qt::CaseInsensitive) == 0)
    {
        QFile file (uri.toLocalFile());
        return list(file);  // Apply file based listing
    }
    
    //  Find all services implementing URI interface and support URI scheme
    QList<XQAiwInterfaceDescriptor> uriHandlers;
    uriHandlers = list(XQI_URI_VIEW, "");

    // Pick up services supporting the required scheme
    foreach (XQAiwInterfaceDescriptor uh, uriHandlers)
    {
        //  Find services that support the scheme
        if (uh.customProperty(XQCUSTOM_PROP_SCHEMES).contains(scheme,Qt::CaseInsensitive))  // Allow multiple schemes in same string
        {
            XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate:: Service %s can handle scheme %s",
                                  qPrintable(uh.serviceName()), qPrintable(scheme))
            result.append(uh);
        }
    }

    return result;
}

//
// List services capable of handling the file by MIME type
//
QList<XQAiwInterfaceDescriptor> XQApplicationManagerPrivate::list(const QFile &file)
{

    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::list (file) %s", qPrintable(file.fileName()));

    // List first MIME handlers for file
    QList<XQAiwInterfaceDescriptor> mimeHandlers;
    mimeHandlers = listMimeHandlers(file);

    // Then list those file services that can support the MIME type
    return listFileHandlers(mimeHandlers);
}

//
// List services capable of handling the sharable file by MIME type
//
QList<XQAiwInterfaceDescriptor> XQApplicationManagerPrivate::list(const XQSharableFile &file)
{

    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::list (handle)");

    // List first MIME handlers for the sharable file
    QList<XQAiwInterfaceDescriptor> mimeHandlers;
    if (!file.isValid())
    {
        return mimeHandlers; // Empty set
    }
    mimeHandlers = listMimeHandlers(file);

    // Then list those file services that can support the MIME type
    return listFileHandlers(mimeHandlers);
}


int XQApplicationManagerPrivate::lastError() const
{
    int err=0;
    err = serviceMgr->latestError();
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate:: lastError=%d", err);
    return err;
}

bool XQApplicationManagerPrivate::isRunning(const XQAiwInterfaceDescriptor& implementation) const
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate isRunning");
    return serviceMgr->isRunning(implementation);
}

bool XQApplicationManagerPrivate::getDrmAttributes(const QFile &file, const QList<int> &attributeNames, QVariantList &attributeValues)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate drmAttributes");
    if (aiwUtilities == 0)
        aiwUtilities = new XQAiwUtils();
    return aiwUtilities->getDrmAttributes(file.fileName(), attributeNames, attributeValues);
}

bool XQApplicationManagerPrivate::getDrmAttributes(const XQSharableFile &file, const QList<int> &attributeNames, QVariantList &attributeValues)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate drmAttributes");
    if (aiwUtilities == 0)
        aiwUtilities = new XQAiwUtils();
    return aiwUtilities->getDrmAttributes(file,attributeNames, attributeValues);
}

int XQApplicationManagerPrivate::status(const XQAiwInterfaceDescriptor& implementation)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate status");
    QString statusKeyValue = implementation.customProperty(XQCUSTOM_PROP_AIW_SERVICE_STATUS);
    if (statusKeyValue.isEmpty())
    {
        // No custom property,  have to assume service is enabled
        XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate no custom property %s",
                              XQCUSTOM_PROP_AIW_SERVICE_STATUS);
        return XQApplicationManager::Unknown;
    }

    if (aiwUtilities == 0)
        aiwUtilities = new XQAiwUtils();
    
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate custom property value=%s",
                          qPrintable(statusKeyValue));
    
    bool b=false;
    int keyId = aiwUtilities->toIntFromHex(statusKeyValue, &b);
    if (!b)
    {
        XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate bad custom property value");
        return XQApplicationManager::Unknown;
    }
    
    int implId = implementation.property(XQAiwInterfaceDescriptor::ImplementationId).toInt();
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate %x,%x", keyId, implId);
    
    XQSettingsKey statusKey (XQSettingsKey::TargetCentralRepository, implId, keyId);
    QVariant value = settingsManager()->readItemValue(statusKey);
    if (value.isNull())
    {
        XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate Cenrep %x does not contain key %x",
                              implId, keyId);
        return XQApplicationManager::Unknown;
    }

    
    int status = value.toInt(&b);
    if (!b)
    {
        XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate invalid status value %s",
                              qPrintable(value.toString()));
        return XQApplicationManager::Unknown;
    }
        
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate status=%d", status);
    
    return status;
   
}

// ---- PRIVATE FUNCTIONS ----

bool XQApplicationManagerPrivate::hasCustomHandler(const QUrl &uri) const
{
    return XQAiwUriDriver::hasCustomHandler(uri);
}

//
// Get file handlers
// TODO: Currently only one, default handler is returned !!!
//       Possibly later all handlers need to be returned.
//
QList<XQAiwInterfaceDescriptor> XQApplicationManagerPrivate::listMimeHandlers(const QFile &file)
{

    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::listMimeHandlers %s", qPrintable(file.fileName()));
    QList<XQAiwInterfaceDescriptor> mimeHandlers;

    if (aiwUtilities == 0)
        aiwUtilities = new XQAiwUtils();
    if (aiwUtilities == 0)
    {
        XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate:: Can not create aiw utils");
        return mimeHandlers;
    }

    XQAiwInterfaceDescriptor descriptor;

    // Find default application that can handle MIME type (only one at the moment)
    int applicationId = 0;
    if (aiwUtilities->findApplication(file, applicationId) != XQService::ENoError)
    {
        return mimeHandlers;  // Empty set
    }

    // Set incomplete descriptor
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate:: Found file handler %x", applicationId);
    descriptor.setProperty(XQAiwInterfaceDescriptor::ImplementationId, QVariant(applicationId));
    
    mimeHandlers.append(descriptor);

    return mimeHandlers;
}

//
// Get handlers for sharable file
// TODO: Currently only one, default handler is returned !!!
//       Possibly later all handlers need to be returned.
//
QList<XQAiwInterfaceDescriptor> XQApplicationManagerPrivate::listMimeHandlers(const XQSharableFile &file)
{

    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::listMimeHandlers (handle)");
    QList<XQAiwInterfaceDescriptor> mimeHandlers;

    if (aiwUtilities == 0)
        aiwUtilities = new XQAiwUtils();
    if (aiwUtilities == 0)
    {
        XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate:: Can not create aiw utils");
        return mimeHandlers;
    }

    XQAiwInterfaceDescriptor descriptor;

    // Find default application that can handle MIME type (only one at the moment)
    int applicationId = 0;
    if (aiwUtilities->findApplication(file, applicationId) != XQService::ENoError)
    {
        return mimeHandlers;  // Empty set
    }

    // Set incomplete descriptor
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate:: Found file handler %x", applicationId);
    descriptor.setProperty(XQAiwInterfaceDescriptor::ImplementationId, QVariant(applicationId));

    mimeHandlers.append(descriptor);

    return mimeHandlers;
}


// To be called only for URI having scheme XQURI_SCHEME_ACTIVITY
// Check that application exists and fill in only the implementationId
bool XQApplicationManagerPrivate::getAppDescriptor(const QUrl &uri,  XQAiwInterfaceDescriptor *descriptor)
{

    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::getAppDescriptor %s)", qPrintable(uri.toString()));

    if (aiwUtilities == 0)
        aiwUtilities = new XQAiwUtils();
    if (aiwUtilities == 0)
    {
        XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate:: Can not create aiw utils");
        return false;
    }

    int applicationId = 0;
    if (aiwUtilities->findApplication(uri, applicationId) != XQService::ENoError)
    {
        return false;  // No application registered for file
    }

    // Create incomplete descriptor
    descriptor->setProperty(XQAiwInterfaceDescriptor::ImplementationId, QVariant(applicationId));
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate:: Found uri handler %x", applicationId);

    return  true; 
}


//
// Get file handlers that can support the give MIME type
//
QList<XQAiwInterfaceDescriptor> XQApplicationManagerPrivate::listFileHandlers(
    const QList<XQAiwInterfaceDescriptor> &mimeHandlers)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate::listFileHandlers");

    //  Find all services implementing URI interface, support "file" scheme and can handle the file
    //  by MIME type
    QList<XQAiwInterfaceDescriptor> fileServices;
    fileServices = list(XQI_FILE_VIEW, "");
    if (fileServices.count() == 0)
    {
        return fileServices; // Empty set
    }

    QList<XQAiwInterfaceDescriptor> result;
    foreach (XQAiwInterfaceDescriptor fs, fileServices)
    {
        //  Find services that support the file scheme
        XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate:: Service %s can handle file", qPrintable(fs.serviceName()));
        int serviceAppId = fs.property(XQAiwInterfaceDescriptor::ImplementationId).toInt();
        foreach (XQAiwInterfaceDescriptor mh, mimeHandlers)
        {
            int mimeAppId = mh.property(XQAiwInterfaceDescriptor::ImplementationId).toInt();
            if (mimeAppId == serviceAppId)
            {
                // Return only those services that support file scheme and claim to support also MIME type
                XQSERVICE_DEBUG_PRINT("XQApplicationManagerPrivate:: Service %s (UID=%x) can handle file and MIME type",
                                      qPrintable(fs.serviceName()), serviceAppId);
                result.append(fs);
            }
        }
    }

    if (result.isEmpty()) {
        // No service support present, try using the MIME handlers via old way
        return mimeHandlers;
    }
    return result;
}

bool XQApplicationManagerPrivate::startNotifications(XQAiwInterfaceDescriptor& serviceImplDescriptor)
{
    QString crcName = QString("%1.%2").arg(serviceImplDescriptor.serviceName()).arg(serviceImplDescriptor.interfaceName()).toLower();
    
    quint32 crc = XQServiceUtil::serviceIdFromName(crcName.toLatin1().data());
    descriptorsMap.insert(crc, serviceImplDescriptor);
    XQSettingsManager *settings = settingsManager();

    bool ok = false;
    
    XQSERVICE_CONNECT(settings, SIGNAL(valueChanged(XQSettingsKey,QVariant)), this, SLOT(valueChanged(XQSettingsKey,QVariant)));
    XQSERVICE_CONNECT(settings, SIGNAL(itemDeleted(XQSettingsKey)), this, SLOT(itemDeleted(XQSettingsKey)));
    
    quint32 uid = serviceImplDescriptor.property(XQAiwInterfaceDescriptor::ImplementationId).toInt(&ok);
    
    if (ok) {
        XQSettingsKey key(XQSettingsKey::TargetPublishAndSubscribe, uid, crc);
        ok = settings->startMonitoring(key);
		// key may be discarded now, as we got full information about recreating it while stopping notifications.
    }   

    return ok;
}

bool XQApplicationManagerPrivate::stopNotifications(XQAiwInterfaceDescriptor& serviceImplDescriptor)
{
    quint32 crc = XQServiceUtil::serviceIdFromName(serviceImplDescriptor.serviceName().toLatin1().data());
    descriptorsMap.remove(crc);
    XQSettingsManager *settings = settingsManager();

	bool ok = false;
    quint32 uid = serviceImplDescriptor.property(XQAiwInterfaceDescriptor::ImplementationId).toInt(&ok);
    if (ok) {
		XQSettingsKey key(XQSettingsKey::TargetPublishAndSubscribe, uid, crc);
		ok = settings->stopMonitoring(key);
	} 
    return ok;
}

void XQApplicationManagerPrivate::valueChanged(const XQSettingsKey& key, const QVariant& value)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::valueChanged");
    bool ok = false;
    XQApplicationManager::ServiceState state = (XQApplicationManager::ServiceState)(value.toInt(&ok));
    
    if (ok) {
    switch (state) {
        case XQApplicationManager::ServiceStarted: 
            {
                XQSERVICE_DEBUG_PRINT("XQApplicationManager::valueChanged : service started");
                XQAiwInterfaceDescriptor descriptor = descriptorsMap.value(key.key());
                //TODO add dummy descriptor check (no entry in map)
                emit v_ptr->serviceStarted(descriptor);
            }
            break;
        case XQApplicationManager::ServiceStopped:
            {
                XQSERVICE_DEBUG_PRINT("XQApplicationManager::valueChanged : service stopped");
                XQAiwInterfaceDescriptor descriptor = descriptorsMap.value(key.key());
                //TODO add dummy descriptor check (no entry in map)
                emit v_ptr->serviceStopped(descriptor);
            }
            break;
        default:
            ok = false;
            break;
        }
    }
    
    if (!ok) {
        //assertion
        XQSERVICE_WARNING_PRINT("XQApplicationManager::valueChanged : Service status undecipherable. Potential bug, please report.");
    }
}
    
void XQApplicationManagerPrivate::itemDeleted(const XQSettingsKey& key)
{
    XQSERVICE_DEBUG_PRINT("XQApplicationManager::itemDeleted");
    Q_UNUSED(key);
}

XQSettingsManager *XQApplicationManagerPrivate::settingsManager()
{
    if (settingsManagerInstance == 0) {
        settingsManagerInstance = new XQSettingsManager();
    }
    return settingsManagerInstance;
}

